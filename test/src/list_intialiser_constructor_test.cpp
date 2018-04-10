#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, intialiser_constructor)
{
	using utils::storage::List;
	List<int> l{ 1, 2, 3 };


	ASSERT_TRUE(l.getSize() == 3);
}