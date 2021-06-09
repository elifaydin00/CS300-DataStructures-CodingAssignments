#include <iostream>
#include "Stack.h"
using namespace std;

//Constructor
template<class T>
Stack<T>::Stack()
{
	top=nullptr;
}

template<class T>
Stack<T>::~Stack() {
	makeEmpty();
}

template<class T>
T Stack<T>::top_return() {
	return this->top->pawn;
}


//Push back elements to the stack
template<class T>
void Stack<T>::push(T& pawnNode)
{
	stackNode<T>* newNode;
	newNode = new stackNode<T>;
	newNode->pawn = pawnNode;
	if(isEmpty())
	{
		top = newNode;
		newNode->next = nullptr;
	}
	else
	{
		newNode->next = top;
		top = newNode;
	}
}

//Pop up elements from the stack
template<class T>
void Stack<T>::pop()
{
	stackNode<T>* temp;
	if(!isEmpty())
	{
		temp = top->next;
		delete top;
		top = temp;
	}
}

//If the stack is empty check function
template<class T>
bool Stack<T>::isEmpty()
{
	return (this->top==nullptr);
}

template<class T>
void Stack<T>::makeEmpty() {
	while (!isEmpty()) {
		pop();
	}
}





