#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, remove)
{
	using utils::storage::List;
	List<int> l;
	l.insert(1);
	l.remove(1);

	ASSERT_TRUE(l.getSize() == 0);
}