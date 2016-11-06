/*
 * Copyright (c) 2011-2016 Anthony Viallard
 *
 *    This file is part of Utk (copied from kernel source tree
 *                                and licensed under the GPL v2
 *                                as per the whole kernel source tree).
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


/**
 * \file list.h (copied from the Linux kernel source tree)
 * \brief Implement doubly linked list (list_head)
 *
 * I made some changes:
 * - I remove the old UTK_LIST_HEAD INIT macro ;
 * - I rename INIT UTK_LIST_HEAD() to utk_list_head_init()
 */

#ifndef _UTK_LIST_H_
#define _UTK_LIST_H_

#include <stddef.h>

/**
 * container_of - cast a member of a descriptor out to the containing descriptor
 *
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
      typeof(((type *)0)->member) *__mptr = (ptr);		\
      (type *)((char *)__mptr - offsetof(type,member));})

/*!
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define UTK_LIST_POISON1  ((void *) 0x00100100)
#define UTK_LIST_POISON2  ((void *) 0x00200200)

/*!
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct utk_list_head {
    struct utk_list_head *next, *prev;
};

#define UTK_LIST_HEAD(name)				\
    struct utk_list_head name = { &(name), &(name) }

static inline void utk_list_head_init(struct utk_list_head *list)
{
    list->next = list;
    list->prev = list;
}

/*!
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void
__utk_list_add(struct utk_list_head *new,
	       struct utk_list_head *prev, struct utk_list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/*!
 * utk_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void utk_list_add(struct utk_list_head *new, struct utk_list_head *head)
{
    __utk_list_add(new, head, head->next);
}

/*!
 * utk_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void utk_list_add_tail(struct utk_list_head *new, struct utk_list_head *head)
{
    __utk_list_add(new, head->prev, head);
}

/*!
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __utk_list_del(struct utk_list_head *prev, struct utk_list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/*!
 * utk_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: utk_list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void utk_list_del(struct utk_list_head *entry)
{
    __utk_list_del(entry->prev, entry->next);
    entry->next = UTK_LIST_POISON1;
    entry->prev = UTK_LIST_POISON2;
}

/*!
 * utk_list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void utk_list_del_init(struct utk_list_head *entry)
{
    __utk_list_del(entry->prev, entry->next);
    utk_list_head_init(entry);
}

/**
 * utk_list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void utk_list_replace(struct utk_list_head *old, struct utk_list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void
utk_list_replace_init(struct utk_list_head *old, struct utk_list_head *new)
{
    utk_list_replace(old, new);
    utk_list_head_init(old);
}

/*!
 * utk_list_swap - swap elements from  list
 * @a: the entry to move to a
 * @b: the entry to move to b
 */
static inline void utk_list_swap(struct utk_list_head *a, struct utk_list_head *b)
{
    if (a->next == b) {
	utk_list_replace(a, b);
	utk_list_add(b, a);
    } else if (a->prev == b) {
	utk_list_replace(b, a);
	utk_list_add(a, b);
    } else {
	struct utk_list_head tmp;

	tmp.next = b->next;
	tmp.prev = b->prev;
	utk_list_replace(a, b);
	utk_list_replace(&tmp, a);
    }
}

/*!
 * utk_list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void utk_list_move(struct utk_list_head *list, struct utk_list_head *head)
{
    __utk_list_del(list->prev, list->next);
    utk_list_add(list, head);
}

/*!
 * utk_list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void
utk_list_move_tail(struct utk_list_head *list, struct utk_list_head *head)
{
    __utk_list_del(list->prev, list->next);
    utk_list_add_tail(list, head);
}

/**
 * utk_list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int
utk_list_is_last(const struct utk_list_head *list, const struct utk_list_head *head)
{
    return list->next == head;
}

/*!
 * utk_list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int utk_list_empty(struct utk_list_head const *head)
{
    return head->next == head;
}

static inline void __utk_list_splice(struct utk_list_head *list, struct utk_list_head *head)
{
    struct utk_list_head *first = list->next;
    struct utk_list_head *last = list->prev;
    struct utk_list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/*!
 * utk_list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void utk_list_splice(struct utk_list_head *list, struct utk_list_head *head)
{
    if (!utk_list_empty(list))
	__utk_list_splice(list, head);
}

/*!
 * utk_list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void
utk_list_splice_init(struct utk_list_head *list, struct utk_list_head *head)
{
    if (!utk_list_empty(list)) {
	__utk_list_splice(list, head);
	utk_list_head_init(list);
    }
}

/*!
 * utk_list_entry - get the struct for this entry
 * @ptr:    the &struct utk_list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the utk_list_struct within the struct.
 */
#define utk_list_entry(ptr, type, member)	\
    container_of(ptr, type, member)

/**
 * utk_list_first_entry - get the first element from a list
 * @ptr:        the list head to take the element from.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define utk_list_first_entry(ptr, type, member) \
    utk_list_entry((ptr)->next, type, member)

/*!
 * utk_list_for_each    -    iterate over a list
 * @pos:    the &struct utk_list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define utk_list_for_each(pos, head)				\
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*!
 * utk_list_for_each_prev    -    iterate over a list backwards
 * @pos:    the &struct utk_list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define utk_list_for_each_prev(pos, head)			\
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/*!
 * utk_list_for_each_safe    -    iterate over a list safe against removal of list entry
 * @pos:    the &struct utk_list_head to use as a loop counter.
 * @n:        another &struct utk_list_head to use as temporary storage
 * @head:    the head for your list.
 */
#define utk_list_for_each_safe(pos, n, head)			\
    for (pos = (head)->next, n = pos->next; pos != (head);	\
	 pos = n, n = pos->next)

/*!
 * utk_list_for_each_entry    -    iterate over list of given type
 * @pos:    the type * to use as a loop counter.
 * @head:    the head for your list.
 * @member:    the name of the utk_list_struct within the struct.
 */
#define utk_list_for_each_entry(pos, head, member)			\
    for (pos = utk_list_entry((head)->next, typeof(*pos), member);	\
	 &pos->member != (head);					\
	 pos = utk_list_entry(pos->member.next, typeof(*pos), member))

/*!
 * utk_list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:    the type * to use as a loop counter.
 * @head:    the head for your list.
 * @member:    the name of the utk_list_struct within the struct.
 */
#define utk_list_for_each_entry_reverse(pos, head, member)		\
    for (pos = utk_list_entry((head)->prev, typeof(*pos), member);	\
	 &pos->member != (head);					\
	 pos = utk_list_entry(pos->member.prev, typeof(*pos), member))

/**
 * utk_list_for_each_entry_continue - continue iteration over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define utk_list_for_each_entry_continue(pos, head, member)		\
    for (pos = utk_list_entry(pos->member.next, typeof(*pos), member);	\
	 &pos->member != (head);					\
	 pos = utk_list_entry(pos->member.next, typeof(*pos), member))

/**
 * utk_list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define utk_list_for_each_entry_from(pos, head, member)			\
    for (; &pos->member != (head);					\
	 pos = utk_list_entry(pos->member.next, typeof(*pos), member))

/*!
 * utk_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop counter.
 * @n:        another type * to use as temporary storage
 * @head:    the head for your list.
 * @member:    the name of the utk_list_struct within the struct.
 */
#define utk_list_for_each_entry_safe(pos, n, head, member)		\
    for (pos = utk_list_entry((head)->next, typeof(*pos), member),	\
	     n = utk_list_entry(pos->member.next, typeof(*pos), member); \
	 &pos->member != (head);					\
	 pos = n, n = utk_list_entry(n->member.next, typeof(*n), member))

/**
 * utk_list_for_each_entry_safe_continue
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define utk_list_for_each_entry_safe_continue(pos, n, head, member)	\
    for (pos = utk_list_entry(pos->member.next, typeof(*pos), member),	\
	     n = utk_list_entry(pos->member.next, typeof(*pos), member); \
	 &pos->member != (head);					\
	 pos = n, n = utk_list_entry(n->member.next, typeof(*n), member))

/**
 * utk_list_for_each_entry_safe_from
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define utk_list_for_each_entry_safe_from(pos, n, head, member)		\
    for (n = utk_list_entry(pos->member.next, typeof(*pos), member);	\
	 &pos->member != (head);					\
	 pos = n, n = utk_list_entry(n->member.next, typeof(*n), member))

/**
 * utk_list_for_each_entry_safe_reverse
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the utk_list_struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define utk_list_for_each_entry_safe_reverse(pos, n, head, member)	\
    for (pos = utk_list_entry((head)->prev, typeof(*pos), member),	\
	     n = utk_list_entry(pos->member.prev, typeof(*pos), member); \
	 &pos->member != (head);					\
	 pos = n, n = utk_list_entry(n->member.prev, typeof(*n), member))

#endif
