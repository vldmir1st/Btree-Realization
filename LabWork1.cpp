#include "Btree.h"

int main()
{
	Btree tree(3);
	//for (int i = 0; i < 30; i++)
	for (int i = 29; i > -1; i--)
		tree.Add(i%11);
	tree.Print();

	//for (int i = -2; i < 15; i++) {
	for (int i = 15; i > -2; i--) {
		tree.Del(2 * i + 1);
		//std::cout << 2 * i + 1 << " deleted\n";
	}
	tree.Print();

	/*
	for (int i = 0; i < 14; i++) {
		tree.Add(2 * i + 1);
	}
	tree.Print();
	*/
	
	//for (int i = -2; i < 15; i++) {
	for (int i = 10; i > 3; i--) {
		tree.Del(2 * i);
		//std::cout << 2 * i + 1 << " deleted\n";
	}
	tree.Print();


	//for (int i = 0; i < 30; i++)
	for (int i = 29; i > -1; i--)
		tree.Add(i%4);
	tree.Print();


	//for (int i = -2; i < 15; i++) {
	for (int i = 6; i > 3; i--) {
		while (tree.Search( i%7))
			tree.Del(i%7);
	}
		//std::cout << 2 * i + 1 << " deleted\n";
	
	tree.Print();

	tree = tree + tree;
	tree.Print();
}

