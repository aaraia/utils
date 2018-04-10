#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, copy_operator)
{
	using utils::storage::Stack;

	Stack<int> s1;
	Stack<int> s2;

	s2.push(42);

	s1 = s2;

	ASSERT_TRUE((s1.getSize() == 1) && (s2.getSize() == 1) & (s1.top() == 42) && (s2.top() == 42));
}