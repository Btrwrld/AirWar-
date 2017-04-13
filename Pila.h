#include "Nodo.h"
#include <iostream>

template <class T>
class Pila{
	public:
		Pila(){
			head = NULL;
			tail = NULL;
			size = 0;
		}
		
		void apilar(const T val);
    	void desapilar();
    	T consultar() const {return tail->valor;}
    	
    	int sizeOf() const {return size;}
		
	private:
		
		struct Nodo<T>* head;
		struct Nodo<T>* tail;
		int size;
};

template <class T>
void Pila<T>::apilar(const T val){
    Nodo<T>* newNodo = new Nodo<T>(val);
    
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
void Pila<T>::desapilar(){
	
    if (!head){
        return;
    } 
	else if (size == 1){
        delete tail;
        head = NULL;
        tail = NULL;
        size = 0;
    } else {
        Nodo<T>* newTail = head;
		
		while (newTail->next != tail){
			newTail = newTail->next;
		}
		
        delete tail;
        tail = newTail;
        --size;
    }
}
