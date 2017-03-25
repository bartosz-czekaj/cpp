#include <iostream>
#include <limits>

template<typename TExpected, typename TGiven>
struct GetMinMaxGiven
{
private:
	template<typename Type>
	constexpr static Type min()
	{
		return  std::numeric_limits<Type>::min();
	}
	template<typename Type>
	constexpr static Type max()
	{
		return  std::numeric_limits<Type>::max();
	}
public:
	constexpr static TGiven maxValue()
	{
		return ((sizeof(TExpected) > sizeof(TGiven)) || ((sizeof(TExpected) == sizeof(TGiven)) && (std::is_signed<TGiven>::value && !std::is_signed<TExpected>::value))) 
			? max<TGiven>()
			: max<TExpected>();
	}

	constexpr static TGiven minValue()
	{
		return (std::is_signed<TGiven>::value == false || std::is_signed<TExpected>::value == false)
			? 0
			: ((sizeof(TExpected) > sizeof(TGiven))
				? min<TGiven>()
				: min<TExpected>());
	}
};


template<typename TExpected, typename TGiven>
constexpr TExpected narrowCast(TGiven given)
{
	static_assert(std::is_integral<TExpected>::value, "Only integer types are supported");
	static_assert(std::is_integral<TGiven>::value, "Only integer types are supported");

	return (GetMinMaxGiven<TExpected, TGiven>().minValue() >= given
			? GetMinMaxGiven<TExpected, TGiven>().minValue()
			: (GetMinMaxGiven<TExpected, TGiven>().maxValue() <= given
				? GetMinMaxGiven<TExpected, TGiven>().maxValue()
				: given));;
}

#define NARROW_CAST_TEST(x, y) static_assert(x == y, "Template narrowCast const expretion is broken for: " #x )

#define SHOW_VAR(x) #x << " == "<< x

#define EXPECT_EQ(x, y) if (x != y) std::cout << "Failed test for " << #x \
		<< "\nexpected: " << y \
		<< "\n  actual: " << x << std::endl; else std::cout<<"test passed\n"
int main()
{
	std::cout << SHOW_VAR(narrowCast<short>(100000)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned short>(100000)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned short>(-100000)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<short>(-100000)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<int>(-1)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<int>(4000000000u)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned int>(4000000000u)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned int>(-2000000000)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned int>(1)) << std::endl;
	std::cout << SHOW_VAR(narrowCast<unsigned short>(-1)) << std::endl;


	// to smaller type signed to signed
	EXPECT_EQ(narrowCast<short>(100000), 32767);
	EXPECT_EQ(narrowCast<short>(-100000), -32768);
	EXPECT_EQ(narrowCast<short>(0), 0);
	EXPECT_EQ(narrowCast<short>(-1), -1);

	// to smaller type unsigned to unsigned
	EXPECT_EQ(narrowCast<unsigned short>(100000u), 65535u);
	EXPECT_EQ(narrowCast<unsigned short>(60000u), 60000u);
	EXPECT_EQ(narrowCast<unsigned short>(0u), 0u);
	EXPECT_EQ(narrowCast<unsigned short>(1u), 1u);

	// to smaller type signed to signed to unsigned
	EXPECT_EQ(narrowCast<unsigned short>(100000), 65535);
	EXPECT_EQ(narrowCast<unsigned short>(-100000), 0);
	EXPECT_EQ(narrowCast<unsigned short>(0), 0);
	EXPECT_EQ(narrowCast<unsigned short>(-1), 0);

	// to smaller type unsigned to signed
	EXPECT_EQ(narrowCast<short>(100000u), 32767);
	EXPECT_EQ(narrowCast<short>(60000u), 32767);
	EXPECT_EQ(narrowCast<short>(0u), 0);
	EXPECT_EQ(narrowCast<short>(1u), 1);

	// to same size type signed to signed ////
	EXPECT_EQ(narrowCast<int>(0x7fffffff), 0x7fffffff);
	EXPECT_EQ(narrowCast<int>(-2147483648LL), -2147483648LL);
	EXPECT_EQ(narrowCast<int>(0), 0);
	EXPECT_EQ(narrowCast<int>(-1), -1);

	// to same size type unsigned to unsigned
	EXPECT_EQ(narrowCast<unsigned int>(0xffffffffu), 0xffffffffu);
	EXPECT_EQ(narrowCast<unsigned int>(60000u), 60000u);
	EXPECT_EQ(narrowCast<unsigned int>(0u), 0u);
	EXPECT_EQ(narrowCast<unsigned int>(1u), 1u);

	// to same size type signed to signed to unsigned
	EXPECT_EQ(narrowCast<unsigned int>(0x7fffffff), 0x7fffffffu);
	EXPECT_EQ(narrowCast<unsigned int>(-2147483648LL), 0u);
	EXPECT_EQ(narrowCast<unsigned int>(0), 0u);
	EXPECT_EQ(narrowCast<unsigned int>(-1), 0u);

	// to same size type unsigned to signed
	EXPECT_EQ(narrowCast<int>(0xffffffffu), 0x7fffffff);
	EXPECT_EQ(narrowCast<int>(0x7fffffffu), 0x7fffffff);
	EXPECT_EQ(narrowCast<int>(0x80000000u), 0x7fffffff);
	EXPECT_EQ(narrowCast<int>(0u), 0);
	EXPECT_EQ(narrowCast<int>(1u), 1);

	// to bigger size type signed to signed ////
	EXPECT_EQ(narrowCast<int64_t>(0x7fffffff), 0x7fffffffll);
	EXPECT_EQ(narrowCast<int64_t>(-2147483648LL), -2147483648ll);
	EXPECT_EQ(narrowCast<int64_t>(0), 0);
	EXPECT_EQ(narrowCast<int64_t>(-1), -1ll);

	// to bigger size type unsigned to unsigned
	EXPECT_EQ(narrowCast<uint64_t>(0xffffffffu), 0xffffffffull);
	EXPECT_EQ(narrowCast<uint64_t>(60000u), 60000ull);
	EXPECT_EQ(narrowCast<uint64_t>(0u), 0ull);
	EXPECT_EQ(narrowCast<uint64_t>(1u), 1ull);

	// to bigger size type signed to signed to unsigned
	EXPECT_EQ(narrowCast<uint64_t>(0x7fffffff), 0x7fffffffull);
	EXPECT_EQ(narrowCast<uint64_t>(-2147483648LL), 0ull);
	EXPECT_EQ(narrowCast<uint64_t>(0), 0ull);
	EXPECT_EQ(narrowCast<uint64_t>(-1), 0ull);

	// to bigger size type unsigned to signed
	EXPECT_EQ(narrowCast<int64_t>(0xffffffffu), 0xffffffffll);
	EXPECT_EQ(narrowCast<int64_t>(0x7fffffffu), 0x7fffffffll);
	EXPECT_EQ(narrowCast<int64_t>(0x80000000u), 0x80000000ll);
	EXPECT_EQ(narrowCast<int64_t>(0u), 0ll);
	EXPECT_EQ(narrowCast<int64_t>(1u), 1ll);

	return 0;
}