/* ---------- headers */

#include "cseries/port_64.h"

#include "cseries/cseries_asserts.h"
#include "cache/physical_memory_map.h"

/* ---------- constants */

/* ---------- definitions */

char const g_ptr32EmptyString[1] = { '\0' };

/* ---------- globals */

/* ---------- public code */

void* ptr32_void_t::ExpandPtr(uns32 const value)
{
	if (value == 0)
		return nullptr;
	if (value == static_cast<uns32>(-1))
		return const_cast<char*>(g_ptr32EmptyString);
	return reinterpret_cast<void*>(virtual_to_physical_base_offset() + static_cast<uns64>(value) * 4ull);
}

uns32 ptr32_void_t::CompressPtr(void const* const ptr)
{
	if (ptr == nullptr)
		return 0;
	if (ptr == g_ptr32EmptyString)
		return static_cast<uns32>(-1);
	uns64 const addr = reinterpret_cast<uns64>(ptr);
	uns64 const base = virtual_to_physical_base_offset();
	ASSERT((addr - base) % 4ull == 0);
	uns64 const index = (addr - base) / 4ull;
	ASSERT(index <= static_cast<uns64>(0xFFFFFFFFull));
	return static_cast<uns32>(index);
}

void ptr32_void_t::SetPtr(void const* const ptr)
{
	m_stored = CompressPtr(ptr);
}

void* ptr32_void_t::GetPtr() const
{
	return ExpandPtr(m_stored);
}

bool ptr32_void_t::IsEqual(void const* const ptr) const
{
	return GetPtr() == ptr;
}

uns32 ptr32_void_t::GetCompressed() const
{
	return m_stored;
}

bool ptr32_void_t::operator==(ptr32_void_t const& other) const
{
	return m_stored == other.m_stored;
}

ptr32_void_t::operator uns64() const
{
	return reinterpret_cast<uns64>(GetPtr());
}

ptr32_void_t::operator char*() const
{
	return static_cast<char*>(GetPtr());
}

ptr32_void_t::operator unsigned char*() const
{
	return static_cast<unsigned char*>(GetPtr());
}

ptr32_void_t::operator char const*() const
{
	return static_cast<char const*>(GetPtr());
}

ptr32_void_t::operator unsigned char const*() const
{
	return static_cast<unsigned char const*>(GetPtr());
}

ptr32_void_t::operator short*() const
{
	return static_cast<short*>(GetPtr());
}

ptr32_void_t::operator unsigned short*() const
{
	return static_cast<unsigned short*>(GetPtr());
}

ptr32_void_t::operator short const*() const
{
	return static_cast<short const*>(GetPtr());
}

ptr32_void_t::operator unsigned short const*() const
{
	return static_cast<unsigned short const*>(GetPtr());
}

ptr32_void_t::operator int*() const
{
	return static_cast<int*>(GetPtr());
}

ptr32_void_t::operator unsigned int*() const
{
	return static_cast<unsigned int*>(GetPtr());
}

ptr32_void_t::operator int const*() const
{
	return static_cast<int const*>(GetPtr());
}

ptr32_void_t::operator unsigned int const*() const
{
	return static_cast<unsigned int const*>(GetPtr());
}

ptr32_void_t::operator long*() const
{
	return static_cast<long*>(GetPtr());
}

ptr32_void_t::operator unsigned long*() const
{
	return static_cast<unsigned long*>(GetPtr());
}

ptr32_void_t::operator long const*() const
{
	return static_cast<long const*>(GetPtr());
}

ptr32_void_t::operator unsigned long const*() const
{
	return static_cast<unsigned long const*>(GetPtr());
}

ptr32_void_t::operator float*() const
{
	return static_cast<float*>(GetPtr());
}

ptr32_void_t::operator float const*() const
{
	return static_cast<float const*>(GetPtr());
}

ptr32_void_t::operator void*() const
{
	return GetPtr();
}

ptr32_void_t::operator void const*() const
{
	return GetPtr();
}

/* ---------- private code */
