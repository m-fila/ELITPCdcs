#ifndef TYPEWRAPPER_H
#define TYPEWRAPPER_H

#include <open62541/types.h>

template <class T>
class TypeWrapper{
    public:

    TypeWrapper(UA_DataType type):type(type){
        UA_init(&content,&type);
    }

    TypeWrapper(T data,UA_DataType type): content(data),type(type){
    }

    ~TypeWrapper(){
        UA_clear(&content,&type);
    }

    TypeWrapper & operator=(const TypeWrapper& w){
        type=w.type;
        UA_clear(&content,&type);
        UA_copy(&w.content,&content,&w.type);
        return *this;
    }

    T* getData(){
        return &content;
    }    
    private:
    
    T content;
    UA_DataType type;
};
#endif // TYPEWRAPPER_H