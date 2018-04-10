#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, insert_copy)
{
	using utils::storage::List;
	List<int> l;

	int num = 1;
	l.insert(num);

	ASSERT_TRUE(l.getSize() == 1);
}