#include "LinkedList.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

template <class HashedObj>
LinkedList<HashedObj>::LinkedList() //default constructor
{
	front = NULL;
}

template <class HashedObj>
LinkedList<HashedObj>::LinkedList(const LinkedList<HashedObj>& ls) //copy constructor
{
	deepCopy(ls);
}
template <class HashedObj>
LinkedList<HashedObj>::~LinkedList() //destructor
{
	deleteList();
}

template <class HashedObj>
LinkedList<HashedObj>& LinkedList<HashedObj>::operator=(const LinkedList<HashedObj>& ls) //assignment operator
{
	if (this != &ls)
	{
		deleteList();
		deepCopy(ls);
	}
	return *this;
}

template <class HashedObj>
bool LinkedList<HashedObj>::insert(const HashedObj& s)
{
	if (search(s))
		return false; //already exists, return false
	front = new Node<HashedObj>(s, front); //add to front, connect rest
	return true;
}

template <class HashedObj>
bool LinkedList<HashedObj>::remove(const HashedObj& s)
{
	Node<HashedObj>* temp = front;
	if (temp == NULL) 
		return false; //list is empty, return false
	if (temp->data == s) //s is first item in list
	{
		front = temp->next;
		delete temp;
		return true;
	}
	else
	{
		while (temp->next != NULL) {
			if (temp->next->data == s)
			{
				Node<HashedObj>* deletedNode = temp->next; //this is the node to be deleted
				temp->next = temp->next->next; //connect to rest
				delete deletedNode; //delete it
				return true; 
			}
			temp = temp->next;
		}
		return false; //not found, return false
	}
}

template <class HashedObj>
bool LinkedList<HashedObj>::search(const HashedObj& s) const
{
	Node<HashedObj>* temp = front;
	while (temp != NULL) //Traverse list
	{
		if (temp->data == s)
			return true; //element found
		temp = temp->next;
	}
	return false; //element not found
}

template<class HashedObj>
HashedObj LinkedList<HashedObj>::find(HashedObj& searched)
{
	if (search(searched)) { //if exists
		Node<HashedObj>* temp = front;
		while (temp != NULL) //Traverse list
		{
			if (temp->data == searched)
				return temp->data; //element found
			temp = temp->next;
		}
	}
}

template <class HashedObj>
void LinkedList<HashedObj>::print() const {
	Node<HashedObj>* temp = front;
	while (temp != NULL) //Traverse list
	{
		cout << temp->data << endl;
		temp = temp->next;
	}
}



template <class HashedObj>
void LinkedList<HashedObj>::deepCopy(const LinkedList<HashedObj>& ls)
{
	front = NULL;
	if (ls.front != NULL) //Only copy if ls is non-empty
	{
		Node<HashedObj>* original = ls.front;
		Node<HashedObj>* copy;
		copy = new Node<HashedObj>(original->data, NULL); //get the front node
		front = copy;
		original = original->next; //now connect the rest, traverse
		while (original != NULL) //Traverse the original copying each node
		{
			copy->next = new Node<HashedObj>(original->data, NULL);
			copy = copy->next;
			original = original->next;
		}
	}
} 

template <class HashedObj>
void LinkedList<HashedObj>::deleteList()
{
	Node<HashedObj>* tmp;
	while (front != NULL) {
		tmp = front->next;
		delete front;
		front = tmp;
	}
}