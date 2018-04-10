#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, move_operator)
{
	using utils::storage::Stack;
	
	Stack<int> s1;
	Stack<int> s2;

	s2.push(1);

	s1 = std::move(s2);

	ASSERT_TRUE(s1.getSize() == 1);
}