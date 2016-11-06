/*
 * Copyright (c) 2011-2013 Anthony Viallard
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
#include <utk/list.h>
#include <utk/unit.h>

UTK_TEST_DEF(test_str_copy)
{
    char buf1[1];
    char buf16[16];
    size_t ret;

    const char *my_string = NULL;

    /* copy of empty string */
    my_string = "";

    ret = utk_str_copy(buf1, sizeof(buf1), my_string);

    UTK_TEST_ASSERT(ret == 0);
    UTK_TEST_ASSERT(strcmp(buf1, my_string) == 0);

    /* copy of normal string without truncated case */
    my_string = "Hello world !";

    ret = utk_str_copy(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret == strlen(buf16));
    UTK_TEST_ASSERT(strcmp(buf16, my_string) == 0);

    /* test return string copied is truncated and NULL terminated */
    my_string = "foobarfoobarfoobarfoo";

    ret = utk_str_copy(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret > sizeof(buf16) - 1);
    UTK_TEST_ASSERT(ret == strlen(my_string));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');
    UTK_TEST_ASSERT(strncmp(my_string, buf16, sizeof(buf16) - 1) == 0);
}

UTK_TEST_DEF(test_str_printf)
{
    char buf16[16];
    int ret;

    const char *my_string = NULL;

    /* test truncation case and null terminated */
    my_string = "0123456789012345";

    ret = utk_str_printf(buf16, sizeof(buf16), "%s", my_string);

    UTK_TEST_ASSERT(ret > (int)sizeof(buf16) - 1);
    UTK_TEST_ASSERT(ret == (int)strlen(my_string));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');
}

UTK_TEST_DEF(test_str_cat)
{
    char buf16[16];
    const char *my_string = NULL;
    size_t ret;

    /* test truncation case and null terminated */
    buf16[0] = '\0';

    my_string = "01234567890123456789";
    ret = utk_str_cat(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret > sizeof(buf16) - 1);
    UTK_TEST_ASSERT(ret == strlen(my_string));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');

    /* basic test and truncation/nullended case */
    buf16[0] = '\0';

    my_string = "12345";

    ret = utk_str_cat(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret == strlen(my_string));
    UTK_TEST_ASSERT(strcmp(buf16, my_string) == 0);

    ret = utk_str_cat(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret == 2 * strlen(my_string));
    UTK_TEST_ASSERT(strcmp(buf16, "1234512345") == 0);

    ret = utk_str_cat(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret == 3 * strlen(my_string));
    UTK_TEST_ASSERT(strcmp(buf16, "123451234512345") == 0);

    ret = utk_str_cat(buf16, sizeof(buf16), my_string);

    UTK_TEST_ASSERT(ret > sizeof(buf16) - 1);
    UTK_TEST_ASSERT(ret == 4 * strlen(my_string));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');
}

UTK_TEST_DEF(test_str_catf)
{
    char buf16[16],
	buf4[4];
    const char *my_string1 = NULL,
	*my_string2 = NULL;
    size_t ret;

    /* test truncation case and null terminated */
    buf16[0] = '\0';

    my_string1 = "0123456789";
    my_string2 = "0123456789";
    ret = utk_str_catf(buf16, sizeof(buf16), "%s%s", my_string1, my_string2);

    UTK_TEST_ASSERT(ret > sizeof(buf16) - 1);
    UTK_TEST_ASSERT(ret == (strlen(my_string1) + strlen(my_string2)));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');

    /* basic test and truncation/nullended case */
    buf16[0] = '\0';

    my_string1 = "12345";

    ret = utk_str_catf(buf16, sizeof(buf16), "%s", my_string1);

    UTK_TEST_ASSERT(ret == strlen(my_string1));
    UTK_TEST_ASSERT(strcmp(buf16, my_string1) == 0);

    ret = utk_str_catf(buf16, sizeof(buf16), "%s", my_string1);

    UTK_TEST_ASSERT(ret == 2 * strlen(my_string1));
    UTK_TEST_ASSERT(strcmp(buf16, "1234512345") == 0);

    ret = utk_str_catf(buf16, sizeof(buf16), my_string1);

    UTK_TEST_ASSERT(ret == 3 * strlen(my_string1));
    UTK_TEST_ASSERT(strcmp(buf16, "123451234512345") == 0);

    ret = utk_str_catf(buf16, sizeof(buf16), my_string1);

    UTK_TEST_ASSERT(ret > sizeof(buf16) - 1);
    UTK_TEST_ASSERT(strcmp(buf16, "123451234512345") == 0);
    UTK_TEST_ASSERT(ret == 4 * strlen(my_string1));
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');

    /* format test */
    buf4[0] = '\0';

    ret = utk_str_catf(buf4, sizeof(buf4), "%d", 4444);

    UTK_TEST_ASSERT(ret > sizeof(buf4) - 1);
    UTK_TEST_ASSERT(strcmp(buf4, "444") == 0);
    UTK_TEST_ASSERT(buf4[sizeof(buf4) - 1] == '\0');
}

UTK_TEST_DEF(test_str_matches)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "teststring");

    UTK_TEST_ASSERT(utk_str_matches(buf, "teststring"));
    UTK_TEST_ASSERT(!utk_str_matches(buf, "test"));
    UTK_TEST_ASSERT(!utk_str_matches(buf, ""));
}

UTK_TEST_DEF(test_str_empty)
{
    UTK_TEST_ASSERT(utk_str_empty(""));
    UTK_TEST_ASSERT(!utk_str_empty("a"));
    UTK_TEST_ASSERT(!utk_str_empty("Hello World !"));
}

UTK_TEST_DEF(test_str_split)
{
    struct utk_str_list list;
    unsigned int count,
	i;
    const char *my_string = NULL;
    struct utk_str_list_item *item = NULL;

    /* basic test */
    const char *items_expect[] = {
	"one",
	"two",
	"three",
	"four",
	"five",
    };

    my_string = "one two three four five";

    count = utk_str_split(my_string, " ", &list);

    UTK_TEST_ASSERT(count == 5);
    i = 0;
    utk_str_list_for_each_entry(&list, item)
    {
	/* printf(" * item = %s | item_expect = %s\n", */
	/*        item->value, items_expect[i]); */
	UTK_TEST_ASSERT(strcmp(items_expect[i], item->value) == 0);
	++i;
    }
    UTK_TEST_ASSERT(count == i);
    utk_str_list_cleanup(&list);

    /* no found delimiter in string */
    const char *items_expect2[] = {
	"therearenospacehere",
    };

    my_string = "therearenospacehere";

    count = utk_str_split(my_string, " ", &list);
    UTK_TEST_ASSERT(count == 1);
    i = 0;
    utk_str_list_for_each_entry(&list, item)
    {
	UTK_TEST_ASSERT(strcmp(items_expect2[i], item->value) == 0);
	/* printf(" * item = %s\n", item->value); */
	++i;
    }
    UTK_TEST_ASSERT(count == i);
    utk_str_list_cleanup(&list);

    /* garbage in string */
    const char *items_expect3[] = {
	"",
	"one",
	"",
	"two",
	"", "",
	"three",
	"","","",
	"four",
	"","","","",
	"five",
	"","","","","",
    };

    my_string = ",one,,two,,,three,,,,four,,,,,five,,,,,";

    count = utk_str_split(my_string, ",", &list);
    /*printf(" * count = %d | expect = %ld\n",
      count, UTK_ARRAY_SIZE(items_expect3));*/
    UTK_TEST_ASSERT(count == UTK_ARRAY_SIZE(items_expect3));
    i = 0;
    utk_str_list_for_each_entry(&list, item)
    {
	/* printf(" * item = %s, strlen()= %zu\n", */
	/*        item->value, strlen(item->value)); */
	UTK_TEST_ASSERT(strcmp(items_expect3[i], item->value) == 0);
	++i;
    }
    UTK_TEST_ASSERT(count == i);
    utk_str_list_cleanup(&list);

    /* empty values */
    const char *items_expect4[] = {
	"dnsmasq",
	"",
	"",
	"",
    };

    my_string = "dnsmasq,,,";

    count = utk_str_split(my_string, ",", &list);
    UTK_TEST_ASSERT(count == UTK_ARRAY_SIZE(items_expect4));
    i = 0;
    utk_str_list_for_each_entry(&list, item)
    {
	/*printf(" * item = %s, strlen()= %zu\n",
	  item->value, strlen(item->value));*/
	UTK_TEST_ASSERT(strcmp(items_expect4[i], item->value) == 0);
	++i;
    }

    utk_str_list_cleanup(&list);
}

UTK_TEST_DEF(test_str_ltrim)
{
    const char *my_string = NULL,
	*ret = NULL;

    /* basic test */
    my_string = " \t  \n \v \r   Hello\n\r\n\t \v";

    ret = utk_str_ltrim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, "Hello\n\r\n\t \v") == 0);

    /* empty string */
    my_string = "";

    ret = utk_str_ltrim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, "") == 0);

    /* zzzzemptyzzzz */
    my_string = "zzzzemptyzzzz";

    ret = utk_str_ltrim(my_string, "z");

    UTK_TEST_ASSERT(strcmp(ret, "emptyzzzz") == 0);
}

UTK_TEST_DEF(test_str_rtrim)
{
    char *my_string = NULL,
	*ret = NULL;

    /* basic test */
    my_string = strdup(" \t  \n  \r \v  Hello\n\r\n\t \v");

    ret = utk_str_rtrim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, " \t  \n  \r \v  Hello") == 0);

    free(my_string);

    /* empty string */
    my_string = strdup("");

    ret = utk_str_rtrim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, "") == 0);

    free(my_string);

    /* zzzzemptyzzzz */
    my_string = strdup("zzzzemptyzzzz");

    ret = utk_str_rtrim(my_string, "z");

    UTK_TEST_ASSERT(strcmp(ret, "zzzzempty") == 0);

    free(my_string);
}

UTK_TEST_DEF(test_str_trim)
{
    char *my_string = NULL;
    const char *ret = NULL;

    /* basic test */
    my_string = strdup(" \t  \n  \r   Hello\n\r\n\t ");

    ret = utk_str_trim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, "Hello") == 0);

    free(my_string);

    /* empty string */
    my_string = strdup("");

    ret = utk_str_trim_blanks(my_string);

    UTK_TEST_ASSERT(strcmp(ret, "") == 0);

    free(my_string);

    /* aaaaemptyzzzz */
    my_string = strdup("aaaaemptyzzzz");

    ret = utk_str_trim(my_string, "az");

    UTK_TEST_ASSERT(strcmp(ret, "empty") == 0);

    free(my_string);
}

UTK_TEST_DEF(test_str_startwith)
{
    UTK_TEST_ASSERT(utk_str_startwith("hello world", "hello"));
    UTK_TEST_ASSERT(!utk_str_startwith("hello world", "ello"));

    UTK_TEST_ASSERT(!utk_str_startwith("hell", "hello"));
}

UTK_TEST_DEF(test_str_endwith)
{
    UTK_TEST_ASSERT(utk_str_endwith("hello world", "world"));
    UTK_TEST_ASSERT(!utk_str_endwith("hello world", "worl"));

    UTK_TEST_ASSERT(!utk_str_endwith("orld", "world"));
}

UTK_TEST_DEF(test_str_replace)
{
    char buffer[256],
	buf16[16];
    int ret;

    /* simple test */
    ret = utk_str_replace("http://www.github.com/",
                          "http", "flocfloc",
                          buffer, sizeof(buffer));

    UTK_TEST_ASSERT(ret == 0);
    UTK_TEST_ASSERT(strcmp(buffer, "flocfloc://www.github.com/") == 0);

    /* test truncation */
    ret = utk_str_replace("01234567890123456789",
                          "1", "0",
                          buf16, sizeof(buf16));

    UTK_TEST_ASSERT(ret != 0);
    UTK_TEST_ASSERT(strcmp(buf16, "002345678900234") == 0);
    UTK_TEST_ASSERT(buf16[sizeof(buf16) - 1] == '\0');
}

UTK_TEST_DEF(test_str_lcut)
{
    const char *p = NULL;

    /* simple test */
    p = utk_str_lcut("http://www.linux.org", "http://");

    UTK_TEST_ASSERT(strcmp("www.linux.org", p) == 0);

    /* no cut this ! */
    p = utk_str_lcut("xhttp://www.linux.org", "http://");

    UTK_TEST_ASSERT(strcmp("xhttp://www.linux.org", p) == 0);
}

UTK_TEST_DEF(test_str_rcut)
{
    char buf256[256];
    char *p = NULL;

    /* simple test */
    snprintf(buf256, sizeof(buf256), "mynameisnobody.txt");

    p = utk_str_rcut(buf256, ".txt");

    UTK_TEST_ASSERT(strcmp("mynameisnobody", p) == 0);

    /* no cut this ! */
    snprintf(buf256, sizeof(buf256), "mynameisnobody.txt");

    p = utk_str_rcut(buf256, ".tx");

    UTK_TEST_ASSERT(strcmp("mynameisnobody.txt", p) == 0);
}

UTK_TEST_DEF(test_str_tol)
{
    const char *my_string = NULL;
    long ret;

    /* basic test */
    my_string = "13";

    ret = utk_str_tol(my_string, NULL, 10, -1);

    UTK_TEST_ASSERT(ret == 13);

    /* empty string */
    my_string = "";

    ret = utk_str_tol(my_string, NULL, 10, -1);

    UTK_TEST_ASSERT(ret == -1);
}

UTK_TEST_DEF(test_str_toll)
{
    const char *my_string = NULL;
    long long ret;

    /* basic test */
    my_string = "13";

    ret = utk_str_toll(my_string, NULL, 10, -1);

    UTK_TEST_ASSERT(ret == 13);

    /* empty string */
    my_string = "";

    ret = utk_str_toll(my_string, NULL, 10, -1);

    UTK_TEST_ASSERT(ret == -1);
}

UTK_TEST_DEF(test_str_list_toarray)
{
    struct utk_str_list list;
    unsigned int count,
	ret,
	i;
    const char *my_string = NULL;
    const char *ip_parts[4];

    /* basic test */
    const char *items_expect[] = {
	"192",
	"168",
	"1",
	"1",
    };

    my_string = "192.168.1.1";

    count = utk_str_split(my_string, ".", &list);

    ret = utk_str_list_toarray(&list, ip_parts, UTK_ARRAY_SIZE(ip_parts));

    UTK_TEST_ASSERT(count == 4);
    UTK_TEST_ASSERT(count == ret);

    for(i = 0; i < UTK_ARRAY_SIZE(ip_parts); ++i)
    {
	/* printf("ip[%d] = %s\n", i, ip_parts[i]); */
	UTK_TEST_ASSERT(strcmp(ip_parts[i], items_expect[i]) == 0);
    }

    utk_str_list_cleanup(&list);

    /* number of item in list != of number of items in array wanted */
    my_string = "foo";

    count = utk_str_split(my_string, ".", &list);

    ret = utk_str_list_toarray(&list, ip_parts, UTK_ARRAY_SIZE(ip_parts));

    UTK_TEST_ASSERT(count == 1);
    UTK_TEST_ASSERT(count == ret);

    utk_str_list_cleanup(&list);
}

UTK_TEST_DEF(test_str_list_add_remove)
{
    struct utk_str_list str_list;
    struct utk_str_list_item *str_list_item;
    const char *items[] = {
	"the world is crazy",
	"allright",
	"one good thing about music",
	"when it hurts you, you feel no pain",
	"allright",
    };
    unsigned int found,
	i;

    utk_str_list_init(&str_list);

    for(i = 0; i < UTK_ARRAY_SIZE(items); ++i)
    {
	UTK_TEST_ASSERT(utk_str_list_add(&str_list, items[i]) == 0);
    }

    utk_str_list_for_each_entry(&str_list, str_list_item)
    {
	for(i = 0; i < UTK_ARRAY_SIZE(items); ++i)
	{
	    found = 0;
	    if(strcmp(str_list_item->value,
		      items[i]) == 0)
	    {
		found = 1;
		break;
	    }
	}

	UTK_TEST_ASSERT(found);
    }

    UTK_TEST_ASSERT(utk_str_list_remove(&str_list, "allright") == 2);

    found = 0;
    utk_str_list_for_each_entry(&str_list, str_list_item)
    {
	if(strcmp(str_list_item->value, "allright") == 0)
	{
	    ++found;
	}
    }

    UTK_TEST_ASSERT(found == 0);

    utk_str_list_cleanup(&str_list);
}

int main(void)
{
    UTK_TEST_MODULE_INIT("utk/str");

    UTK_TEST_RUN(test_str_copy);
    UTK_TEST_RUN(test_str_printf);
    UTK_TEST_RUN(test_str_cat);
    UTK_TEST_RUN(test_str_catf);
    UTK_TEST_RUN(test_str_matches);
    UTK_TEST_RUN(test_str_empty);

    UTK_TEST_RUN(test_str_split);
    UTK_TEST_RUN(test_str_ltrim);
    UTK_TEST_RUN(test_str_rtrim);
    UTK_TEST_RUN(test_str_trim);

    UTK_TEST_RUN(test_str_startwith);
    UTK_TEST_RUN(test_str_endwith);

    UTK_TEST_RUN(test_str_replace);

    UTK_TEST_RUN(test_str_lcut);
    UTK_TEST_RUN(test_str_rcut);

    UTK_TEST_RUN(test_str_tol);
    UTK_TEST_RUN(test_str_toll);

    UTK_TEST_RUN(test_str_list_toarray);
    UTK_TEST_RUN(test_str_list_add_remove);

    return UTK_TEST_MODULE_RETURN;
}
