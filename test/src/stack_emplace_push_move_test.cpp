#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, emplace_push_move)
{
	using utils::storage::Stack;
	Stack<int> s;
	s.emplace(1);

	ASSERT_TRUE(s.getSize() == 1);
}