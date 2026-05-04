#ifndef __STATIC_ARRAYS_H__
#define __STATIC_ARRAYS_H__
#pragma once

/* ---------- headers */

#include "cseries/types.h"
#include "cseries/cseries_macros.h"
#include "cseries/cseries_asserts.h"

/* ---------- constants */

#define BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT) (((BIT_COUNT) + (LONG_BITS - 1)) >> 5)
#define BIT_VECTOR_SIZE_IN_BYTES(BIT_COUNT) (4 * BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT))
#define BIT_VECTOR_TEST_FLAG(BIT_VECTOR, BIT) ((BIT_VECTOR[BIT >> 5] & (1 << (BIT & (LONG_BITS - 1)))) != 0)
#define BIT_VECTOR_OR_FLAG(BIT_VECTOR, BIT) (void)(BIT_VECTOR[BIT >> 5] |= (1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_AND_FLAG(BIT_VECTOR, BIT) (void)(BIT_VECTOR[BIT >> 5] &= ~(1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_SET_FLAG(BIT_VECTOR, BIT, VALUE) (void)(BIT_VECTOR[BIT >> 5] = VALUE ? (BIT_VECTOR[BIT >> 5] | (1 << (BIT & (LONG_BITS - 1)))) : (BIT_VECTOR[BIT >> 5] & ~(1 << (BIT & (LONG_BITS - 1)))))

/* ---------- definitions */

template<typename t_type, long k_count>
class c_static_array
{
public:
	typedef t_type* iterator;
	typedef t_type const* const_iterator;

	void set_all(t_type const&);
	void set_memory(long);
	t_type& element(long index);
	t_type const& element(long index) const;
	t_type& operator[](long index);
	t_type const& operator[](long) const;
	t_type const* get_elements() const;
	t_type* get_elements();
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();
	void sort(long, __int32(*)(void const*, void const*));
	long get_total_element_size() const;

	static bool valid(long index);
	static long get_element_size();
	static long get_count();

private:
	t_type m_elements[k_count]; // 0x0
};
static_assert(sizeof(c_static_array<long, 6>) == 24);


template<typename t_type, long k_count>
class c_static_stack
{
public:
	typedef t_type* iterator;
	typedef t_type const* const_iterator;

	c_static_stack();
	void clear();
	bool valid(long index) const;
	bool valid() const;
	bool empty() const;
	bool full() const;
	void resize(long);
	long push();
	void push_back(t_type const& value);
	void pop();
	long top() const;
	long count() const;
	t_type& operator[](long);
	t_type const& operator[](long) const;
	t_type* get(long index);
	t_type const* get(long) const;
	t_type* get_elements();
	t_type const* get_elements() const;
	const_iterator get_top() const;
	iterator get_top();
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();

private:
	long m_count; // 0x0
	t_type m_elements[k_count]; // 0x4
};
static_assert(sizeof(c_static_stack<long, 8>) == 36);

template<typename t_type, long k_count, long k_alignment>
class c_static_aligned_stack_allocator
{
public:
	c_static_aligned_stack_allocator();
	void clear();
	bool valid() const;
	bool empty() const;
	bool full() const;
	long count() const;
	long remaining() const;
	t_type* allocate(long count);
	t_type* reallocate(t_type* element, long count);
	void deallocate(t_type* element);

private:
	long m_count; // 0x0
	long m_pad0; // 0x4
	long m_pad1; // 0x8
	long m_pad2; // 0xC
	t_type m_elements[5120]; // 0x10
};
// static_assert(sizeof(c_static_aligned_stack_allocator<real_point2d,5120,2>) == 40976);

template<long k_count>
class c_static_flags_no_init
{
public:
	enum
	{
		k_size_in_dwords = BIT_VECTOR_SIZE_IN_LONGS(k_count)
	};

	void clear();
	void set_all();
	void fill(long count, unsigned char pattern);
	void set(long index, bool flag);
	bool test(long index) const;
	unsigned long const* get_bits_direct() const;
	unsigned long* get_writeable_bits_direct();
	void set_bits_direct_destructive(long count, unsigned long const* new_bits);
	void or_range(c_static_flags_no_init const* vector_a, c_static_flags_no_init const* vector_b, long count);
	void and_range(c_static_flags_no_init const* vector_a, c_static_flags_no_init const* vector_b, long count);
	void and_not_range(c_static_flags_no_init const* vector_a, c_static_flags_no_init const* vector_b, long count);
	void or_bits(unsigned long const* bits, long count);
	void and_bits(unsigned long const* bits, long count);
	void and_not_bits(unsigned long const* bits, long count);
	void set_range(long count);
	void clear_range(long count);
	void copy(c_static_flags_no_init const* vector_a);
	void and_(c_static_flags_no_init const* vector_a);
	void and_not(c_static_flags_no_init const* vector_a);
	void or_(c_static_flags_no_init const* vector_a);
	void invert_bits();
	void keep_range(long count);
	bool is_clear() const;
	long count_bits_set() const;
	long count_bits_set_in_range(long count) const;
	long lowest_bit_set() const;
	long lowest_bit_set_in_range(long count) const;
	long highest_bit_set() const;
	long highest_bit_set_in_range(long count) const;

private:
	unsigned long m_flags[k_size_in_dwords]; // 0x0
};
static_assert(sizeof(c_static_flags_no_init<256>) == 32);

template<long k_count>
class c_static_flags :
	public c_static_flags_no_init<k_count>
{
public:
	c_static_flags(long count, unsigned char pattern);
	c_static_flags();
};
static_assert(sizeof(c_static_flags<256>) == 32);

template<long k_count>
class c_static_string
{
public:
	typedef char t_char_buffer[k_count];
	static long const element_count;
	c_static_string(unsigned long);
	c_static_string(wchar_t const* src);
	c_static_string(char const* src);
	c_static_string();
	void clear();
	void set(char const* src);
	void set_wchar(wchar_t const* src);
	void set_bounded(char const* src, long length);
	void set_length(long desired_length);
	char const* append(char const* src);
	char const* print(char const* format, ...);
	void print_va(char const* format, char* argument_list);
	char const* append_print_va(char const*, char*);
	char const* append_print(char const* format, ...);
	void null_terminate_buffer();
	void to_lower();
	void to_upper();
	void replace(char, char);
	void trim_start();
	void trim_end();
	void trim();
	bool is_equal(char const* string, bool case_sensitive) const;
	bool is_equal(char const* string) const;
	bool is_equal_or_cropped(char const*) const;
	bool is_empty() const;
	long index_of(char const*) const;
	long next_index_of(char const*, long) const;
	bool starts_with(char const* string) const;
	bool ends_with(char const* string) const;
	bool substring(long start_index, long count, c_static_string<k_count>* sub) const;
	long length() const;
	static size_t max_length();
	char* copy_to(char* string, size_t string_length) const;
	char const* get_string() const;
	explicit operator const char* () const;
	t_char_buffer& get_buffer();
	char* from_tag(unsigned long);
	unsigned long to_tag() const;
	static void copy_string_to_buffer(char const* source_string, t_char_buffer& destination_buffer);
	static void copy_wchar_string_to_buffer(wchar_t const* source_string, t_char_buffer& destination_buffer);

private:
	t_char_buffer m_string; // 0x0
};

template<long k_count>
long const c_static_string<k_count>::element_count = k_count;

template<long k_count>
inline c_static_string<k_count>::c_static_string()
{
	m_string[0] = '\0';
}

template<long k_count>
inline c_static_string<k_count>::c_static_string(char const* src)
{
	if (src == nullptr)
	{
		m_string[0] = '\0';
		return;
	}
	std::strncpy(m_string, src, static_cast<size_t>(k_count - 1));
	m_string[k_count - 1] = '\0';
}

template<long k_count>
inline char const* c_static_string<k_count>::get_string() const
{
	return m_string;
}

static_assert(sizeof(c_static_string<256>) == 256);

class c_string_builder :
	public c_static_string<256>
{
public:
	c_string_builder(char const* src, ...);
	c_string_builder();
	~c_string_builder();
};
static_assert(sizeof(c_string_builder) == 256);

template<long k_count>
class c_static_wchar_string
{
public:
	static __int32 const k_buffer_size;
	typedef wchar_t t_wchar_buffer[k_count];
	c_static_wchar_string(wchar_t const* src);
	c_static_wchar_string(char const* src);
	c_static_wchar_string();
	void clear();
	void set(wchar_t const* src);
	void set_char(char const* src);
	void set_bounded(wchar_t const*, long);
	void set_utf8(utf8 const* src);
	void set_length(long);
	wchar_t const* append(wchar_t const* src);
	wchar_t const* print(wchar_t const* format, ...);
	void print_va(wchar_t const* format, va_list argument_list);
	wchar_t const* append_print(wchar_t const* format, ...);
	void replace(wchar_t, wchar_t);
	void trim_start();
	void trim_end();
	void trim();
	bool is_equal(wchar_t const* string, bool case_sensitive) const;
	bool is_equal(wchar_t const* string) const;
	bool is_empty() const;
	long index_of(wchar_t const*) const;
	long next_index_of(wchar_t const*, long) const;
	long length() const;
	wchar_t* copy_to(wchar_t* string, unsigned long long string_length) const;
	wchar_t const* get_string() const;
	explicit operator const wchar_t* () const;
	static void copy_string_to_buffer(wchar_t const*, t_wchar_buffer& destination_buffer);
	static void copy_char_string_to_buffer(char const*, t_wchar_buffer& destination_buffer);

private:
	t_wchar_buffer m_string; // 0x0
};
static_assert(sizeof(c_static_wchar_string<256>) == 512);

template<typename t_enum, typename t_storage, t_enum k_num_flags>
class c_flags_no_init
{
public:
	// static_assert(is_enum<t_enum>, "t_enum is not enum type");

	void clear();
	void set_all();
	bool test(t_enum bit) const;
	bool test_range(t_enum start_bit, t_enum end_bit) const;
	void set(t_enum bit, bool value);
	void toggle(t_enum bit);
	bool valid() const;
	bool is_empty() const; // $reach this is renamed to `is_clear`
	t_enum count() const;
	bool operator==(c_flags_no_init const& rsa) const;
	bool operator!=(c_flags_no_init const& rsa) const;
	t_storage get_unsafe() const;
	void set_unsafe(t_storage new_flags);
	c_flags_no_init& operator|=(c_flags_no_init const& rsa);
	c_flags_no_init& operator&=(c_flags_no_init const& rsa);
	c_flags_no_init operator~() const;
	c_flags_no_init operator&(t_storage const) const;
	c_flags_no_init operator&(c_flags_no_init const& rsa) const;
	c_flags_no_init operator|(t_storage const) const;
	c_flags_no_init operator|(c_flags_no_init const& rsa) const;

	static bool valid_bit(t_enum bit);

private:
	t_storage m_flags; // 0x0
};

template<typename t_enum, typename t_storage, t_enum k_num_flags>
class c_flags :
	public c_flags_no_init<t_enum, t_storage, k_num_flags>
{
public:
	typedef c_flags_no_init<t_enum, t_storage, k_num_flags> t_flags_no_init;
	c_flags(t_storage flags);
	c_flags();
};

template<typename t_enum, typename t_storage, t_enum min, t_enum max>
class c_enum
{
public:
	// static_assert(is_enum<t_enum>, "t_enum is not enum type");

	operator t_enum() const;
	bool operator==(t_enum) const;
	c_enum& operator=(t_enum new_value);
	bool in_range() const;
	void set_raw_value(t_storage new_value);

private:
	t_storage m_enum_value; // 0x0
};

template<long k_alignment_bytes>
struct __declspec(align(k_alignment_bytes))  s_aligned_type_for_union_alignment
{
	unsigned char _; // 0x0
};
static_assert(sizeof(s_aligned_type_for_union_alignment<1>) == 1);
static_assert(sizeof(s_aligned_type_for_union_alignment<1024>) == 1024);

template<typename t_type, long k_size = sizeof(t_type), long k_alignment_bytes = alignof(t_type)>
class c_opaque_data
{
public:
	t_type* get();
	t_type const* get_const() const;

private:
	union
	{
		s_aligned_type_for_union_alignment<k_alignment_bytes> m_force_alignment; // 0x0
		unsigned char m_opaque_data[k_size]; // 0x0
	};
};
// static_assert(sizeof(c_opaque_data<c_eslib_cache_file_decompressor,39056,8>) == 39056);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __STATIC_ARRAYS_H__
