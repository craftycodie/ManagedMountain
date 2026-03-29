#ifndef __CSERIES_MACROS_H__
#define __CSERIES_MACROS_H__
#pragma once

/* ---------- headers */

#include "cseries/constants.h"

/* ---------- constants */

#ifdef INT64_BITS
#undef INT64_BITS
#endif
#ifdef INT64_BITS_BITS
#undef INT64_BITS_BITS
#endif
#ifdef UNSIGNED_LONG_MAX
#undef UNSIGNED_LONG_MAX
#endif
#ifdef LONG_MAX
#undef LONG_MAX
#endif
#ifdef LONG_MIN
#undef LONG_MIN
#endif
#ifdef LONG_BITS
#undef LONG_BITS
#endif
#ifdef LONG_BITS_BITS
#undef LONG_BITS_BITS
#endif
#ifdef LONG_MASK
#undef LONG_MASK
#endif
#ifdef UNSIGNED_SHORT_MAX
#undef UNSIGNED_SHORT_MAX
#endif
#ifdef SHORT_BITS
#undef SHORT_BITS
#endif
#ifdef SHORT_BITS_BITS
#undef SHORT_BITS_BITS
#endif
#ifdef SHORT_MASK
#undef SHORT_MASK
#endif
#ifdef UNSIGNED_CHAR_MAX
#undef UNSIGNED_CHAR_MAX
#endif
#ifdef CHAR_MAX
#undef CHAR_MAX
#endif
#ifdef CHAR_MIN
#undef CHAR_MIN
#endif
#ifdef CHAR_BITS
#undef CHAR_BITS
#endif
#ifdef CHAR_BITS_BITS
#undef CHAR_BITS_BITS
#endif
#ifdef CHAR_MASK
#undef CHAR_MASK
#endif
#ifdef BYTE_BITS
#undef BYTE_BITS
#endif
#ifdef BYTE_BITS_BITS
#undef BYTE_BITS_BITS
#endif
#ifdef WCHAR_BITS
#undef WCHAR_BITS
#endif

enum
{
#ifndef INT64_BITS
	INT64_BITS = 64, // 0x0040
#endif // INT64_BITS 
#ifndef INT64_BITS_BITS
	INT64_BITS_BITS = 6, // 0x0006
#endif // INT64_BITS_BITS 
#ifndef UNSIGNED_LONG_MAX
	UNSIGNED_LONG_MAX = -1, // 0xFF
#endif // UNSIGNED_LONG_MAX 
#ifndef LONG_MAX
	LONG_MAX = 2147483647, // 0x7FFFFFFF
#endif // LONG_MAX 
#ifndef LONG_MIN
	LONG_MIN = -2147483648, // 0x80000000
#endif // LONG_MIN 
#ifndef LONG_BITS
	LONG_BITS = 32, // 0x0020
#endif // LONG_BITS 
#ifndef LONG_BITS_BITS
	LONG_BITS_BITS = 5, // 0x0005
#endif // LONG_BITS_BITS 
#ifndef LONG_MASK
	LONG_MASK = -1, // 0xFF
#endif // LONG_MASK 
#ifndef UNSIGNED_SHORT_MAX
	UNSIGNED_SHORT_MAX = 65535, // 0xFFFF
#endif // UNSIGNED_SHORT_MAX 
#ifndef SHORT_BITS
	SHORT_BITS = 16, // 0x0010
#endif // SHORT_BITS 
#ifndef SHORT_BITS_BITS
	SHORT_BITS_BITS = 4, // 0x0004
#endif // SHORT_BITS_BITS 
#ifndef SHORT_MASK
	SHORT_MASK = 65535, // 0xFFFF
#endif // SHORT_MASK 
#ifndef UNSIGNED_CHAR_MAX
	UNSIGNED_CHAR_MAX = 255, // 0x00FF
#endif // UNSIGNED_CHAR_MAX 
#ifndef CHAR_MAX
	CHAR_MAX = 127, // 0x007F
#endif // CHAR_MAX 
#ifndef CHAR_MIN
	CHAR_MIN = -128, // 0x80
#endif // CHAR_MIN 
#ifndef CHAR_BITS
	CHAR_BITS = 8, // 0x0008
#endif // CHAR_BITS 
#ifndef CHAR_BITS_BITS
	CHAR_BITS_BITS = 3, // 0x0003
#endif // CHAR_BITS_BITS 
#ifndef CHAR_MASK
	CHAR_MASK = 255, // 0x00FF
#endif // CHAR_MASK 
#ifndef BYTE_BITS
	BYTE_BITS = 8, // 0x0008
#endif // BYTE_BITS 
#ifndef BYTE_BITS_BITS
	BYTE_BITS_BITS = 3, // 0x0003
#endif // BYTE_BITS_BITS 
#ifndef WCHAR_BITS
	WCHAR_BITS = 16, // 0x0010
#endif // WCHAR_BITS 
};

#define ENUM_OPERATORS(TYPE) \
_WARNINGS_PUSH() \
_IGNORE_WARNING("-Wunused-function") \
static TYPE& operator++(TYPE& value, int __formal) \
{ \
    unused(__formal); \
    value = static_cast<TYPE>(value + 1); \
    return value; \
} \
 \
static TYPE& operator--(TYPE& value, int __formal) \
{ \
    unused(__formal); \
    value = static_cast<TYPE>(value - 1); \
    return value; \
} \
_WARNINGS_POP()



#define GET_BITS(bitfield, shift, mask) (((bitfield) & (mask)) >> (shift))
#define SET_BITS(value, shift, mask) (((value) << (shift)) & (mask))
#define ROUND_UP_TO_MULTIPLE(value, multiple) (((value) + (multiple) - 1) / (multiple) * (multiple))

#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)
#define BYTES_TO_BITS(bytes) ((bytes) * 8)

#define FLAG(shift) (1u << (shift))
#define FLAG_RANGE(min_bit, max_bit) (RANGE(max_bit) & ~RANGE(min_bit))

#define SET_BIT(flags, bit, enable) do { if ((enable)) { (flags) |= FLAG((bit)); } else { (flags) &= ~FLAG((bit)); } } while(false)

#define TEST_MASK(flags, mask) (((flags) & (mask)) != 0)
#define TEST_BIT(flags, shift) (((flags) & static_cast<decltype(flags)>(FLAG(shift))) != 0)
#define OFFSETOF(t, d) __builtin_offsetof(t, d)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define SIZEOF_BITS(expression) (sizeof(expression) * 8)
#define IN_RANGE(value, begin, end) ((value) > (begin) && (value) < (end))
#define IN_RANGE_INCLUSIVE(value, begin, end) ((value) >= (begin) && (value) <= (end))
#define VALID_BITS(flags, max_bits) (((flags) & ~RANGE((max_bits))) == 0)
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count)) 
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count)) 

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(x, low) ((x) < (low) ? (x) : (low))
#define MAX(x, high) ((x) > (high) ? (x) : (high))
#define CLAMP(x, low, high) ((x) < (low) ? (low) : (x) > (high) ? (high) : (x))
#define CLAMP_LOWER(x, low, high) ((x) >= (high) - (low) ? (x) - (high) : (low))
#define CLAMP_UPPER(x, low, high) ((x) <= (high) - (low) ? (x) + (low) : (high))

#define CONCATX(x, y) x##y
#define CONCAT(x, y)  CONCATX(x, y)

#define static_assert_remove_me_later(_expression) static_assert(_expression)
#define static_todo(_expression)

#include <initializer_list>
#include <limits>
#include <type_traits>

template<typename t_result, typename t_type>
constexpr t_result FLAGS(std::initializer_list<t_type> flags)
{
	static_assert(std::is_integral_v<t_type> || std::is_enum_v<t_type>, "FLAGS requires an integral type");

	t_result result = 0;
	for (t_type flag : flags)
	{
		result |= static_cast<t_result>(t_result{ 1 } << flag);
	}
	return result;
}

template<typename t_result, typename t_type, typename... t_rest>
constexpr t_result FLAGS(t_type first, t_rest... rest)
{
	return FLAGS<t_result>({ first, rest... });
}

template<typename t_type, typename... t_rest>
constexpr unsigned long FLAGS(t_type first, t_rest... rest)
{
	return FLAGS<unsigned long>({ first, rest... });
}

template<typename t_type>
inline t_type t_min(t_type a, t_type b)
{
	return a < b ? a : b;
}

template<typename t_type>
inline t_type t_min3(t_type a, t_type b, t_type c)
{
	t_type min_ab = a < b ? a : b;
	return min_ab < c ? min_ab : c;
}

template<typename t_type>
inline t_type t_min4(t_type a, t_type b, t_type c, t_type d)
{
	t_type min_ab = a < b ? a : b;
	t_type min_cd = c < d ? c : d;
	return min_ab < min_cd ? min_ab : min_cd;
}

#ifdef _DEBUG

#define constexpr_assert(expression) do { if(!(expression)) { throw 0; } } while(false)

template<typename T_to, typename T_from>
constexpr T_to value_cast(T_from from)
{
    if constexpr (std::is_constant_evaluated())
    {
        if constexpr (std::is_enum_v<T_to>)
        {
            using T_underlying = std::underlying_type_t<T_to>;
            constexpr_assert(static_cast<T_underlying>(from) <= std::numeric_limits<T_underlying>::max());
            constexpr_assert(static_cast<T_underlying>(from) >= std::numeric_limits<T_underlying>::min());
            return static_cast<T_to>(from);
        }
        else
        {
            constexpr_assert(static_cast<T_to>(from) <= std::numeric_limits<T_to>::max());
            constexpr_assert(static_cast<T_to>(from) >= std::numeric_limits<T_to>::min());
            return static_cast<T_to>(from);
        }
    }
    else
    {
        if constexpr (std::is_enum_v<T_to>)
        {
            using T_underlying = std::underlying_type_t<T_to>;
            T_underlying result = static_cast<T_underlying>(from);
            assert(result <= std::numeric_limits<T_underlying>::max());
            assert(result >= std::numeric_limits<T_underlying>::min());
            return result;
        }
        else
        {
            T_to result = static_cast<T_to>(from);
            assert(result <= std::numeric_limits<T_to>::max());
            assert(result >= std::numeric_limits<T_to>::min());
            return result;
        }
    }
}

template<typename t_type>
t_type& check_reference(t_type& ref)
{
    assert(&ref != nullptr);
    return ref;
}

template<typename t_type>
t_type const& check_reference(t_type const& ref)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
#endif
    assert(&ref != nullptr);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    return ref;
}

#else
#define value_cast static_cast
#define check_reference(...) (__VA_ARGS__)
#endif

template<typename t_explicit_cast, typename t_lhs, typename t_rhs>
t_explicit_cast value_add_cast(t_lhs const& l, t_rhs const& r)
{
    using t_result = std::underlying_type_t<t_explicit_cast>;
    t_explicit_cast result = value_cast<t_explicit_cast>(value_cast<t_result>(l) + value_cast<t_result>(r));
    return result;
}

#define vsizeof(VALUE_TYPE, ...) (value_cast<VALUE_TYPE>(sizeof(__VA_ARGS__)))

template<typename t_result, typename t_type, typename... t_rest>
constexpr t_result RANGE(t_type bits)
{
    unsigned long long result = (1ull << value_cast<unsigned long long>(bits)) - 1;
    return value_cast<t_result>(result);
}

template<typename t_type, typename... t_rest>
constexpr unsigned long RANGE(t_type bits)
{
    unsigned long long result = (1ull << value_cast<unsigned long long>(bits)) - 1;
    return value_cast<unsigned long>(result);
}

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __CSERIES_MACROS_H__
