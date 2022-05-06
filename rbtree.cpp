#include <iostream>
#include <cstring>
#include <math.h>
#include "rbtree.h"

#if 0
Justin Iness
5/6/2022
C++ Programming
Red Black Tree
#endif

using namespace std;

node::node(int number) { // parametized constructor
	data = number; // set values
	left = NULL;
	right = NULL;
	parent = NULL;
}
node::~node() { // destructor

}
