#include <iostream>
#include "rbtree.h"
#include <cstring>
#include <fstream>

#if 0
Justin Iness
4/20/2022
C++ Programming
Binary Search Tree
#endif

#define RED true
#define BLACK false

using namespace std;
void balance(node* &n, node* &root);

void add(int num, node* &root, node* &r, node* last) { // add numbers
	if (r == NULL) {
		r = new node(num);
		r->rob = RED;
		r->parent = last;
		if (r->parent != NULL) {
			cout << r->parent->data << endl;
		}
		balance(r, root);
	}
	else if (num < r->data) {
		add(num, root, r->left, r);
		return;
	}
	else if (num > r->data) {
		add(num, root, r->right, r);
		return;
	}
}
void balance (node* &n, node* &root) {
	cout << "balancing" << endl;
	// case2 defaults
	// get uncle (case 3)
	node* uncle = NULL;
	if (n->parent != NULL && n->parent->parent != NULL && n->parent->parent->left != NULL && n->parent->parent->right != NULL) { // if has uncle	
		cout << "has not null uncle" << endl;
		if (n->parent == n->parent->parent->left) { // right of gp is uncle
			uncle = n->parent->parent->right;
		}
		else if (n->parent == n->parent->parent->right) { // left of gp is uncle
			uncle = n->parent->parent->left;
		}
		if (uncle->rob == RED && n->parent->rob == RED) { // case 3
			cout << "push blackness" << endl;
			n->parent->rob = BLACK;
			uncle->rob = BLACK;
			n->parent->parent->rob = RED;
			balance(n->parent->parent, root);
			return;
		}
	}
	if (uncle == NULL || uncle->rob == BLACK) { //(case 4 and 5)
		cout << n->data <<  " uncle black" << endl;
		if (n->parent == n->parent->parent->left && n == n->parent->right && n->parent->rob == RED) { // case 4 rotate left
			node* temp = n->parent;
			n->parent->parent->left = n;
			temp->right = n->left;
			n->left = temp;
			//fix parents
			n->parent = n->parent->parent;
			n->left->parent = n;
			n->left->right->parent = n->left;
			balance(n->left, root); // case5 parent
		}
		else if (n->parent == n->parent->parent->right && n == n->parent->left && n->parent->rob == RED) { // case 4 rotate right
			node* temp = n->parent;
			n->parent->parent->right = n;
			temp->left = n->right;
			n->right = temp;
			//fix parents
			n->parent = n->parent->parent;
			n->right->parent = n;
			n->right->left->parent = n->right;
			balance(n->right, root); //case 5 parent
		}
		if (n->parent == n->parent->parent->left && n == n->parent->left && n->parent->rob == RED) { // case 5 rotate left
			node* temp = n->parent->right;
			n->parent->right = n->parent->parent;
			if (n->parent->parent->parent->left == n->parent->parent) {
				n->parent->parent->parent->left = n->parent;
			}
			else if (n->parent->parent->parent->right == n->parent->parent) {
				n->parent->parent->parent->right = n->parent;
			}
			n->parent->right->left = temp;
			//fix parents
			n->parent->parent = n->parent->parent->parent;
			n->parent->right->parent = n->parent;
			n->parent->right->left->parent = n->parent->right;
			//swap colors
			n->parent->rob = !(n->parent->rob);
			n->parent->right->rob = !(n->parent->right->rob);
		}
		else if (n->parent == n->parent->parent->right && n == n->parent->right && n->parent->rob == RED) { // case 5 rotate right
			node* temp = n->parent->left;
			n->parent->left = n->parent->parent;
			if (n->parent->parent->parent->left == n->parent->parent) {
				n->parent->parent->parent->left = n->parent;
			}
			else if (n->parent->parent->parent->right == n->parent->parent) {
				n->parent->parent->parent->right = n->parent;
			}
			n->parent->left->right = temp;
			//fix parents
			n->parent->parent = n->parent->parent->parent;
			n->parent->left->parent = n->parent;
			n->parent->left->right->parent = n->parent->left;
			//swap colors
			n->parent->rob = !(n->parent->rob);
			n->parent->left->rob = !(n->parent->left->rob);
		}
	}
	
	// climb tree(case1)
	cout << "climbing tree" << endl;
	node* p = n;
	while (p->parent != NULL) {
		p = p->parent;
		//check double reds and black pushes
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
