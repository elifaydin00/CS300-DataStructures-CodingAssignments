#include <iostream>
#include "BinaryTree.h"
#include <string>
using namespace std;


template <class typeName>
BinaryTreeNode<typeName>::BinaryTreeNode(const typeName& _elmnt, BinaryTreeNode<typeName>* _lc, BinaryTreeNode<typeName>* _rc): data(_elmnt), left(_lc), right(_rc){}

//constructor
template <class typeName>
BinaryTree<typeName>::BinaryTree(): root(nullptr){}

//cc
template <class typeName>
BinaryTree<typeName>::BinaryTree(const BinaryTree<typeName>& rhs): root(nullptr)
{
	*this = rhs;
}
//destructor
template <class typeName>
BinaryTree<typeName>::~BinaryTree()
{
	this->makeEmpty();
}

template <class typeName>
bool BinaryTree<typeName>::isEmpty()
{
	return (root == nullptr);
}

//return element at a specific node (be careful, you should do a ifExists() check on main, otherwise you will get error
template <class typeName>
typeName& BinaryTree<typeName>::elementAt(BinaryTreeNode<typeName>* ptr)
{
		return ptr->data;	
}

template <class typeName>
typeName& BinaryTree<typeName>::findMin()
{
	return this->elementAt(this->findMin(root));
}

template <class typeName>
BinaryTreeNode<typeName>* BinaryTree<typeName>::findMin(BinaryTreeNode<typeName>* ptr)
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
typeName& BinaryTree<typeName>::findMax()
{
	return this->elementAt(this->findMax(root));
}

template <class typeName>
BinaryTreeNode<typeName>* BinaryTree<typeName>::findMax(BinaryTreeNode<typeName>* ptr)
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
typeName& BinaryTree<typeName>::find(const typeName& searched)
{
	if (doesExists(searched)) {
		return this->elementAt(this->find(searched, root));
	}
}

template <class typeName>
BinaryTreeNode<typeName>* BinaryTree<typeName>::find(const typeName& searched, BinaryTreeNode<typeName>* ptr)
{
	if (ptr == nullptr)
	{
		return nullptr; //not found
	}
	else if (searched < ptr->data)
	{
		return this->find(searched, ptr->left);
	}	
	else if (searched > ptr->data)
	{
		return this->find(searched, ptr->right);
	}
	else
	{
		return ptr; //matched
	}
	/*while (ptr != nullptr) {
		// pass right subtree as new tree
		if (searched > ptr->data)
			ptr = ptr->right;

		// pass left subtree as new tree
		else if (searched < ptr->data)
			ptr = ptr->left;
		else //matched
			return ptr;
	}*/
}

template <class typeName>
bool BinaryTree<typeName>::doesExists(const typeName& searched) {
	if (this->find(searched, root) == nullptr) {
		return false; //does not exists
	}
	return true; //does exists
	/*BinaryTreeNode<typeName>* ptr = root;
	while (ptr != nullptr) {
		// pass right subtree as new tree
		if (searched > ptr->data)
			ptr = ptr->right;

		// pass left subtree as new tree
		else if (searched < ptr->data)
			ptr = ptr->left;
		else //matched
			return true;
	}
	return false;*/ //does not exists
}

template <class typeName>
void BinaryTree<typeName>::insert(const typeName& inserted)
{
	this->insert(inserted, root);
	
}

template <class typeName>
void BinaryTree<typeName>::insert(const typeName& inserted, BinaryTreeNode<typeName>*& ptr)
{
	
	BinaryTreeNode<typeName>* newNode;
	newNode= new BinaryTreeNode<typeName>(inserted, nullptr, nullptr);
	BinaryTreeNode<typeName>* nodePtr;

	if (!root)	// Is the tree empty
		root = newNode;
	else
	{
		//we have a root, need to find the location for the next item
		nodePtr = root;
		while (nodePtr) {
			if (inserted < nodePtr->data) {
				if (nodePtr->left)
					nodePtr = nodePtr->left;
				else {
					nodePtr->left = newNode;
					break;
				}
			}
			else if (inserted > nodePtr->data) {
				if (nodePtr->right)
					nodePtr = nodePtr->right;
				else {
					nodePtr->right = newNode;
					break;
				}
			}
			else { //num is not greater or smaller. So it is equal, duplicate
				break;
			}
		}
	}

}

template <class typeName>
void BinaryTree<typeName>::remove(const typeName& removedObj)
{
	this->remove(removedObj, root);
}

template <class typeName>
void BinaryTree<typeName>::remove(const typeName& removed, BinaryTreeNode<typeName>*& ptr)
{
	if (ptr == nullptr) //not found
	{
		return;
	}
	if (removed < ptr->data) //on left side
	{
		this->remove(removed, ptr->left);
	}	
	else if (removed > ptr->data) //on right side
	{
		this->remove(removed, ptr->right);
	}
	//matched
	else if (ptr->left != nullptr && ptr->right != nullptr) //has 2 child
    {
        ptr->data = findMin(ptr->right)->data; //find min of right side and make it equal to 
        remove(ptr->data, ptr->right); //get rid of other one
    }
    else //single or no child, simply connect
    {
        BinaryTreeNode<typeName> *oldNode = ptr;
        ptr = (ptr->left != nullptr) ? ptr->left : ptr->right;
        delete oldNode;
    }
}

template <class typeName>
void BinaryTree<typeName>::makeEmpty()
{
	this->makeEmpty(root);
}

template <class typeName>
void BinaryTree<typeName>::makeEmpty(BinaryTreeNode<typeName>*& ptr)
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
void BinaryTree<typeName>::printTree()
{
    if (!this->isEmpty())
	{
		this->printTree(root);
	}
    
}

//inOrder traversal
template <class typeName>
void BinaryTree<typeName>::printTree(BinaryTreeNode<typeName>* ptr)
{
	if (ptr != nullptr)
    {
        this->printTree(ptr->left);
		cout << ptr->data << endl;
		this->printTree(ptr->right);
    }
}
//assignment operator overloading
template <class typeName>
BinaryTree<typeName>& BinaryTree<typeName>::operator=(const BinaryTree<typeName>& rhs)
{
	this->makeEmpty(); //
	root = rhs.clone(rhs.root);
	return *this;
}

template<class typeName>
BinaryTreeNode<typeName>* BinaryTree<typeName>::returnRoot()
{
	return root;
}



template <class typeName>
BinaryTreeNode<typeName>* BinaryTree<typeName>::clone(BinaryTreeNode<typeName>* ptr) const
{
	if (ptr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return new BinaryTreeNode<typeName>(ptr->data, this->clone(ptr->left), this->clone(ptr->right)); //clone 
	}
}