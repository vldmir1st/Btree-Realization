#pragma once
//Каждая страница содержит не более 2n-1 ключей
//Каждая страница, кроме корневой, имееет не менее n-1 ключей
//Каждая страница либо представляет собой лист, либо m+1 потомков,
//где m - число ключей на этой странице
//Все страницы-листья находятся на одном уровне


class Node {
	friend class Btree;
private:
	int rank;
	int* keys;
	int elCount;
	Node** children;
	bool leaf;
public:
	Node(int rank, bool leaf);
	void InsertNonFull(int value);
	void SplitChild(int index, Node* child);
};

class Btree {		//обход постфиксный(обратный)
private:
	int rank;
	Node* root;
public:
	Btree(int rank = 2);
	Btree(Btree& tree);
	Btree& operator = (Btree& S);
	void Add(int value);
	void Del(int value);	//замена на максимальный
	bool Search(int value);
	void Print();
	//обход дерева
	//перегрузка оператора +(объединение)

};



