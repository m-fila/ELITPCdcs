#include "DCSObject.h"

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

DCSVariable &DCSObject::addVariable(std::string variableName,
                                    UA_DataType variableType) {
  if (variables.find(variableName) == variables.end()) {
    DCSVariable variable{server, objectNodeId, variableName, variableType};
    return variables.insert({variableName, variable}).first->second;
  } else {
    throw std::runtime_error("None unique variable id: " + objectName + "/" +
                             variableName);
  }
}

void DCSObject::addMethod(
    std::string methodName, std::string methodDescription,
    std::vector<methodArgs>inputs, std::vector<methodArgs> outputs,
    const std::function<void(const UA_Variant *, UA_Variant *)> & methodBody) {

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
                          outputArguments, this, &methodNodeId);

  UA_MethodAttributes_deleteMembers(&methodAttr);
  UA_QualifiedName_deleteMembers(&methodQName);

  UA_Array_delete(inputArguments, inputs.size(), &UA_TYPES[UA_TYPES_ARGUMENT]);
  UA_Array_delete(outputArguments, outputs.size(),
                  &UA_TYPES[UA_TYPES_ARGUMENT]);

  methods.insert({methodNodeId, methodBody});
}

UA_StatusCode DCSObject::methodCallback(
    UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *methodId, void *methodContext, const UA_NodeId *objectId,
    void *objectContext, size_t inputSize, const UA_Variant *input,
    size_t outputSize, UA_Variant *output) {

  auto object = static_cast<DCSObject *>(objectContext);
  // auto methodName=*static_cast<std::string*>(methodContext);
  // auto method=*static_cast<std::function<void(const UA_Variant*,
  // UA_Variant*)>*>(methodContext);
  object->methods.at (*methodId)(input, output);
  return UA_STATUSCODE_GOOD;
}
