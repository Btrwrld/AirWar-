//
// Created by gosaenz
//

#ifndef AIRWAR_NODO_H
#define AIRWAR_NODO_H


#include <cstddef>

template <class T>
struct QueueNode{
    QueueNode(T value) : next(NULL), value(value) {}

    QueueNode<T>* next;
    T value;
};
#endif //AIRWAR_NODO_H

