#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, emplace_move)
{
	using utils::storage::List;
	List<int> l;

	l.emplace(1);

	ASSERT_TRUE(l.getSize() == 1);
}