/*
 * Copyright (c) 2011-2012 Anthony Viallard
 *
 *    This file is part of Utk.
 *
 * Utk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Utk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Utk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UTK_UNIT_H_
#define _UTK_UNIT_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <utk/vt102.h>

/**
 * unit.h - unit test framework
 *
 * - You can enable color logs by defining ENABLE_UTK_VT102_COLOR macro
 *   (before any utk includes).
 *
 * Architecture:
 * -------------
 *
 *  - a C file for unit test is composed of main() function
 *    with a definition of the "test module";
 *  - a "test module" is a set of test;
 *  - a test is composed of assert to test some conditions.
 *
 * Example:
 * --------
 *
 * #include <utk/unit.h>
 *
 * UTK_TEST_DEF(test_foo)
 * {
 *	UTK_TEST_ASSERT(0 + 0 == 0);
 * }
 * 
 * int main(void)
 * {
 *      UTK_TEST_MODULE_INIT("foo");
 *
 *      UTK_TEST_RUN(test_foo);
 *
 *      return UTK_TEST_MODULE_RETURN;
 * }
 */

/**
 * Test return struct
 */
struct utk_test_result {
    int ret;
    int line;
    char msg[256];
};

/**
 * Print messages following what happens
 */
#define UTK_TEST_PRINT_ERROR(m, ...)			\
    printf(UTK_VT102_COLOR_RED(m "\n"), ##__VA_ARGS__)
#define UTK_TEST_PRINT_RUNTIME_ERROR(m, ...)			\
    printf(UTK_VT102_COLOR_LIGHT_RED(m "\n"), ##__VA_ARGS__)
#define UTK_TEST_PRINT_WARNING(m, ...)			\
    printf(UTK_VT102_COLOR_YELLOW(m "\n"), ##__VA_ARGS__)
#define UTK_TEST_PRINT_OK(m, ...)			\
    printf(UTK_VT102_COLOR_GREEN(m "\n"), ##__VA_ARGS__)

/**
 * Run a test definition
 */
#define UTK_TEST_RUN(func) do					\
    {								\
	struct utk_test_result tr;				\
	tr.ret = 0;                                             \
								\
	func(&tr);                                              \
	if(tr.ret != 0)                                         \
	{							\
	    UTK_TEST_PRINT_ERROR("KO: %s (line:%d): %s",        \
				 #func, tr.line, tr.msg);	\
	}							\
	else							\
	{							\
	    UTK_TEST_PRINT_OK("OK: %s", #func);                 \
	}							\
	main_return |= tr.ret;                                  \
    } while(0)

/**
 * Define a test function (Unit Test)
 *
 * A test function return -1 if test has failed, 0 otherwise
 */
#define UTK_TEST_DEF(name)				\
    static void name(struct utk_test_result* __tr)

/**
 * Produce assertions
 */
#define UTK_TEST_RAW_ASSERT(test, message, ...) do	\
    {							\
	if(!(test))					\
	{						\
	    __tr->ret = -1;				\
	    __tr->line = __LINE__;			\
	    snprintf(__tr->msg,				\
		     sizeof(__tr->msg),			\
		     message, ##__VA_ARGS__);		\
	    return;					\
	}						\
    } while(0)

#define UTK_TEST_ASSERT(condition)				\
    UTK_TEST_RAW_ASSERT(condition, "Condition isn't TRUE")

/**
 * Define a test module of unit tests
 */
#define UTK_TEST_MODULE_INIT(name)			\
    int main_return = 0;				\
    printf(UTK_VT102_COLOR_BLUE("@@ TEST " name " @@\n"))

#define UTK_TEST_MODULE_RETURN main_return

#endif
