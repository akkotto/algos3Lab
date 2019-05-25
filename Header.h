#pragma once
#include <iostream>
#include <stdexcept>

using namespace std;

template<class T, typename T1>
class Tree
{
	class Node
	{
	public:
		Node* parent; //parent node
		Node* left, *right; //left and right child node
		int color; //color red = 1, black = 0
		T data; //data
		T1 key; //key
		Node(T data_1, T1 key_1, Node* par_1, int color_1 = 1) : parent(par_1), left(nullptr), right(nullptr), color(color_1), data(data_1), key(key_1) {}
	};
	Node* root; //root
	void RotateLeft(Node* node); //left rotation
	void RotateRight(Node* node); //right rotation
	void balanceAfterInsert(Node* new_elem); //balancing after inserting a new item 
	bool isRightChild(Node* node); //is a right child?
	void GoToMaxLeft(Node* node); //looking for the maximum node from the left subtree
	void balanceAfterDelete(Node* son, Node* uncle); 
	void DeleteRed(Node* node);
	void DeleteBlack(Node* node); 
public:
	Tree() : root(nullptr) {}
	~Tree() { clear(root); }
	void insert(T data, T1 key); //inserting node
	void clear(Node* ptr); //recursive delete map
	void get_keys(Node* ptr); //returns a list of keys
	void get_values(Node* ptr); //returns a list of data
	void remove(T1 key_n); //remove node by the key
	Node* find(T1 key_n) //finds a node by key
	{
		Node* temp = root; //make new pointer on root
		if (temp != nullptr) { //if tree exists
							   //cycle for search temp with such key
			while ((temp->right != nullptr && key_n > temp->key) || (temp->left != nullptr && key_n < temp->key)) {
				key_n > temp->key ? temp = temp->right : temp = temp->left;
			}
			if (temp->key == key_n) //if the keys match
				return temp; //return pointer to node
		}
		return nullptr; //if the tree doesn't exist or the node with such a key isn't found, returns nullptr
	}

	Node* GetRoot() { //method for getting the root of a tree
		return(root); //returns the pointer to the root
	}
};

template<class T, typename T1>
bool Tree<T, T1>::isRightChild(Node* node) { 
	return(node->parent->right == node); 
}

template<class T, typename T1>
void Tree<T, T1>::RotateLeft(Node* node) //left rotation
{
	Node* node_right = node->right; //writes the right child of node
	bool ifRoot = (node == root ? true : false); //is the node root?
	if (!ifRoot) { //if the node isn't root
		if (isRightChild(node)) //if the node is right, then
			node->parent->right = node_right; //the right son of the node becomes the son of the father of the node
		else
			node->parent->left = node_right; //the right son of the node becomes the son of the father of the node
	}
	node_right->parent = node->parent; //do the parent node of his son his father
	node->right = node_right->left; //do the right son of the node left son of his son
	if (node_right->left != nullptr) //if the node grand son isn't a nullptr
		node_right->left->parent = node; //create a connection between node and node_right left son
	node_right->left = node; //create a connection between node and node_right
	node->parent = node_right; //node_right is the node parent now
	if (ifRoot) //if node is root
		root = node_right; //make new root node_right
}

template<class T, typename T1>
void Tree<T, T1>::RotateRight(Node* node) //right rotation 
{
	Node* node_left = node->left; //writes the left child of node
	bool ifRoot = (node == root ? true : false); //is the node root?
	if (!ifRoot) { //if the node isn't root
		if (isRightChild(node)) //if the node is right, then
			node->parent->right = node_left;  //the left son of the node becomes the son of the father of the node
		else
			node->parent->left = node_left; //the left son of the node becomes the son of the father of the node
	}
	node_left->parent = node->parent; //do the parent node of his son his father
	node->left = node_left->right; //do the left son of the node right son of his son
	if (node_left->right != nullptr) //if the node grand son isn't a nullptr
		node_left->right->parent = node; //create a connection between node and node_left right son
	node_left->right = node; //create a connection between node and node_left
	node->parent = node_left; //node_left is the node parent now
	if (ifRoot) //if node is root
		root = node_left; //make new root node_left
}

template<class T, typename T1>
void Tree<T, T1>::balanceAfterInsert(Node* new_elem) //balancing after inserting a new item
{
	Node* father = new_elem->parent;
	Node* gr_father = father->parent;
	if (isRightChild(father)) { //if the new node father is the right son
		if (gr_father->left != nullptr && gr_father->left->color == 1) { //if the uncle of new node exists and his color is red
			father->color = 0; //recolor father in black
			gr_father->left->color = 0; //recolor uncle in black
			if (gr_father != root) { //grand father isn't a root
				gr_father->color =1; //recolor grand father in red
				if (gr_father->parent->color == 1) //if uncle color is red
					balanceAfterInsert(gr_father); //call balancing for grandfather, grand-grand father and grand-grand-grand father
			}
		}
		else { //if uncle is nullptr or uncle color is black
			if (isRightChild(new_elem)) { //if the new element is the right son
				RotateLeft(gr_father); //call left rotation of grand father
				father->color = 0; //recolor father in black
				gr_father->color =1; //recolor grandfather in red	
			}
			else { //if the new element is the left son
				RotateRight(father); //call right rotation of father
				balanceAfterInsert(father);
			}
		}
	}
	else { //if the new node father is the left son
		if (gr_father->right != nullptr && gr_father->right->color == 1) { //if the uncle of new node exists and his color is red
			father->color = 0; //recolor father in black
			gr_father->right->color = 0; //recolor uncle in black
			if (gr_father != root) { //grand father isn't a root
				gr_father->color =1; //recolor grand father in red
				if (gr_father->parent->color == 1) //if uncle color is red
					balanceAfterInsert(gr_father); //call balancing for grandfather, grand-grand father and grand-grand-grand father
			}
		}
		else { //if uncle is nullptr or uncle color is black
			if (isRightChild(new_elem)) { //if the new element is the right son
				RotateLeft(father); //call left rotation of father
				balanceAfterInsert(father); //call balancing for father, new element and grandfather
			}
			else { //if the new element is the left son
				RotateRight(gr_father); //call the right rotation of the grandfather
				father->color = 0; //recolor father in black
				gr_father->color =1; //recolor grandfather in red
			}
		}
	}
}

template<class T, typename T1>
void Tree<T, T1>::insert(T data_n, T1 key_n) //adding new item
{
	Node* temp = root, *new_node = nullptr; //make a new pointer to root and null pointer new_node
	if (temp == nullptr) //if tree is empty
		root = new Node(data_n, key_n, nullptr, 0);
	else {
		while ((temp->right != nullptr && key_n >= temp->key) || (temp->left != nullptr && key_n < temp->key)) { //find position of new elem
			key_n < temp->key ? temp = temp->left : temp = temp->right;
		}
		if (key_n > temp->key) {
			temp->right = new Node(data_n, key_n, temp); 
			new_node = temp->right;
		}
		else if (key_n < temp->key) {
			temp->left = new Node(data_n, key_n, temp); 
			new_node = temp->left;
		}
		else
			temp->data = data_n;
		if (temp->color != 0) //new color is red
			balanceAfterInsert(new_node);
	}
}

template<class T, typename T1>
void Tree<T, T1>::clear(Node* ptr)
{
	if (ptr != nullptr) { 
		Node* leftptr = ptr->left; 
		Node* rightptr = ptr->right; 
		delete ptr;
		clear(leftptr);
		clear(rightptr); 
	}
}

template<class T, typename T1>
void Tree<T, T1>::get_keys(Node* ptr) //returns a list of keys
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		cout << " " << ptr->key << " "; //key output for node
		get_keys(ptr->left); //recursive call for the left branch
		get_keys(ptr->right); //recursive call for the right branch
	}
}

template<class T, typename T1>
void Tree<T, T1>::get_values(Node* ptr) //returns a list of data
{
	if (ptr != nullptr) { //if the node exists
		Node* leftptr = ptr->left; //transition to the left branch of a tree
		Node* rightptr = ptr->right; //transition to the right branch of a tree
		cout << " " << ptr->data << " "; //data output for node
		get_values(ptr->left); //recursive call for the left branch
		get_values(ptr->right); //recursive call for the right branch
	}
}

template<class T, typename T1>
void Tree<T, T1>::GoToMaxLeft(Node* node) //looking for the maximum node from the left subtree (for remove)
{
	Node* temp = node->left; //transition to the left branch of a tree
	while (temp->right != nullptr) { //go to the right branch until you reach the leaf
		temp = temp->right; //transition to the right branch of a tree
	}
	node->data = temp->data; //rewriting the data to the max node from the left subtree
	node->key = temp->key; //rewriting the key to the max node from the left subtree
	if (temp->color == 1) //if max left node color is red
		DeleteRed(temp); //go to method to remove red node
	else
		DeleteBlack(temp); //go to method to remove black node
}

template<class T, typename T1>
void Tree<T, T1>::DeleteRed(Node* temp) //delete red node
{
	if (isRightChild(temp)) //division into cases for left and right nodes
	{
		if (temp->right == nullptr && temp->left == nullptr) { //both sons of node are leaves
			temp->parent->right = nullptr; //sever father and son
			delete temp; //delete node
		}
		else if (temp->right != nullptr && temp->left != nullptr) //the node has two sons
			GoToMaxLeft(temp); //select the max node from the left subtree for replacement
		else
		{
			if (temp->right == nullptr) { //the case when the node has one left son
				temp->parent->right = temp->left; //make grandson son
				temp->left->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
			else {                                 //the node has one right son
				temp->parent->right = temp->right; //make grandson son
				temp->right->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
		}
	}
	else {                                                     //temp is left son
		if (temp->right == nullptr && temp->left == nullptr) { //both sons of node are leaves
			temp->parent->left = nullptr; //sever father and son
			delete temp; //delete node
		}
		else if (temp->right != nullptr && temp->left != nullptr) //the node has two sons
			GoToMaxLeft(temp); //select the max node from the left subtree for replacement
		else
		{
			if (temp->right == nullptr) { //the case when the node has one left son
				temp->parent->left = temp->left; //make grandson son
				temp->left->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
			else {                                //the node has one right son
				temp->parent->left = temp->right; //make grandson son
				temp->right->parent = temp->parent; //make new father for son of the temp
				delete temp; //delete node
			}
		}
	}
}

template<class T, typename T1>
void Tree<T, T1>::DeleteBlack(Node* temp) //delete black node
{
	if (temp->right == nullptr && temp->left == nullptr) //both sons of node are leaves
	{
		if (temp == root) { //if temp if root
			delete root; //detele node
			root = nullptr; //make root nullptr
		}
		else {
			balanceAfterDelete(temp, isRightChild(temp) ? temp->parent->left : temp->parent->right); //call fixup for the node, his father and his brother
			isRightChild(temp) ? temp->parent->right = nullptr : temp->parent->left = nullptr; //node right/left break his relationship with his father
			delete temp; //delete node
		}
	}
	else if (temp->right == nullptr) // the case when the node has one left son
	{
		if (temp->left->color == 1) //if left son color is red then
		{
			if (temp == root) { //is temp a root?
				root = temp->left; //make a root left son of the node
				root->color = 0; //make a root color black
				root->parent = nullptr; //make a father of root nullptr
				delete temp; //delete node
			}
			else //if temp is not root then
			{
				if (isRightChild(temp)) { //if node is the right son
					temp->left->parent = temp->parent; //make grandson son
					temp->parent->right = temp->left; //break the bond of father and son
					temp->left->color = 0; //make his son color black
					delete temp; //delete node
				}
				else {                                 //if node is the left son
					temp->left->parent = temp->parent; //make grandson son
					temp->parent->left = temp->left; //break the bond of father and son
					temp->left->color = 0; //make his son color black
					delete temp; //delete node
				}
			}
		}
		else //if the left son color is black
		{
			if (isRightChild(temp)) { //if node is the right son
				temp->parent->right = temp->left; //break the bond of father and son
				temp->left->parent = temp->parent; //make grandson son
				balanceAfterDelete(temp->left, temp->parent->right); //call balanceAfterDelete for the temp left son, temp father and temp brother
			}
			else {                               //if node is the left son
				temp->parent->left = temp->left; //break the bond of father and son
				temp->left->parent = temp->parent; //make grandson son
				balanceAfterDelete(temp->left, temp->parent->right); //call balanceAfterDelete for the temp left son, temp father and temp brother
			}
			delete temp; //delete node
		}
	}
	else if (temp->left == nullptr) //if temp has one right son
	{
		if (temp->right->color == 1) //if left son color is red
		{
			if (temp == root) { //is temp a root?
				root = temp->right; //make a root right son of the node
				root->color = 0; //make a root color black
				root->parent = nullptr; //make a father of root nullptr
				delete temp; //delete node
			}
			else //if temp son color is black
			{
				if (isRightChild(temp)) { //if node is the right son
					temp->right->parent = temp->parent; //make grandson son
					temp->parent->right = temp->right; //break the bond of father and son
					temp->right->color = 0; //make his son color black
					delete temp; //delete node
				}
				else {
					temp->right->parent = temp->parent; //make grandson son
					temp->parent->left = temp->right; //break the bond of father and son
					temp->right->color = 0; //make his son color black
					delete temp; //delete node
				}
			}
		}
		else //if the node son color black
		{
			if (isRightChild(temp)) { //if the node is the right son
				temp->parent->right = temp->right; //break the bond of father and son
				temp->right->parent = temp->parent; //make grandson son
				balanceAfterDelete(temp->right, temp->parent->left); //call balanceAfterDelete for temp right son, temp father and temp brother
			}
			else { //if the node is the left son
				temp->parent->left = temp->right; //break the bond of father and son
				temp->right->parent = temp->parent; //make grandson son
				balanceAfterDelete(temp->right, temp->parent->left); //call balanceAfterDelete for temp right son, temp father and temp brother
			}
			delete temp; //delete node
			temp = nullptr; //temp pointer nullptr
		}
	}
	else //node has two sons
		GoToMaxLeft(temp); //select the max node from the left subtree for replacement
}

template<class T, typename T1>
void Tree<T, T1>::balanceAfterDelete(Node* node, Node* brother) //recursive method for fix up changes after node removing
{
	Node* nodePar = node->parent;
	//division into several cases depending on the color of the node father and node brother sons
	if (nodePar->color == 0 && (brother->left == nullptr || brother->left->color == 0) && (brother->right == nullptr || brother->right->color == 0))
	{
		brother->color =1; //recolor brother in red
		if (nodePar != root) //if node parent  isn't a root, call balanceAfterDelete for node father, node grandfather and node brother
			balanceAfterDelete(nodePar, isRightChild(nodePar) ? nodePar->parent->left : nodePar->parent->right);
	}
	else if (nodePar->color == 1 && (brother->left == nullptr || brother->left->color == 0) && (brother->right == nullptr || brother->right->color == 0))
	{
		nodePar->color = 0; //recolor father in black
		brother->color =1; //recolor brother in red
	}
	else if (!isRightChild(node) && brother->right != nullptr && brother->right->color == 1)
	{
		brother->right->color = 0; //recolor brother right son in black
		brother->color = nodePar->color; //recolor brother in father color
		nodePar->color = 0; //recolor father in black
		RotateLeft(nodePar); //call left rotation for father
	}
	else if (isRightChild(node) && brother->left != nullptr && brother->left->color == 1)
	{
		brother->left->color = 0; //recolor brother left son in black
		brother->color = nodePar->color; //recolor brother in father color
		nodePar->color = 0; //recolor father in black
		RotateRight(nodePar); //call right rotation for father
	}
	else if (!isRightChild(node) && brother->left != nullptr && brother->left->color == 1)
	{
		brother->left->color = nodePar->color; //recolor brother left son in father color
		nodePar->color = 0; //recolor father in black
		RotateRight(brother); //call right rotation for brother
		RotateLeft(nodePar); //call left rotation for father
	}
	else if (isRightChild(node) && brother->right != nullptr && brother->right->color == 1)
	{
		brother->right->color = nodePar->color; //recolor brother right son in father color
		nodePar->color = 0; //recolor father in black
		RotateLeft(brother); //call left rotation for brother
		RotateRight(nodePar); //call right rotation for father
	}
}

template <class T, typename T1>
void Tree<T, T1>::remove(T1 key_n) 
{
		Node* temp = find(key_n); 
		if (temp != nullptr) {
			if (temp->color == 1) 
				DeleteRed(temp); 
			else
				DeleteBlack(temp); 
		}
}
