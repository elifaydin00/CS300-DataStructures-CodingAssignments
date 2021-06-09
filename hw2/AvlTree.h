#ifndef _AVLTREE_H
#define _AVLTREE_H
#include <iostream>
using namespace std;

template <class typeName>
class AvlTree;

template <class typeName>
class AvlTreeNode
{
	typeName element;
	AvlTreeNode* left;
	AvlTreeNode* right;
	int height;

	AvlTreeNode(const typeName& theElement,AvlTreeNode* lt, AvlTreeNode* rt, int h = 0)
		: element(theElement), left(lt), right(rt), height(h) { }

	friend class AvlTree<typeName>;
};


template <class typeName>
class AvlTree
{
public:
	explicit AvlTree(); //CONSTRUCTOR
	AvlTree<typeName>(const AvlTree<typeName>&); //CC
	~AvlTree(); //DESTRUCTOR
	typeName& findMin();
	typeName& findMax();
	typeName& find(const typeName&);
	bool doesExists(const typeName&);
	bool isEmpty();
	void printTree();
	void makeEmpty();
	void insert(const typeName&);
	void remove(const typeName&);
	AvlTree<typeName>& operator=(const AvlTree<typeName>&);
	AvlTreeNode<typeName>* returnRoot();

private:
	AvlTreeNode<typeName>* root;

	//internal methods
	typeName& elementAt(AvlTreeNode<typeName>*);
	void insert(const typeName&, AvlTreeNode<typeName>*&);
	void remove(const typeName&, AvlTreeNode<typeName>*&);
	AvlTreeNode<typeName>* findMin(AvlTreeNode<typeName>*);
	AvlTreeNode<typeName>* findMax(AvlTreeNode<typeName>*);
	AvlTreeNode<typeName>* find(const typeName&, AvlTreeNode<typeName>*);
	void makeEmpty(AvlTreeNode<typeName>*&);
	void printTree(AvlTreeNode<typeName>*);
	AvlTreeNode<typeName>* clone(AvlTreeNode<typeName>*) const;

	// Avl manipulations
	int height(AvlTreeNode<typeName>* t) const;
	int max(int lhs, int rhs) const;
	void balance(AvlTreeNode<typeName>*& t); 
	void rotateWithLeftChild(AvlTreeNode<typeName>*& k2) const; 
	void rotateWithRightChild(AvlTreeNode<typeName>*& k1) const;
	void doubleWithLeftChild(AvlTreeNode<typeName>*& k3) const;
	void doubleWithRightChild(AvlTreeNode<typeName>*& k1) const;


};



#endif