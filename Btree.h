#pragma once
//������ �������� �������� �� ����� 2n-1 ������
//������ ��������, ����� ��������, ������ �� ����� n-1 ������
//������ �������� ���� ������������ ����� ����, ���� m+1 ��������,
//��� m - ����� ������ �� ���� ��������
//��� ��������-������ ��������� �� ����� ������


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
	bool Search(int value);
	void Print();
	//����� ������
	//���������� ��������� +(�����������)

};



