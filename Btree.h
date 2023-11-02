#pragma once

#include <iostream>

//Каждая страница содержит не более 2n-1 ключей
//Каждая страница, кроме корневой, имееет не менее n-1 ключей
//Каждая страница либо представляет собой лист, либо m+1 потомков,
//где m - число ключей на этой странице
//Все страницы-листья находятся на одном уровне

class Node;

struct SearchResult {
	int index;
	Node* node;
};

class Node {
	friend class Btree;
	friend Btree operator + (Btree& B1, Btree& B2);
private:
	int rank;
	int* keys;
	int elCount;
	Node** children;
	bool leaf;

	SearchResult* Search(int value);  //может вернуть null
	void Print(int level);
	Node(int rank, bool leaf);
	Node(Node& node);
	~Node();
	void InsertNonFull(int value);
	void SplitChild(int index, Node* child);
	void Traverse();
	void TraverseForPlusOperator(Btree* TreeToAdd);
};

class Btree {
	friend Btree operator + (Btree& B1, Btree& B2);
private:
	int rank;
	Node* root;
public:
	Btree(int rank = 2);
	Btree(Btree& tree);
	~Btree();
	Btree& operator = (const Btree& btree);
	void Add(int value);
	void Del(int value);	//замена на максимальный
	void Print();
	bool Search(int value);
	void Traverse();	//обход дерева (постфиксный)
};