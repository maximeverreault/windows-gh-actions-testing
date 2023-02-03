/**
 * @file Tests.coo
 * @author Maxime Verreault
 * @date 2023-02-03
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief 
 */




/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <catch2/catch_test_macros.hpp>
#include "header.h"

TEST_CASE( "BasicTestSuite", "BasicTest" ) {
    REQUIRE( getState() == true );
}
