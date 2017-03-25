#include <cstddef>

template <class T>
struct Nodo{
    Nodo(const T val) : next(NULL), valor(val) {}

    Nodo<T>* next;
    T valor;
};


