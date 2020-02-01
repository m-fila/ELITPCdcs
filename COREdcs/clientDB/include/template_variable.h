#ifndef TEMPLATE_VARIABLE
#define TEMPLATE_VARIABLE
#include "abstract_variable.h"
#include <sstream>
#include <iostream>
#include <ctime>

template <class T>
class template_variable: public abstract_variable
{
public:
    template_variable(std::string ObjectName, std::string VariableName);
    ~template_variable();
    T data;
    virtual std::string translateValue();
    virtual void updateData(UA_DataValue* value);
    virtual std::string translateKeys();

};
template <class T>
std::string template_variable<T>::translateValue(){
    std::ostringstream oss;
    oss<<data;
    return oss.str();
}
template<class T>
std::string template_variable<T>::translateKeys(){
    return "UA_timestamp INTEGER PRIMARY KEY, "+VariableName+" INTEGER";
}

template <class T>
void template_variable<T>::updateData(UA_DataValue* value){
    data=*(static_cast<T*>(value->value.data));
    std::cout<<data<<std::endl;

}

template <class T>
template_variable<T>::template_variable(std::string OName, std::string VName): abstract_variable(OName,VName){
}

template <class T>
template_variable<T>::~template_variable(){
 //   UA_NodeId_deleteMembers(&VariableNodeId);
}

typedef template_variable<bool> status_variable;
typedef template_variable<int> state_variable;
#endif // TEMPLATE_VARIABLE

