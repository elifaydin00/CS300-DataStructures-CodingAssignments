#ifndef _STACK_H
#define _STACK_H
#include <iostream>

template<class T>
struct stackNode {
	T pawn;
	int indexx;
	int indexy;
	stackNode* next;
};


template <class T>
class Stack
{
private:
  stackNode<T>* top;
 
public:
  Stack(void);
  ~Stack();
  T top_return(void);
  void push(T&);
  void pop();
  bool isEmpty();
  void makeEmpty();
};

#endif
