#include "gtest/gtest.h"
#include "../../lib/include/stack.h"

TEST(stack, pop)
{
	using utils::storage::Stack;
	Stack<int> s;
	s.push(1);
	s.pop();
	
	ASSERT_TRUE(s.empty() == true);
}