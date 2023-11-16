#pragma once

#include <iostream>

//������ �������� �������� �� ����� 2n-1 ������
//������ ��������, ����� ��������, ������ �� ����� n-1 ������
//������ �������� ���� ������������ ����� ����, ���� m+1 ��������,
//��� m - ����� ������ �� ���� ��������
//��� ��������-������ ��������� �� ����� ������

class Node;

struct SearchResult {
	int index;
	Node* node;
};

class Node {
	friend class Btree;
	friend Btree operator + (const Btree& B1, const Btree& B2);
private:
	int rank;
	int* keys;
	int elCount;
	Node** children;
	bool leaf;

	SearchResult* Search(int value);  //����� ������� null
	void Print(int level);
	Node(int rank, bool leaf);
	Node(Node& node);
	~Node();
	void InsertNonFull(int value);
	void SplitChild(int index, Node* child);
	void Traverse();
	void TraverseForPlusOperator(Btree* TreeToAdd);
	void Remove(int value);
	int FindKey(int value);
	void RemoveFromLeaf(int index);
	void RemoveFromNonLeaf(int index);
	int GetSuccessor(int index);
	int GetPredecessor(int index);
	void Merge(int index);
	void Fill(int index);
	void BorrowFromPrev(int index);
	void BorrowFromNext(int index);
};

class Btree {
	friend Btree operator + (const Btree& B1, const Btree& B2);
private:
	int rank;
	Node* root;
public:
	Btree(int rank = 2);
	Btree(const Btree& tree);
	~Btree();
	Btree& operator = (const Btree& btree);
	void Add(int value);
	void Del(int value);	//������ �� ������������
	void Print();
	bool Search(int value);
	void Traverse();	//����� ������ (�����������)
};