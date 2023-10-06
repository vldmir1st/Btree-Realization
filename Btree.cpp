#include "Btree.h"

Btree::Btree(int rank) {
	this->rank = rank;
	this->root = nullptr;
}

Node::Node(int rank) {
	this->rank = rank;
	this->children = nullptr;
	this->keys = nullptr;
}

void Btree::Add(int value) {
	if (root == nullptr) {

	}
}
