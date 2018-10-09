/*
 * SHA2.hpp
 *
 *  Created on: Oct 9, 2018
 *      Author: connor
 */

#ifndef __SHA2_HPP__2018_10_09_08_23_01
#define __SHA2_HPP__2018_10_09_08_23_01
#include <lclib-cxx/TypeTraits.hpp>
typedef unsigned word;
typedef unsigned long long lword;
namespace detail{

	constexpr const std::size_t k[] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	constexpr std::size_t blockSize(std::size_t N){
		N = N+9;
		return N+(64-(N%64))-1;
	}
	constexpr word fromBytes(const unsigned char*arr){
		return static_cast<word>(arr[0])<<24|static_cast<word>(arr[1])<<16
				|static_cast<word>(arr[2])<<8|static_cast<word>(arr[3]);
	}
	template<typename Byte> constexpr void toBytes(Byte* out,word w){
		out[0] = w>>24;
		out[1] = w>>16;
		out[2] = w>>8;
		out[3] = w;
	}
	constexpr word rrotate(word w,std::size_t b){
		b &=0x1f;
		return w>>b|w<<(32-b);
	}
	template<typename T> constexpr word zero(std::size_t){
		return 0;
	}
	template<typename T,std::size_t... Is> constexpr std::array<T,sizeof...(Is)> zero(std::index_sequence<Is...>){
		return {zero<T>(Is)...};
	}

	template<std::size_t N,typename Byte>
		constexpr void SHA2_1(const Byte(&arr)[N],word(&h)[8]){
		std::array<unsigned char,blockSize(N)> main{zero<unsigned char>(std::make_index_sequence<blockSize(N)>{})};
		const unsigned char* ptr = main;
		const std::size_t LenPos = blockSize(N)-8;
		const std::size_t Blocks = blockSize(N)/64;
		uint64_t szBits = N*8;
		for(std::size_t n=0;n<N;n++)
			main[n] = static_cast<unsigned char>(arr[n]);
		main[N] = 0x80;
		for(std::size_t n=N+1;n<LenPos;n++)
			main[n] = 0;
		main[LenPos] = szBits>>56;
		main[LenPos+1] = szBits>>48;
		main[LenPos+2] = szBits>>40;
		main[LenPos+3] = szBits>>32;
		main[LenPos+4] = szBits>>24;
		main[LenPos+5] = szBits>>16;
		main[LenPos+6] = szBits>>8;
		main[LenPos+7] = szBits;
		for(std::size_t q = 0;q<Blocks;q++){
			std::array<word,64> w{zero<word>(std::make_index_sequence<64>{})};
			for(std::size_t n=0;n<16;n++){
				w[n] = fromBytes(ptr);
				ptr+=4;
			}
			for(std::size_t n=16;n<64;n++){
				word s0 = rrotate(w[n-15],7)^rrotate(w[n-15],18)^(w[n-16]>>3);
				word s1 = rrotate(w[n-2],17)^rrotate(w[n-2],19)^(w[n-2]>>10);
				w[n] = w[n-16]+s0+w[n-7]+s1;
			}
			word a = h[0];
			word b = h[1];
			word c = h[2];
			word d = h[3];
			word e = h[4];
			word f = h[5];
			word g = h[6];
			word i = h[7];
			for(std::size_t n=0;n<64;n++){
				word S1 = rrotate(e,6)^rrotate(e,11)^rrotate(e,25);
				word ch = (e&f)^((~e)&g);
				word temp1 = i + S1 + ch + k[n]+w[n];
				word S0 = rrotate(a,2)^rrotate(a,13)^rrotate(a,22);
				word maj = (a&b)^(a&c)^(b&c);
				word temp2 = S0+maj;
				i = g;
				g = f;
				f = e;
				e = d+temp1;
				d = c;
				c = b;
				b = a;
				a = temp1+temp2;
			}
			h[0] += a;
			h[1] += b;
			h[2] += c;
			h[3] += d;
			h[4] += e;
			h[5] += f;
			h[6] += g;
			h[7] += i;
		}
	}
	constexpr std::size_t lblockSize(std::size_t N){
		N += 9;
		return N+(128-(N%128))-1;
	}
	constexpr lword lfromBytes(const unsigned char* bytes){
		return static_cast<lword>(bytes[0])<<56|static_cast<lword>(bytes[1])<<48
				|static_cast<lword>(bytes[2])<<40|static_cast<lword>(bytes[3])<<32
				|static_cast<lword>(bytes[4])<<24|static_cast<lword>(bytes[5])<<16
				|static_cast<lword>(bytes[6])<<8|static_cast<lword>(bytes[7]);
	}
	template<typename Byte> constexpr void ltoBytes(Byte* bytes,lword w){
		bytes[0] = w>>56;
		bytes[1] = w>>48;
		bytes[2] = w>>40;
		bytes[3] = w>>32;
		bytes[4] = w>>24;
		bytes[5] = w>>16;
		bytes[6] = w>>8;
		bytes[7] = w;
	}
	constexpr lword rrotate(lword w,std::size_t n){
		n &=0x3f;
		return w>>n|w<<(64-n);
	}
	constexpr const lword lk[] ={
		 0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
		 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
		 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
		 0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
		 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
		 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
		 0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
		 0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
		 0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
		 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
		 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
		 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
		 0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
		 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
		 0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
		 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
	};
	template<std::size_t N,typename Byte>
		constexpr void SHA2_2(const Byte(&arr)[N],lword(&h)[8]){
		std::array<unsigned char,lblockSize(N)> main{zero<unsigned char>(std::make_index_sequence<lblockSize(N)>{})};
		const unsigned char* ptr = main;
		const std::size_t LenPos = lblockSize(N)-8;
		const std::size_t Blocks = lblockSize(N)/64;
		uint64_t szBits = N*8;
		for(std::size_t n=0;n<N;n++)
			main[n] = static_cast<unsigned char>(arr[n]);
		main[N] = 0x80;
		main[LenPos] = szBits>>56;
		main[LenPos+1] = szBits>>48;
		main[LenPos+2] = szBits>>40;
		main[LenPos+3] = szBits>>32;
		main[LenPos+4] = szBits>>24;
		main[LenPos+5] = szBits>>16;
		main[LenPos+6] = szBits>>8;
		main[LenPos+7] = szBits;
		for(std::size_t q = 0;q<Blocks;q++){
			std::array<lword,80> w{zero<lword>(std::make_index_sequence<64>{})};
			for(std::size_t n=0;n<16;n++){
				w[n] = lfromBytes(ptr);
				ptr+=4;
			}
			for(std::size_t n=16;n<80;n++){
				lword s0 = rrotate(w[n-15],1)^rrotate(w[n-15],8)^(w[n-16]>>7);
				lword s1 = rrotate(w[n-2],19)^rrotate(w[n-2],61)^(w[n-2]>>6);
				w[n] = w[n-16]+s0+w[n-7]+s1;
			}
			lword a = h[0];
			lword b = h[1];
			lword c = h[2];
			lword d = h[3];
			lword e = h[4];
			lword f = h[5];
			lword g = h[6];
			lword i = h[7];
			for(std::size_t n=0;n<64;n++){
				lword S1 = rrotate(e,14)^rrotate(e,18)^rrotate(e,41);
				lword ch = (e&f)^((~e)&g);
				lword temp1 = i + S1 + ch + k[n]+w[n];
				lword S0 = rrotate(a,28)^rrotate(a,34)^rrotate(a,39);
				lword maj = (a&b)^(a&c)^(b&c);
				lword temp2 = S0+maj;
				i = g;
				g = f;
				f = e;
				e = d+temp1;
				d = c;
				c = b;
				b = a;
				a = temp1+temp2;
			}
			h[0] += a;
			h[1] += b;
			h[2] += c;
			h[3] += d;
			h[4] += e;
			h[5] += f;
			h[6] += g;
			h[7] += i;
		}
	}
	constexpr const unsigned char SHA512_256_STR[]{'S','H','A','-','5','1','2','/','2','5','6'};
	constexpr const unsigned char SHA512_224_STR[]{'S','H','A','-','5','1','2','/','2','2','4'};
	template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
		constexpr void SHA512_N_IV_GEN(const Byte(&in)[N],lword(&out)[8]){
			lword h[] = {
				0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
				0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
			};
			for(std::size_t n =0;n<8;n++)
				h[n] = h[n]^0xa5a5a5a5a5a5a5a5;
			SHA2_2(in, h);
		}
}

template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA256(const Byte(&in)[N],Byte* out){
		word h[] = {
				0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
				0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
		};
		detail::SHA2_1(in, h);
		for(std::size_t n=0;n<8;n++){
			detail::toBytes(out, h[n]);
			out+=4;
		}
	}

template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA224(const Byte(&in)[N],Byte* out){
		word h[] = {
			0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
			0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
		};
		detail::SHA2_1(in, h);
		for(std::size_t n=0;n<7;n++){
			detail::toBytes(out, h[n]);
			out+=4;
		}
	}

template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA512(const Byte(&in)[N],Byte* out){
		lword h[] = {
			0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
			0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
		};
		detail::SHA2_2(in, h);
		for(std::size_t n=0;n<8;n++){
			detail::ltoBytes(out,h[n]);
			out+=8;
		}
	}

template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA384(const Byte(&in)[N],Byte* out){
		lword h[] = {
			0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
			0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
		};
		detail::SHA2_2(in, h);
		for(std::size_t n=0;n<6;n++){
			detail::ltoBytes(out,h[n]);
			out+=8;
		}
	}


template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA512_256(const Byte(&in)[N],Byte* out){
		lword h[8]={0,0,0,0,0,0,0,0};
		detail::SHA512_N_IV_GEN(detail::SHA512_256_STR, h);
		detail::SHA2_2(in, h);
		for(std::size_t n=0;n<4;n++){
			detail::ltoBytes(out, h[n]);
			out += 8;
		}
	}

template<std::size_t N,typename Byte,typename=std::enable_if_t<is_byte_v<Byte>>>
	constexpr void SHA512_224(const Byte(&in)[N],Byte* out){
		lword h[8]={0,0,0,0,0,0,0,0};
		detail::SHA512_N_IV_GEN(detail::SHA512_224_STR, h);
		detail::SHA2_2(in, h);
		for(std::size_t n=0;n<3;n++){
			detail::ltoBytes(out, h[n]);
			out += 8;
		}
		detail::toBytes(out,h[4]>>32);
	}






#endif /* __SHA2_HPP__2018_10_09_08_23_01 */
