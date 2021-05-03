#pragma once
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

template <typename T>
struct BSTNode {
	BSTNode<T>* lchild;
	BSTNode<T>* rchild;
	T data;
};

template <typename T>
class BST {
	BSTNode<T>* root;
	int nodeCount;
public:
	BST();
	BST(std::vector<T>&);
	BST(std::vector<T>&&);
	~BST();
	int size();
	bool insert(T);
	bool remove(T);
	BSTNode<T>* find(T);
	void balance();
	void deleteTree();
	bool deleteSubtree(T);

	std::vector<BSTNode<T>*> LevelOrder();

	std::vector<T> RPreorder();
	std::vector<T> RInorder();
	std::vector<T> RReverseInorder();
	std::vector<T> RPostorder();
	void RPreorderHelper(BSTNode<T>*, std::vector<T>&, int&);
	void RInorderHelper(BSTNode<T>*, std::vector<T>&, int&);
	void RReverseInorderHelper(BSTNode<T>*, std::vector<T>&, int&);
	void RPostorderHelper(BSTNode<T>*, std::vector<T>&, int&);

	std::vector<T> preOrder();
	std::vector<T> inOrder();
	std::vector<T> reverseInOrder();
	std::vector<T> postOrder();
	std::vector<T> postOrder2Stacks(); //simple but slower then default
	std::vector<T> postOrder2ndMethod(); //slower then default and 2stacks

	std::vector<T> threadedPreOrder();
	std::vector<T> threadedInOrder();
	std::vector<T> threadedReverseInOrder();
	std::vector<T> threadedPostOrder();
	std::vector<T> threadedPostOrder2ndMethod();
protected:
	BSTNode<T>* removeHelper(BSTNode<T>*, T);
	BSTNode<T>* minValue(BSTNode<T>*);
	void threadedInOrderPointers(std::vector<BSTNode<T>*>&);
	void balanceConstructorHelper(const std::vector<T>&, int, int);
	BSTNode<T>* balanceHelper(std::vector<BSTNode<T>*>&, int, int);
	BSTNode<T>* deleteSubtreeFind(T);
	std::vector<T> deleteSubtreeThreadedInOrder();
};



template <typename T>
BST<T>::BST() {
	root = nullptr;
	nodeCount = 0;
}

template <typename T>
BST<T>::BST(std::vector<T>& v) {
	root = nullptr;
	nodeCount = 0;
	std::sort(v.begin(), v.end());
	balanceConstructorHelper(v, 0, v.size() - 1);
}

template <typename T>
BST<T>::BST(std::vector<T>&& v) {
	root = nullptr;
	nodeCount = 0;
	std::sort(v.begin(), v.end());
	balanceConstructorHelper(v, 0, v.size() - 1);
}

template <typename T>
BST<T>::~BST() {
	deleteTree();
}

template <typename T>
int BST<T>::size() {
	return nodeCount;
}

template <typename T>
bool BST<T>::insert(T item) {
	if (root == nullptr) {
		root = new BSTNode<T>();
		root->data = item;
		root->lchild = nullptr;
		root->rchild = nullptr;
		nodeCount++;
		return true;
	}
	BSTNode<T>* it = root;
	while (1) {
		if (item > it->data) {
			if (it->rchild != nullptr) {
				it = it->rchild;
			}
			else {
				it->rchild = new BSTNode<T>();
				it = it->rchild;
				it->data = item;
				it->lchild = nullptr;
				it->rchild = nullptr;
				nodeCount++;
				return true;
			}
		}
		else if (item < it->data) {
			if (it->lchild != nullptr) {
				it = it->lchild;
			}
			else {
				it->lchild = new BSTNode<T>();
				it = it->lchild;
				it->data = item;
				it->lchild = nullptr;
				it->rchild = nullptr;
				nodeCount++;
				return true;
			}
		}
		else return false;
	}
}

template <typename T>
bool BST<T>::remove(T item) {
	if (find(item) != nullptr) {
		root = removeHelper(root, item);
		nodeCount--;
		return true;
	}
	return false;
}

template <typename T>
BSTNode<T>* BST<T>::find(T item) {
	if (root == nullptr) {
		return nullptr;
	}
	BSTNode<T>* it = root;
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

template <typename T>
void BST<T>::balance() {
	std::vector<BSTNode<T>*> v(nodeCount);
	threadedInOrderPointers(v);
	root = balanceHelper(v, 0, nodeCount - 1);
}

template <typename T>
void BST<T>::deleteTree() {
	BSTNode<T>* current, * pre;
	std::vector<BSTNode<T>*> v(nodeCount);
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

template <typename T>
bool BST<T>::deleteSubtree(T item) {
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

template <typename T>
std::vector<BSTNode<T>*> BST<T>::LevelOrder() {
	if (root == nullptr)  return std::vector<BSTNode<T>*>();

	std::vector<BSTNode<T>*> v(nodeCount);

	v[0] = root;

	size_t i = 0;
	size_t j = 1;
	while (v.size() > i) {
		if (v[i]->lchild != nullptr)
			v[j++] = v[i]->lchild;

		if (v[i]->rchild != nullptr)
			v[j++] = v[i]->rchild;
		i++;
	}
	return v;
}

template <typename T>
std::vector<T> BST<T>::RPreorder() {
	BSTNode<T>* node = root;
	if (node == nullptr)
		return std::vector<T>();

	std::vector<int> order(nodeCount);
	int i = 0;
	RPreorderHelper(node, order, i);
	return order;
}

template <typename T>
std::vector<T> BST<T>::RInorder() {
	BSTNode<T>* node = root;
	if (node == nullptr)
		return std::vector<T>();

	std::vector<T> order(nodeCount);
	int i = 0;
	RInorderHelper(node, order, i);
	return order;
}

template <typename T>
std::vector<T> BST<T>::RReverseInorder() {
	BSTNode<T>* node = root;
	if (node == nullptr)
		return std::vector<T>();

	std::vector<T> order(nodeCount);
	int i = 0;
	RReverseInorderHelper(node, order, i);
	return order;
}

template <typename T>
std::vector<T> BST<T>::RPostorder() {
	BSTNode<T>* node = root;
	if (node == nullptr)
		return std::vector<T>();

	std::vector<T> order(nodeCount);
	int i = 0;
	RPostorderHelper(node, order, i);
	return order;
}

template <typename T>
void BST<T>::RPreorderHelper(BSTNode<T>* node, std::vector<T>& order, int& i) {
	order[i++] = node->data;
	if (node->lchild != nullptr)
		RPreorderHelper(node->lchild, order, i);
	if (node->rchild != nullptr)
		RPreorderHelper(node->rchild, order, i);
}

template <typename T>
void BST<T>::RInorderHelper(BSTNode<T>* node, std::vector<T>& order, int& i) {
	if (node->lchild != nullptr)
		RInorderHelper(node->lchild, order, i);
	order[i++] = node->data;
	if (node->rchild != nullptr)
		RInorderHelper(node->rchild, order, i);
}

template <typename T>
void BST<T>::RReverseInorderHelper(BSTNode<T>* node, std::vector<T>& order, int& i) {
	if (node->rchild != nullptr)
		RReverseInorderHelper(node->rchild, order, i);
	order[i++] = node->data;
	if (node->lchild != nullptr)
		RReverseInorderHelper(node->lchild, order, i);
}

template <typename T>
void BST<T>::RPostorderHelper(BSTNode<T>* node, std::vector<T>& order, int& i) {
	if (node->lchild != nullptr)
		RPostorderHelper(node->lchild, order, i);
	if (node->rchild != nullptr)
		RPostorderHelper(node->rchild, order, i);
	order[i++] = node->data;
}

template <typename T>
std::vector<T> BST<T>::preOrder() {
	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> s;
	BSTNode<T>* curr = root;

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

template <typename T>
std::vector<T> BST<T>::inOrder() {
	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> s;
	BSTNode<T>* curr = root;

	while (curr != nullptr || s.empty() == false) {
		while (curr != nullptr) {
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

template <typename T>
std::vector<T> BST<T>::reverseInOrder() {
	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> s;
	BSTNode<T>* curr = root;

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

template <typename T>
std::vector<T> BST<T>::postOrder() {
	if (root == nullptr)
		return std::vector<T>();

	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> stack;

	BSTNode<T>* node = root;

	do {
		while (node) {
			if (node->rchild)
				stack.push(node->rchild);
			stack.push(node);
			node = node->lchild;
		}

		node = stack.top();
		stack.pop();

		if (node->rchild && stack.empty() == false && (stack.top() == node->rchild)) {
			stack.pop();
			stack.push(node);
			node = node->rchild;
		}
		else {
			v[i++] = node->data;
			node = nullptr;
		}
	} while (stack.empty() == false);
	return v;
}

template <typename T>
std::vector<T> BST<T>::postOrder2Stacks() {
	if (root == nullptr)
		return std::vector<T>();

	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> s1, s2;

	s1.push(root);
	BSTNode<T>* node;

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

template <typename T>
std::vector<T> BST<T>::postOrder2ndMethod() {
	if (root == nullptr)
		return std::vector<T>();

	std::vector<T> v(nodeCount);
	int i = 0;
	std::stack<BSTNode<T>*> stack;

	BSTNode<T>* node = root;

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

template <typename T>
std::vector<T> BST<T>::threadedPreOrder() {
	BSTNode<T>* current, * pre;
	std::vector<T> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<T>();

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

template <typename T>
std::vector<T> BST<T>::threadedInOrder() { //Morris traversal
	BSTNode<T>* current, * pre;
	std::vector<T> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<T>();

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current->data;
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
				v[i++] = current->data;
				current = current->rchild;
			}
		}
	}
	return v;
}

template <typename T>
std::vector<T> BST<T>::threadedReverseInOrder() {
	BSTNode* current, * pre;
	std::vector<T> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<T>();

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

template <typename T>
std::vector<T> BST<T>::threadedPostOrder() {
	if (root == nullptr)
		return std::vector<T>();

	BSTNode<T>* dummyRoot = new BSTNode<T>();
	dummyRoot->lchild = root;
	std::vector<T> v(nodeCount);
	int i = 0;

	BSTNode<T>* p = dummyRoot, * pred, * first, * middle, * last;
	while (p != nullptr) {
		if (p->lchild == nullptr) {
			p = p->rchild;
		}
		else {
			pred = p->lchild;
			while (pred->rchild != nullptr && pred->rchild != p) {
				pred = pred->rchild;
			}

			if (pred->rchild == nullptr) {
				pred->rchild = p;
				p = p->lchild;
			}
			else {
				first = p;
				middle = p->lchild;
				while (middle != p) {
					last = middle->rchild;
					middle->rchild = first;
					first = middle;
					middle = last;
				}
				first = p;
				middle = pred;
				while (middle != p) {
					v[i++] = middle->data;
					last = middle->rchild;
					middle->rchild = first;
					first = middle;
					middle = last;
				}

				pred->rchild = nullptr;
				p = p->rchild;
			}
		}
	}
	return v;
}

template <typename T>
std::vector<T> BST<T>::threadedPostOrder2ndMethod() {
	BSTNode<T>* current, * pre;
	std::vector<T> v(nodeCount);
	int i = 0;

	if (root == nullptr)
		return std::vector<T>();

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

	for (std::size_t i = 0; i < v.size() - 1 - i; i++) {
		int tmp = v[i];
		v[i] = v[v.size() - 1 - i];
		v[v.size() - 1 - i] = tmp;
	}
	return v;
}


/*----------------------------------------------------------------------*/
/*---------------------------- PROTECTED -------------------------------*/
/*----------------------------------------------------------------------*/


template <typename T>
BSTNode<T>* BST<T>::removeHelper(BSTNode<T>* root, T item) {
	if (root == nullptr) return nullptr;

	if (item < root->data)
		root->lchild = removeHelper(root->lchild, item);

	else if (item > root->data)
		root->rchild = removeHelper(root->rchild, item);

	else {
		if (root->lchild == nullptr) {
			BSTNode* temp = root->rchild;
			delete root;
			return temp;
		}
		else if (root->rchild == nullptr) {
			BSTNode* temp = root->lchild;
			delete root;
			return temp;
		}

		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		BSTNode* temp = minValue(root->rchild);

		root->data = temp->data;

		root->rchild = removeHelper(root->rchild, temp->data);
	}
	return root;
}

template <typename T>
BSTNode<T>* BST<T>::minValue(BSTNode<T>* node) {
	BSTNode<T>* current = node;
	while (current && current->lchild != nullptr)
		current = current->lchild;

	return current;
}

template <typename T>
void BST<T>::threadedInOrderPointers(std::vector<BSTNode<T>*>& v) {
	BSTNode<T>* current, * pre;

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

template <typename T>
void BST<T>::balanceConstructorHelper(const std::vector<T>& v, int left, int right) {
	if (left > right)
		return;

	int mid = (left + right) / 2;
	insert(v[mid]);

	balanceConstructorHelper(v, left, mid - 1);
	balanceConstructorHelper(v, mid + 1, right);
}

template <typename T>
BSTNode<T>* BST<T>::balanceHelper(std::vector<BSTNode<T>*>& v, int left, int right) {
	if (left > right)
		return nullptr;

	int mid = (left + right) / 2;
	BSTNode* node = v[mid];

	node->lchild = balanceHelper(v, left, mid - 1);
	node->rchild = balanceHelper(v, mid + 1, right);
	return node;
}

template <typename T>
BSTNode<T>* BST<T>::deleteSubtreeFind(T item) {
	if (root == nullptr) {
		return nullptr;
	}
	int state = 0;
	BSTNode<T>* it = root, * pre = nullptr;
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

template <typename T>
std::vector<T> BST<T>::deleteSubtreeThreadedInOrder() {
	if (root == nullptr)
		return std::vector<T>();

	BSTNode<T>* current, * pre;
	std::vector<T> v(nodeCount);
	int i = 0;

	current = root;
	while (current != nullptr) {

		if (current->lchild == nullptr) {
			v[i++] = current->data;
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
				v[i++] = current->data;
				current = current->rchild;
			}
		}
	}
	v.resize(i);
	return v;
}