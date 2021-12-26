#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

//#include <arm_neon.h>
#include "sse_to_neon.hpp"
#include "noname.hpp"	// cryptonight_aesni.h



//#define GETOPTIMAL
#ifdef GETOPTIMAL
template<size_t N>
struct GetOptimalSqrtType
{
        using type = __m128i;
};

template<>
struct GetOptimalSqrtType<1u>
{
        using type = uint64_t;
};
template<size_t N>
using GetOptimalSqrtType_t = typename GetOptimalSqrtType<N>::type;
#endif



int main(int argc, char** argv){
	__m128i out1, out2;
	unsigned char dataArr[128];
	unsigned char dataOut[128];
	uint64_t* h0;
	uint64_t temp64;
	int64_t  iTemp64;
#ifdef GETOPTIMAL
	GetOptimalSqrtType_t<0> sqrt_result;
#endif

	printf("arm neon test\n");

	// 1> _mm_load_si128() test
	printf("_mm_load_si128() test!!\n");

	dataArr[0] = 0x01;
	dataArr[1] = 0x02;
	dataArr[2] = 0x03;
	dataArr[3] = 0x04;
	dataArr[4] = 0x05;
	dataArr[5] = 0x06;
	dataArr[6] = 0x07;
	dataArr[7] = 0x08;
	dataArr[8] = 0x09;
	dataArr[9] = 0x0a;
	dataArr[10] = 0x0b;
	dataArr[11] = 0x0c;
	dataArr[12] = 0x0d;
	dataArr[13] = 0x0e;
	dataArr[14] = 0x0f;
	dataArr[15] = 0x10;

	out1 = _mm_load_si128((__m128i*)dataArr);

	_mm_storeu_si128((__m128i*)dataOut, out1);
	for(int i=0;i<16;i++){
		printf("%02xh, ", dataOut[i]);
	}
	printf("\n\n");

	// 2> _mm_cvtsi64_si128() test
	printf("_mm_load_si128() test!!\n");

	temp64 = 0x1234567887654321;
	temp64 = -65535;
	iTemp64 = -65535;
	//out1 = (__m128i)vmovq_n_u64(temp64);
	//out1 = (__m128i)vdupq_lane_u32((uint32x2_t)temp64, 0);
	out1 = (__m128i)vmovq_n_u16(0);
	//out1 = (__m128i)vld1q_u64(&temp64);
	out1 = (__m128i)vld1q_s64(&iTemp64);
	//out1 = vld1q_u64(&temp64);

	for(int i=0;i<16;i++){
		dataOut[i] = 0;
	}

	_mm_storeu_si128((__m128i*)dataOut, out1);
	for(int i=0;i<16;i++){
		printf("%02xh, ", dataOut[i]);
	}
	printf("\n");
	iTemp64 = (int64_t)&dataOut[0];
	printf("iTemp64 = %jd\n", iTemp64);	// signed int 64bit
	//printf("iTemp64 = %ju\n", iTemp64);	// unsigned int 64bit
	printf("\n");

	// 3>

	h0 = (uint64_t*)dataArr;

#ifdef GETOPTIMAL
	assign(sqrt_result, h0[0]);
#endif

	return 0;
}
