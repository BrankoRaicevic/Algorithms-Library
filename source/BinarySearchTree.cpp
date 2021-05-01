#include "BinarySearchTree.h"
#include <stack>
#include <iostream>
#include <algorithm>

BST::BST() {
	root = nullptr;
	nodeCount = 0;
}

BST::BST(std::vector<int>& v) {
	root = nullptr;
	nodeCount = 0;
	std::sort(v.begin(), v.end());
	balanceConstructorHelper(v, 0, v.size() - 1);
}

BST::BST(std::vector<int> &&v) {
	root = nullptr;
	nodeCount = 0;
	std::sort(v.begin(), v.end());
	balanceConstructorHelper(v, 0, v.size() - 1);
}

BST::~BST() {
	deleteTree();
}

void BST::insert(int item) {
	if (root == nullptr) {
		root = new BSTNode();
		root->data = item;
		root->lchild = nullptr;
		root->rchild = nullptr;
		nodeCount++;
		return;
	}
	BSTNode* it = root;
	while (1) {
		if (item > it->data) {
			if (it->rchild != nullptr) {
				it = it->rchild;
			}
			else {
				it->rchild = new BSTNode();
				it = it->rchild;
				it->data = item;
				it->lchild = nullptr;
				it->rchild = nullptr;
				nodeCount++;
				return;
			}
		}
		else if (item < it->data) {
			if (it->lchild != nullptr) {
				it = it->lchild;
			}
			else {
				it->lchild = new BSTNode();
				it = it->lchild;
				it->data = item;
				it->lchild = nullptr;
				it->rchild = nullptr;
				nodeCount++;
				return;
			}
		}
		else return;
	}
}

bool BST::remove(int item) {
	if (find(item) != nullptr) {
		root = removeHelper(root, item);
		nodeCount--;
		return true;
	}
	return false;
}

BSTNode* BST::find(int item) {
	if (root == nullptr) {
		return nullptr;
	}
	BSTNode* it = root;
	while (1) {
		if (item > it->data) {
			if (it->rchild != nullptr) {
				it = it->rchild;
			}
			else {
				return nullptr;
			}
		}
		else if (item < it->data) {
			if (it->lchild != nullptr) {
				it = it->lchild;
			}
			else {
				return nullptr;
			}
		}
		else return it;
	}
}

void BST::balance() {
	std::vector<BSTNode*> v(nodeCount);
	threadedInOrderPointers(v);
	root = balanceHelper(v, 0, nodeCount - 1);
}

void BST::deleteTree() {
	BSTNode* current, * pre;
	std::vector<BSTNode*> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return;

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current;
			current = current->rchild;
		}
		else {
			pre = current->lchild;
			while (pre->rchild != nullptr && pre->rchild != current)
				pre = pre->rchild;

			if (pre->rchild == nullptr) {
				pre->rchild = current;
				current = current->lchild;
			}
			else {
				pre->rchild = nullptr;
				v[i++] = current;
				current = current->rchild;
			}
		}
	}
	for (std::size_t i = 0; i < v.size() - 1; i++) {
		delete v[i];
	}
	root = nullptr;
}

bool BST::deleteSubtree(int item) {
	BSTNode* it = deleteSubtreeFind(item);
	if (it != nullptr) {
		BSTNode* tmp = root;
		root = it;
		nodeCount -= (deleteSubtreeThreadedInOrder()).size();
		deleteTree();
		root = tmp;
		return true;
	}
	return false;
}

std::vector<BSTNode*> BST::LevelOrder() {
	if (root == nullptr)  return std::vector<BSTNode*>();

	std::vector<BSTNode*> v(nodeCount);

	v[0] = root;

	size_t i = 0;
	size_t j = 1;
	while (v.size() > i) {
		//std::cout << v[i]->data << std::endl;
		if (v[i]->lchild != nullptr)
			v[j++] = v[i]->lchild;

		if (v[i]->rchild != nullptr)
			v[j++] = v[i]->rchild;
		i++;
	}
	return v;
}

std::vector<int> BST::RPreorder() {
	BSTNode* node = root;
	if (node == nullptr)
		return std::vector<int>();

	std::vector<int> order(nodeCount);
	int i = 0;
	RPreorderHelper(node, order, i);
	return order;
}

std::vector<int> BST::RInorder() {
	BSTNode* node = root;
	if (node == nullptr)
		return std::vector<int>();

	std::vector<int> order(nodeCount);
	int i = 0;
	RInorderHelper(node, order, i);
	return order;
}

std::vector<int> BST::RReverseInorder() {
	BSTNode* node = root;
	if (node == nullptr)
		return std::vector<int>();

	std::vector<int> order(nodeCount);
	int i = 0;
	RReverseInorderHelper(node, order, i);
	return order;
}

std::vector<int> BST::RPostorder(){
	BSTNode* node = root;
	if (node == nullptr)
		return std::vector<int>();

	std::vector<int> order(nodeCount);
	int i = 0;
	RPostorderHelper(node, order, i);
	return order;
}

void BST::RPreorderHelper(BSTNode* node, std::vector<int>& order, int& i) {
	order[i++] = node->data;
	if(node->lchild != nullptr)
		RPreorderHelper(node->lchild, order, i);
	if (node->rchild != nullptr)
		RPreorderHelper(node->rchild, order, i);
}

void BST::RInorderHelper(BSTNode* node, std::vector<int>& order, int& i) {
	if (node->lchild != nullptr)
		RInorderHelper(node->lchild, order, i);
	order[i++] = node->data;
	if (node->rchild != nullptr)
		RInorderHelper(node->rchild, order, i);
}

void BST::RReverseInorderHelper(BSTNode* node, std::vector<int>& order, int& i) {
	if (node->rchild != nullptr)
		RReverseInorderHelper(node->rchild, order, i);
	order[i++] = node->data;
	if (node->lchild != nullptr)
		RReverseInorderHelper(node->lchild, order, i);
}

void BST::RPostorderHelper(BSTNode* node, std::vector<int>& order, int& i) {
	if (node->lchild != nullptr)
		RPostorderHelper(node->lchild, order, i);
	if (node->rchild != nullptr)
		RPostorderHelper(node->rchild, order, i);
	order[i++] = node->data;
}

std::vector<int> BST::preOrder() {
	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> s;
	BSTNode* curr = root;

	while (curr != nullptr || s.empty() == false) {
		while (curr != nullptr) {
			s.push(curr);
			v[i++] = curr->data;
			curr = curr->lchild;
		}
		curr = s.top();
		s.pop();
		curr = curr->rchild;
	}
	return v;
}

std::vector<int> BST::inOrder(){
	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> s;
	BSTNode* curr = root;

	while (curr != nullptr || s.empty() == false){
		while (curr != nullptr){
			s.push(curr);
			curr = curr->lchild;
		}
		curr = s.top();
		s.pop();
		v[i++] = curr->data;
		curr = curr->rchild;
	}
	return v;
}

std::vector<int> BST::reverseInOrder() {
	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> s;
	BSTNode* curr = root;

	while (curr != nullptr || s.empty() == false) {
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->rchild;
		}
		curr = s.top();
		s.pop();
		v[i++] = curr->data;
		curr = curr->lchild;
	}
	return v;
}

std::vector<int> BST::postOrder() {
	if (root == nullptr)
		return std::vector<int>();

	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> stack;

	BSTNode* node = root;

	do {
		while (node) {
			// Push root's right child and then root to stack. 
			if (node->rchild)
				stack.push(node->rchild);
			stack.push(node);

			// Set root as root's left child   
			node = node->lchild;
		}

		node = stack.top();
		stack.pop();

		if (node->rchild && stack.empty() == false && (stack.top() == node->rchild)) {
			stack.pop();  // remove right child from stack 
			stack.push(node);  // push root back to stack 
			node = node->rchild; // change root so that the right  
								// child is processed next 
		}
		else {
			v[i++] = node->data;
			node = nullptr;
		}
	} while (stack.empty() == false);
	return v;
}

std::vector<int> BST::postOrder2Stacks() {
	if (root == nullptr)
		return std::vector<int>();

	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> s1, s2;

	s1.push(root);
	BSTNode* node;

	while (!s1.empty()) {
		node = s1.top();
		s1.pop();
		s2.push(node);

		if (node->lchild)
			s1.push(node->lchild);
		if (node->rchild)
			s1.push(node->rchild);
	}

	while (!s2.empty()) {
		node = s2.top();
		s2.pop();
		v[i++] = node->data;
	}
	return v;
}


std::vector<int> BST::postOrder2ndMethod() {
	if (root == nullptr)
		return std::vector<int>();

	std::vector<int> v(nodeCount);
	int i = 0;
	std::stack<BSTNode*> stack;

	BSTNode* node = root;

	while (true) {
		while (node != nullptr) {
			stack.push(node);
			stack.push(node);
			node = node->lchild;
		}

		if (stack.empty()) break;
		node = stack.top();
		stack.pop();

		if (!stack.empty() && stack.top() == node) {
			node = node->rchild;
		}

		else {
			v[i++] = node->data;
			node = nullptr;
		}
	}
	return v;
}

std::vector<int> BST::threadedPreOrder() {
	BSTNode* current, * pre;
	std::vector<int> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<int>();

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current->data;
			current = current->rchild;
		}
		else {
			pre = current->lchild;
			while (pre->rchild != nullptr && pre->rchild != current) {
				pre = pre->rchild;
			}

			if (pre->rchild == current) {
				pre->rchild = nullptr;
				current = current->rchild;
			}
			else {
				v[i++] = current->data;
				pre->rchild = current;
				current = current->lchild;
			}
		}
	}
	return v;
}

std::vector<int> BST::threadedInOrder(){ //Morris traversal
	BSTNode* current, * pre;
	std::vector<int> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<int>();

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current->data;
			current = current->rchild;
		}
		else {

			/* Find the inorder predecessor of current */
			pre = current->lchild;
			while (pre->rchild != nullptr && pre->rchild != current)
				pre = pre->rchild;

			/* Make current as the right child of its inorder
			   predecessor */
			if (pre->rchild == nullptr) {
				pre->rchild = current;
				current = current->lchild;
			}

			/* Revert the changes made in the 'if' part to restore
			   the original tree i.e., fix the right child
			   of predecessor */
			else {
				pre->rchild = nullptr;
				v[i++] = current->data;
				current = current->rchild;
			} /* End of if condition pre->right == NULL */
		} /* End of if condition current->left == NULL*/
	} /* End of while */
	return v;
}

std::vector<int> BST::threadedReverseInOrder() {
	BSTNode* current, * pre;
	std::vector<int> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<int>();

	current = root;
	while (current != nullptr) {

		if (current->rchild == nullptr) {
			v[i++] = current->data;
			current = current->lchild;
		}
		else {
			pre = current->rchild;
			while (pre->lchild != nullptr && pre->lchild != current)
				pre = pre->lchild;

			if (pre->lchild == nullptr) {
				pre->lchild = current;
				current = current->rchild;
			}
			else {
				pre->lchild = nullptr;
				v[i++] = current->data;
				current = current->lchild;
			}
		}
	}
	return v;
}

std::vector<int> BST::threadedPostOrder() {
	if (root == nullptr)
		return std::vector<int>();

	BSTNode* dummyRoot = new BSTNode();
	dummyRoot->lchild = root;
	std::vector<int> v(nodeCount);
	int i = 0;

	//Think of P as the current node 
	BSTNode* p = dummyRoot, * pred, * first, * middle, * last;
	while (p != nullptr) {
		if (p->lchild == nullptr) {
			p = p->rchild;
		}
		else {
			/* p has a left child => it also has a predeccessor
			   make p as right child predeccessor of p
			*/
			pred = p->lchild;
			while (pred->rchild != nullptr && pred->rchild != p) {
				pred = pred->rchild;
			}

			if (pred->rchild == nullptr) {

				// predeccessor found for first time
				// modify the tree
				pred->rchild = p;
				p = p->lchild;
			}
			else {

				// predeccessor found second time
				// reverse the right references in chain from pred to p
				first = p;
				middle = p->lchild;
				while (middle != p) {
					last = middle->rchild;
					middle->rchild = first;
					first = middle;
					middle = last;
				}

				// visit the nodes from pred to p
				// again reverse the right references from pred to p    
				first = p;
				middle = pred;
				while (middle != p) {
					v[i++] = middle->data;
					last = middle->rchild;
					middle->rchild = first;
					first = middle;
					middle = last;
				}

				// remove the pred to node reference to restore the tree structure
				pred->rchild = nullptr;
				p = p->rchild;
			}
		}
	}
	return v;
}

std::vector<int> BST::threadedPostOrder2ndMethod() {
	BSTNode* current, * pre;
	std::vector<int> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<int>();

	current = root;
	while (current != nullptr) {

		if (current->rchild == nullptr) {
			v[i++] = current->data;
			current = current->lchild;
		}
		else {
			pre = current->rchild;
			while (pre->lchild != nullptr && pre->lchild != current)
				pre = pre->lchild;

			if (pre->lchild == nullptr) {
				pre->lchild = current;
				v[i++] = current->data;
				current = current->rchild;
			}
			else {
				pre->lchild = nullptr;
				current = current->lchild;
			}
		}
	}

	for (std::size_t i = 0; i < v.size() - 1 - i; i++) { //need to reverse
		int tmp = v[i];
		v[i] = v[v.size() - 1 - i];
		v[v.size() - 1 - i] = tmp;
	}
	return v;
}



int BST::size() {
	return nodeCount;
}





BSTNode* BST::removeHelper(BSTNode* root, int item) {
	if (root == nullptr) return nullptr;

	// If the key to be deleted is smaller than the root's key, 
	// then it lies in left subtree 
	if (item < root->data)
		root->lchild = removeHelper(root->lchild, item);

	// If the key to be deleted is greater than the root's key, 
	// then it lies in right subtree 
	else if (item > root->data)
		root->rchild = removeHelper(root->rchild, item);

	// if key is same as root's key, then This is the node 
	// to be deleted 
	else
	{
		// node with only one child or no child 
		if (root->lchild == nullptr)
		{
			BSTNode* temp = root->rchild;
			delete root;
			return temp;
		}
		else if (root->rchild == nullptr)
		{
			BSTNode* temp = root->lchild;
			delete root;
			return temp;
		}

		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		BSTNode* temp = minValue(root->rchild);

		// Copy the inorder successor's content to this node 
		root->data = temp->data;

		// Delete the inorder successor 
		root->rchild = removeHelper(root->rchild, temp->data);
	}
	return root;
}

BSTNode* BST::minValue(BSTNode* node)
{
	BSTNode* current = node;

	/* loop down to find the leftmost leaf */
	while (current && current->lchild != nullptr)
		current = current->lchild;

	return current;
}

void BST::threadedInOrderPointers(std::vector<BSTNode*>& v) {
	BSTNode* current, * pre;

	if (root == nullptr)
		return;

	int i = 0;
	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current;
			current = current->rchild;
		}
		else {
			pre = current->lchild;
			while (pre->rchild != nullptr && pre->rchild != current)
				pre = pre->rchild;
			if (pre->rchild == nullptr) {
				pre->rchild = current;
				current = current->lchild;
			}
			else {
				pre->rchild = nullptr;
				v[i++] = current;
				current = current->rchild;
			}
		}
	}
}

void BST::balanceConstructorHelper(const std::vector<int>& v, int left, int right) {
	if (left > right)
		return;

	int mid = (left + right) / 2;
	insert(v[mid]);

	balanceConstructorHelper(v, left, mid - 1);
	balanceConstructorHelper(v, mid + 1, right);
}

BSTNode* BST::balanceHelper(std::vector<BSTNode*>& v, int left, int right) {
	if (left > right)
		return nullptr;

	int mid = (left + right) / 2;
	BSTNode* node = v[mid];

	node->lchild = balanceHelper(v, left, mid - 1);
	node->rchild = balanceHelper(v, mid + 1, right);
	return node;
}

BSTNode* BST::deleteSubtreeFind(int item) {
	if (root == nullptr) {
		return nullptr;
	}
	int state = 0;
	BSTNode* it = root, *pre = nullptr;
	while (1) {
		if (item > it->data) {
			if (it->rchild != nullptr) {
				pre = it;
				it = it->rchild;
				state = 2;
			}
			else {
				return nullptr;
			}
		}
		else if (item < it->data) {
			if (it->lchild != nullptr) {
				pre = it;
				it = it->lchild;
				state = 1;
			}
			else {
				return nullptr;
			}
		}
		else { 
			if (state == 1) {
				pre->lchild = nullptr;
			}
			else if (state == 2) {
				pre->rchild = nullptr;
			}
			return it; 
		}
	}
}

std::vector<int> BST::deleteSubtreeThreadedInOrder() { //Morris traversal
	BSTNode* current, * pre;
	std::vector<int> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<int>();

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current->data;
			current = current->rchild;
		}
		else {

			/* Find the inorder predecessor of current */
			pre = current->lchild;
			while (pre->rchild != nullptr && pre->rchild != current)
				pre = pre->rchild;

			/* Make current as the right child of its inorder
			   predecessor */
			if (pre->rchild == nullptr) {
				pre->rchild = current;
				current = current->lchild;
			}

			/* Revert the changes made in the 'if' part to restore
			   the original tree i.e., fix the right child
			   of predecessor */
			else {
				pre->rchild = nullptr;
				v[i++] = current->data;
				current = current->rchild;
			} /* End of if condition pre->right == NULL */
		} /* End of if condition current->left == NULL*/
	} /* End of while */
	v.resize(i);
	return v;
}