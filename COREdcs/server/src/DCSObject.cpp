#include "DCSObject.h"
#include "DCSUAJson.h"

bool DCSObject::addObjectNode() {
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(1, objectName.c_str());
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", objectName.c_str());
    UA_NodeId objectTypeId = UA_NODEID_STRING_ALLOC(1, objectType.c_str());
    auto retv = UA_Server_addObjectNode(server, objectNodeId,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qName,
                                        objectTypeId, attr, this, nullptr);
    UA_ObjectAttributes_deleteMembers(&attr);
    UA_QualifiedName_deleteMembers(&qName);
    UA_NodeId_deleteMembers(&objectTypeId);
    return retv == UA_STATUSCODE_GOOD;
}

DCSObject::~DCSObject() {
    UA_NodeId_deleteMembers(&objectNodeId);
    for(auto i : variables) {
        delete i.second;
    }
}

DCSVariable &DCSObject::addVariable(std::string variableName,
                                    const UA_DataType *variableType, bool isArray) {
    if(variables.find(variableName) == variables.end()) {
        return *variables
                    .insert({variableName,
                             new DCSVariable(server, objectNodeId, objectName,
                                             variableName, variableType, isArray)})
                    .first->second;
    } else {
        throw std::runtime_error("Not unique variable id: " + objectName + "/" +
                                 variableName);
    }
}

DCSObject::Method &DCSObject::addMethod(
    std::string methodName, std::string methodDescription,
    std::vector<DCSObject::MethodArgs> inputs, std::vector<DCSObject::MethodArgs> outputs,
    const std::function<void(const UA_Variant *, UA_Variant *)> &methodBody,
    void *context) {

    Method method;
    method.method = methodBody;
    method.context = context;
    UA_Argument *inputArguments = static_cast<UA_Argument *>(
        UA_Array_new(inputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]));

    for(size_t i = 0; i < inputs.size(); ++i) {
        method.inputsNames.push_back(inputs.at(i).name);
        UA_Argument_init(&inputArguments[i]);
        inputArguments[i].description =
            UA_LOCALIZEDTEXT_ALLOC("en-US", inputs.at(i).description.c_str());
        inputArguments[i].name = UA_String_fromChars(inputs.at(i).name.c_str());
        inputArguments[i].dataType = inputs.at(i).dataType->typeId;
        inputArguments[i].valueRank = UA_VALUERANK_SCALAR;
    }

    UA_Argument *outputArguments = static_cast<UA_Argument *>(
        UA_Array_new(outputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]));

    for(size_t i = 0; i < outputs.size(); ++i) {
        method.outputsNames.push_back(outputs.at(i).name);
        UA_Argument_init(&outputArguments[i]);
        outputArguments[i].description =
            UA_LOCALIZEDTEXT_ALLOC("en-US", outputs.at(i).description.c_str());
        outputArguments[i].name = UA_String_fromChars(outputs.at(i).name.c_str());
        outputArguments[i].dataType = outputs.at(i).dataType->typeId;
        outputArguments[i].valueRank = UA_VALUERANK_SCALAR;
    }

    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", methodDescription.c_str());
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", methodName.c_str());
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName = UA_QUALIFIEDNAME_ALLOC(1, methodName.c_str());

    UA_NodeId methodNodeId;

    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1, 0), objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName, methodAttr, methodCallback, inputs.size(),
                            inputArguments, outputs.size(), outputArguments, nullptr,
                            &methodNodeId);

    auto &methodContext = methods.insert({methodName, method}).first->second;
    UA_Server_setNodeContext(server, methodNodeId, &methodContext);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_QualifiedName_deleteMembers(&methodQName);

    UA_Array_delete(inputArguments, inputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]);
    UA_Array_delete(outputArguments, outputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]);
    if(context != nullptr) {
        UA_Server_setMethodNodeAsync(server, methodNodeId, UA_TRUE);
    }
    return methodContext;
}

UA_StatusCode DCSObject::methodCallback(UA_Server *server, const UA_NodeId *sessionId,
                                        void *sessionContext, const UA_NodeId *methodId,
                                        void *methodContext, const UA_NodeId *objectId,
                                        void *objectContext, size_t inputSize,
                                        const UA_Variant *input, size_t outputSize,
                                        UA_Variant *output) {

    auto object = static_cast<DCSObject *>(objectContext);
    auto method = static_cast<Method *>(methodContext);

    UA_LocalizedText methodName;
    UA_Server_readDisplayName(server, *methodId, &methodName);
    auto mName = std::string(reinterpret_cast<char *>(methodName.text.data),
                             methodName.text.length);
    UA_LocalizedText_deleteMembers(&methodName);
    {
        std::string message =
            object->objectName + " method \"" + mName + "\" called with arguments: ";
        if(inputSize == 0) {
            message += "void";
        } else {
            for(size_t i = 0; i < inputSize; i++) {
                message +=
                    method->inputsNames.at(i) + ":" +
                    nlohmann::json::parse(
                        DCSUAJson::toString(&input[i], &UA_TYPES[UA_TYPES_VARIANT]))
                        .at("Body")
                        .dump() +
                    " ";
            }
        }
        UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, "%s",
                    message.c_str());
    }
    // actuall call
    (*method)(input, output);

    std::string message = object->objectName + " method \"" + mName + "\" returned: ";
    if(outputSize == 0) {
        message += "void";
    } else {
        for(size_t i = 0; i < outputSize; i++) {
            message += method->outputsNames.at(i) + ":" +
                       nlohmann::json::parse(
                           DCSUAJson::toString(&output[i], &UA_TYPES[UA_TYPES_VARIANT]))
                           .at("Body")
                           .dump() +
                       " ";
        }
    }
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, "%s", message.c_str());
    return UA_STATUSCODE_GOOD;
}

void DCSObject::init(std::string type, std::string name, UA_Server *host,
                     const Options &options) {
    server = host;
    objectName = name;
    objectNodeId = UA_NODEID_STRING_ALLOC(1, name.c_str());
    objectType = type;
    addObjectNode();
    addChildren(options);
}

void DCSObject::fastEvent(const std::string &sourceName, size_t severity,
                          const std::string &message) {
    auto event = createEvent();
    event.setSourceName(sourceName);
    event.setMessage(message);
    event.setSeverity(severity);
    event.trigger();
}
