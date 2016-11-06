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

#define ENABLE_UTK_VT102_COLOR 1
#include <utk/str.h>
#include <utk/array.h>
#include <utk/math.h>
#include <utk/unit.h>
#include <utk/io.h>

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

UTK_TEST_DEF(test_io_write_and_read)
{
    int fd;
    const char *str_to_wr = NULL;
    ssize_t ret;
    char buf[128];
    char little_buf[8];

    str_to_wr = "hello there !";

    /* write */
    fd = open("/tmp/test_io_write", O_CREAT | O_WRONLY, 0666);

    UTK_TEST_ASSERT(fd >= 0);

    ret = utk_io_write(fd, str_to_wr, strlen(str_to_wr));

    UTK_TEST_ASSERT(ret == (ssize_t)strlen(str_to_wr));

    close(fd);

    /* reopen, read and compare */
    fd = open("/tmp/test_io_write", O_RDONLY);

    UTK_TEST_ASSERT(fd >= 0);

    ret = utk_io_read(fd, buf, sizeof(buf));

    UTK_TEST_ASSERT(ret == (ssize_t)strlen(str_to_wr));
    buf[ret] = '\0';
    UTK_TEST_ASSERT(strcmp(str_to_wr, buf) == 0);

    close(fd);

    /* reopen, read and compare */
    fd = open("/tmp/test_io_write", O_RDONLY);

    UTK_TEST_ASSERT(fd >= 0);

    ret = utk_io_read(fd, little_buf, sizeof(little_buf));

    UTK_TEST_ASSERT(ret != (ssize_t)strlen(str_to_wr)
                    && ret == sizeof(little_buf));
    UTK_TEST_ASSERT(strncmp(str_to_wr, little_buf, sizeof(little_buf)) == 0);

    close(fd);

    unlink("/tmp/test_io_write");
}

UTK_TEST_DEF(test_io_file_write_and_read)
{
    ssize_t ret;
    const char *str_to_wr = NULL;
    char buf[128];
    char little_buf[8];

    str_to_wr = "hello there !";

    /* write */
    ret = utk_io_file_write("/tmp/test_io_write",
			    str_to_wr, strlen(str_to_wr));

    UTK_TEST_ASSERT(ret == (ssize_t)strlen(str_to_wr));

    /* reopen, read and compare */
    ret = utk_io_file_read("/tmp/test_io_write", buf, sizeof(buf));

    UTK_TEST_ASSERT(ret == (ssize_t)strlen(str_to_wr));
    buf[ret] = '\0';
    UTK_TEST_ASSERT(strcmp(str_to_wr, buf) == 0);

    /* reopen, read and compare */
    ret = utk_io_file_read("/tmp/test_io_write",
			   little_buf, sizeof(little_buf));

    UTK_TEST_ASSERT(ret != (ssize_t)strlen(str_to_wr)
                    && ret == sizeof(little_buf));
    UTK_TEST_ASSERT(strncmp(str_to_wr, little_buf, sizeof(little_buf)) == 0);

    unlink("/tmp/test_io_write");
}

int main(void)
{
    UTK_TEST_MODULE_INIT("utk/io");

    UTK_TEST_RUN(test_io_write_and_read);

    UTK_TEST_RUN(test_io_file_write_and_read);

    return UTK_TEST_MODULE_RETURN;
}
