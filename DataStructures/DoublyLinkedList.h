//
// Created by gosaenz
//

#ifndef AIRWAR_LISTA_H
#define AIRWAR_LISTA_H


#include "DoubleNode.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template <class T>
class DoublyLinkedList{
	public:
		DoublyLinkedList(){
			head = NULL;
			tail = NULL;
			size = 0;
			savedSize = 0;
		}
		
		void addLast( T val);
		void addFirst( T val);
    	void remove( int pos);
    	T* get( int pos);
    	T* operator[](int pos);
    	
    	void write();
    	void read();
    	DoublyLinkedList<T> split(int chunk);
    	DoublyLinkedList<T> paginate(int chunk);
    	
    	int sizeOf() {return size;}
		
	private:
		
		void posError();
		
		struct DoubleNode<T>* head;
		struct DoubleNode<T>* tail;
		int size;
		int savedSize;
};

template <class T>
void DoublyLinkedList<T>::addLast( T val){
	DoubleNode<T>* newNode = new DoubleNode<T>(val);
    
    if (head == NULL){
    	head = newNode;
        tail = newNode;
    } 
    
	else{
        tail->next = newNode;
	    newNode->back = tail;
        tail = newNode;
    }
    
    ++size;
}

template <class T>
void DoublyLinkedList<T>::addFirst( T val){
	DoubleNode<T>* newNode = new DoubleNode<T>(val);
    
    if (head == NULL){
    	head = newNode;
        tail = newNode;
    } 
    
	else{
        head->back = newNode;
	    newNode->next = head;
        head = newNode;
    }
    
    ++size;
}

template <class T>
void DoublyLinkedList<T>::remove( int pos){
	
    if (pos >= size){
        posError();
    } 
	else if (pos == 0 && size == 1){
        delete head;
        head = NULL;
        tail = NULL;
        size = 0;
    } 
	else if (pos == 0){
	    DoubleNode<T>* newHead = head->next;
		newHead->back = NULL;
		delete head;
		head = newHead;
		--size;
	}
	else if (pos == size - 1){
	    DoubleNode<T>* newTail = tail->back;
		newTail->next = NULL;
		delete tail;
		tail = newTail;
		--size;
	}
	else {
	    DoubleNode<T>* temp = head;
        
        for (int i = 0; i < pos; i++){
        	temp = temp->next;
		}
	
	    DoubleNode<T>* tempBack = temp->back;
	    DoubleNode<T>* tempNext = temp->next;
        
        tempBack->next = tempNext;
        tempNext->back = tempBack;
	
	    delete temp;
	    
        --size;
    }
}

template <class T>
T* DoublyLinkedList<T>::get(int pos){
	
    if (pos >= size){
        posError();
    } else {
	    DoubleNode<T>* temp = head;
		
        for (int i = 0; i < pos; i++){
        	temp = temp->next;
		}
		
		return &temp->valor;
    }
}

template <class T>
T* DoublyLinkedList<T>::operator[](int pos) {
    return get(pos);
}

template <class T>
void DoublyLinkedList<T>::write(){
	
	int size = this->size;
	this->savedSize = size;
	
	vector<T> vector (size);
	T* ptr = vector.data();
	
	for (int i = size - 1; i != -1; i--){
		ptr[i] = this->*get(i);
		this->remove(i);
	}
	
	ofstream offile("data.bin", ios::out | ios::binary);
    offile.write(reinterpret_cast<const char *>(vector.data()), vector.size() * sizeof(T));
    offile.close();
	
}


template <class T>
void DoublyLinkedList<T>::read(){
	int size = this->savedSize;
	vector<T> vector (size);
	T* ptr = vector.data();
	
	ifstream infile("data.bin", ios::in | ios::binary);
    infile.read(reinterpret_cast<char *>(vector.data()), vector.size() * sizeof(T));
    infile.close();
    
    for (int i = 0; i < size; i++){
		this->addLast(ptr[i]);
	}
}

template <class T>
DoublyLinkedList<T> DoublyLinkedList<T>::split(int chunk) {
	DoublyLinkedList<T> temp;
	
	for (int i = chunk - 1; i != -1; i--){
		temp.addFirst(this->*get(i));
		this->remove(i);
	}
	
	return temp;
}

template <class T>
DoublyLinkedList<T> DoublyLinkedList<T>::paginate(int chunk) {
	
	this->read();
	
	DoublyLinkedList<T> temp = this->split(chunk);
	
	this->write();
	
	return temp;
}


template <typename T>
void DoublyLinkedList<T>::posError() {
    std::cout << "ERROR: Posicion fuera de rango";
        exit(EXIT_FAILURE);
}

#endif //AIRWAR_LISTA_H
