//
// Created by gosaenz
//

#ifndef AIRWAR_COLA_H
#define AIRWAR_COLA_H


#include "QueueNode.h"
#include <iostream>

template <class T>
class Queue{
	public:
		Queue(){
			head = NULL;
			tail = NULL;
			size = 0;
		}
		
		void push(T val);
    	void dequeue();
    	T* get()  {return &head->value;}
    	
    	int sizeOf()  {return size;}
		
	private:
		
		struct QueueNode<T>* head;
		struct QueueNode<T>* tail;
		int size;
};

template <class T>
void Queue<T>::push(T val){
    QueueNode<T>* newNodo = new QueueNode<T>(val);
    
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
void Queue<T>::dequeue(){
	
    if (!head){
        return;
    } 
	else if (!head->next){
        delete head;
        head = NULL;
        tail = NULL;
        size = 0;
    } else {
        QueueNode<T>* newHead = head->next;
		
        delete head;
        head = newHead;
        --size;
    }
}

#endif //AIRWAR_COLA_H