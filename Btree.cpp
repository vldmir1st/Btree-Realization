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
	for (int i = 0; i <= elCount; i++)
		delete children[i];
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
	if (leaf) {
		while (i >= 0 && keys[i] > value)
		{
			keys[i + 1] = keys[i];
			i--;
		}
		keys[i + 1] = value;
		elCount++;
	}
	else {
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

Btree& Btree::operator = (const Btree& tree) {
	if (this != &tree && tree.root != nullptr) {
		this->~Btree();
		root = new Node(*tree.root);
	}
	this->rank = tree.rank;
	return *this;
}

Btree::Btree(const Btree& tree) {
	rank = tree.rank;
	if (tree.root != nullptr) { root = new Node(*tree.root); }
	else { root = nullptr; }
}

void Btree::Print() {
	if (root != nullptr) {
		root->Print(0);
		std::cout << "\n";
	}
	else
		std::cout << "The tree is empty\n";
}

void Node::Print(int level) {
	if (this != nullptr) {
		for (int i = 0; i < level; i++) { std::cout << "\t"; }
		for (int i = 0; i < elCount; i++) { printf("%3d", keys[i]); }
		std::cout << "\n";
		for (int i = 0; i <= elCount; i++) { children[i]->Print(level + 1); }
	}
}

void Btree::Traverse() {
	root->Traverse();
	std::cout << "\n";
}

void Node::Traverse() {
	if (this != nullptr) {
		for (int i = 0; i < elCount; i++) {
			children[i]->Traverse();
			std::cout << keys[i] << " ";
		}
		children[elCount]->Traverse();
	}
}

Btree operator + (const Btree& B1, const Btree& B2) {
	if (B1.root == nullptr) { return {B2}; }
	Btree B(B1);
	if (B2.root != nullptr) { B2.root->TraverseForPlusOperator(&B);	}
	return B;
}

void Node::TraverseForPlusOperator(Btree* TreeToAdd) {
	if (this != nullptr) {
		for (int i = 0; i < elCount; i++) {
			children[i]->TraverseForPlusOperator(TreeToAdd);
			TreeToAdd->Add(keys[i]);
		}
		children[elCount]->TraverseForPlusOperator(TreeToAdd);
	}
}

void Btree::Del(int value) {
	if (!root) {
		std::cout << "The tree is empty\n";
		return;
	}
	root->Remove(value);
	if (root->elCount == 0) {
		Node* tmp = root;
		if (root->leaf)
			root = nullptr;
		else
			root = root->children[0];

		//чтобы деструктор не начал удалять нижние ноды
		for (int i = 0; i <= tmp->elCount; i++)
			tmp->children[i] = nullptr;

		delete tmp;
	}
}

void Node::Remove(int value) {
	int index = FindKey(value);
	if (index < elCount && keys[index] == value) {
		if (leaf)
			RemoveFromLeaf(index);
		else
			RemoveFromNonLeaf(index);
	}
	else {
		if (leaf) {
			std::cout << "The key " << value << " is does not exist in the tree\n";
			return;
		}
		bool flag = ((index == elCount) ? true : false);
		if (children[index]->elCount < rank)
			Fill(index);
		if (flag && index > elCount)
			children[index - 1]->Remove(value);
		else
			children[index]->Remove(value);
	}
	return;
}

int Node::FindKey(int value) {
	int index = 0;
	while (index < elCount && keys[index] < value)
		++index;
	return index;
}

void Node::RemoveFromLeaf(int index) {
	for (int i = index + 1; i < elCount; ++i)
		keys[i - 1] = keys[i];
	elCount--;
}

void Node::RemoveFromNonLeaf(int index) {
	int key = keys[index];
	if (children[index + 1]->elCount >= rank) {
		int succ = GetSuccessor(index);
		keys[index] = succ;
		children[index + 1]->Remove(succ);
	}
	else if (children[index]->elCount >= rank) {
		int pred = GetPredecessor(index);
		keys[index] = pred;
		children[index]->Remove(pred);
	}
	else {
		Merge(index);
		children[index]->Remove(key);
	}
}

int Node::GetSuccessor(int index) {
	Node* cur = children[index + 1];
	while (!cur->leaf)
		cur = cur->children[0];
	return cur->keys[0];
}

int Node::GetPredecessor(int index) {
	Node* cur = children[index];
	while (!cur->leaf)
		cur = cur->children[cur->elCount];
	return cur->keys[cur->elCount - 1];
}

void Node::Merge(int index) {
	Node* child = children[index];
	Node* sibling = children[index + 1];
	child->keys[rank - 1] = keys[index];
	for (int i = 0; i < sibling->elCount; ++i)
		child->keys[i + rank] = sibling->keys[i];
	if (!child->leaf) {
		for (int i = 0; i <= sibling->elCount; ++i)
			child->children[i + rank] = sibling->children[i];
	}
	for (int i = index + 1; i < elCount; ++i)
		keys[i - 1] = keys[i];
	for (int i = index + 2; i <= elCount; ++i)
		children[i - 1] = children[i];
	child->elCount += sibling->elCount + 1;
	elCount--;

	//чтобы вызванный ниже деструктор не удалил детей, которые были перенесены к другому ноду
	for (int i = 0; i <= sibling->elCount; i++)
		sibling->children[i] = nullptr;
	
	delete(sibling);
}

void Node::Fill(int index) {
	if (index != 0 && children[index - 1]->elCount >= rank)
		BorrowFromPrev(index);
	else if (index != elCount && children[index + 1]->elCount >= rank)
		BorrowFromNext(index);
	else {
		if (index != elCount)
			Merge(index);
		else
			Merge(index - 1);
	}
}

void Node::BorrowFromPrev(int index) {
	Node* child = children[index];
	Node* sibling = children[index - 1];
	for (int i = child->elCount - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];
	if (!child->leaf) {
		for (int i = child->elCount; i >= 0; --i)
			child->children[i + 1] = child->children[i];
	}
	child->keys[0] = keys[index - 1];
	if (!child->leaf)
		child->children[0] = sibling->children[sibling->elCount];
	keys[index - 1] = sibling->keys[sibling->elCount - 1];

	child->elCount += 1;
	sibling->elCount -= 1;
}

void Node::BorrowFromNext(int index) {
	Node* child = children[index];
	Node* sibling = children[index + 1];
	child->keys[(child->elCount)] = keys[index];
	if (!(child->leaf))
		child->children[(child->elCount) + 1] = sibling->children[0];
	keys[index] = sibling->keys[0];
	for (int i = 1; i < sibling->elCount; ++i)
		sibling->keys[i - 1] = sibling->keys[i];
	if (!sibling->leaf) {
		for (int i = 1; i <= sibling->elCount; ++i)
			sibling->children[i - 1] = sibling->children[i];
	}
	child->elCount += 1;
	sibling->elCount -= 1;
}