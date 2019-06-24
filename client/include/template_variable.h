#ifndef TEMPLATE_VARIABLE
#define TEMPLATE_VARIABLE
#include "abstract_variable.h"
#include <sstream>



template <class T>
class template_variable: public abstract_variable
{
public:
    template_variable(std::string name);
    ~template_variable();
    T data;
    virtual std::string translateValue();
    virtual std::string translateName();
    virtual void updateData(UA_DataValue *value);
    virtual void addSubsciption(){}

};
template <class T>
std::string template_variable<T>::translateValue(){
    std::ostringstream oss;
    oss<<data;
    return oss.str();
}
template <class T>
std::string template_variable<T>::translateName(){
    return VariableName;
}

template <class T>
void template_variable<T>::updateData(UA_DataValue *value){

    data=*static_cast<T*>(value->value.data);
}

template <class T>
template_variable<T>::template_variable(std::string name): abstract_variable(name){
}

template <class T>
template_variable<T>::~template_variable(){
    UA_NodeId_deleteMembers(&VariableId);
}

typedef template_variable<int> status_Variable;
typedef template_variable<int> state_Variable;
#endif // TEMPLATE_VARIABLE

