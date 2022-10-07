
#include <iostream>
#include <map>
#include "bst.h"

int main()
{
	jp::Tree<int, int> treetree;
	treetree.insert(8, 80);
	treetree.insert(3, 30);
	treetree.insert(1, 10);
	treetree.insert(6, 60);
	treetree.insert(7, 70);
	treetree.insert(4, 40);
	treetree.insert(10, 100);
	treetree.insert(14, 140);
	treetree.insert(13, 130);

	jp::Tree<int, int> tree;
	tree = treetree;

	for (jp::Tree<int, int>::iterator it = tree.begin(); it != tree.end(); ++it)
	{
		std::cout << it->first << " -> " << it->second << std::endl;
	}
	std::cout << std::endl;

	for (const auto& [key, value] : tree)
	{
		std::cout << key << " -> " << value << std::endl;
	}

	jp::Tree<int, int>::iterator it1 = tree.find(8);
	if (it1 != tree.end())
	{
		std::cout << (*it1).second << std::endl;
	}

	tree.print();

	tree.remove(6);

	std::cout << std::endl;
	tree.print();

	return 0;
}
