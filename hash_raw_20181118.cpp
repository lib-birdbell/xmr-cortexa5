static void hash_raw(const void* input, size_t len, void* output, cryptonight_ctx** ctx) {
	uint64_t MASK = 2097136;
	uint64_t ITERATIONS = 524288;
	uint64_t MEM = 2097152;

	printer::inst()->print_msg(L0, "hash_test()");

	keccak((const uint8_t *)input, len, ctx[0]->hash_state, 200);

	// input (__m128i*)ctx[0]->hash_state
	// output (__m128i*)ctx[0]->long_state
	// KEY cn_explode_scratchpad() ======================================================================
	// This is more than we have registers, compiler will assign 2 keys on the stack
	__m128i xin0, xin1, xin2, xin3, xin4, xin5, xin6, xin7;
	__m128i k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;

	//aes_genkey<SOFT_AES>(input, &k0, &k1, &k2, &k3, &k4, &k5, &k6, &k7, &k8, &k9);
	__m128i xout0, xout2;

	xout0 = _mm_load_si128((__m128i*)ctx[0]->hash_state);
	xout2 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 1);
	k0 = xout0;
	k1 = xout2;

	
	soft_aes_genkey_sub(&xout0, &xout2, 0x01);
	
	k2 = xout0;
	k3 = xout2;

	soft_aes_genkey_sub(&xout0, &xout2, 0x02);
	k4 = xout0;
	k5 = xout2;

	soft_aes_genkey_sub(&xout0, &xout2, 0x04);
	k6 = xout0;
	k7 = xout2;

	soft_aes_genkey_sub(&xout0, &xout2, 0x08);
	k8 = xout0;
	k9 = xout2;
	////End of aes_genkey()

	xin0 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 4);
	xin1 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 5);
	xin2 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 6);
	xin3 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 7);
	xin4 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 8);
	xin5 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 9);
	xin6 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 10);
	xin7 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 11);

	for (size_t i = 0; i < MEM / sizeof(__m128i); i += 8)
	{
		soft_aes_round(k0, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k1, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k2, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k3, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k4, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k5, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k6, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k7, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k8, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);
		soft_aes_round(k9, &xin0, &xin1, &xin2, &xin3, &xin4, &xin5, &xin6, &xin7);

		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 0, xin0);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 1, xin1);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 2, xin2);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 3, xin3);

		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 4, xin4);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 5, xin5);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 6, xin6);
		_mm_store_si128((__m128i*)ctx[0]->long_state + i + 7, xin7);
	}
	// KEY end ======================================================================

	__m128i ax0;
	uint64_t idx0;
	__m128i bx0;
	uint8_t* l0 = ctx[0]->long_state;
	/* BEGIN cryptonight_monero_v8 variables */
	__m128i bx1;
	__m128i division_result_xmm;
	GetOptimalSqrtType_t<0> sqrt_result;
	/* END cryptonight_monero_v8 variables */
	{
		uint64_t* h0 = (uint64_t*)ctx[0]->hash_state;
		idx0 = h0[0] ^ h0[4];
		ax0 = _mm_set_epi64x(h0[1] ^ h0[5], idx0);
		bx0 = _mm_set_epi64x(h0[3] ^ h0[7], h0[2] ^ h0[6]);

		bx1 = _mm_set_epi64x(h0[9] ^ h0[11], h0[8] ^ h0[10]);
		division_result_xmm = _mm_cvtsi64_si128(h0[12]);
		assign(sqrt_result, h0[13]);
		set_float_rounding_mode();
	}

	__m128i *ptr0;

#if 0
	printer::inst()->print_msg(L0, "hash_raw() (uint32_t*)ctx[0]->hash_state[0]=%08x, %08x, %08x, %08x", (uint32_t*)ctx[0]->hash_state[0], (uint32_t*)ctx[0]->hash_state[1], (uint32_t*)ctx[0]->hash_state[2], (uint32_t*)ctx[0]->hash_state[3]);////debug
	printer::inst()->print_msg(L0, "hash_raw() (uint32_t*)ctx[0]->long_state[0]=%08x, %08x, %08x, %08x", (uint32_t*)ctx[0]->long_state[0], (uint32_t*)ctx[0]->long_state[1], (uint32_t*)ctx[0]->long_state[2], (uint32_t*)ctx[0]->long_state[3]);////debug
#endif

	// Optim - 90% time boundary
	for (size_t i = 0; i < ITERATIONS; i++)
	{
		__m128i cx;
		ptr0 = (__m128i *)&l0[idx0 & MASK];
		cx = _mm_load_si128(ptr0);

		cx = soft_aesenc(cx, ax0);

		{
			const uint64_t idx1 = idx0 & MASK;
			const __m128i chunk1 = _mm_load_si128((__m128i *)&l0[idx1 ^ 0x10]);
			const __m128i chunk2 = _mm_load_si128((__m128i *)&l0[idx1 ^ 0x20]);
			const __m128i chunk3 = _mm_load_si128((__m128i *)&l0[idx1 ^ 0x30]);
			_mm_store_si128((__m128i *)&l0[idx1 ^ 0x10], _mm_add_epi64(chunk3, bx1));
			_mm_store_si128((__m128i *)&l0[idx1 ^ 0x20], _mm_add_epi64(chunk1, bx0));
			_mm_store_si128((__m128i *)&l0[idx1 ^ 0x30], _mm_add_epi64(chunk2, ax0));
		}
		////debug End of REPEAT_1 CN_STEP1

		_mm_store_si128((__m128i *)ptr0, _mm_xor_si128(bx0, cx));
		idx0 = _mm_cvtsi128_si64(cx);

		ptr0 = (__m128i *)&l0[idx0 & MASK];
		////debug End of REPEAT_1 CN_STEP2

		uint64_t lo, cl, ch;
		uint64_t al0 = _mm_cvtsi128_si64(ax0);
		uint64_t ah0 = ((uint64_t*)&ax0)[1];
		cl = ((uint64_t*)ptr0)[0];
		ch = ((uint64_t*)ptr0)[1];
		{
			uint64_t sqrt_result_tmp;
			assign(sqrt_result_tmp, sqrt_result);
			/* Use division and square root results from the _previous_ iteration to hide the latency */
			const uint64_t cx_64 = _mm_cvtsi128_si64(cx);
			cl ^= static_cast<uint64_t>(_mm_cvtsi128_si64(division_result_xmm)) ^ (sqrt_result_tmp << 32);
			const uint32_t d = (cx_64 + (sqrt_result_tmp << 1)) | 0x80000001UL;
			/* Most and least significant bits in the divisor are set to 1
			* to make sure we don't divide by a small or even number,
			* so there are no shortcuts for such cases
			*
			* Quotient may be as large as (2^64 - 1)/(2^31 + 1) = 8589934588 = 2^33 - 4
			* We drop the highest bit to fit both quotient and remainder in 32 bits
			*/
			/* Compiler will optimize it to a single div instruction */
			const uint64_t cx_s = _mm_cvtsi128_si64(_mm_srli_si128(cx, 8));
			const uint64_t division_result = static_cast<uint32_t>(cx_s / d) + ((cx_s % d) << 32);
			division_result_xmm = _mm_cvtsi64_si128(static_cast<int64_t>(division_result));
			/* Use division_result as an input for the square root to prevent parallel implementation in hardware */
			assign(sqrt_result, int_sqrt33_1_double_precision(cx_64 + division_result));
		}
		////debug END of CN_MONERO_V8_DIV

		{
			uint64_t hi;
			lo = _umul128(idx0, cl, &hi);
			{
				const uint64_t idx1 = idx0 & MASK;
				const __m128i chunk1 = _mm_xor_si128(_mm_load_si128((__m128i *)&l0[idx1 ^ 0x10]), _mm_set_epi64x(lo, hi));
				const __m128i chunk2 = _mm_load_si128((__m128i *)&l0[idx1 ^ 0x20]);
				hi ^= ((uint64_t*)&chunk2)[0];
				lo ^= ((uint64_t*)&chunk2)[1];
				const __m128i chunk3 = _mm_load_si128((__m128i *)&l0[idx1 ^ 0x30]);
				_mm_store_si128((__m128i *)&l0[idx1 ^ 0x10], _mm_add_epi64(chunk3, bx1));
				_mm_store_si128((__m128i *)&l0[idx1 ^ 0x20], _mm_add_epi64(chunk1, bx0));
				_mm_store_si128((__m128i *)&l0[idx1 ^ 0x30], _mm_add_epi64(chunk2, ax0));
			}
			////debug End of CN_MONERO_V8_SHUFFLE_1
			ah0 += lo;
			al0 += hi;
		}
		
		bx1 = bx0;
		bx0 = cx;

		((uint64_t*)ptr0)[0] = al0;
		////debug End of REPEAT_1 CN_STEP3

		((uint64_t*)ptr0)[1] = ah0;
		al0 ^= cl;
		ah0 ^= ch;
		ax0 = _mm_set_epi64x(ah0, al0);
		idx0 = al0;
		////debug End of REPEAT_1 CN_STEP4

		////debug End of REPEAT_1 CN_STEP5
	}

#if 0
	printer::inst()->print_msg(L0, "hash_raw() (uint32_t*)ctx[0]->hash_state[0]=%08x, %08x, %08x, %08x", (uint32_t*)ctx[0]->hash_state[0], (uint32_t*)ctx[0]->hash_state[1], (uint32_t*)ctx[0]->hash_state[2], (uint32_t*)ctx[0]->hash_state[3]);////debug
	printer::inst()->print_msg(L0, "hash_raw() (uint32_t*)ctx[0]->long_state[0]=%08x, %08x, %08x, %08x", (uint32_t*)ctx[0]->long_state[0], (uint32_t*)ctx[0]->long_state[1], (uint32_t*)ctx[0]->long_state[2], (uint32_t*)ctx[0]->long_state[3]);////debug
#endif

	//cn_implode_scratchpad<MEM, true, false, cryptonight_monero_v8>((__m128i*)ctx[0]->long_state, (__m128i*)ctx[0]->hash_state);
	//input (__m128i*)ctx[0]->long_state
	//output (__m128i*)ctx[0]->hash_state
	//void cn_implode_scratchpad(const __m128i* input, __m128i* output)
	{
		// This is more than we have registers, compiler will assign 2 keys on the stack
		__m128i xout0, xout1, xout2, xout3, xout4, xout5, xout6, xout7;
		__m128i k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;

		//aes_genkey<SOFT_AES>(output + 2, &k0, &k1, &k2, &k3, &k4, &k5, &k6, &k7, &k8, &k9);
		////aes_genkey
		{
			__m128i xout00, xout02;

			xout00 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 2);
			xout02 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 2 + 1);
			k0 = xout00;
			k1 = xout02;

			soft_aes_genkey_sub(&xout00, &xout02, 0x01);

			k2 = xout00;
			k3 = xout02;

			soft_aes_genkey_sub(&xout00, &xout02, 0x02);

			k4 = xout00;
			k5 = xout02;

			soft_aes_genkey_sub(&xout00, &xout02, 0x04);
			k6 = xout00;
			k7 = xout02;


			soft_aes_genkey_sub(&xout00, &xout02, 0x08);

			k8 = xout00;
			k9 = xout02;
		}
		////End of aes_genkey

		xout0 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 4);
		xout1 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 5);
		xout2 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 6);
		xout3 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 7);
		xout4 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 8);
		xout5 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 9);
		xout6 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 10);
		xout7 = _mm_load_si128((__m128i*)ctx[0]->hash_state + 11);

		for (size_t i = 0; i < MEM / sizeof(__m128i); i += 8)
		{
			xout0 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 0), xout0);
			xout1 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 1), xout1);
			xout2 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 2), xout2);
			xout3 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 3), xout3);

			xout4 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 4), xout4);
			xout5 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 5), xout5);
			xout6 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 6), xout6);
			xout7 = _mm_xor_si128(_mm_load_si128((__m128i*)ctx[0]->long_state + i + 7), xout7);

			soft_aes_round(k0, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k1, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k2, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k3, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k4, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k5, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k6, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k7, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k8, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
			soft_aes_round(k9, &xout0, &xout1, &xout2, &xout3, &xout4, &xout5, &xout6, &xout7);
		}

		_mm_store_si128((__m128i*)ctx[0]->hash_state + 4, xout0);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 5, xout1);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 6, xout2);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 7, xout3);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 8, xout4);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 9, xout5);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 10, xout6);
		_mm_store_si128((__m128i*)ctx[0]->hash_state + 11, xout7);
	}
	////End of cn_implode_scratchpad

	////End of cn_implode_scratchpad
	/* Optim - 99% time boundary */
	keccakf((uint64_t*)ctx[0]->hash_state, 24);
	extra_hashes[ctx[0]->hash_state[0] & 3](ctx[0]->hash_state, 200, (char*)output + 32 * 0);
}
