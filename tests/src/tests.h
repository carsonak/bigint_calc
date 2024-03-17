#ifndef _TESTS_H_
#define _TESTS_H_

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif

#include "../../src/infiX.h"

#endif /*_TESTS_H_*/
