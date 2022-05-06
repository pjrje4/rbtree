#if 0
Justin Iness
5/6/2022
C++ Programming
Red Black Tree
#endif

class node {
private:
public:
	node(int number); // Constructor
	~node(); // Destructor
	// stored data
	node* left;
	node* right;
	node* parent;
	int data;
	bool rob;
};

