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

#ifndef _UTK_STR_H_
#define _UTK_STR_H_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "utk/list.h"

/*
 * Structures used when deal with list of string.
 */
struct utk_str_list_item {
    char *value;
    struct utk_list_head node;
};

struct utk_str_list {
    struct utk_list_head head;
    unsigned int count;
};

/* 
 * utk_str_copy (aka "safe strcpy") - wrapper function to strncpy
 *
 *  Copy string into buffer.
 *
 *  - unlike strncpy(), destination buffer is always null terminated;
 *  - unlike strncpy(), return count of char copied (or should have 
 *    been copied if truncation occurred);
 *  - if return > dst_size - 1, truncation occurred.
 *
 * \param dst Destination buffer where source string will be copied
 * \param dst_size Size of destination buffer
 * \param src Source String
 * \return count of char copied (or should have been copied in case
 *                               of truncation)
 */
size_t utk_str_copy(char *dst, size_t dst_size, const char *src);

/*
 * utk_str_vprintf (aka "safe vsprintf") - wrapper function to vsnprintf
 *
 *  Copy formatted output conversion into buffer.
 *
 *  - vsnprintf as specified in ISO C99 is guaranteed to null-terminate but it
 *    seems the behavior on certain platforms is different;
 *  - never call this function with a fmt given by user input
 *    (FIO30-C.+Exclude+user+input+from+format+strings);
 *  - destination buffer is always null terminated even if error occurred;
 *  - if return > dst_size - 1, truncation occurred.
 *
 * \param dst Destination buffer where output will be copied
 * \param dst_size Size of destination buffer
 * \param fmt Formated string
 * \param args va_list
 * \return same return as vsnprintf
 */
int utk_str_vprintf(char *dst, size_t dst_size, const char *fmt, va_list args);

/*
 * utk_str_printf (aka "safe sprintf") - wrapper function snprintf
 *
 *  Copy formatted output conversion into buffer.
 *
 *  - snprintf as specified in ISO C99 is guaranteed to null-terminate but it
 *    seems the behavior on certain platforms is different;
 *  - never call this function with a fmt given by user input
 *    (FIO30-C.+Exclude+user+input+from+format+strings);
 *  - destination buffer is always null terminated even if error occurred;
 *  - if return > dst_size - 1, truncation occurred.
 *
 * \param dst Destination buffer where output will be copied
 * \param dst_size Size of destination buffer
 * \param fmt Formated string
 * \param ... The arguments
 * \return same return as snprintf
 */
int utk_str_printf(char *dst, size_t dst_size, const char *fmt, ...);

/* 
 * utk_str_cat (aka "simple strcat")
 *
 *  Concat string into buffer.
 *
 * - Be carefull, dst string buffer must be initiliazed before first call
 *   (with dst[0] = '\0' for example);
 * - destination buffer is always null terminated;
 * - unlike strncat, dst_size is the full size of dst, not space left !;
 * - copy at most dst_size - 1 characters;
 * - if return > dst_size - 1, truncation occurred.
 *
 * \param dst Destination buffer where source string will be concat
 * \param dst_size Size of destination buffer (Total size not remaining)
 * \param src Source String
 * \return total count of char in destination buffer (or should have been 
 *                                                    in buffer in case of
 *                                                    truncation)
 */
size_t utk_str_cat(char *dst, size_t dst_size, const char *src);

/*
 * utk_str_catf
 *
 *  Concat formatted string into buffer.
 *
 * - Be carefull, dst string buffer must be initiliazed before first call
 *   (with dst[0] = '\0' for example);
 * - destination buffer is always null terminated;
 * - unlike strncat, dst_size is the full size of dst, not space left !;
 * - copy at most dst_size - 1 characters;
 * - if return > dst_size - 1, truncation occurred.
 *
 * \param dst Destination buffer where source string will be concat
 * \param dst_size Size of destination buffer (Total size not remaining)
 * \param fmt Formated string
 * \param ... The arguments
 * \return total count of char in destination buffer (or should have been
 *                                                    in buffer in case of
 *                                                    truncation)
 */
size_t utk_str_catf(char *dst, size_t dst_size, const char *fmt, ...);

/*
 * utk_str_matches
 *
 *  Compare a string to a constant string.
 *
 * - Be carefull, c_str must be an constant string !
 *
 * \param data Data string
 * \param cstr Must be an constant string !
 * \return 0 if strings doesn't matches, != 0 if matches.
 */
#define utk_str_matches(s, c_str)			\
    ({							\
	const char __dummy[] = c_str;			\
	(void)(&__dummy);				\
	(memcmp (s, c_str, sizeof(c_str)) == 0);	\
    })

/*
 * utk_str_empty
 *
 *  Test if string is empty.
 *
 * \return 0 if string isn't empty, different from 0 if empty
 */
int utk_str_empty(const char *str);

/*
 * utk_str_split
 *
 *  Split string into a list of NONEMPTY words,
 *   using sep as the word delimiter.
 *
 * - Think to cleanup list (with utk_str_list_cleanup()) after you finished with it;
 * - utk_str_split("foo    bar", " ", &list) returns a list of 2 items (foo and bar)
 *
 * Example:
 *
 *      count = utk_str_split("Hello World!", " ", &list);
 *      utk_str_list_for_each_entry(item, &list)
 *      {
 *             // you stuff //
 *      }
 *      utk_str_list_cleanup(&list);
 *
 * \param str Data string
 * \param sep The word delimiter
 * \param list Pointer to an empty list where utk_str_split put
 *             struct utk_str_list_item items.
 * \return count of words found and stored in list
 */
unsigned int utk_str_split(const char *str, const char *sep,
			   struct utk_str_list *list);

/*
 * utk_str_ltrim
 *
 * Remove caracters defined in trimchr argument at start of the string.
 *
 * \param str string to left trim
 * \param trimchr caracters to remove
 * \return pointer to the string left trimed
 */
const char* utk_str_ltrim(const char *str, const char *trimchr);

/*
 * utk_str_rtrim
 *
 * Remove caracters defined in trimchr argument at start of the string.
 *
 * \param str string to right trim
 * \param trimchr caracters to remove
 * \return string right trimed
 */
char* utk_str_rtrim(char *str, const char *trimchr);

/*
 * utk_str_trim
 *
 * Remove caracters defined in trimchr argument at start and end of the string.
 *
 * \param str string to trim
 * \return string trimed
 */
static inline const char* utk_str_trim(char *str, const char *trimchr)
{
    return utk_str_ltrim(utk_str_rtrim(str, trimchr), trimchr);
}

/*
 * Helper macros for trim spaces, \t, \n, \r and \v.
 */
#define utk_str_ltrim_blanks(str) utk_str_ltrim(str, " \t\n\r\v")
#define utk_str_rtrim_blanks(str) utk_str_rtrim(str, " \t\n\r\v")
#define utk_str_trim_blanks(str) utk_str_trim(str, " \t\n\r\v")

/*
 * utk_str_startwith
 *
 *  Tell if one string is at start of another one
 *
 * \param haystack the base string
 * \param word the string to seek at start of haystack
 * \return 1 if found, 0 otherwise
 */
static inline int utk_str_startwith(const char *haystack, const char *word)
{
    return (strncmp(haystack, word, strlen(word)) == 0);
}

/*
 * utk_str_endwith
 *
 *  Tell if one string is at end of another one
 *
 * \param haystack the base string
 * \param word the string to seek at end of haystack
 * \return 1 if found, 0 otherwise
 */
static inline int utk_str_endwith(const char *haystack, const char *word)
{
    size_t word_len = strlen(word),
	haystack_len = strlen(haystack);

    if(word_len > haystack_len)
    {
	return 0;
    }

    return strncmp(haystack + haystack_len - word_len,
		   word,
		   word_len) == 0;
}

/*
 * utk_str_replace
 *
 *  replace one string to another one in a string
 *
 * \param haystack the base string
 * \param fromword the string to replace
 * \param toword the new string which replace fromword
 * \param output a buffer with a good size to store result
 * \param output_size the size of buffer output
 * \return 0 if string has been replaced, 1 otherwise
 */
int utk_str_replace(const char *haystack, const char *fromword, const char *toword,
		    char *output, size_t output_size);

/*
 * utk_str_lcut
 *
 *  remove one string at left of another string
 *
 * \param haystack the base string
 * \param word the string to cut at left of base string
 * \return the string cutted (or not, if word hasn't found at left of haystack)
 */
static inline const char *utk_str_lcut(const char *haystack, const char *word)
{
    return (utk_str_startwith(haystack, word) ?
	    haystack + strlen(word) : haystack);
}

/*
 * utk_str_rcut
 *
 *  remove one string at right of another string
 *
 * \param haystack the base string
 * \param word the string to cut at right of base string
 * \return the string cutted (or not, if word hasn't found at right of haystack)
 */
static inline char *utk_str_rcut(char *haystack, const char *word)
{
    if(utk_str_endwith(haystack, word))
    {
	haystack[strlen(haystack) - strlen(word)] = '\0';
    }

    return haystack;
}

/*
 * utk_str_tol (aka "simple strtol") - wrapper to strtol
 *
 *  Convert a string to a long integer
 *
 * - This function request new argument "dfl" unlike strtol;
 * - Check errno return. If strtol fails, return dfl value.
 *
 * \brief Convert a string to a long integer
 * \param str string to convert
 * \param endptr first character in str which is not integer
 * \param base the base of the integer in string
 * \param dfl default value if convertion failed
 * \return long integer
 */
long utk_str_tol(const char *str, char **endptr, int base, long dfl);

/*
 * utk_str_toll (aka "simple strtoll") - wrapper to strtoll
 *
 *  Convert a string to a long long integer
 *
 * - This function request new argument "dfl" unlike strtoll;
 * - Check errno return. If strtol fails, return dfl value.
 *
 * \param str string to convert
 * \param endptr first character in str which is not integer
 * \param base the base of the integer in string
 * \param dfl default value if convertion failed
 * \return long long integer
 */
long long utk_str_toll(const char *str, char **endptr, int base, long long dfl);

/*
 * utk_str_list_init
 *
 * Init a list of str.
 *
 * \param list The list which will be initialized
 * \return void
 */
void utk_str_list_init(struct utk_str_list *list);

/*
 * utk_str_list_cleanup
 *
 * Free a list of str.
 *
 * \param list The list which will be freed
 * \return void
 */
void utk_str_list_cleanup(struct utk_str_list *list);

/*
 * utk_str_list_add
 *
 * Add a string in list.
 *
 * \param list The list where the string will be added
 * \param str The string to be added
 * \return 0 if the string was added, -1 otherwise
 */
int utk_str_list_add(struct utk_str_list *list, const char *str);

/*
 * utk_str_list_remove
 *
 * Remove a string in list.
 *
 * - Each matched string item in list will be removed.
 *
 * \param list The list where the string will be added
 * \param str The string to be removed
 * \return the number of remove or 0 if none string item was removed
 */
int utk_str_list_remove(struct utk_str_list *list, const char *str);

/*
 * utk_str_list_length
 *
 * Return the length of the list
 *
 * \param list The list where the string will be added
 * \return number of item in the list
 */
unsigned int utk_str_list_length(struct utk_str_list *list);

/*
 * utk_str_list_toarray
 *
 * Fill a string array with a list of str.
 *
 * \param list The list which contains utk_str_list_item
 * \param array The array of const char
 * \param size The size of array 
 * \return int number of item copied
 */
unsigned int utk_str_list_toarray(struct utk_str_list *list,
				  const char **array, size_t size);

/*
 * Walk over string list
 */
#define utk_str_list_for_each_entry(list, item)		\
    utk_list_for_each_entry(item, &((list)->head), node)

#endif

