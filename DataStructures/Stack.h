//
// Created by gosaenz
//

#ifndef AIRWAR_STACK_H
#define AIRWAR_STACK_H


#include "DoubleNode.h"
#include <iostream>

template <class T>
class Stack{
public:
	Stack(){
		head = NULL;
		tail = NULL;
		size = 0;
	}
	
	void push_back( T val);
	void pop_back();
	T* peek()  {return &tail->valor;}
	
	int sizeOf() const {return size;}

private:
	
	struct DoubleNode<T>* head;
	struct DoubleNode<T>* tail;
	int size;
};

template <class T>
void Stack<T>::push_back( T val){
	DoubleNode<T>* newNodo = new DoubleNode<T>(val);
	
	if (head == NULL){
		head = newNodo;
		tail = newNodo;
	}
	
	else{
		tail->next = newNodo;
		tail = newNodo;
	}
	
	++size;
}

template <class T>
void Stack<T>::pop_back(){
	
	if (!head){
		return;
	}
	else if (size == 1){
		delete tail;
		head = NULL;
		tail = NULL;
		size = 0;
	} else {
		DoubleNode<T>* newTail = head;
		
		while (newTail->next != tail){
			newTail = newTail->next;
		}
		
		delete tail;
		tail = newTail;
		--size;
	}
}


#endif //AIRWAR_STACK_H
