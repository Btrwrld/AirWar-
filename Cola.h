#include "Nodo.h"
#include <iostream>

template <class T>
class Cola{
	public:
		Cola(){
			head = NULL;
			tail = NULL;
			size = 0;
		}
		
		void encolar(const T val);
    	void desencolar();
    	T consultar() const {return head->valor;}
    	
    	int sizeOf() const {return size;}
		
	private:
		
		struct Nodo<T>* head;
		struct Nodo<T>* tail;
		int size;
};

template <class T>
void Cola<T>::encolar(const T val){
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
void Cola<T>::desencolar(){
	
    if (!head){
        return;
    } 
	else if (!head->next){
        delete head;
        head = NULL;
        tail = NULL;
        size = 0;
    } else {
        Nodo<T>* newHead = head->next;
		
        delete head;
        head = newHead;
        --size;
    }
}
