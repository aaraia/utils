#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, emplace_push_copy)
{
	using utils::storage::Stack;
	Stack<int> s;

	int num = 1;
	s.emplace(num);

	ASSERT_TRUE(s.getSize() == 1);
}