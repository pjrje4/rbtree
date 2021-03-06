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
void delbal(node* r, node* &root);
void checkparents(node* r);

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
void search(int num, node* r) {
	if (r == NULL) { // not in tree
		cout << num << " not in tree." << endl;
		return;
	}
	else if (r->data == num) { // in tree
		cout << num << " is in tree." << endl;
		return;
	}
	else if (num < r->data) { // search left
		search(num, r->left);
	}
	else if (num > r->data) { // search right
		search(num, r->right);
	}
}
void del(int num, node* &r, node* &root) {
	if (r == NULL) { // not in tree
                cout << num << " not in tree." << endl;
                return;
        }
        else if (r->data == num) { // in tree
		//checkparents(root);
		cout << num << " is in tree. Deleting" << endl;
		if (r->left == NULL || r->right == NULL) { // 1 long or 1 child lists
			bool m = RED;
			bool c = RED;
			if (r->left == NULL && r->right == NULL) { // 1 long list
				m = r->rob;
				c = BLACK;
				r->data = 0;
			}
			else { // one child lists
				if (r->left != NULL) { // left child
					m = r->rob;
					c = r->left->rob;
					
					r->left->parent = r->parent;
					r = r->left;
				}
				else if (r->right != NULL) { // right child
					m = r->rob;
					c = r->right->rob;

					r->right->parent = r->parent;
					r = r->right;
				}
			}
			if (m == RED && c == BLACK) {
			}
			else if (m == BLACK && c == RED) {
				r->rob = BLACK;
			}
			else if (m == BLACK && c == BLACK) {
				delbal(r, root);
			}
			if (r->data == 0) {
				r = NULL;
			}
		}
		else { // list with 2 children
			 node* prevInList = r->left;
			 while (prevInList->right != NULL) {
				 prevInList = prevInList->right;
			 }
			 r->data = prevInList->data;
			 del(prevInList->data, r->left, root);
		}
		//checkparents(root);
                return;
        }
        else if (num < r->data) { // search left
                del(num, r->left, root);
        }
        else if (num > r->data) { // search right
                del(num, r->right, root);
        }
}
void delbal(node* r, node* &root) {
	if (r->parent != NULL) { // case 2, 3
		if (r->parent->left == r) { // case 2, 3 left
			if (r->parent->right->rob == RED) { // case 2 left
				cout << "Case 2 left" << endl;
                                node* temp = r->parent->right->left;
				node* sibling = r->parent->right;
                                r->parent->right->left = r->parent;
                                r->parent->right = temp;

                                if (r->parent->parent != NULL) { // grandparents (DONT CHANGE)
                                        if (r->parent->parent->left == r->parent) {
                                                r->parent->parent->left = sibling;
                                                r->parent->parent->left->parent = r->parent->parent;

                                        }
                                        else if (r->parent->parent->right == r->parent) {
                                                r->parent->parent->right = sibling;
                                                r->parent->parent->right->parent = r->parent->parent;


                                        }
                                }
                                else {
                                        root = sibling;
					sibling->parent = NULL;
                                }

                                //fix parents
				if (temp != NULL) {
                                	r->parent->right->parent = r->parent;
				}
                                r->parent->parent = sibling;
           			//swap P and S
				r->parent->rob = !(r->parent->rob);
				r->parent->parent->rob = !(r->parent->parent->rob);
			}
			if (r->parent->rob == BLACK && r->parent->right->rob == BLACK && (r->parent->right->left == NULL || r->parent->right->left->rob == BLACK) && (r->parent->right->right == NULL || r->parent->right->right->rob == BLACK)) { // case 3 right
				cout << "Case 3 left on "<< r->data << endl;
				r->parent->right->rob = RED;
				delbal(r->parent, root);
			}
			//case 4
			if (r->parent->rob == RED && r->parent->right->rob == BLACK && (r->parent->right->left == NULL || r->parent->right->left->rob == BLACK) && (r->parent->right->right == NULL || r->parent->right->right->rob == BLACK)) {
				cout << "Case 4 left" << endl;
				r->parent->rob = BLACK;
				r->parent->right->rob = RED;
				return;
			}
			//case 5
			if (r->parent->right->rob == BLACK && (r->parent->right->right == NULL || r->parent->right->right->rob == BLACK) && (r->parent->right->left != NULL && r->parent->right->left->rob == RED)) {
				cout << "Case 5 left" << endl;
				node* temp = r->parent->right->left->right;
				r->parent->right->left->right = r->parent->right;
				r->parent->right = r->parent->right->left;
				r->parent->right->right->left = temp;
				//fix parents
				if (temp != NULL) {
					r->parent->right->right->left->parent = r->parent->right->right;
				}
				r->parent->right->right->parent = r->parent->right;
				r->parent->right->parent = r->parent;
				//colors
				r->parent->right->rob = BLACK;
				r->parent->right->right->rob = RED;
			}
			//case 6
                        if (r->parent->right->rob == BLACK && (r->parent->right->right != NULL && r->parent->right->right->rob == RED)) {
				cout << "Case 6 left" << endl;
                                node* temp = r->parent->right->left;
				node* sibling = r->parent->right;
                                r->parent->right->left = r->parent;
                                r->parent->right = temp;

                                if (r->parent->parent != NULL) { // grandparents (DONT CHANGE)
                                        if (r->parent->parent->left == r->parent) {
                                                r->parent->parent->left = sibling;
                                                r->parent->parent->left->parent = r->parent->parent;

                                        }
                                        else if (r->parent->parent->right == r->parent) {
                                                r->parent->parent->right = sibling;
                                                r->parent->parent->right->parent = r->parent->parent;


                                        }
                                }
                                else { // else change root
                                       	root = sibling;
					sibling->parent = NULL;
                                }
                                //fix parents
				if (temp != NULL) {
                                	r->parent->right->parent = r->parent;
				}
                                r->parent->parent = sibling;
                                //colors
                                r->parent->parent->rob = r->parent->rob;
                                r->parent->rob = BLACK;
				sibling->right->rob = BLACK;
                        }

		}
		else if (r->parent->right == r) { // right
			if (r->parent->left->rob == RED) { // right, sibling is red, case 2
				cout << "Case 2 right" << endl;
				node* temp = r->parent->left->right;
				node* sibling = r->parent->left;
				r->parent->left->right = r->parent;
				r->parent->left = temp;

				if (r->parent->parent != NULL) { // grandparents (DONT CHANGE)
                        	        if (r->parent->parent->left == r->parent) {
                                	        r->parent->parent->left = sibling;
						r->parent->parent->left->parent = r->parent->parent;

                              		}
                              		else if (r->parent->parent->right == r->parent) {
                              	        	r->parent->parent->right = sibling;
						r->parent->parent->right->parent = r->parent->parent;


                                	}
	                        }
        	                else { // else change root
                	                root = sibling;
					sibling->parent = NULL;
                       	 	}

				//fix parents
				if (temp != NULL) {
					r->parent->left->parent = r->parent;
				}
				r->parent->parent = sibling;
           			//swap P and S
				r->parent->rob = !(r->parent->rob);
				r->parent->parent->rob = !(r->parent->parent->rob);
			} // end case 2
			if (r->parent->rob == BLACK && r->parent->left->rob == BLACK && (r->parent->left->right == NULL || r->parent->left->right->rob == BLACK) && (r->parent->left->left == NULL || r->parent->left->left->rob == BLACK)) { // case 3 right
				cout << "Case 3 right on " << r->data << endl;
				
				r->parent->left->rob = RED;
				delbal(r->parent, root);
			}
			//case 4
			if (r->parent->rob == RED && r->parent->left->rob == BLACK && (r->parent->left->left == NULL || r->parent->left->left->rob == BLACK) && (r->parent->left->right == NULL || r->parent->left->right->rob == BLACK)) {
				cout << "Case 4 right" << endl;
				r->parent->rob = BLACK;
				r->parent->left->rob = RED;
				return;
			}
			//case 5
			if (r->parent->left->rob == BLACK && (r->parent->left->left == NULL || r->parent->left->left->rob == BLACK) && (r->parent->left->right != NULL && r->parent->left->right->rob == RED)) {
				cout << "Case 5 right" << endl;
				node* temp = r->parent->left->right->left;
				r->parent->left->right->left = r->parent->left;
				r->parent->left = r->parent->left->right;
				r->parent->left->left->right = temp;
				//fix parents
				if (temp != NULL) { 
					r->parent->left->left->right->parent = r->parent->left->left;
				}
				r->parent->left->left->parent = r->parent->left;
				r->parent->left->parent = r->parent;
				//colors
				r->parent->left->rob = BLACK;
				r->parent->left->left->rob = RED;
			}
			//case 6
			if (r->parent->left->rob == BLACK && (r->parent->left->left != NULL && r->parent->left->left->rob == RED)) {
				cout << "Case 6 right" << endl;
				node* temp = r->parent->left->right;
				node* sibling = r->parent->left;
                                r->parent->left->right = r->parent;
                                r->parent->left = temp;

                                if (r->parent->parent != NULL) { // grandparents (DONT CHANGE)
                                        if (r->parent->parent->left == r->parent) {
                                                r->parent->parent->left = sibling;
                                                r->parent->parent->left->parent = r->parent->parent;

                                        }
                                        else if (r->parent->parent->right == r->parent) {
                                                r->parent->parent->right = sibling;
                                                r->parent->parent->right->parent = r->parent->parent;


                                        }
                                }
                                else { // else change root
                                        root = sibling;
					sibling->parent = NULL;
                                }
				//fix parents
				if (temp != NULL) {
                                	r->parent->left->parent = r->parent;
				}
                                r->parent->parent = sibling;
				//colors
				r->parent->parent->rob = r->parent->rob;
				r->parent->rob = BLACK;
				sibling->left->rob = BLACK;
			}
		}
	}
}
void checkparents(node* r) { // output all parents and left right for debug
	cout << r->data;
	cout << "|";
	if (r->parent != NULL) {
		cout << "P: " << r->parent->data;
	}
	else {
		cout << "P: NULL";
	}
	cout << "|";
	if (r->left != NULL) {
		cout << "L: " << r->left->data;
	}
	else {
		cout << "L: NULL";
	}
	cout << "|";
	if (r->right != NULL) {
		cout << "R: " << r->right->data;
	}
	else {
		cout << "R: NULL";
	}
	cout << endl;
	if (r->left != NULL) {
		checkparents(r->left);
	}
	if (r->right != NULL) {
		checkparents(r->right);
	}
	return;
}

int main() {
	//create new root bst
	node* root = NULL;
	char input[5000];
	while (true) {
		cout << "Enter a command (ADD), (READ), (PRINT), (SEARCH), (DELETE), or (QUIT): " << endl;
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
		else if (strcmp(input, "SEARCH") == 0) { // Search number
                        cout << "Enter a number to search for: " << endl;
			int number = 0;
			cin >> number;
			if (number != 0) {
				search(number, root);
			}
                }	
		else if (strcmp(input, "DELETE") == 0) { // Delete number
			cout << "Enter a number to delete: " << endl;
                        int number = 0;
                        cin >> number;
                        if (number != 0) {
                                del(number, root, root);
                        }		
		}
		else if (strcmp(input, "QUIT") == 0) { // Quit program
			return 0;
		}
	}
}
