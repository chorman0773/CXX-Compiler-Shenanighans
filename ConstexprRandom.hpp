/*
 * ConstexprRandom.hpp
 *
 *  Created on: Oct 2, 2018
 *      Author: connor
 */

#ifndef __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49
#define __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49
#include <string>
#include <lclib-cxx/CppHelpers.hpp>
#include <cstddef>
#include <cstdint>
#include <Maths.hpp>
namespace detail{
	template<std::size_t N> constexpr uint64_t hash(const char(&str)[N]){
		char nhash[25];
		uint64_t hash{15250827463389383777};
		const uint64_t prime{2683043564619300371};
		nameHash(str,nhash);
		for(char c:nhash){
			if(c==0)
				break;
			hash *= prime;
			hash += (c&0xff)*53703548477009;
		}
		return hash;
	}
	constexpr uint64_t initRandomizeSeed(uint64_t seed){
		return (seed ^ 0x5DEECE66DL) & ((1L << 48) - 1);
	}
}
#define STRINGIFY(...) #__VA_ARGS__
#define genConstexprSeed() detail::hash(__FILE__ __DATE__ __TIME__ STRINGIFY(__LINE__))


class ConstexprRandom{
private:
	uint64_t seed;
	bool haveNextNextGaussian;
	double nextNextGaussian{0};
	constexpr uint32_t next(int bits){
		seed = (seed*0x5DEECE66DL +0xBL)& ((1L << 48) - 1);
		return seed>>(48-bits);
	}
public:
	constexpr ConstexprRandom(uint64_t seed):seed{detail::initRandomizeSeed(seed)},haveNextNextGaussian{false}{}

	constexpr void setSeed(uint64_t seed){
		this->seed = detail::initRandomizeSeed(seed);
		this->haveNextNextGaussian = false;
	}
	constexpr int nextInt(){
		return next(32);
	}
	constexpr int nextInt(int bound){
		if(bound<=0)
			throw "Bound must greater then 0";

		if ((bound & -bound) == bound)  // i.e., bound is a power of 2
			return (int)((bound * (uint64_t)next(31)) >> 31);

		int bits, val;
		do {
			bits = next(31);
			val = bits % bound;
		} while (bits - val + (bound-1) < 0);
			return val;
	}
	constexpr int64_t nextLong(){
		return nextInt()<<32L|nextInt();
	}
	constexpr float nextFloat(){
		return  next(24) / ((float)(1 << 24));
	}
	constexpr double nextDouble(){
		return ((uint64_t)next(26) << 27 + next(27))/((double)(1L << 53));
	}
	constexpr bool nextBoolean(){
		return next(1)!=0;
	}
	constexpr double nextGuassian(){
		if (haveNextNextGaussian) {
				haveNextNextGaussian = false;
				return nextNextGaussian;
		} else {
			double v1, v2, s;
			do {
				v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
				v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
				s = v1 * v1 + v2 * v2;
			} while (s >= 1 || s == 0);
			double multiplier = cxmaths::sqrt(-2 * cxmaths::log(s)/s);
			nextNextGaussian = v2 * multiplier;
			haveNextNextGaussian = true;
			return v1 * multiplier;
		}
	}
};



#endif /* __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49 */
