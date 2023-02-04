/**
 * @file Tests
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief 
 */




/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "gtest/gtest.h"
#include "header.h"
using namespace ::testing;

 
TEST(BasicTestSuite, BasicTest)
{
    EXPECT_TRUE(getState());
}