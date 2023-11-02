#include "Btree.h"

Btree::Btree(int rank) {
	this->rank = rank;
	this->root = nullptr;
}

Node::Node(int rank, bool leaf) {
	this->rank = rank;
	this->children = new Node*[2 * rank];
	for (int i = 0; i < 2 * rank; i++) { children[i] = nullptr; }
	this->keys = new int[2*rank-1];
	this->elCount = 0;
	this->leaf = leaf;
}

Btree::~Btree() {
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

Node::~Node() {
	delete[] keys;
	for (int i = 0; i < elCount; i++) { delete children[i]; }
}

void Btree::Add(int value) {
	if (root == nullptr) {
		root = new Node(rank, true);
		root->keys[0] = value;
		root->elCount = 1;
	}
	else {
		if (root->elCount == 2*rank - 1) {
			Node* nodeToSplit = root;
			root = new Node(this->rank, false);
			root->children[0] = nodeToSplit;
			root->SplitChild(0, root->children[0]);
			root->InsertNonFull(value);
		}
		else {
			root->InsertNonFull(value);
		}
	}
}

void Node::SplitChild(int index, Node* child) {
	Node* newChild = new Node(rank, child->leaf);
	newChild->elCount = rank - 1;
	for (int i = 0; i < rank-1; i++) {
		newChild->keys[i] = child->keys[i + rank];
	}
	if (!child->leaf) {
		for (int i = 0; i < rank; i++) {
			newChild->children[i] = child->children[i + rank];
		}
	}
	child->elCount = rank - 1;
	for (int i = elCount; i >= index + 1; i--) {
		children[i + 1] = children[i];
	}
	children[index + 1] = newChild;
	for (int i = elCount-1; i >= index; i--) {
		keys[i + 1] = keys[i];
	}
	keys[index] = child->keys[rank-1];
	elCount++;
}

void Node::InsertNonFull(int value) {
	int i = elCount - 1;
	if (leaf)
	{
		while (i >= 0 && keys[i] > value)
		{
			keys[i + 1] = keys[i];
			i--;
		}
		keys[i + 1] = value;
		elCount++;
	}
	else
	{
		while (i >= 0 && keys[i] > value)
			i--;
		if (children[i + 1]->elCount == 2 * rank - 1)
		{
			SplitChild(i + 1, children[i + 1]);
			if (keys[i + 1] < value)
				i++;
		}
		children[i + 1]->InsertNonFull(value);
	}
}

bool Btree::Search(int value) {
	if (root == nullptr) { return false; }
	if (root->Search(value) == nullptr) { return false; }
	return true;
}

SearchResult* Node::Search(int value) {
	int i;
	for (i = 0; (keys[i] < value && i < elCount); i++) { }
	if (i < elCount && value == keys[i]) { return new SearchResult{ i, this }; }
	if (leaf) { return nullptr; }
	return children[i]->Search(value);
}

Node::Node(Node& node) {
	elCount = node.elCount;
	leaf = node.leaf;
	rank = node.rank;
	keys = new int[2*rank-1];
	for (int i = 0; i < elCount; i++) { keys[i] = node.keys[i]; }
	children = new Node * [2 * rank];
	if (!leaf) {
		for (int i = 0; i <= elCount; i++) { children[i] = new Node(*node.children[i]); }
		for (int i = elCount + 1; i < 2 * rank; i++) { children[i] = nullptr; }
	}
	else { for (int i = 0; i < 2 * rank; i++) { children[i] = nullptr; } }
}

Btree& Btree::operator = (Btree& tree) {
	if (this != &tree) {
		this->~Btree();
		root = new Node(*tree.root);
	}
	return *this;
}

void Btree::Print() {
	root->Print(0);
	std::cout << "\n";
}

void Node::Print(int level) {
	if (this != nullptr) {
		for (int i = 0; i < level; i++) { std::cout << "\t\t"; }
		for (int i = 0; i < elCount; i++) { printf("%3d", keys[i]); }
		std::cout << "\n";
		for (int i = 0; i <= elCount; i++) { children[i]->Print(level + 1); }
	}
}