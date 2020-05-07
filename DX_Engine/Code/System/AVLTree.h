#ifndef _AVL_TREE_
#define _AVL_TREE_
#pragma once

#include <tuple>

template <typename K, typename V>
class AvlTree final {
private:
	struct Node;

public:
	AvlTree();
	~AvlTree();

	AvlTree(AvlTree const&) = delete;
	AvlTree& operator=(AvlTree const&) = delete;
	AvlTree(AvlTree&&) = delete;
	AvlTree& operator=(AvlTree&&) = delete;

	void preorder(std::vector<std::tuple<K, V>>* = nullptr);
	void insert(const K&, const V&);
	void remove(const K&);
	V* search(const K&);

private:
	void preorder(Node*, std::vector<std::tuple<K, V>>* = nullptr);
	Node* insert(const K&, const V&, Node*);
	Node* remove(const K&, Node*);
	Node* search(const K&, Node*);

	int getHeight(Node*) const;
	int getBalance(Node*) const;
	Node* getMin(Node*) const;

	Node* rotateLeft(Node*);
	Node* rotateRight(Node*);

private:
	Node* root;
};

template <typename K, typename V>
struct AvlTree<K, V>::Node final {
public:
	Node(const K& key, const V& value)
		: left(nullptr)
		, right(nullptr)
		, height(1)
		, key(key)
		, value(value)
	{}

	~Node() {
		if (left != nullptr) {
			delete left;
			left = nullptr;
		}

		if (right != nullptr) {
			delete right;
			right = nullptr;
		}
	}

	Node(Node const&) = delete;
	Node& operator=(Node const&) = delete;
	Node(Node&&) = delete;
	Node& operator=(Node&&) = delete;

public:
	Node* left;
	Node* right;
	int height;
	K key;
	V value;
};


#endif //_AVL_TREE_
