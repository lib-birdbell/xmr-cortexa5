#include <stdio.h>
#include <inttypes.h>	// uint64_t
#include <x86intrin.h>


int main(int argc, char** argv){
	__m128i out1, out2;
	unsigned char dataArr[128];
	unsigned char dataOut[128];
	uint64_t temp64;
	int64_t iTemp64;

	printf("x64 test\n");

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

	//temp64 = 0x1234567887654321;
	iTemp64 = -65535;

	out1 = _mm_cvtsi64_si128(iTemp64);

	for(int i=0;i<16;i++){
		dataOut[i] = 0;
	}

	_mm_storeu_si128((__m128i*)dataOut, out1);

	for(int i=0;i<16;i++){
		printf("%02xh, ", dataOut[i]);
	}
	printf("\n");

	printf("iTemp64 = %jd\n", (int64_t*)&dataOut[0]);

	return 0;
}
