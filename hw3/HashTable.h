#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <iostream>
#include <vector>
#include "LinkedList.h"
#include "LinkedList.cpp"

template <class HashedObj>
class HashTable
{
public:
	HashTable<HashedObj>(); //default constructor
	HashTable<HashedObj>(int); //one parameter constructor
	HashTable<HashedObj>(const HashTable<HashedObj>&); //copy constructor
	~HashTable<HashedObj>(); //destructor
	HashTable<HashedObj>& operator=(const HashTable<HashedObj>&); //assignment operator
	bool insert(const HashedObj&,int &hashedVal);
	bool remove(const HashedObj&, int &hashedVal);
	bool search(const HashedObj&, int&hashedVal) const;
	HashedObj find(HashedObj&, int& hashedVal);
	int size() const; //return numOfItems
	int maxSize() const; //return arrSize
	double loadFactor() const;
	

private:
	LinkedList<HashedObj>* arr;
	int arrSize;
	int numOfItems;
	int hashFunc(const HashedObj&) const;
	int getPrime(int) const;
	bool isPrime(int) const;
	void deepCopy(const HashTable<HashedObj>& h);
};

#endif