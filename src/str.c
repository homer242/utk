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

#include "utk/str.h"
#include "utk/list.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

size_t utk_str_copy(char *dst, size_t dst_size, const char *src)
{
    size_t src_len = strlen(src);

    strncpy(dst, src, dst_size);
    dst[dst_size - 1] = '\0';

    return (src_len >= dst_size ? src_len : strlen(dst));
}

int utk_str_vprintf(char *dst, size_t size, const char *fmt, va_list args)
{
    int ret;

    ret = vsnprintf(dst, size, fmt, args);
    if(ret < 0)
    {
	dst[0] = '\0';
    }
    else if(ret > (int)size - 1)
    {
	/* buffer has been truncated */
	dst[size - 1] = '\0';
    }

    return ret;
}

int utk_str_printf(char *dst, size_t size, const char *fmt, ...)
{
    int ret;
    va_list args;

    va_start(args, fmt);
    ret = utk_str_vprintf(dst, size, fmt, args);
    va_end(args);

    return ret;
}

size_t utk_str_cat(char *dst, size_t dst_size, const char *src)
{
    size_t dst_len = strlen(dst);
    size_t dst_remain_size;
    char *dst_p = NULL;
    size_t ret;

    dst_p = dst + dst_len;
    dst_remain_size = dst_size - dst_len;

    ret = utk_str_copy(dst_p, dst_remain_size, src);

    return ret + dst_len;
}

size_t utk_str_catf(char *dst, size_t dst_size, const char *fmt, ...)
{
    size_t dst_len = strlen(dst);
    size_t dst_remain_size;
    char *dst_p = NULL;
    int ret;
    va_list args;

    dst_p = dst + dst_len;
    dst_remain_size = dst_size - dst_len;

    va_start(args, fmt);
    ret = utk_str_vprintf(dst_p, dst_remain_size, fmt, args);
    va_end(args);

    if(ret < 0)
	ret = 0;

    return (size_t)ret + dst_len;
}

int utk_str_empty(const char *str)
{
    return (str[0] == '\0');
}

unsigned int utk_str_split(const char *str, const char *sep,
			   struct utk_str_list *list)
{
    char *strrw = NULL,
	*strrw_p = NULL,
	*sep_in_str = NULL;
    size_t len_sep = strlen(sep);

    utk_str_list_init(list);

    strrw = strdup(str);
    if(strrw == NULL)
    {
	return 0;
    }

    strrw_p = strrw;
    while((sep_in_str = strstr(strrw_p, sep)) != NULL)
    {
	*sep_in_str = '\0';

	if(utk_str_list_add(list, strrw_p) != 0)
	{
	    utk_str_list_cleanup(list);
	    goto ex_on_populate;
	}

	strrw_p = sep_in_str + len_sep;
    }

    /* copy the last argument */
    if(utk_str_list_add(list, strrw_p) != 0)
    {
	utk_str_list_cleanup(list);
	goto ex_on_populate;
    }

ex_on_populate:
    free(strrw);

    return utk_str_list_length(list);
}

const char* utk_str_ltrim(const char *str, const char *trimchr)
{
    while(*str != '\0')
    {
	if(strchr(trimchr, *str) == NULL)
	{
	    return str;
	}

	++str;
    }

    return str;
}

char* utk_str_rtrim(char *str, const char *trimchr)
{
    char *end = str + strlen(str);

    while(end-- > str)
    {
	if(strchr(trimchr, *end) == NULL)
	{
	    return str;
	}

	*end = 0;
    }

    return str;
}

int utk_str_replace(const char *haystack, const char *fromword, const char *toword,
		    char *output, size_t output_size)
{
    const char *p = NULL,
	*haystack_p = haystack;
    size_t fromword_len = strlen(fromword);

    output[0] = '\0';

    while((p = strstr(haystack_p, fromword)) != NULL)
    {
	if(utk_str_catf(output, output_size,
			"%.*s", p - haystack_p, haystack_p) >= output_size
	   || utk_str_cat(output, output_size, toword) >= output_size)
	{
	    /* truncation */
	    return -1;
	}

	p += fromword_len;
	haystack_p = p;
    }

    if(utk_str_cat(output, output_size, haystack_p) >= output_size)
    {
	/* truncation */
	return -1;
    }

    return 0;
}

long utk_str_tol(const char *str, char **endptr, int base, long dfl)
{
    long ret;

    if(*str == '\0')
    {
	return dfl;
    }

    errno = 0;
    ret = strtol(str, endptr, base);
    if (errno != 0)
    {
	return dfl;
    }

    return ret;
}

long long utk_str_toll(const char *str, char **endptr, int base, long long dfl)
{
    long long ret;

    if(*str == '\0')
    {
	return dfl;
    }

    errno = 0;
    ret = strtoll(str, endptr, base);
    if (errno != 0)
    {
	return dfl;
    }

    return ret;      
}

void utk_str_list_init(struct utk_str_list *list)
{
    utk_list_head_init(&list->head);
    list->count = 0;
}

int utk_str_list_add(struct utk_str_list *list, const char *str)
{
    struct utk_str_list_item *item;

    item = calloc(1, sizeof(*item));
    if(item == NULL)
    {
	return -1;
    }

    item->value = strdup(str);
    if(item->value == NULL)
    {
	free(item);
	return -1;
    }

    utk_list_add_tail(&item->node, &list->head);
    ++list->count;

    return 0;
}

int utk_str_list_remove(struct utk_str_list *list, const char *str)
{
    struct utk_str_list_item *item = NULL,
	*item_safe = NULL;
    int found = 0;

    utk_list_for_each_entry_safe(item, item_safe, &list->head, node)
    {
	if(strcmp(item->value, str) == 0)
	{
	    utk_list_del(&(item->node));
	    free(item->value);
	    free(item);
	    ++found;
	    --list->count;
	}
    }

    return found;
}

unsigned int utk_str_list_length(struct utk_str_list *list)
{
    return list->count;
}

void utk_str_list_cleanup(struct utk_str_list *list)
{
    struct utk_str_list_item *item = NULL,
	*item_safe = NULL;

    utk_list_for_each_entry_safe(item, item_safe, &list->head, node)
    {
	utk_list_del(&(item->node));
	free(item->value);
	free(item);
    }

    list->count = 0;
}

unsigned int utk_str_list_toarray(struct utk_str_list *list,
				  const char **array, size_t size)
{
    struct utk_str_list_item *item = NULL;
    unsigned int count = 0;

    utk_list_for_each_entry(item, &list->head, node)
    {
	array[count] = item->value;

	++count;

	if(count == size)
	{
	    break;
	}
    }

    return count;
}
