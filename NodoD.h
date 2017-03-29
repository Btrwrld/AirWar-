#include <cstddef>

template <class T>
struct Nodo{
    Nodo(const T val) : next(NULL), back(NULL), valor(val) {}

    Nodo<T>* next;
    Nodo<T>* back;
    T valor;
};


