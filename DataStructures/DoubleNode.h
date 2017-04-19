//
// Created by gosaenz
//
#ifndef AIRWAR_NODOD_H
#define AIRWAR_NODOD_H

#include <cstddef>

template <class T>
struct DoubleNode{
	DoubleNode(T val) : next(NULL), back(NULL), valor(val) {}
	
	DoubleNode<T>* next;
	DoubleNode<T>* back;
    T valor;
};

#endif //AIRWAR_NODOD_H
