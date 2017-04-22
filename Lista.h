#include "NodoD.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template <class T>
class Lista{
	public:
		Lista(){
			head = NULL;
			tail = NULL;
			size = 0;
			savedSize = 0;
		}
		
		void agregarFinal(const T val);
		void agregarInicio(const T val);
    	void quitar(const int pos);
    	T consultar(const int pos);
    	T operator[](const size_t position);
    	
    	void write();
    	void read();
    	Lista<T> split(int chunk);
    	Lista<T> paginate(int chunk);
    	
    	
    	int sizeOf() const {return size;}
		
	private:
		
		void posError();
		
		struct Nodo<T>* head;
		struct Nodo<T>* tail;
		int size;
		int savedSize;
};

template <class T>
void Lista<T>::agregarFinal(const T val){
    Nodo<T>* newNodo = new Nodo<T>(val);
    
    if (head == NULL){
    	head = newNodo;
        tail = newNodo;
    } 
    
	else{
        tail->next = newNodo;
        newNodo->back = tail;
        tail = newNodo;
    }
    
    ++size;
}

template <class T>
void Lista<T>::agregarInicio(const T val){
    Nodo<T>* newNodo = new Nodo<T>(val);
    
    if (head == NULL){
    	head = newNodo;
        tail = newNodo;
    } 
    
	else{
        head->back = newNodo;
        newNodo->next = head;
        head = newNodo;
    }
    
    ++size;
}

template <class T>
void Lista<T>::quitar(const int pos){
	
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
		Nodo<T>* newHead = head->next;
		newHead->back = NULL;
		delete head;
		head = newHead;
		--size;
	}
	else if (pos == size - 1){
		Nodo<T>* newTail = tail->back;
		newTail->next = NULL;
		delete tail;
		tail = newTail;
		--size;
	}
	else {
        Nodo<T>* temp = head;
        
        for (int i = 0; i < pos; i++){
        	temp = temp->next;
		}
		
        Nodo<T>* tempBack = temp->back;
        Nodo<T>* tempNext = temp->next;
        
        tempBack->next = tempNext;
        tempNext->back = tempBack;
        
        delete temp;
        
        --size;
    }
}

template <class T>
T Lista<T>::consultar(const int pos){
	
    if (pos >= size){
        posError();
    } else {
        Nodo<T>* temp = head;
		
        for (int i = 0; i < pos; i++){
        	temp = temp->next;
		}
		
		return temp->valor;
    }
}

template <class T>
T Lista<T>::operator[](const size_t pos) {
    return consultar(pos);
}

template <class T>
void Lista<T>::write(){
	
	int size = this->size;
	this->savedSize = size;
	
	vector<T> vector (size);
	T* ptr = vector.data();
	
	for (int i = size - 1; i != -1; i--){
		ptr[i] = this->consultar(i);
		this->quitar(i);
	}
	
	ofstream offile("data.bin", ios::out | ios::binary);
    offile.write(reinterpret_cast<const char *>(vector.data()), vector.size() * sizeof(T));
    offile.close();
	
}


template <class T>
void Lista<T>::read(){
	int size = this->savedSize;
	vector<T> vector (size);
	T* ptr = vector.data();
	
	ifstream infile("data.bin", ios::in | ios::binary);
    infile.read(reinterpret_cast<char *>(vector.data()), vector.size() * sizeof(T));
    infile.close();
    
    for (int i = 0; i < size; i++){
		this->agregarFinal(ptr[i]);
	}
}

template <class T>
Lista<T> Lista<T>::split(int chunk) {
	Lista<T> temp;
	
	for (int i = chunk - 1; i != -1; i--){
		temp.agregarInicio(this->consultar(i));
		this->quitar(i);
	}
	
	return temp;
}

template <class T>
Lista<T> Lista<T>::paginate(int chunk) {
	
	this->read();
	
	Lista<T> temp = this->split(chunk);
	
	this->write();
	
	return temp;
}

template <typename T>
void Lista<T>::posError() {
    std::cout << "ERROR: Posicion fuera de rango";
        exit(EXIT_FAILURE);
}
