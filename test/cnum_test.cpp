//#include "ToDoCore/ToDo.h"

#include <string>
  using std::string;

#include "gtest/gtest.h"
  using ::testing::Test;


int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

namespace cnum
{
namespace test
{
    class CNumTest : public Test
    {
    protected:
    	CNumTest(){}
        ~CNumTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}
    };

    TEST(CNumTest, sample_test)
	{
    	EXPECT_EQ(1, 1);
	}
}
}
