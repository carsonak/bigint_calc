#ifndef DS_DEQUE_H
#define DS_DEQUE_H

typedef struct MyString
{
	int len;
	char *s;
} MyString;

#define DEQUE_UNIQUE_SUFFIX mystr
#define DEQUE_DATA_TYPE MyString
#include "Deque_struct_def.h"

#define DEQUE_UNIQUE_SUFFIX str
#define DEQUE_DATA_TYPE char *
#include "Deque_struct_def.h"

#define DEQUE_UNIQUE_SUFFIX llint
#define DEQUE_DATA_TYPE long long int
#include "Deque_struct_def.h"

#define DEQUE_UNIQUE_SUFFIX mystr
#define DEQUE_DATA_TYPE MyString
#include "Deque_prototypes.h"

#define DEQUE_UNIQUE_SUFFIX str
#define DEQUE_DATA_TYPE char *
#include "Deque_prototypes.h"

#define DEQUE_UNIQUE_SUFFIX llint
#define DEQUE_DATA_TYPE long long int
#include "Deque_prototypes.h"

#endif /* DS_DEQUE_H */
