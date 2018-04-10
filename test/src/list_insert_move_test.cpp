#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, insert_move)
{
	using utils::storage::List;
	List<int> l;

	l.insert(1);

	ASSERT_TRUE(l.getSize() == 1);
}