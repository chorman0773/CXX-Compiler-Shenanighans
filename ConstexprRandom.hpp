/*
 * ConstexprRandom.hpp
 *
 *  Created on: Oct 2, 2018
 *      Author: connor
 */

#ifndef __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49
#define __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49
#include <string>
#include <lclib-cxx/Config.hpp>
#include <lclib-cxx/TypeTraits.hpp>
#include <cstddef>
#include <cstdint>
#include <Maths.hpp>
#include <algorithm>

#ifndef __TS
#define __TS 1539121689889
#endif
#ifndef __RAND
#define __RAND sg53xnlYdLT2J/sbFkNvM4ekbtzMe96MyXHKlRLAAXw=
#endif
namespace detail{
	template<std::size_t N> constexpr uint64_t hash(const char(&str)[N]){
		uint64_t hash{15250827463389383777uLL};
		const uint64_t prime{2683043564619300371uLL};
		for(char c:str){
			if(c==0)
				break;
			hash *= prime;
			hash += (static_cast<uint64_t>(c)&0xff)*53703548477009uLL;
		}
		return hash;
	}
	constexpr uint64_t hash(uint64_t u){
		uint64_t hash{15250827463389383777uLL};
		const uint64_t prime{2683043564619300371uLL};
		for(int i = 0;i<8;i++)
			(hash*=prime,hash+=u*53703548477009uLL,u>>8);
		return hash;
	}
	constexpr uint64_t initRandomizeSeed(uint64_t seed){
		return (seed ^ 0x5DEECE66DL) & ((1L << 48) - 1);
	}
}
#define STRINGIFY0(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY0(__VA_ARGS__)
#define EXPAND(...) __VA_ARGS__
#ifndef __COUNTER__
#define genConstexprSeed()\
	detail::hash(__FILE__ " " __DATE__ " " __TIME__ )\
		+detail::hash(__LINE__)\
		+detail::hash(__cplusplus)\
		+detail::hash(__TIMESTAMP__)\
		+detail::hash(STRINGIFY(__RAND))\
		+detail::hash(__TS)
#else
#define genConstexprSeed()\
	detail::hash(__FILE__ " " __DATE__ " " __TIME__ )\
		+detail::hash(__LINE__)\
		+detail::hash(__cplusplus)\
		+detail::hash(__TIMESTAMP__)\
		+detail::hash(STRINGIFY(__RAND))\
		+detail::hash(__TS)\
		+detail::hash(__COUNTER__)
#endif


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

		int bits{0}, val{0};
		do {
			bits = next(31);
			val = bits % bound;
		} while (bits - val + (bound-1) < 0);
			return val;
	}
	constexpr int64_t nextLong(){
		return next(32)<<32LL|next(32);
	}
	constexpr float nextFloat(){
		return  next(24) / ((float)(1 << 24));
	}
	constexpr double nextDouble(){
		return ((next(26) << 27uLL) + next(27))/((double)(1L << 53));
	}
	constexpr bool nextBoolean(){
		return next(1)!=0;
	}
	constexpr double nextGaussian(){
		if (haveNextNextGaussian) {
				haveNextNextGaussian = false;
				return nextNextGaussian;
		} else {
			double v1{0.0}, v2{0.0}, s{0.0};
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
	template<typename Byte,std::size_t N,typename=std::enable_if_t<is_byte_v<Byte>>> constexpr void nextBytes(Byte(&arr)[N]){
		for (int i = 0; i < N; )
			for (int rnd = nextInt(), n = std::min<std::size_t>(N - i, 4);
				  n-- > 0; rnd >>= 8)
				 arr[i++] = (char)rnd;
	}
};



#endif /* __CONSTEXPRRANDOM_HPP__2018_10_02_20_27_49 */
