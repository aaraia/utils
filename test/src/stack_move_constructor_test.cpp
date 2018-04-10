#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, move_constructor)
{
	using utils::storage::Stack;

	Stack<int> s2;
	s2.push(1);

	Stack<int> s1 = std::move(s2);;

	ASSERT_TRUE(s1.getSize() == 1);
}