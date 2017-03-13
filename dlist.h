#ifndef DLIST_H_INCLUDED
#define DLIST_H_INCLUDED

/**********************************************************************************
* Simple doubly-linked list
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* dlist.h */

#include "sal_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* simple double-linked list: (next,prev,first,last - are never NULLs)

    ----------------------------------------------------------
   /     entry       dlist      entry                         \
   \     ------     -------     ------             ------     /
    ---> |next| --> |first| --> |next| --> ... --> |next| >---
    ---< |prev| <-- |last | <-- |prev| <--     <-- |prev| <---
   /     |data|     -------     |data|             |data|     \
   \     ------                 ------             ------     /
    ----------------------------------------------------------
*/

struct dlist_entry {
	struct dlist_entry *next;
	struct dlist_entry *prev;
	/* ... user data ... */
};

/* list head without data */
struct dlist {
	struct dlist_entry e;
};

#define dlist_first e.next
#define dlist_last  e.prev

/* aaaa + bbbbb  -> bbbbbaaaa
   ^a     ^s  ^e    ^a        */
A_Nonnull_all_args
A_Pre_satisfies(s != *a)
A_Pre_satisfies(e != *a)
static inline void _dlist_prepend_(
	A_Inout A_At(*a, A_Inout) struct dlist_entry **a,
	A_Inout struct dlist_entry *s/*==e?*/,
	A_Inout struct dlist_entry *e)
{
	struct dlist_entry *A_Restrict f = *a;
	*a = s;
	f->prev = e;
	e->next = f;
}

/* aaaa + bbbbb  -> aaaabbbbb
      ^a  ^s  ^e            ^a */
A_Nonnull_all_args
A_Pre_satisfies(s != *a)
A_Pre_satisfies(e != *a)
static inline void _dlist_append_(
	A_Inout A_At(*a, A_Inout) struct dlist_entry **a,
	A_Inout struct dlist_entry *s/*==e?*/,
	A_Inout struct dlist_entry *e)
{
	struct dlist_entry *A_Restrict l = *a;
	*a = e;
	l->next = s;
	s->prev = l;
}

/* NOTE: doesn't sets e->prev
 - this may be useful if adding many entries at front in a batch */
/*
  example: add e0<->e1<->e2<-> at front of list

  _dlist_add_front_finish(
    _dlist_add_front(
      _dlist_add_front(
        _dlist_add_front(dlist, e2), e1), e0), e0);
*/
A_Nonnull_all_args
A_Pre_satisfies(dlist->dlist_first != e)
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *_dlist_add_front(
	A_Inout struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict e)
{
	_dlist_prepend_(&dlist->dlist_first, e, e);
	return dlist;
}

/* NOTE: doesn't sets e->next
 - this may be useful if adding many entries at back in a batch */
/*
  example: add <->e0<->e1<->e2 at back of list

  _dlist_add_back_finish(
    _dlist_add_back(
      _dlist_add_back(
        _dlist_add_back(dlist, e0), e1), e2), e2);
*/
A_Nonnull_all_args
A_Pre_satisfies(dlist->dlist_last != e)
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *_dlist_add_back(
	A_Inout struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict e)
{
	_dlist_append_(&dlist->dlist_last, e, e);
	return dlist;
}

/* complete _dlist_add_front(): set first->prev */
A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *_dlist_add_front_finish(
	A_In struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict first)
{
	first->prev = &dlist->e;
	return dlist;
}

/* complete _dlist_add_back(): set last->next */
A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *_dlist_add_back_finish(
	A_In struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict last)
{
	last->next = &dlist->e;
	return dlist;
}

/* complete _dlist_add_front(): set dlist->dlist_first->prev */
A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *dlist_add_front_finish(
	A_In struct dlist *dlist)
{
	return _dlist_add_front_finish(dlist, dlist->dlist_first);
}

/* complete _dlist_add_back(): set dlist->dlist_last->next */
A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *dlist_add_back_finish(
	A_In struct dlist *dlist)
{
	return _dlist_add_back_finish(dlist, dlist->dlist_last);
}

A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *dlist_add_front(
	A_Inout struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict e)
{
	return _dlist_add_front_finish(
		_dlist_add_front(dlist, e), e);
}

A_Nonnull_all_args
A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist *dlist_add_back(
	A_Inout struct dlist *dlist,
	A_Inout struct dlist_entry *A_Restrict e)
{
	return _dlist_add_back_finish(
		_dlist_add_back(dlist, e), e);
}

/* NOTE: doesn't sets current->next->prev
 - this may be useful if inserting many entries after current in a batch */
/*
  example: insert <->e0<->e1<->e2 after the current entry

  _dlist_insert_after_finish(
    _dlist_insert_after(
      _dlist_insert_after(
        _dlist_insert_after(current, e0), e1), e2));
*/
A_Nonnull_all_args
A_Pre_satisfies(current != e)
A_Ret_never_null A_Ret_range(==,e)
static inline struct dlist_entry *_dlist_insert_after(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist_entry *A_Restrict e)
{
	struct dlist_entry *A_Restrict n = current->next;
	current->next = e;
	e->next = n;
	e->prev = current;
	return e;
}

/* NOTE: doesn't sets current->prev->next
 - this may be useful if inserting many entries before current in a batch */
/*
  example: insert e0<->e1<->e2<-> before the current entry

  _dlist_insert_before_finish(
    _dlist_insert_before(
      _dlist_insert_before(
        _dlist_insert_before(current, e2), e1), e0));
*/
A_Nonnull_all_args
A_Pre_satisfies(current != e)
A_Ret_never_null A_Ret_range(==,e)
static inline struct dlist_entry *_dlist_insert_before(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist_entry *A_Restrict e)
{
	struct dlist_entry *A_Restrict p = current->prev;
	current->prev = e;
	e->prev = p;
	e->next = current;
	return e;
}

/* complete _dlist_insert_after(): set current->next->prev */
A_Nonnull_all_args
static inline void _dlist_insert_after_finish(
	A_Inout struct dlist_entry *current)
{
	current->next->prev = current;
}

/* complete _dlist_insert_before(): set current->prev->next */
A_Nonnull_all_args
static inline void _dlist_insert_before_finish(
	A_Inout struct dlist_entry *current)
{
	current->prev->next = current;
}

A_Nonnull_all_args
A_Pre_satisfies(current != e)
static inline void dlist_insert_after(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist_entry *A_Restrict e)
{
	_dlist_insert_after_finish(
		_dlist_insert_after(current, e));
}

A_Nonnull_all_args
A_Pre_satisfies(current != e)
static inline void dlist_insert_before(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist_entry *A_Restrict e)
{
	_dlist_insert_before_finish(
		_dlist_insert_before(current, e));
}

A_Nonnull_all_args
A_Pre_satisfies(e != e->next)
A_Pre_satisfies(e != e->prev)
A_Ret_never_null A_Ret_range(==,e)
static inline struct dlist_entry *dlist_remove(
	A_In struct dlist_entry *A_Restrict e)
{
	struct dlist_entry *n = e->next;
	struct dlist_entry *p = e->prev;
	n->prev = p;
	p->next = n;
	return e;
}

/* if there are no entries between e->prev and e->next,
  may restore previously removed e */
A_Nonnull_all_args
A_Pre_satisfies(e != e->next)
A_Pre_satisfies(e != e->prev)
static inline void dlist_restore(
	A_In struct dlist_entry *A_Restrict e)
{
	struct dlist_entry *n = e->next;
	struct dlist_entry *p = e->prev;
	n->prev = e;
	p->next = e;
}

/* replace old entry in list with a new one */
A_Nonnull_all_args
A_Pre_satisfies(o != n)
A_Ret_never_null A_Ret_range(==,o)
static inline struct dlist_entry *dlist_replace(
	A_Inout struct dlist_entry *A_Restrict o,
	A_Inout struct dlist_entry *A_Restrict n)
{
	(n->prev = o->prev)->next = n;
	(n->next = o->next)->prev = n;
	return o;
}

A_Nonnull_all_args
A_Check_return A_Ret_never_null A_Ret_range(==,dlist)
static inline struct dlist_entry *dlist_end(
	A_Notnull const struct dlist *dlist)
{
	union {
		const struct dlist_entry *ct;
		struct dlist_entry *t;
	} u;
	u.ct = &dlist->e;
	return u.t;
}

A_Nonnull_all_args
static inline void dlist_init(
	A_Out struct dlist *dlist)
{
	dlist->dlist_first = dlist_end(dlist);
	dlist->dlist_last = dlist_end(dlist);
}

/* statically initialize dlist */
#define DLIST_DECLARE(list) struct dlist list = {{&list.e, &list.e}}

/* note: doesn't checks dlist->dlist_last */
A_Nonnull_all_args
A_Check_return A_Ret_range(0,1)
static inline int dlist_is_empty(
	A_In const struct dlist *dlist)
{
	return dlist_end(dlist) == dlist->dlist_first;
}

/* move items from non-empy src list to dst */
/* note: dst list may be uninitialized before the call */
/* NOTE: src list is invalid after the call */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
A_Pre_satisfies(src->e.prev != &src->e)
A_Pre_satisfies(src->e.next != &src->e)
static inline void _dlist_move(
	A_Out struct dlist *A_Restrict dst/*initialized?*/,
	A_In const struct dlist *A_Restrict src/*in:non-empty,out:invalid*/)
{
	*dst = *src;
	dst->dlist_first->prev = dlist_end(dst);
	dst->dlist_last->next = dlist_end(dst);
}

/* move items from src list to dst, then clear src list */
/* note: dst list may be uninitialized before the call */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
static inline void dlist_move(
	A_Out struct dlist *A_Restrict dst/*initialized?*/,
	A_Inout struct dlist *A_Restrict src/*out:cleared*/)
{
	if (!dlist_is_empty(src)) {
		_dlist_move(dst, src);
		dst = src;
	}
	dlist_init(dst);
}

/* prepend items from non-empty src list to dst */
/* NOTE: src list is invalid after the call */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
A_Pre_satisfies(src->e.prev != &src->e)
A_Pre_satisfies(src->e.next != &src->e)
static inline void _dlist_add_list_front(
	A_Inout struct dlist *A_Restrict dst,
	A_In const struct dlist *A_Restrict src/*in:non-empty,out:invalid*/)
{
	struct dlist_entry *src_first = src->dlist_first; /* may be == src->dlist_last */
	_dlist_prepend_(&dst->dlist_first, src_first, src->dlist_last);
	_dlist_add_front_finish(dst, src_first);
}

/* prepend items from src list to dst, then clear src list */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
static inline void dlist_add_list_front(
	A_Inout struct dlist *A_Restrict dst,
	A_Inout struct dlist *A_Restrict src/*out:cleared*/)
{
	if (!dlist_is_empty(src)) {
		_dlist_add_list_front(dst, src);
		dlist_init(src);
	}
}

/* append items from non-empty src list to dst */
/* NOTE: src list is invalid after the call */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
A_Pre_satisfies(src->e.prev != &src->e)
A_Pre_satisfies(src->e.next != &src->e)
static inline void _dlist_add_list_back(
	A_Inout struct dlist *A_Restrict dst,
	A_In const struct dlist *A_Restrict src/*in:non-empty,out:invalid*/)
{
	struct dlist_entry *src_last = src->dlist_last; /* may be == src->dlist_first */
	_dlist_append_(&dst->dlist_last, src->dlist_first, src_last);
	_dlist_add_back_finish(dst, src_last);
}

/* append items from src list to dst, then clear src list */
A_Nonnull_all_args
A_Pre_satisfies(dst != src)
static inline void dlist_add_list_back(
	A_Inout struct dlist *A_Restrict dst,
	A_Inout struct dlist *A_Restrict src/*out:cleared*/)
{
	if (!dlist_is_empty(src)) {
		_dlist_add_list_back(dst, src);
		dlist_init(src);
	}
}

/* insert items from non-empty src list after the current item */
/* NOTE: src list is invalid after the call */
A_Nonnull_all_args
A_Pre_satisfies(current != src->dlist_first)
A_Pre_satisfies(current != src->dlist_last)
A_Pre_satisfies(src->e.prev != &src->e)
A_Pre_satisfies(src->e.next != &src->e)
static inline void _dlist_insert_list_after(
	A_Inout struct dlist_entry *A_Restrict current,
	A_In const struct dlist *src/*in:non-empty,out:invalid*/)
{
	struct dlist_entry *src_first = src->dlist_first; /* may be == src->dlist_last */
	_dlist_prepend_(&current->next, src_first, src->dlist_last);
	src_first->prev = current;
}

/* insert items from src list after the current item, then clear src list */
A_Nonnull_all_args
A_Pre_satisfies(current != src->dlist_first)
A_Pre_satisfies(current != src->dlist_last)
static inline void dlist_insert_list_after(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist *src/*out:cleared*/)
{
	if (!dlist_is_empty(src)) {
		_dlist_insert_list_after(current, src);
		dlist_init(src);
	}
}

/* insert items from non-empty src list before the current item */
/* NOTE: src list is invalid after the call */
A_Nonnull_all_args
A_Pre_satisfies(current != src->dlist_first)
A_Pre_satisfies(current != src->dlist_last)
A_Pre_satisfies(src->e.prev != &src->e)
A_Pre_satisfies(src->e.next != &src->e)
static inline void _dlist_insert_list_before(
	A_Inout struct dlist_entry *A_Restrict current,
	A_In const struct dlist *src/*in:non-empty,out:invalid*/)
{
	struct dlist_entry *src_last = src->dlist_last; /* may be == src->dlist_first */
	_dlist_append_(&current->prev, src->dlist_first, src_last);
	src_last->next = current;
}

/* insert items from src list before the current item, then clear src list */
A_Nonnull_all_args
A_Pre_satisfies(current != src->dlist_first)
A_Pre_satisfies(current != src->dlist_last)
static inline void dlist_insert_list_before(
	A_Inout struct dlist_entry *A_Restrict current,
	A_Inout struct dlist *src/*cleared*/)
{
	if (!dlist_is_empty(src)) {
		_dlist_insert_list_before(current, src);
		dlist_init(src);
	}
}

/* iterate on dlist in generic way:

  struct my_type {
    ...
    struct dlist_entry list_entry;
    ...
  };
  struct dlist_entry *e;
  dlist_iterate(dlist, e) {
    struct my_type *t = CONTAINER_OF(e, struct my_type, list_entry);
    t->...
  }

  NOTE: iterator e will be NULL after iteration completes
   - to be able to check if iteration was interrupted by 'break'
*/

#define dlist_iterate_from_until(from, until, dir, e) \
	for (e = (from); (until) != e ? 1 : ((e = NULL), 0); e = e->dir)

#define dlist_iterate(dlist, e) \
	dlist_iterate_from_until((dlist)->dlist_first, dlist_end(dlist), next, e)

#define dlist_iterate_backward(dlist, e) \
	dlist_iterate_from_until((dlist)->dlist_last, dlist_end(dlist), prev, e)

/* iterate on dlist of struct my_type:

  struct my_type {
    ...
    struct dlist_entry list_entry;
    ...
  };
  struct my_type *t;
  dlist_iterate_typed(struct my_type, list_entry, dlist, t) {
    t->...
  }

  NOTE: iterator t will be NULL after iteration completes
   - to be able to check if iteration was interrupted by 'break'
*/

#define dlist_iterate_typed_from_until(entry_type, entry_member, from, until, dir, t) \
	for (t = CONTAINER_OF(from, entry_type, entry_member); \
		CONTAINER_OF(until, entry_type, entry_member) != t ? 1 : ((t = NULL), 0); \
		t = CONTAINER_OF(t->entry_member.dir, entry_type, entry_member))

#define dlist_iterate_typed(entry_type, entry_member, dlist, t) \
	dlist_iterate_typed_from_until(entry_type, entry_member, (dlist)->dlist_first, dlist_end(dlist), next, t)

#define dlist_iterate_typed_backward(entry_type, entry_member, dlist, t) \
	dlist_iterate_typed_from_until(entry_type, entry_member, (dlist)->dlist_last, dlist_end(dlist), prev, t)

/* iterate on dlist of struct my_ref which contains a pointer to struct my_type:

  struct my_ref {
    ...
    struct dlist_entry list_entry;
    ...
    struct my_type *ref;
    ...
  };
  struct my_type **r;
  dlist_iterate_ref_typed(struct my_ref, list_entry, ref, dlist, r) {
    (*r)->...
  }

  NOTE: iterator r will be NULL after iteration completes
   - to be able to check if iteration was interrupted by 'break'
*/

#define dlist_iterate_ref_typed_from_until(entry_type, entry_member, ref_member, from, until, dir, r) \
	for (r = &CONTAINER_OF(from, entry_type, entry_member)->ref_member; \
		&CONTAINER_OF(until, entry_type, entry_member)->ref_member != r ? 1 : ((r = NULL), 0); \
		r = &CONTAINER_OF(CONTAINER_OF(r, entry_type, ref_member)->entry_member.dir, entry_type, entry_member)->ref_member)

#define dlist_iterate_ref_typed(entry_type, entry_member, ref_member, dlist, r) \
	dlist_iterate_ref_typed_from_until(entry_type, entry_member, ref_member, (dlist)->dlist_first, dlist_end(dlist), next, r)

#define dlist_iterate_ref_typed_backward(entry_type, entry_member, ref_member, dlist, r) \
	dlist_iterate_ref_typed_from_until(entry_type, entry_member, ref_member, (dlist)->dlist_last, dlist_end(dlist), prev, r)

/* same as above, but allow to delete entry referenced by iterator.
  NOTE: iterator will be NULL after iteration completes */

#define dlist_iterate_delete_from_until(from, until, dir, e, n) \
	for (n = (from); (until) != n ? ((e = n), (n = n->dir), 1) : ((e = NULL), 0);)

#define dlist_iterate_delete(dlist, e, n) \
	dlist_iterate_delete_from_until((dlist)->dlist_first, dlist_end(dlist), next, e, n)

#define dlist_iterate_delete_backward(dlist, e, p) \
	dlist_iterate_delete_from_until((dlist)->dlist_last, dlist_end(dlist), prev, e, n)

#define dlist_iterate_from_until_delete_typed(entry_type, entry_member, from, until, dir, t, n) \
	for (n = CONTAINER_OF(from, entry_type, entry_member); \
		CONTAINER_OF(until, entry_type, entry_member) != n ? ((t = n), \
			(n = CONTAINER_OF(n->entry_member.dir, entry_type, entry_member)), 1) : ((t = NULL), 0);)

#define dlist_iterate_delete_typed(entry_type, entry_member, dlist, t, n) \
	dlist_iterate_from_until_delete_typed(entry_type, entry_member, (dlist)->dlist_first, dlist_end(dlist), next, t, n)

#define dlist_iterate_delete_typed_backward(entry_type, entry_member, dlist, t, p) \
	dlist_iterate_from_until_delete_typed(entry_type, entry_member, (dlist)->dlist_last, dlist_end(dlist), prev, t, n)

#define dlist_iterate_from_until_delete_ref_typed(entry_type, entry_member, ref_member, from, until, dir, r, n) \
	for (n = &CONTAINER_OF(from, entry_type, entry_member)->ref_member; \
		&CONTAINER_OF(until, entry_type, entry_member)->ref_member != n ? (r = n, \
			(n = &CONTAINER_OF(CONTAINER_OF(n, entry_type, ref_member)->entry_member.dir, \
				entry_type, entry_member)->ref_member), 1) : ((r = NULL), 0);)

#define dlist_iterate_delete_ref_typed(entry_type, entry_member, ref_member, dlist, r, n) \
	dlist_iterate_from_until_delete_ref_typed(entry_type, entry_member, ref_member, (dlist)->dlist_first, dlist_end(dlist), next, r, n)

#define dlist_iterate_delete_ref_typed_backward(entry_type, entry_member, ref_member, dlist, r, n) \
	dlist_iterate_from_until_delete_ref_typed(entry_type, entry_member, ref_member, (dlist)->dlist_last, dlist_end(dlist), prev, r, n)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define __dlist_iterator(iterator_name, entry_type, entry_member, dir) \
class iterator_name { \
private: \
	struct dlist_entry *e; \
	struct dlist_entry *n; \
	struct dlist_entry *until; \
public: \
	A_Nonnull_all_args \
	iterator_name(A_In struct dlist_entry *iterator_name##_from, \
			A_In struct dlist_entry *iterator_name##_until) : \
		n(iterator_name##_from), until(iterator_name##_until) {} \
	iterator_name(struct dlist &iterator_name##_list) : \
		n(dlist_end(&iterator_name##_list)->dir), until(dlist_end(&iterator_name##_list)) {} \
	A_Check_return \
	bool has_next() const { \
		return until != n; \
	} \
	A_Check_return \
	bool operator()() { \
		if (until == n) \
			return false; \
		e = n; \
		n = e->dir; \
		return true; \
	} \
	A_Check_return A_Ret_never_null \
	entry_type *operator->() const { \
		return CONTAINER_OF(e, entry_type, entry_member); \
	} \
	A_Check_return \
	entry_type &entry() const { \
		return *CONTAINER_OF(e, entry_type, entry_member); \
	} \
	A_Ret_never_null \
	entry_type *unlink() const { \
		return CONTAINER_OF(dlist_remove(e), entry_type, entry_member); \
	} \
}

#define __dlist_const_iterator(iterator_name, entry_type, entry_member, dir) \
class iterator_name { \
private: \
	const struct dlist_entry *e; \
	const struct dlist_entry *until; \
public: \
	iterator_name(const struct dlist &iterator_name##_list) : \
		e(dlist_end(&iterator_name##_list)), until(dlist_end(&iterator_name##_list)) {} \
	A_Check_return \
	bool has_next() const { \
		return until != e->dir; \
	} \
	A_Check_return \
	bool operator()() { \
		if (until == e->dir) \
			return false; \
		e = e->dir; \
		return true; \
	} \
	A_Check_return A_Ret_never_null \
	const entry_type *operator->() const { \
		return CONTAINER_OF(e, const entry_type, entry_member); \
	} \
	A_Check_return \
	const entry_type &entry() const { \
		return *CONTAINER_OF(e, const entry_type, entry_member); \
	} \
}

#define __line_dlist_iterator(pref, line, entry_type, entry_member, dir) \
	__dlist##pref##iterator(__dlist##pref##iterator_##line, entry_type, entry_member, dir)
#define ___line_dlist_iterator(pref, line, entry_type, entry_member, dir) \
	__line_dlist_iterator(pref, line, entry_type, entry_member, dir)

/* create dlist iterator, allows deletion of current entry from the list, example:
  struct my_type {
    ...
    struct dlist_entry list_entry;
    ...
  };
  void foo(struct dlist &list)
  {
    dlist_iterator(struct my_type, list_entry) it(list);
    while (it()) {
      xx(it->bar);
      delete it.unlink();
    }
  } */
#define dlist_iterator(entry_type, entry_member) \
	___line_dlist_iterator(_, __LINE__, entry_type, entry_member, next)

#define dlist_backward_iterator(entry_type, entry_member) \
	___line_dlist_iterator(_, __LINE__, entry_type, entry_member, prev)

/* const iterator, doesn't allows to delete entries from the list */
#define dlist_const_iterator(entry_type, entry_member) \
	___line_dlist_iterator(_const_, __LINE__, entry_type, entry_member, next)

#define dlist_backward_const_iterator(entry_type, entry_member) \
	___line_dlist_iterator(_const_, __LINE__, entry_type, entry_member, prev)

#endif /* __cplusplus */

#endif /* DLIST_H_INCLUDED */
