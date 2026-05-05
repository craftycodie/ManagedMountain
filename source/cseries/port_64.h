#ifndef __PORT_64_H__
#define __PORT_64_H__
#pragma once

/* ---------- headers */

#include "cseries/types.h"

/* ---------- constants */

/* ---------- definitions */

extern char const g_ptr32EmptyString[1];

class ptr32_void_t
{
public:
	operator uns64() const;
	operator char*() const;
	operator unsigned char*() const;
	operator char const*() const;
	operator unsigned char const*() const;
	operator short*() const;
	operator unsigned short*() const;
	operator short const*() const;
	operator unsigned short const*() const;
	operator int*() const;
	operator unsigned int*() const;
	operator int const*() const;
	operator unsigned int const*() const;
	operator long*() const;
	operator unsigned long*() const;
	operator long const*() const;
	operator unsigned long const*() const;
	operator float*() const;
	operator float const*() const;
	operator void*() const;
	operator void const*() const;
	bool operator==(ptr32_void_t const& other) const;
	static uns32 CompressPtr(void const* ptr);
	static void* ExpandPtr(uns32 value);
	void SetPtr(void const* ptr);
	void* GetPtr() const;
	bool IsEqual(void const* ptr) const;
	uns32 GetCompressed() const;

	template<typename t_type_ptr>
	static t_type_ptr ExpandPtrAs(uns32 value)
	{
		return static_cast<t_type_ptr>(ExpandPtr(value));
	}

private:
	uns32 m_stored; // 0x0
};
static_assert(sizeof(ptr32_void_t) == 4);

template<typename t_type>
class ptr32_t
{
private:
	ptr32_void_t m_ptr; // 0x0

public:
	operator t_type*() const;
	ptr32_t& operator=(t_type const* ptr);
	t_type* operator->() const;
	void SetPtr(t_type* ptr);
	t_type* GetPtr() const;
	uns32 GetCompressed() const;
};
static_assert(sizeof(ptr32_t<int>) == 4);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

template<typename t_type>
ptr32_t<t_type>::operator t_type*() const
{
	return static_cast<t_type*>(m_ptr.GetPtr());
}

template<typename t_type>
ptr32_t<t_type>& ptr32_t<t_type>::operator=(t_type const* ptr)
{
	m_ptr.SetPtr(ptr);
	return *this;
}

template<typename t_type>
t_type* ptr32_t<t_type>::operator->() const
{
	return static_cast<t_type*>(m_ptr.GetPtr());
}

template<typename t_type>
void ptr32_t<t_type>::SetPtr(t_type* ptr)
{
	m_ptr.SetPtr(ptr);
}

template<typename t_type>
t_type* ptr32_t<t_type>::GetPtr() const
{
	return static_cast<t_type*>(m_ptr.GetPtr());
}

template<typename t_type>
uns32 ptr32_t<t_type>::GetCompressed() const
{
	return m_ptr.GetCompressed();
}

#endif /* __PORT_64_H__ */
