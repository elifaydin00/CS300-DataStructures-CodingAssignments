#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

template <class HashedObj>
class LinkedList
{
public:
	LinkedList<HashedObj>(); //default constructor
	LinkedList<HashedObj>(const LinkedList<HashedObj>& ls);//copy constructor
	~LinkedList<HashedObj>(); //destructor
	LinkedList<HashedObj>& operator=(const LinkedList<HashedObj>&); //assignment operator
	bool insert(const HashedObj&);
	bool remove(const HashedObj&);
	bool search(const HashedObj&) const;
	HashedObj find(HashedObj&);
	void print() const;
	

private:
	template <class HashedObj>
	class Node{
	public:
		HashedObj data;
		Node<HashedObj>* next;
		Node(HashedObj s) :data(s), next(nullptr) {};
		Node(HashedObj s, Node<HashedObj>* nd) :data(s), next(nd) {};
	};
	Node<HashedObj>* front; //we are going to add new element to front
	void deepCopy(const LinkedList<HashedObj>& ls); //helper for copy constructor
	void deleteList(); //helper for destructor
};

#endif