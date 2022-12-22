#include <gtest/gtest.h>

#include "globalincs/generational.h"
#include "tl/optional.hpp"

TEST(GenerationalIndex, access) {
	{
		gVector<float> data;
		gIndex first = data.add(1.0f);
		gIndex second = data.add(2.0f);
		gIndex third = data.add(3.0f);
		data.remove(second);
		gIndex fourth = data.add(4.0f); 
		auto bad_access = data[second];
		auto good_access = data[fourth];
		ASSERT_EQ(tl::nullopt, bad_access);
		ASSERT_EQ(4.0f, good_access);
	}
}
TEST(GenerationalIndex, indexes) {
	{
		gVector<float> data;
		gIndex first = data.add(1.0f);
		gIndex second = data.add(2.0f);
		gIndex third = data.add(3.0f);
		data.remove(second);
		gIndex fourth = data.add(4.0f);
		gIndex fifth = data.add(5.0f);
		ASSERT_EQ(0, first.index);
		ASSERT_EQ(0, first.generation);
		ASSERT_EQ(1, second.index);
		ASSERT_EQ(0, second.generation);
		ASSERT_EQ(2, third.index);
		ASSERT_EQ(0, third.generation);
		ASSERT_EQ(1, fourth.index);
		ASSERT_EQ(1, fourth.generation);
		ASSERT_EQ(3, fifth.index);
		ASSERT_EQ(0, fifth.generation);
	}
}