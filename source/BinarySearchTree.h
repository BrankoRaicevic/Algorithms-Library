#pragma once
#include <vector>

struct BSTNode {
	//BSTNode* parent;
	BSTNode* lchild;
	BSTNode* rchild;
	int data;
};

class BST {
	BSTNode* root;
	int nodeCount;
public:
	BST();
	BST(std::vector<int>&);
	BST(std::vector<int>&&);
	~BST();
	void insert(int);
	bool remove(int);
	BSTNode* find(int);
	void balance();
	void deleteTree();
	bool deleteSubtree(int);
	std::vector<BSTNode*> LevelOrder();
	std::vector<int> RPreorder();
	std::vector<int> RInorder();
	std::vector<int> RReverseInorder();
	std::vector<int> RPostorder();
	void RPreorderHelper(BSTNode*, std::vector<int>&, int&);
	void RInorderHelper(BSTNode*, std::vector<int>&, int&);
	void RReverseInorderHelper(BSTNode*, std::vector<int>&, int&);
	void RPostorderHelper(BSTNode*, std::vector<int>&, int&);
	std::vector<int> preOrder();
	std::vector<int> inOrder();
	std::vector<int> reverseInOrder();
	std::vector<int> postOrder();
	std::vector<int> postOrder2Stacks(); //simple but slower then default
	std::vector<int> postOrder2ndMethod(); //slower then default and 2stacks
	std::vector<int> threadedPreOrder();
	std::vector<int> threadedInOrder();
	std::vector<int> threadedReverseInOrder();
	std::vector<int> threadedPostOrder();
	std::vector<int> threadedPostOrder2ndMethod();


	int size();
protected:
	BSTNode* removeHelper(BSTNode*, int);
	BSTNode* minValue(BSTNode*);
	void threadedInOrderPointers(std::vector<BSTNode*>&);
	void balanceConstructorHelper(const std::vector<int>&, int, int);
	BSTNode* balanceHelper(std::vector<BSTNode*>&, int, int);
	BSTNode* deleteSubtreeFind(int);
	std::vector<int> deleteSubtreeThreadedInOrder();
};