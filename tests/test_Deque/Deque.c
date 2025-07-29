#include <string.h> /* strcpy */

#include "Deque.h"

#define DEQUE_UNIQUE_SUFFIX mystr
#define DEQUE_DATA_TYPE MyString
#include "Deque_methods.c"

#define DEQUE_UNIQUE_SUFFIX str
#define DEQUE_DATA_TYPE char *
#include "Deque_methods.c"

#define DEQUE_UNIQUE_SUFFIX llint
#define DEQUE_DATA_TYPE long long int
#include "Deque_methods.c"
