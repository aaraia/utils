#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, move_constructor)
{
	using utils::storage::List;
	List<int> l2{ 1, 2, 3 };
	List<int> l1 = std::move(l2);

	ASSERT_TRUE(l1.getSize() == 3);
}