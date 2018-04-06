#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, resize)
{
	using utils::storage::Stack;
	Stack<int> testStack;

	std::size_t prevCapacity = testStack.getCapacity();
	
	testStack.push(1);

	std::size_t currentCapacity = testStack.getCapacity();

	ASSERT_TRUE(prevCapacity == 0 && currentCapacity == 5);
}