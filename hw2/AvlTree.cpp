#include <iostream>
#include "AvlTree.h"
#include <string>
using namespace std;


//constructor
template <class typeName>
AvlTree<typeName>::AvlTree() : root(nullptr) {}

//cc
template <class typeName>
AvlTree<typeName>::AvlTree(const AvlTree<typeName>& rhs) : root(nullptr)
{
	*this = rhs;
}
//destructor
template <class typeName>
AvlTree<typeName>::~AvlTree()
{
	this->makeEmpty();
}

template <class typeName>
bool AvlTree<typeName>::isEmpty()
{
	return (root == nullptr);
}

//return element at a specific node (be careful, you should do a ifExists() check on main, otherwise you will get error
template <class typeName>
typeName& AvlTree<typeName>::elementAt(AvlTreeNode<typeName>* ptr)
{
	return ptr->element;
}

template <class typeName>
int AvlTree<typeName>::height(AvlTreeNode<typeName>* t) const //return height of node
{
	if (t == NULL)
		return -1;

	return t->height;
}


/**
		* Return maximum of lhs and rhs.
*/
template <class typeName>
int AvlTree<typeName>::max(int lhs, int rhs) const
{
	if (lhs > rhs)
		return lhs;

	return rhs;
}



template <class typeName>
typeName& AvlTree<typeName>::findMin()
{
	return this->elementAt(this->findMin(root));
}

template <class typeName>
AvlTreeNode<typeName>* AvlTree<typeName>::findMin(AvlTreeNode<typeName>* ptr)
{
	if (ptr == nullptr)
	{
		return nullptr;
	}
	if (ptr->left == nullptr)
	{
		return ptr;
	}
	return ptr;
}

template <class typeName>
typeName& AvlTree<typeName>::findMax()
{
	return this->elementAt(this->findMax(root));
}

template <class typeName>
AvlTreeNode<typeName>* AvlTree<typeName>::findMax(AvlTreeNode<typeName>* ptr)
{
	if (ptr == nullptr)
	{
		return nullptr;
	}
	if (ptr->right == nullptr)
	{
		return ptr;
	}
	return ptr;
}

//search a specific object
template <class typeName>
typeName& AvlTree<typeName>::find(const typeName& searched)
{
	if (doesExists(searched)) {
		return this->elementAt(this->find(searched, root));
	}
}

template <class typeName>
AvlTreeNode<typeName>* AvlTree<typeName>::find(const typeName& searched, AvlTreeNode<typeName>* ptr)
{
	if (ptr == nullptr)
	{
		return nullptr; //not found
	}
	else if (searched < ptr->element)
	{
		return this->find(searched, ptr->left);
	}
	else if (searched > ptr->element)
	{
		return this->find(searched, ptr->right);
	}
	else
	{
		return ptr; //matched
	}
}

template <class typeName>
bool AvlTree<typeName>::doesExists(const typeName& searched) {
	if (this->find(searched, root) == nullptr) {
		return false; //does not exists
	}
	return true; //does exists
}

template <class typeName>
void AvlTree<typeName>::insert(const typeName& inserted)
{
	this->insert(inserted, root);

}

template<class typeName>
void AvlTree<typeName>::balance(AvlTreeNode<typeName>*& t){
	if (t == nullptr) {
		return;
	}
	if (height(t->left) - height(t->right) > 1) {
		if (height(t->left->left) >= height(t->left->right)) {
			rotateWithLeftChild(t);
		}
		else {
			doubleWithLeftChild(t);
		}
	}
	else {
		if (height(t->right) - height(t->left) > 1) {
			if (height(t->right->right) >= height(t->right->left)) {
				rotateWithRightChild(t);
			}
			else {
				doubleWithRightChild(t);
			}
		}
	}
	t->height = max(height(t->left), height(t->right)) + 1;
}



template <class typeName>
void AvlTree<typeName>::rotateWithLeftChild(AvlTreeNode<typeName>*& k2) const {
	AvlTreeNode<typeName>* k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->left), k2->height) + 1;
	k2 = k1;
}

template <class typeName>
void AvlTree<typeName>::rotateWithRightChild (AvlTreeNode<typeName>*& k1) const {
	AvlTreeNode<typeName>* k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max(height(k1->left), height(k1->right)) + 1;
	k2->height = max(height(k2->right), k1->height) + 1;
	k1 = k2;
}

template <class typeName>
void AvlTree<typeName>::doubleWithLeftChild(AvlTreeNode<typeName>*& k3) const{
	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template <class typeName>
void AvlTree<typeName>::doubleWithRightChild (AvlTreeNode<typeName>*& k1) const
{
	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}

template <class typeName>
void AvlTree<typeName>::insert(const typeName& x, AvlTreeNode<typeName>*& t) {
	if (t == NULL)
		t = new AvlTreeNode<typeName>(x, NULL, NULL);

	else if (x < t->element) { // X should be inserted to the left tree!		
		insert(x, t->left);
		if (height(t->left) - height(t->right) == 2) { //left subtree grew in height!)
			if (x < t->left->element)  // X was inserted to the left-left subtree!
				rotateWithLeftChild(t);
			else	// X was inserted to the left-right subtree!
				doubleWithLeftChild(t);
		}
	}
	else if (t->element < x){ // Otherwise X is inserted to the right subtree   
		insert(x, t->right);
		if (height(t->right) - height(t->left) == 2) //// height of the right subtree increased
			if (t->right->element < x) // X was inserted to right-right subtree
				rotateWithRightChild(t);
			else // X was inserted to right-left subtree
				doubleWithRightChild(t);
	}
	else
		;  // Duplicate; do nothing

// update the height the node
	t->height = max(height(t->left), height(t->right)) + 1;
}

template <class typeName>
void AvlTree<typeName>::remove(const typeName& removedObj)
{
	this->remove(removedObj, root);
}

template <class typeName>
void AvlTree<typeName>::remove(const typeName& removed, AvlTreeNode<typeName>*& t)
{
	if (t == nullptr) //not found
	{
		return;
	}
	if (removed < t->element) //on left side
	{
		this->remove(removed, t->left);
	}
	else if (removed > t->element) //on right side
	{
		this->remove(removed, t->right);
	}
	//matched
	else if (t->left != nullptr && t->right != nullptr) //has 2 child
	{
		t->element = findMin(t->right)->element; //find min of right side and make it equal to 
		remove(t->element, t->right); //get rid of other one
	}
	else //single or no child, simply connect
	{
		AvlTreeNode<typeName>* oldNode = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete oldNode;
	}
	//balance
	balance(t); //recursively balance the path
	
}
	

template <class typeName>
void AvlTree<typeName>::makeEmpty()
{
	this->makeEmpty(root);
}

template <class typeName>
void AvlTree<typeName>::makeEmpty(AvlTreeNode<typeName> * &ptr)
{
	if (ptr != nullptr)
	{
		this->makeEmpty(ptr->left);
		this->makeEmpty(ptr->right);
		delete ptr;
	}
	ptr = nullptr;
}

template <class typeName>
void AvlTree<typeName>::printTree()
{
	if (!this->isEmpty())
	{
		this->printTree(root);
	}
}

//inOrder traversal
template <class typeName>
void AvlTree<typeName>::printTree(AvlTreeNode<typeName> * ptr)
{
	if (ptr != nullptr)
	{
		this->printTree(ptr->left);
		cout << ptr->element << endl;
		this->printTree(ptr->right);
	}
}
//assignment operator overloading
template <class typeName>
AvlTree<typeName>& AvlTree<typeName>::operator=(const AvlTree<typeName> & rhs)
{
	this->makeEmpty(); //
	root = rhs.clone(rhs.root);
	return *this;
}

template<class typeName>
AvlTreeNode<typeName>* AvlTree<typeName>::returnRoot()
{
	return root;
}

template <class typeName>
AvlTreeNode<typeName>* AvlTree<typeName>::clone(AvlTreeNode<typeName>* ptr) const {
	if (ptr == nullptr){
		return nullptr;
	}
	else{
		return new AvlTreeNode<typeName>(ptr->element, this->clone(ptr->left), this->clone(ptr->right),ptr->height); //clone 
	}
}


