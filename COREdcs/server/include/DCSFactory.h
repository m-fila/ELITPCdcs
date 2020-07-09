#ifndef DCS_FACTORY_H
#define DCS_FACTORY_H

#include <map>
#include <string>
// struct Key {
//  Key(std::string str = "Generic") : str(str) {}
//  const std::string str;
//};

template <class Base, class... Args> class DCSFactory {
  public:
    typedef Base *(*Allocator)(Args...);
    static std::map<const std::string, Allocator> &allocators() {
        static std::map<const std::string, Allocator> s;
        return s;
    }
    static Base *create(std::string registeredName, Args... args) {
        auto found = allocators().find(registeredName);
        if(found != allocators().end()) {
            return found->second(args...);
        }
        return nullptr;
    }
    template <class T> static bool insert() {
        return allocators()
            .insert({T::GetType(),
                     [](Args... args) -> Base * {
                         auto n = new T;
                         n->init(T::GetType(), args...);
                         return n;
                     }})
            .second;
    }

  public:
    template <class T> class Register {
        friend T;

      private:
        Register() { (void)registered; }
        static bool registered;
    };
};
template <class Base, class... Args>
template <class T>
bool DCSFactory<Base, Args...>::Register<T>::registered =
    DCSFactory<Base, Args...>::insert<T>();

#endif  //  DCS_FACTORY_H
