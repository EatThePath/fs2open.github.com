#include <gtest/gtest.h>

#include "globalincs/generational.h"

TEST(GenerationalIndex, basics) {
	{
		gVector<float> ds;
		gIndex a = ds.add(1.0f);
		gIndex b = ds.add(2.0f);
		gIndex c = ds.add(3.0f);
		ds.remove(b);
		gIndex d = ds.add(4.0f);
		ASSERT_EQ(1, d.index);
	}
}