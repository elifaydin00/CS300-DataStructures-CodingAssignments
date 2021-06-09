#ifndef _BINARYTREE_H
#define _BINARYTREE_H
#include <iostream>
using namespace std;


template <class typeName>
class BinaryTreeNode
{
private:
	typeName data;
	BinaryTreeNode<typeName>* left;
	BinaryTreeNode<typeName>* right;
	BinaryTreeNode(const typeName&, BinaryTreeNode<typeName>*, BinaryTreeNode<typeName>*);
	

template <class typeName> friend class BinaryTree;
};

template <class typeName>
class BinaryTree
{
public:
	explicit BinaryTree(); //CONSTRUCTOR
	BinaryTree<typeName>(const BinaryTree<typeName>&); //CC
	~BinaryTree(); //DESTRUCTOR
	typeName& findMin();
	typeName& findMax();
	typeName& find(const typeName&);
	bool doesExists(const typeName&);
	bool isEmpty();
	void printTree();
	void makeEmpty();
	void insert(const typeName&);
	void remove(const typeName&); 
	BinaryTree<typeName>& operator=(const BinaryTree<typeName>&); 
	BinaryTreeNode<typeName>* returnRoot(); 

private:
	BinaryTreeNode<typeName>* root;
	
	//internal methods
	typeName& elementAt(BinaryTreeNode<typeName>*);
    void insert(const typeName&, BinaryTreeNode<typeName>*&);
    void remove(const typeName&, BinaryTreeNode<typeName>*&);
    BinaryTreeNode<typeName>* findMin(BinaryTreeNode<typeName>*);
    BinaryTreeNode<typeName>* findMax(BinaryTreeNode<typeName>*);
    BinaryTreeNode<typeName>* find(const typeName&, BinaryTreeNode<typeName>*); 
    void makeEmpty(BinaryTreeNode<typeName>*&);
    void printTree(BinaryTreeNode<typeName>*);
    BinaryTreeNode<typeName>* clone(BinaryTreeNode<typeName>*) const;

};


#endif