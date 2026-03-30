#ifndef __WRAPPED_ARRAYS_H__
#define __WRAPPED_ARRAYS_H__
#pragma once

/* ---------- headers */

#include <cstddef>

#include "memory/bitvector.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- definitions */

template<typename t_type>
class c_basic_buffer
{
public:
	class c_basic_buffer_checked_caster;
	class c_basic_buffer_checked_caster_const;
	c_basic_buffer(c_basic_buffer rhs, size_t offset, size_t size);
	c_basic_buffer(t_type* start, size_t size);
	c_basic_buffer(t_type* start, t_type* end);
	c_basic_buffer();
	bool is_equal(c_basic_buffer const& rhs);
	bool in_range(t_type const*);
	void set_buffer(c_basic_buffer, size_t, size_t);
	void set_buffer(t_type* start, t_type* end);
	void set_buffer(t_type* start, unsigned long size);
	void clear();
	t_type* begin() const;
	t_type* end() const;
	bool empty() const;
	size_t size() const;
	void advance_start(size_t offset);
	void grow_buffer_locked_end(size_t growth_size, c_basic_buffer bounds);
	t_type const* get_range(size_t offset, size_t size) const;
	t_type* get_range(size_t offset, size_t size);
	c_basic_buffer::c_basic_buffer_checked_caster_const checked_cast() const;
	c_basic_buffer::c_basic_buffer_checked_caster checked_cast();
private:
	t_type* m_buffer_start; // 0x0
	size_t m_size; // 0x8
};
static_assert(sizeof(c_basic_buffer<void>) == 16);

template<typename t_array>
class c_recursive_template_pattern
{
public:
	t_array const* get_super_class() const;
	t_array* get_super_class();
};
static_assert(sizeof(c_recursive_template_pattern<void>) == 1);

template<typename t_type>
class c_bit_vector_operator_interface :
	private c_recursive_template_pattern<t_type>
{
	friend class c_recursive_template_pattern<t_type>;
public:
	void clear();
	void set_all();
	void set(long, bool);
	bool test(long index) const;
	unsigned long const* get_bits() const;
	unsigned long* get_bits_unsafe();
	long bit_count() const;
private:
	bool verify_index(unsigned long bit_index) const;
	unsigned long const* get_bit_storage() const;
	unsigned long* get_bit_storage();
};
static_assert(sizeof(c_bit_vector_operator_interface<void>) == 1);

class c_wrapped_flags :
	public c_bit_vector_operator_interface<c_wrapped_flags>
{
	friend class c_bit_vector_operator_interface<c_wrapped_flags>;

public:
	c_wrapped_flags();
	c_wrapped_flags(void* storage, unsigned long bit_count);
	void set_bits(c_basic_buffer<void> const& storage, unsigned long max_bit_count);
	void set_bits(void* storage, unsigned long bit_count);
	long bit_count() const;

private:
	unsigned long const* get_bit_storage() const;
	unsigned long* get_bit_storage();

	unsigned long* m_bit_storage; // 0x0
	unsigned long m_bit_count; // 0x8
};
static_assert(sizeof(c_wrapped_flags) == 16);

template<typename t_type>
inline unsigned long const* c_bit_vector_operator_interface<t_type>::get_bits() const
{
	return static_cast<t_type const*>(this)->get_bit_storage();
}

template<typename t_type>
inline unsigned long* c_bit_vector_operator_interface<t_type>::get_bits_unsafe()
{
	return static_cast<t_type*>(this)->get_bit_storage();
}

template<typename t_type>
inline long c_bit_vector_operator_interface<t_type>::bit_count() const
{
	return static_cast<t_type const*>(this)->bit_count();
}

template<typename t_type>
inline bool c_bit_vector_operator_interface<t_type>::verify_index(unsigned long bit_index) const
{
	long const bits = bit_count();
	return bits > 0 && static_cast<long>(bit_index) < bits;
}

template<typename t_type>
inline void c_bit_vector_operator_interface<t_type>::clear()
{
	long const bits = bit_count();
	if (bits <= 0)
	{
		return;
	}
	unsigned long* const storage = static_cast<t_type*>(this)->get_bit_storage();
	if (storage == nullptr)
	{
		return;
	}
	int const words = static_cast<int>((bits + 31L) / 32L);
	for (int i = 0; i < words; ++i)
	{
		storage[i] = 0;
	}
}

template<typename t_type>
inline void c_bit_vector_operator_interface<t_type>::set_all()
{
	long const bits = bit_count();
	if (bits <= 0)
	{
		return;
	}
	unsigned long* const storage = static_cast<t_type*>(this)->get_bit_storage();
	if (storage == nullptr)
	{
		return;
	}
	int const full_words = static_cast<int>(bits / 32L);
	for (int i = 0; i < full_words; ++i)
	{
		storage[i] = ~0ul;
	}
	int const rem = static_cast<int>(bits % 32L);
	if (rem != 0)
	{
		storage[full_words] |= (1ul << rem) - 1ul;
	}
}

template<typename t_type>
inline void c_bit_vector_operator_interface<t_type>::set(long index, bool value)
{
	auto* const self = static_cast<t_type*>(this);
	if (!self->verify_index(static_cast<unsigned long>(index)))
	{
		return;
	}
	auto* const words = reinterpret_cast<uns32*>(self->get_bit_storage());
	if (words == nullptr)
	{
		return;
	}
	if (value)
	{
        words[index >> 5] |= (1u << (index & 31));
	}
	else
	{
		words[index >> 5] &= ~(1u << (index & 31));
	}
}

template<typename t_type>
inline bool c_bit_vector_operator_interface<t_type>::test(long index) const
{
	auto const* const self = static_cast<t_type const*>(this);
	if (!self->verify_index(static_cast<unsigned long>(index)))
	{
		return false;
	}
	auto const* const words = reinterpret_cast<uns32 const*>(self->get_bit_storage());
	if (words == nullptr)
	{
		return false;
	}
	unsigned long const u = static_cast<unsigned long>(index);
	return (words[u >> 5] & (1u << (u & 31u))) != 0;
}

template<typename t_array, typename t_type>
class c_array_operator_interface :
	private c_recursive_template_pattern<t_array>
{
public:
	typedef t_type* iterator;
	typedef t_type const* const_iterator;

	t_type const& operator[](long index) const;
	t_type& operator[](long index);
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();
	t_type const* get_element(long index) const;
	t_type* get_element(long index);
	t_type const* get_elements() const;
	t_type* get_elements();
	long get_total_element_size() const;
	bool valid_index(long index) const;
	long count() const;
	bool empty() const;
	void set_all(t_type const& new_value);
};
static_assert(sizeof(c_array_operator_interface<int, int>) == 1);

template<typename t_array, typename t_type>
class c_resizeable_array_operator_interface :
	public c_array_operator_interface<t_array, t_type>
{
public:
	long new_element_index();
	t_type& add_element();
	t_type& new_element();
	t_type* get_new_element();
	bool resize(long new_size);
private:
	t_array const* get_super_class() const;
	t_array* get_super_class();
};

template<typename t_type>
class c_typed_tag_block :
	public c_resizeable_array_operator_interface<c_typed_tag_block<t_type>, t_type>,
	public s_tag_block
{
public:
	t_type const* begin() const;
	t_type* begin();
	t_type const* end() const;
	t_type* end();
	long count() const;
};
static_assert(sizeof(c_typed_tag_block<long>) == 12);

template<typename t_type>
inline long c_typed_tag_block<t_type>::count() const
{
	return static_cast<long>(static_cast<s_tag_block const&>(*this).count);
}

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __WRAPPED_ARRAYS_H__