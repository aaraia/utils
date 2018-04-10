#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, emplace_copy)
{
	using utils::storage::List;
	List<int> l;

	int num = 1;
	l.emplace(num);

	ASSERT_TRUE(l.getSize() == 1);
}