#include <iostream>
#include "rbtree.h"
#include <cstring>
#include <fstream>

#if 0
Justin Iness
5/6/2022
C++ Programming
Red Black Tree
#endif

#define RED true
#define BLACK false

using namespace std;
//declare
void balance(node* n, node* &root);
void print(node* r, int level);

void add(int num, node* &root, node* &r, node* last) { // add numbers
	if (r == NULL) { // insert here
		r = new node(num);
		r->rob = RED;
		r->parent = last;
		balance(r, root);// balance added node
	}
	else if (num < r->data) { // left
		add(num, root, r->left, r);
		return;
	}
	else if (num > r->data) { // right
		add(num, root, r->right, r);
		return;
	}
}
void balance (node* n, node* &root) { // balance on node
	// case2 defaults
	// get uncle (case 3)
	bool hasUncle = false;
	node* uncle = NULL;
	if (n->parent != NULL && n->parent->parent != NULL) { // if uncle	
		hasUncle = true;
		if (n->parent->parent->left != NULL && n->parent->parent->right != NULL) { //not null uncle
			if (n->parent == n->parent->parent->left) { // right of gp is uncle
				uncle = n->parent->parent->right;
			}
			else if (n->parent == n->parent->parent->right) { // left of gp is uncle
				uncle = n->parent->parent->left;
			}
			if (uncle->rob == RED && n->parent->rob == RED) { // case 3
				n->parent->rob = BLACK;
				uncle->rob = BLACK;
				n->parent->parent->rob = RED;
				balance(n->parent->parent, root);
				return;
			}
		}
	}
	if ((hasUncle && uncle == NULL) || (hasUncle && uncle->rob == BLACK)) { //(case 4 and 5)
		if (n->parent == n->parent->parent->left && n == n->parent->right && n->parent->rob == RED) { // case 4 rotate left
			node* temp = n->parent;
			n->parent->parent->left = n;
			temp->right = n->left;
			n->left = temp;
			//fix parents
			n->parent = n->parent->parent;
			n->left->parent = n;
			if (n->left->right != NULL) {
				n->left->right->parent = n->left;
			}
			balance(n->left, root); // case5 parent
			return;
		}
		else if (n->parent == n->parent->parent->right && n == n->parent->left && n->parent->rob == RED) { // case 4 rotate right
			node* temp = n->parent;
			n->parent->parent->right = n;
			temp->left = n->right;
			n->right = temp;
			//fix parents
			n->parent = n->parent->parent;
			n->right->parent = n;
			if (n->right->left != NULL) {
				n->right->left->parent = n->right;
			}
			balance(n->right, root); //case 5 parent
			return;
		}
		if (n->parent == n->parent->parent->left && n == n->parent->left && n->parent->rob == RED) { // case 5 rotate left
			node* temp = n->parent->right;
			n->parent->right = n->parent->parent;
			if (n->parent->parent->parent != NULL) {
				if (n->parent->parent->parent->left == n->parent->parent) { // fix 3 long tree 3, 1, 2 break
					n->parent->parent->parent->left = n->parent;
				}
				else if (n->parent->parent->parent->right == n->parent->parent) {
					n->parent->parent->parent->right = n->parent;
				}
			}
			else {
				root = n->parent;
			}
			n->parent->right->left = temp;
			//fix parents
			if (temp != NULL) {
				temp->parent = n->parent->right;
			}
			if (root != n->parent) {
				n->parent->parent = n->parent->right->parent;
			}
			else {
				n->parent->parent = NULL;
			}
			n->parent->right->parent = n->parent;
			//swap colors
			n->parent->rob = !(n->parent->rob);
			n->parent->right->rob = !(n->parent->right->rob);
		}
		else if (n->parent == n->parent->parent->right && n == n->parent->right && n->parent->rob == RED) { // case 5 rotate right
			node* temp = n->parent->left;
			n->parent->left = n->parent->parent;
			if (n->parent->parent->parent != NULL) {
				if (n->parent->parent->parent->left == n->parent->parent) {
					n->parent->parent->parent->left = n->parent;
				}
				else if (n->parent->parent->parent->right == n->parent->parent) {
					n->parent->parent->parent->right = n->parent;
				}
			}
			else {
				root = n->parent;
			}
			n->parent->left->right = temp;
			//fix parents
			if (temp != NULL) {
				temp->parent = n->parent->left;
			}
			if (root != n->parent) {
				n->parent->parent = n->parent->left->parent;
			}
			else {
				n->parent->parent = NULL;
			}
			n->parent->left->parent = n->parent;
			//swap colors
			n->parent->rob = !(n->parent->rob);
			n->parent->left->rob = !(n->parent->left->rob);
		}
	}
	
	// climb tree(case1)
	node* p = n;
	while (p->parent != NULL) {
		p = p->parent;
	}
	if (p->rob == RED && p->parent == NULL) { // case1
		p->rob = BLACK;
	}
	return;
}
void print(node* r, int level) { // print tree visually
	// emptee tree
	if (r == NULL) {
		return;
	}

	// increase spaces
	level += 1;

	// right 
	print(r->right, level);

	// current data and space
	for (int i = 1; i < level; i++) {
		cout << "   ";
	}
	if (r->rob == RED) {
		cout << 'r';
	}
	else {
		cout << 'b';
	}
	cout << r->data << endl;

	// left
	print(r->left, level);
}

int main() {
	//create new root bst
	node* root = NULL;
	char input[5000];
	while (true) {
		cout << "Enter a command (ADD), (READ), (PRINT) or (QUIT): " << endl;
		cin >> input;
		if (strcmp(input, "ADD") == 0) { // Add number
			cout << "Enter a number to add: " << endl;
			int number = 0;
			cin >> number;
			if (number != 0) {
				add(number, root, root, NULL);
			}
		}
		else if (strcmp(input, "READ") == 0) { // Read in numbers
			fstream iFile;
			cout << "What is the file's name?" << endl;
			cin >> input;
			// open file and read numbers from it
			iFile.open(input, ios::in);
			if (!iFile) {
				cout << "Can't find " << input << ".  Try including .txt?" << endl;
				return 0;
			}
			else {
				cout << "opened file" << endl;
				int number = 0;
				while (iFile >> number) {
					if (number != 0) {
						add(number, root, root, NULL); //add number
					}
				}
			}
		}
		else if (strcmp(input, "PRINT") == 0) { // Print number
			print(root, 0);
		}
		else if (strcmp(input, "QUIT") == 0) { // Quit program
			return 0;
		}
	}
}
