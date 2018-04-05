#include <iostream>

#include "../../lib/include/stack.h"
#include "../../lib/include/list.h"
#include "../../lib/include/queue.h"
#include "../../lib/include/bst.h"

int main()
{
	using utils::storage::Stack;
	using utils::storage::CList;
	using utils::storage::Queue;
	using utils::storage::BST;


	Stack<int> stack;
	stack.push(1);
	stack.pop();

	CList<int> list;
	list.Insert(1);
	list.Remove(1);

	Queue<int> queue;
	queue.push_back(1);
	queue.pop_front();

	BST<int,int> bst;
	bst.insert(1,1);
	bst.remove(1);

	std::cout << "hello, world!" << std::endl;

	return 0;
}