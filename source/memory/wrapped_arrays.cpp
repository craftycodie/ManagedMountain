/* ---------- headers */

#include "memory/wrapped_arrays.h"

/* ---------- definitions */

c_wrapped_flags::c_wrapped_flags() : m_bit_storage(nullptr), m_bit_count(0)
{
}

c_wrapped_flags::c_wrapped_flags(void* storage, unsigned long bit_count)
	: m_bit_storage(static_cast<unsigned long*>(storage))
	, m_bit_count(bit_count)
{
}

void c_wrapped_flags::set_bits(void* storage, unsigned long bit_count)
{
	m_bit_storage = static_cast<unsigned long*>(storage);
	m_bit_count = bit_count;
}

void c_wrapped_flags::set_bits(c_basic_buffer<void> const& storage, unsigned long max_bit_count)
{
	m_bit_storage = static_cast<unsigned long*>(storage.begin());
	m_bit_count = max_bit_count;
}

unsigned long const* c_wrapped_flags::get_bit_storage() const
{
	return m_bit_storage;
}

unsigned long* c_wrapped_flags::get_bit_storage()
{
	return m_bit_storage;
}

long c_wrapped_flags::bit_count() const
{
	return static_cast<long>(m_bit_count);
}

template<>
void* c_basic_buffer<void>::begin() const
{
	return m_buffer_start;
}
