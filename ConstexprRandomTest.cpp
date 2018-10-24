/*
 * ContexprRandomTest.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: connor
 */
#include <ConstexprRandom.hpp>
#include <iostream>
#include <iomanip>
#include <type_traits>


static_assert(ConstexprRandom{genConstexprSeed()}.nextBoolean(),"Failed to generate random number");

int main(){
	std::cout << std::boolalpha << ConstexprRandom{genConstexprSeed()}.nextBoolean() << std::endl;
	return 0;
}


