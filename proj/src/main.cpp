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
	int testInt = 3;
	stack.push(testInt);
	stack.push(1);
	stack.emplace(2);
	int testInt2 = 4;
	stack.emplace(testInt2);
	stack.pop();
	stack.empty();
	int top = stack.top();

	{
		Stack<int> stackFrom;
		stackFrom.push(8);

		Stack<int> stackTo;

		stackTo = stackFrom;
	}

	{
		Stack<int> stackFrom;
		stackFrom.push(8);

		Stack<int> stackTo = stackFrom;
	}

	{
		Stack<int> stackFrom;
		stackFrom.push(8);

		Stack<int> stackTo;

		stackTo = std::move(stackFrom);
	}

	{
		Stack<int> stackFrom;
		stackFrom.push(8);

		Stack<int> stackTo = std::move(stackFrom);
	}

	CList<int> list;
	list.Insert(1);
	list.Remove(1);

	Queue<int> queue;
	queue.push_back(1);
	queue.push_front(4);
	queue.emplace_back(2);
	queue.emplace_front(3);
	queue.pop_front();
	queue.clear();

	{
		Queue<int> queueFrom;
		queueFrom.push_back(8);

		Queue<int> queueTo;

		queueTo = queueFrom;
	}

	{
		Queue<int> queueFrom;
		queueFrom.push_back(8);

		Queue<int> queueTo = queueFrom;
	}

	{
		Queue<int> queueFrom;
		queueFrom.push_back(8);

		Queue<int> queueTo;

		queueTo = std::move(queueFrom);
	}

	{
		Queue<int> queueFrom;
		queueFrom.push_back(8);

		Queue<int> queueTo = std::move(queueFrom);
	}


	BST<int,int> bst;
	bst.insert(1,1);
	bst.remove(1);

	std::cout << "hello, world!" << std::endl;

	return 0;
}