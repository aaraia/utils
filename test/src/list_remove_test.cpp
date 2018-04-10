#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, remove)
{
	using utils::storage::List;
	List<int> l;

	l.insert(1);
	l.insert(2);
	l.insert(3);
	l.insert(4);

	l.remove(1);
	l.remove(4);
	l.remove(2);
	l.remove(3);

	ASSERT_TRUE(l.getSize() == 0);
}