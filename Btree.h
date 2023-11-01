#pragma once
//������ �������� �������� �� ����� 2n-1 ������
//������ ��������, ����� ��������, ������ �� ����� n-1 ������
//������ �������� ���� ������������ ����� ����, ���� m+1 ��������,
//��� m - ����� ������ �� ���� ��������
//��� ��������-������ ��������� �� ����� ������

struct SearchResult {
	int index;
	Node* node;
};


class Node {
	friend class Btree;
	friend SearchResult* Search(Node* node, int value);
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

class Btree {		//����� �����������(��������)
private:
	int rank;
	Node* root;
public:
	Btree(int rank = 2);
	Btree(Btree& tree);
	Btree& operator = (Btree& S);
	void Add(int value);
	void Del(int value);	//������ �� ������������
	void Print();	//� �������� ������
	//����� ������
	//���������� ��������� +(�����������)


};