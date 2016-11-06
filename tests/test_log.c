/*
 * Copyright (c) 2011 Anthony Viallard
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

#define ENABLE_UTK_VT102_COLOR 1
#define ENABLE_UTK_LOG_DEBUG 1
#include <utk/log.h>
#include <utk/array.h>
#include <utk/unit.h>

UTK_TEST_DEF(test_log)
{
    /* only test if there macro issues */
    utk_log_open("utk_test_log", LOG_PID, LOG_USER);

    utk_log_write(LOG_INFO, "Hello World !");

    utk_log_info("It's a log info message");
    utk_log_notice("It's a log notice message");
    utk_log_error("It's a log error message");
    utk_log_debug("It's a log debug message");

    utk_log_close();

    UTK_TEST_ASSERT(1);
}

int main(void)
{
    UTK_TEST_MODULE_INIT("utk/log");

    UTK_TEST_RUN(test_log);

    return UTK_TEST_MODULE_RETURN;
}
