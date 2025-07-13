#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "Deque_struct_macros.h"

typedef struct MyString
{
	int len;
	char *s;
} MyString;

DEFINE_DEQUE_STRUCT(mystr, MyString)

DEFINE_DEQUE_STRUCT(str, char *)

DEFINE_DEQUE_STRUCT(llint, long long int)

#include "Deque_prototypes_macros.h"

DECLARE_DEQUE_PROTOTYPES(mystr, MyString)

DECLARE_DEQUE_PROTOTYPES(str, char *)

DECLARE_DEQUE_PROTOTYPES(llint, long long int)

#endif /* DS_DEQUE_H */
