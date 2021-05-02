#pragma once
#include <vector>
#include <iostream>
#include <limits>

template <typename T>
struct ListNode {
	T data;
	ListNode* next;
};

template <typename T>
class LinkedList {
	ListNode<T>* root;
	ListNode<T>* last;
	mutable ListNode<T>* lastAccessed;
	mutable int lastAccessedIndex;
	int nodeCount;
public:
	LinkedList();
	LinkedList(const std::vector<T>&);
	LinkedList(const LinkedList&);
	LinkedList(const LinkedList&, const LinkedList&);
	~LinkedList();
	void push_back(T);
	void push_front(T);
	void insert_at(int, T);
	void remove(int);
	bool detect_loop();
	bool reverse_sublist(int, int);
	T get(int) const;
	T& get(int);
	void concat(const LinkedList&);
	T operator [](int) const;
	T& operator [](int);
	int size() const;
};




template <typename T>
LinkedList<T>::LinkedList() {
	root = nullptr;
	last = nullptr;
	nodeCount = 0;
	lastAccessed = nullptr;
	lastAccessedIndex = std::numeric_limits<int>::max();
}

template <typename T>
LinkedList<T>::LinkedList(const std::vector<T>& list) {
	for (int i = 0; i < nodeCount; i++) {
		this->push_back(list[i]);
	}
	lastAccessed = nullptr;
	lastAccessedIndex = std::numeric_limits<int>::max();
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& list) {
	for (int i = 0; i < nodeCount; i++) {
		this->push_back(list.get(i));
	}
	lastAccessed = nullptr;
	lastAccessedIndex = std::numeric_limits<int>::max();
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& list1, const LinkedList& list2) {
	for (int i = 0; i < list1.nodeCount; i++) {
		this->push_back(list1.get(i));
	}
	for (int i = 0; i < list2.nodeCount; i++) {
		this->push_back(list2.get(i));
	}
	lastAccessed = nullptr;
	lastAccessedIndex = std::numeric_limits<int>::max();
}

template <typename T>
LinkedList<T>::~LinkedList() {
	ListNode<T>* it = root;
	ListNode<T>* tmp_it;
	while (--nodeCount) {
		tmp_it = it;
		it = it->next;
		delete tmp_it;
	}
}

template <typename T>
void LinkedList<T>::push_back(T n) {
	if (root == nullptr) {
		root = new ListNode<T>();
		root->data = n;
		root->next = nullptr;
		last = root;
		nodeCount++;
		return;
	}
	last->next = new ListNode<T>();
	last = last->next;
	last->data = n;
	last->next = nullptr;
	nodeCount++;
}

template <typename T>
void LinkedList<T>::push_front(T n) {
	ListNode<T>* new_node = new ListNode<T>();
	new_node->data = n;
	new_node->next = root;
	root = new_node;
	if (nodeCount == 0) {
		last = root;
	}
	nodeCount++;
}

template <typename T>
void LinkedList<T>::insert_at(int index, T n) {
	if (index > nodeCount - 1 || index < 0) {
		throw std::out_of_range("index out of range");
	}
	if (root == nullptr) {
		root = new ListNode<T>();
		root->data = n;
		root->next = nullptr;
		last = root;
		nodeCount++;
		return;
	}
	if (index == 0) {
		ListNode<T>* new_node = new ListNode<T>();
		new_node->data = n;
		new_node->next = root;
		root = new_node;
	}
	else {
		ListNode<T>* it = root;
		while (index-- > 1) {
			it = it->next;
		}
		ListNode<T>* previousNode = it;
		it = it->next;

		ListNode<T>* new_node = new ListNode<T>();
		new_node->data = n;
		new_node->next = it;

		previousNode->next = new_node;
	}
	nodeCount++;
}

template <typename T>
void LinkedList<T>::remove(int index) {
	if (index > nodeCount - 1 || index < 0) {
		throw std::out_of_range("index out of range");
	}
	if (root == nullptr) {
		return;
	}
	if (index == 0) {
		ListNode<T>* it = root;
		root = it->next;
		delete it;
	}
	else {
		ListNode<T>* it = root;
		int whileIndex = index;
		while (whileIndex-- > 1) {
			it = it->next;
		}
		ListNode<T>* previousNode = it;
		it = it->next;
		ListNode<T>* nextNode = it->next;

		delete it;

		previousNode->next = nextNode;
	}
	nodeCount--;
}

template <typename T>
T LinkedList<T>::get(int index) const {
	if (index > nodeCount - 1 || index < 0) {
		throw std::out_of_range("index out of range");
	}
	if (index == nodeCount - 1) {
		return last->data;
	}
	ListNode<T>* it;
	if (index > lastAccessedIndex) {
		it = lastAccessed;
		int temp = index;
		index -= lastAccessedIndex;
		lastAccessedIndex = temp;
	}
	else {
		it = root;
		lastAccessedIndex = index;
	}
	while (index--) {
		it = it->next;
	}
	lastAccessed = it;
	return it->data;
}

template <typename T>
T& LinkedList<T>::get(int index) {
	if (index > nodeCount - 1 || index < 0) {
		throw std::out_of_range("index out of range");
	}
	if (index == nodeCount - 1) {
		return last->data;
	}
	ListNode<T>* it;
	if (index > lastAccessedIndex) {
		it = lastAccessed;
		int temp = index;
		index -= lastAccessedIndex;
		lastAccessedIndex = temp;
	}
	else {
		it = root;
		lastAccessedIndex = index;
	}
	while (index--) {
		it = it->next;
	}
	lastAccessed = it;
	return it->data;
}

template <typename T>
bool LinkedList<T>::detect_loop() {
	ListNode* slow_p = root, * fast_p = root;

	while (slow_p && fast_p && fast_p->next) {
		slow_p = slow_p->next;
		fast_p = fast_p->next->next;
		if (slow_p == fast_p) {
			return true;
		}
	}
	return false;
}

template <typename T>
bool LinkedList<T>::reverse_sublist(int indexA, int indexB) {
	ListNode* node = root;
	int index = 1;
	ListNode* prev = nullptr;

	indexA++;
	indexB++;
	if (indexA < 1 || indexA >= indexB || root == nullptr) { return false; }
	if (indexA != 1) {
		while (index != indexA - 1) {
			node = node->next;
			index++;
		}
		prev = node;
		node = node->next;
	}

	ListNode<T>* dummy = nullptr;
	ListNode<T>* next = nullptr;
	ListNode<T>* tmp = node;

	for (int i = 0; i < (indexB - indexA + 1); i++) {
		next = node->next;
		node->next = dummy;
		dummy = node;
		node = next;
	}
	tmp->next = next;
	if (prev != nullptr) {
		prev->next = dummy;
	}
	else {
		root = dummy;
	}
	return true;
}

template <typename T>
void LinkedList<T>::concat(const LinkedList& list) {
	for (int i = 0; i < list.nodeCount; i++) {
		this->push_back(list.get(i));
	}
}

template <typename T>
T LinkedList<T>::operator [](int index) const { return this->get(index); }

template <typename T>
T& LinkedList<T>::operator [](int index) { return this->get(index); }

template <typename T>
int LinkedList<T>::size() const {
	return nodeCount;
}