#include "DCSObject.h"
#include "DCSUAJson.h"
#include "json.hpp"
DCSObject::DCSObject(UA_Server *server, std::string name)
    : server(server), objectName(name),
      objectNodeId(UA_NODEID_STRING_ALLOC(1, name.c_str())) {
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
  UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(1, objectName.c_str());
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", objectName.c_str());
  UA_Server_addObjectNode(
      server, objectNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qName,
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), attr, this, nullptr);
  UA_ObjectAttributes_deleteMembers(&attr);
  UA_QualifiedName_deleteMembers(&qName);
}

DCSObject::~DCSObject() {
  UA_NodeId_deleteMembers(&objectNodeId);
  for (auto i : variables) {
    delete i.second;
  }
}

DCSVariable &DCSObject::addVariable(std::string variableName,
                                    UA_DataType variableType) {
  if (variables.find(variableName) == variables.end()) {
    return *variables
                .insert({variableName,
                         new DCSVariable(server, objectNodeId, objectName,
                                         variableName, variableType)})
                .first->second;
  } else {
    throw std::runtime_error("Not unique variable id: " + objectName + "/" +
                             variableName);
  }
}

void DCSObject::addMethod(
    std::string methodName, std::string methodDescription,
    std::vector<methodArgs> inputs, std::vector<methodArgs> outputs,
    const std::function<void(const UA_Variant *, UA_Variant *)> &methodBody,
    void *context) {

  UA_Argument *inputArguments = static_cast<UA_Argument *>(
      UA_Array_new(inputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]));

  for (size_t i = 0; i < inputs.size(); ++i) {
    UA_Argument_init(&inputArguments[i]);
    inputArguments[i].description =
        UA_LOCALIZEDTEXT_ALLOC("en-US", inputs.at(i).description.c_str());
    inputArguments[i].name = UA_String_fromChars(inputs.at(i).name.c_str());
    inputArguments[i].dataType = inputs.at(i).dataType.typeId;
    inputArguments[i].valueRank = UA_VALUERANK_SCALAR;
  }

  UA_Argument *outputArguments = static_cast<UA_Argument *>(
      UA_Array_new(outputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]));

  for (size_t i = 0; i < outputs.size(); ++i) {
    UA_Argument_init(&outputArguments[i]);
    outputArguments[i].description =
        UA_LOCALIZEDTEXT_ALLOC("en-US", outputs.at(i).description.c_str());
    outputArguments[i].name = UA_String_fromChars(outputs.at(i).name.c_str());
    outputArguments[i].dataType = outputs.at(i).dataType.typeId;
    outputArguments[i].valueRank = UA_VALUERANK_SCALAR;
  }

  UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
  methodAttr.description =
      UA_LOCALIZEDTEXT_ALLOC("en-US", methodDescription.c_str());
  methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", methodName.c_str());
  methodAttr.executable = true;
  methodAttr.userExecutable = true;
  UA_QualifiedName methodQName = UA_QUALIFIEDNAME_ALLOC(1, methodName.c_str());

  UA_NodeId methodNodeId;

  UA_Server_addMethodNode(server, UA_NODEID_NULL, objectNodeId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                          methodQName, methodAttr, methodCallback,
                          inputs.size(), inputArguments, outputs.size(),
                          outputArguments, context, &methodNodeId);

  UA_MethodAttributes_deleteMembers(&methodAttr);
  UA_QualifiedName_deleteMembers(&methodQName);

  UA_Array_delete(inputArguments, inputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]);
  UA_Array_delete(outputArguments, outputs.size(),
                  &UA_TYPES[UA_TYPES_ARGUMENT]);
  if (context != nullptr) {
    UA_Server_setMethodNodeAsync(server, methodNodeId, UA_TRUE);
  }
  methods.insert({methodNodeId, methodBody});
}

UA_StatusCode DCSObject::methodCallback(
    UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *methodId, void *methodContext, const UA_NodeId *objectId,
    void *objectContext, size_t inputSize, const UA_Variant *input,
    size_t outputSize, UA_Variant *output) {

  auto object = static_cast<DCSObject *>(objectContext);

  {
    std::string message = object->objectName + " called method \"";
    UA_LocalizedText methodName;
    UA_Server_readDisplayName(server, *methodId, &methodName);
    message += std::string(reinterpret_cast<char *>(methodName.text.data),
                           methodName.text.length) +
               "\" with arguments: ";
    if (inputSize == 0) {
      message += "none";
    } else {
      for (size_t i = 0; i < inputSize; i++) {
        message +=
            nlohmann::json::parse(
                DCSUAJson::toString(&input[i], &UA_TYPES[UA_TYPES_VARIANT]))
                .at("Body")
                .dump() +
            " ";
      }
    }
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                message.c_str());
  }

  object->methods.at (*methodId)(input, output);
  std::string message = object->objectName + " method \"";
  UA_LocalizedText methodName;
  UA_Server_readDisplayName(server, *methodId, &methodName);
  message += std::string(reinterpret_cast<char *>(methodName.text.data),
                         methodName.text.length) +
             "\" returned: ";
  if (outputSize == 0) {
    message += "void";
  } else {
    for (size_t i = 0; i < outputSize; i++) {
      message +=
          nlohmann::json::parse(
              DCSUAJson::toString(&output[i], &UA_TYPES[UA_TYPES_VARIANT]))
              .at("Body")
              .dump() +
          " ";
    }
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                message.c_str());
  }
  UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, message.c_str());
  return UA_STATUSCODE_GOOD;
}
