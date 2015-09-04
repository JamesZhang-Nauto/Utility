#include "gtest/gtest.h"
#include "statistics.h"
#include <utility> 

TEST(Statistics_test, find_peak)
{
	vector<double> input = { 1,2,3,4,5 };
	unsigned int lower = 0, higher = 10;
	pair<double, double> ret = statistics::find_peak(input, 1, 0);
	ASSERT_EQ(4, ret.first);
	ASSERT_EQ(5, ret.second);


}