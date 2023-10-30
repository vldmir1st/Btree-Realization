#include "Btree.h"

Btree::Btree(int rank) {
	this->rank = rank;
	this->root = nullptr;
}

Node::Node(int rank, bool leaf) {
	this->rank = rank;
	this->children = new Node*[2*rank];
	this->keys = new int[2*rank-1];
	this->elCount = 0;
	this->leaf = leaf;
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
		newChild->keys[i + rank] = child->keys[i];
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
