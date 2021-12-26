
/** optimal type for sqrt
 *
 * Depending on the number of hashes calculated the optimal type for the sqrt value will be selected.
 *
 * @tparam N number of hashes per thread
 */
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

/** assign a value and convert if necessary
 *
 * @param output output type
 * @param input value which is assigned to output
 * @{
 */
inline void assign(__m128i& output, const uint64_t input)
{
	//Original code
	//output = _mm_cvtsi64_si128(input);
	//output = (__m128i)vmovq_n_s16(0);
	output = (__m128i)vmovq_n_u16(0);
	output = (__m128i)vld1q_u64(&input);
}

inline void assign(uint64_t& output, const uint64_t input)
{
	output = input;
}

inline void assign(uint64_t& output, const __m128i& input)
{
	//output = _mm_cvtsi128_si64(input);
}
