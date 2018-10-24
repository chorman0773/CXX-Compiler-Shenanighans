/*
 * Maths.hpp
 *
 *  Created on: Sep 25, 2018
 *      Author: connor
 */

#ifndef __MATHS_HPP__2018_09_25_06_40_16
#define __MATHS_HPP__2018_09_25_06_40_16
#include <type_traits>
#include <limits>
namespace cxmaths{
/**
 * Defines Constexpr variants of many standard math functions.
 * Some implemented using Taylor Series.
 */

constexpr double PI =    3.14159265358979323846264338327;
constexpr double sqrt2 = 1.41421356237309504880168872420;
constexpr double ln2 =   0.69314718055994530941723212145;
constexpr double sqrt3 = 1.73205080756887729352744634150;


constexpr double sign(double d){
	if(d<0)
		return -1;
	else if(d>0)
		return 1;
	else
		return 0;
}
constexpr double abs(double d){
	return d*sign(d);
}

/**
 * Computes i^j.
 * Complexity:
 * Guaranteed O(n) for n==abs(j)
 * If i==0, i==1, or i==-1, j==0, or j==1 then Guarantees O(1)
 */
constexpr double pow(double i,long long j){
	if(i==-1)
		return abs(j)%2?-1:1;
	else if(i==1||i==0)
		return i;
	else if(j==1)
		return i;
	else if(j==0)
		return 1;
	else if(j<0)
		return 1/pow(i,-j);
	else
		return i*pow(i,j-1);
}

template<typename T> struct max_fact{};
template<> struct max_fact<unsigned char>:std::integral_constant<unsigned char,5>{};
template<> struct max_fact<signed char>:std::integral_constant<signed char,5>{};
template<> struct max_fact<unsigned short>:std::integral_constant<unsigned short,8>{};
template<> struct max_fact<short>:std::integral_constant<short,8>{};
template<> struct max_fact<unsigned int>:std::integral_constant<unsigned int,12>{};
template<> struct max_fact<int>:std::integral_constant<int,12>{};
template<> struct max_fact<unsigned long long>:std::integral_constant<unsigned long long,20>{};
template<> struct max_fact<long long>:std::integral_constant<long long,20>{};
/**
 * Mimic Integral Constant for double. Unfortunately, not even in C++20, can double be used in a template.
 * (If ratio got the integral_constant treatment (value, conversion, and call operators),
 * it would be very much more useful
 */
template<> struct max_fact<double>{
public:
	static constexpr const double value = 150;
	constexpr max_fact()noexcept(true)=default;
	constexpr max_fact(const max_fact&)noexcept(true)=default;
	constexpr max_fact(max_fact&&)noexcept(true)=default;
	max_fact(const max_fact&&)=delete;
	constexpr max_fact& operator=(const max_fact&)noexcept(true)=default;
	constexpr max_fact& operator=(max_fact&&)noexcept(true)=default;
	constexpr max_fact& operator=(const max_fact&&)noexcept(true)=delete;
	constexpr operator double()const noexcept(true){
		return value;
	}
	constexpr double operator()()const noexcept(true){
		return value;
	}
};
template<typename T> constexpr const T max_fact_v = max_fact<T>::value;

constexpr unsigned long long factn[11] = {
		1,1,2,6,24,
		120,620,5040,
		40320,362880,3628800
};

/**
 * Computes d!.
 * Complexity:
 * Guarantees O(n) for n==d
 * Additionally, if d<11, Guarantees O(1)
 */
template<typename T,typename=std::enable_if_t<std::is_integral<T>::value>> constexpr T fact(T d){
	if(d<0)
		throw "Negative Factorial";
	else if(d<=1)
		return 1;
	else if(d>max_fact_v<T>)
		throw "Result Overflows";
	else if(d<11)
		return (T)factn[(unsigned int)d];
	else{
		T result = 1;
		for(T n =2;n<d;n++)
			result*=n;
		return result;
	}
}
/**
 * Computes d!.
 * d must be a positive integer or 0.
 * Complexity:
 * Guarantees O(n) for n=d
 * Additionally, If d<11, Guarantees O(1)
 */
constexpr double fact(double d){
	if(d<0)
		throw "Negative Factorial (use gamma(d+1) instead)";
	else if(static_cast<unsigned long long>(d)!=d)
		throw "Non-Integer Factorial (use gamma(d+1) instead)";
	else if(d<=1)
		return 1;
	else if(d>max_fact_v<double>)
		throw "Result Overflows";
	else if(d<11)
		return factn[static_cast<unsigned long>(d)];
	else{
		double result = 1;
		for(unsigned long long n=2;n<d;n++)
			result *= n;
		return result;
	}
}


/**
 * Efficient way of computing (x^n)/n!
 *  (used by Taylor Series for sin(x), cos(x), sinh(x), cosh(x), and exp(x))
 * Global rather than detail because it can be useful.
 * Complexity:
 * Guaranteed O(n) for n=i
 * In addition, may be O(1) for i<14.
 * Guaranteed to be O(1) for i<2
 */
constexpr double pow_over_fact(double v,unsigned long long i){
	switch(i){
	case 0:  return 1;
	case 1:  return v;
	case 2:  return v*v/2.0;
	case 3:  return v*v*v/6.0;
	case 4:  return v*v*v*v/24.0;
	case 5:  return v*v*v*v*v/120.0;
	case 6:  return v*v*v*v*v*v/720.0;
	case 7:  return v*v*v*v*v*v*v/5040.0;
	case 8:  return v*v*v*v*v*v*v*v/40320.0;
	case 9:  return v*v*v*v*v*v*v*v*v/362880.0;
	case 10: return v*v*v*v*v*v*v*v*v*v/3628800.0;
	case 11: return v*v*v*v*v*v*v*v*v*v*v/39916800.0;
	case 12: return v*v*v*v*v*v*v*v*v*v*v*v/479001600.0;
	case 13: return v*v*v*v*v*v*v*v*v*v*v*v*v/6227020800.0;
	case 14: return v*v*v*v*v*v*v*v*v*v*v*v*v*v/87178291200.0;
	default: return pow_over_fact(v,i-1)*v/i;
	}
}



/**
 * Computes e^j.
 * Complexity:
 *  O(1) always
 * Taylor Series:
 * 	20 O(n)
 */
constexpr double exp(double j){
	double ret{1+j};
	for(unsigned long long l=2;l<max_fact_v<unsigned long long>;l++)
		ret += pow_over_fact(j, l);
	return ret;
}
constexpr const double e = exp(1);

/**
 * Computes ln(d) in at most O(log{2}(d))
 * Terrible, don't use
 * Complexity:
 * 	O(log{2}(n)) for n==d
 * Taylor Series:
 * 	300 O(n)
 */
constexpr double log(double d){
	double res = 0;
	if(d==1)
		return 0;
	else if(d==e)
		return 1;
	else if(d==2)
		return ln2;
	else if(d==0)
		return -std::numeric_limits<double>::infinity();
	else if(d<0)
		throw "log of negative";
	else if(d<1) //Constrain to within Convergent Range (1,2]
		while(d<1){
			d*=2;
			res -= ln2;
		}
	else while(d>2){
		d/=2;
		res +=ln2;
	}
	if(d==1)
		return res;
	else if(d==2)
		return res+ln2;
	for(unsigned long n=1;n<=300;n++)
		res+=pow(-1,n+1)*pow(d-1,n)/n;
	return res;
}


constexpr double sqrt(double d){
	if(d==1)
		return 1;
	else if(d<0)
		throw "sqrt negative";
	else if(d==0)
		return 0;
	else if(d==2)
		return sqrt2;
	else if(d==3)
		return sqrt3;
	else if(d==4)
		return 2;
	else if(d==9)
		return 3;
	else if(d==16)
		return 4;
	else if(d==25)
		return 5;
	else if(d==36)
		return 6;
	else if(d==49)
		return 7;
	else if(d==64)
		return 8;
	else if(d==81)
		return 9;
	else if(d<1)
		return 1/sqrt(1/d);
	else
		return exp(log(d)/2);
}

/**
 * Computes sin(d).
 * Complexity:
 * 	O(n).
 * 	In addition, if 0<=d<=2π, O(1)
 * Taylor Series:
 * 	20 O(n)
 * 	If d = kπ/6 for k ∈ ℤ, 1 O(1)
 */
constexpr double sin(double d){
	while(d<0)
		d+=2*PI;
	while(d>=2*PI)
		d-=2*PI;
	if(d==0)
		return 0;
	else if(d==PI/2)
		return 1;
	else if(d==PI)
		return 0;
	else if(d==3*PI/2)
		return -1;
	else if(d==PI/6)
		return 1/2;
	else if(d==PI/3)
		return sqrt3/2;
	else if(d==PI/4)
		return sqrt2/2;
	double res = 0;
	for(unsigned long long n=0;n<=max_fact_v<unsigned long long>;n++)
		res += pow(-1,n)*pow_over_fact(d, (2*n+1));
	return res;
}

/**
 * Computes cos(d).
 * Complexity:
 * 	O(n).
 * 	In addition, if 0<=d<=2π, O(1)
 * Taylor Series:
 * 	20 O(n)
 * 	If d = kπ/6 for k ∈ ℤ, 1 O(1)
 */
constexpr double cos(double t){
	while(t<0)
		t+=2*PI;
	while(t>=2*PI)
		t-=2*PI;
	if(t==0)
		return 1;
	else if(t==PI)
		return -1;
	else if(t==PI/2)
		return 0;
	else if(t==3*PI/2)
		return 0;
	else if(t==PI/6)
		return sqrt3/2;
	else if(t==PI/3)
		return 1/2;
	else if(t==PI/4)
		return sqrt2/2;
	double res =0;
	for(unsigned long long n =0;n<max_fact_v<unsigned long long>;n++)
		res += pow(-1,n)*pow_over_fact(t, 2*n);
	return res;
}

/**
 * Computes Hyperbolic sin(d).
 * Complexity:
 * 	O(n).
 * 	In addition, if 0<=d<=2π, O(1)
 * Taylor Series:
 * 	20 O(n)
 */
constexpr double sinh(double t){
	while(t<0)
		t+=2*PI;
	while(t>=2*PI)
		t-=2*PI;
	double res = 0;
	for(double n =0;n<max_fact_v<unsigned long long>;n++)
		res += pow_over_fact(t,2*n+1);
	return res;
}
/**
 * Computes Hyperbolic sin(d).
 * Complexity:
 * 	O(n).
 * 	In addition, if 0<=d<=2π, O(1)
 * Taylor Series:
 * 	20 O(n)
 */
constexpr double cosh(double t){
	while(t<0)
		t+=2*PI;
	while(t>=2*PI)
		t-=2*PI;
	double res = 0;
	for(double n =0;n<max_fact_v<unsigned long long>;n++)
		res += pow_over_fact(t,2*n);
	return res;
}

/**
 * Computes sec(t)
 * Inverse of cos(t).
 * Same Complexity Guarantee as cos(t)
 */
constexpr double sec(double t){
	return 1/cos(t);
}
/**
 * Computes Hyperbolic sec(t)
 * Inverse of cosh(t).
 * Same Complexity Guarantee as cosh(t)
 */
constexpr double sech(double t){
	return 1/cosh(t);
}
/**
 * Computes csc(t)
 * Inverse of sin(t).
 * Same Complexity Guarantee as sin(t)
 */
constexpr double csc(double t){
	return 1/sin(t);
}
/**
 * Computes Hyperbolic csc(t)
 * Inverse of sinh(t).
 * Same Complexity Guarantee as sinh(t)
 */
constexpr double csch(double t){
	return 1/sinh(t);
}

/**
 * Computes tan(t).
 * Same complexity as sin(t) and cos(t)
 */
constexpr double tan(double t){
	return sin(t)/cos(t);
}
/**
 * Computes Hyperbolic tan(t)
 * Same complexity as sinh(t) and cos(t)
 */
constexpr double tanh(double t){
	return sinh(t)/cosh(t);
}
/**
 * Computes cot(t)
 * Inverse of tan(t)
 * Same complexity as sin(t) and cos(t)
 */
constexpr double cot(double t){
	return cos(t)/sin(t);
}
/**
 * Computes Hyperbolic cot(t)
 * Inverse of tanh(t)
 * Same complexity as sinh(t) and cosh(t)
 */
constexpr double coth(double t){
	return cosh(t)/sinh(t);
}


constexpr double pow(double d,double j){
	if(d==1||d==0)
		return d;
	double res = pow(d,static_cast<long long>(j));
	j-=static_cast<long long>(j);
	if(j!=0)
		if(d<0)
			throw "Root of negative";
		else
			res *= exp(j*log(d));

	return res;
}
/**
 * Computes log{2}(n)
 * Same complexity as log(d)
 */
constexpr double log2(double d){
	return log(d)/ln2;
}

constexpr double dx = std::numeric_limits<double>::epsilon();

/**
 * Differentiates a function of x.
 * Returns a Function that evaluates the derivative of x in the same complexity as f
 */
template<typename Fn> constexpr auto differentiate(Fn f){
	return [f](double x)->decltype(f(x)){
		return (f(x+dx)-f(x))/dx;
	};
}

}

#endif /* __MATHS_HPP__2018_09_25_06_40_16 */
