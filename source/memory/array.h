#ifndef __ARRAY_H__
#define __ARRAY_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

struct dynamic_array
{
	int element_size;
	int count;
	char* elements;
};

/* ---------- prototypes */

void* __fastcall dynamic_array_get_element(dynamic_array* array, int index, int element_size);

/* ---------- globals */

/* ---------- public code */

#endif // __NEW_HEADER_H__
