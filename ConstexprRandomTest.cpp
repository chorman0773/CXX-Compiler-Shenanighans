/*
 * ContexprRandomTest.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: connor
 */
#include <ConstexprRandom.hpp>
#include <Maths.hpp>
#include <iostream>
#include <iomanip>
#include <type_traits>

struct ret_t{
	int i;
	bool b;
	double d;
	float g;
};

constexpr ret_t randomNumbers(){
	ConstexprRandom rand{genConstexprSeed()};
	return {rand.nextInt(),rand.nextBoolean(),rand.nextDouble(),rand.nextFloat()};
}

constexpr const bool val{ConstexprRandom{genConstexprSeed()}.nextBoolean()};
static_assert(val,"Random Number was false");
int main(){
	constexpr ret_t nums = randomNumbers();
	ConstexprRandom rand2{genConstexprSeed()};
	int i{0}; // @suppress("Statement has no effect")
	int i2{0};
	if constexpr(nums.b){
		rand2.nextInt();
	}
	i++;
	i2++;
	std::cout << rand2.nextInt() << std::endl
			<< std::boolalpha << rand2.nextBoolean() << std::endl;
	return rand2.nextBoolean();
}


