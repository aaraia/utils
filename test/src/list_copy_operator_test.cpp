#include "gtest/gtest.h"
#include "../../lib/include/list.h"

TEST(list, copy_operator)
{
    using utils::storage::List;
    List<int> l2{ 1, 2, 3 };
    List<int> l1;
        
    l1 = l2;

    ASSERT_TRUE((l1.getSize() == 3) && (l2.getSize() == 3));
}