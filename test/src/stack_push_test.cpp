#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, push)
{
	using utils::storage::Stack;
	Stack<int> s;
	s.push(1);

	ASSERT_TRUE(s.getSize() == 1);
}