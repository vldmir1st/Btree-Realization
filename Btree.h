#pragma once
class NodeData {
private:
	int key;
	int count = 1;
	NodeData* next;
};

class Node {
	friend class Btree;
private:
	int rank;
	NodeData* keys;
	Node* children;
public:
	Node(int rank);
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



