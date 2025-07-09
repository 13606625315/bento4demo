//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//
#ifndef __IMOUOS_OSAPI_UTILS_LIST_H__
#define __IMOUOS_OSAPI_UTILS_LIST_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */


#ifdef __cplusplus
extern "C" {
#endif
#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Compiler.h"
#include "OSA/Error.h"
#include "OSA/RWlock.h"

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* 用于初始化链表节点 */
#define IMOUOS_LIST_POISON1  ((void *) 0x100)
#define IMOUOS_LIST_POISON2  ((void *) 0x200)

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* 链表头 */
typedef struct imouos_list_node
{
    struct imouos_list_node *next;
    struct imouos_list_node *prev;
} imouos_list_node;

typedef struct imouos_hlist_head {
    struct imouos_hlist_node *first;
}imouos_hlist_head;

typedef struct imouos_hlist_node {
    struct imouos_hlist_node *next, **pprev;
}imouos_hlist_node;


#ifndef imouos_offsetof
#ifdef __compiler_offsetof
#define imouos_offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define imouos_offsetof(TYPE, MEMBER) ((Sizet) &((TYPE *)0)->MEMBER)
#endif
#endif

#ifndef imouos_containerOf
#define imouos_containerOf(ptr, type, member) ({         \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - imouos_offsetof(type,member) );})
#endif

/* 链表静态定义和初始化 */
#define IMOUOS_LIST_HEAD(name) \
    imouos_list_node name = { &(name), &(name) }


/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

/* 链表初始化 */
static inline void IMOUOS_INIT_LIST_HEAD(imouos_list_node *list)
{
    WRITE_ONCE(list->next, list);
    list->prev = list;
}

static inline void __imouos_list_add(imouos_list_node *newList,
                                      imouos_list_node *prev,
                                      imouos_list_node *next)
{
    next->prev = newList;
    newList->next = next;
    newList->prev = prev;
    WRITE_ONCE(prev->next, newList);
}


/**
* list_add - add a new entry
* @new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
static inline void imouos_list_add(imouos_list_node *newList, imouos_list_node *head)
{
    __imouos_list_add(newList, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void imouos_list_add_tail(imouos_list_node *newList, imouos_list_node *head)
{
    __imouos_list_add(newList, head->prev, head);
}


/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __imouos_list_del(imouos_list_node * prev, imouos_list_node * next)
{
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}


/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void imouos_list_del(imouos_list_node *entry)
{
    __imouos_list_del(entry->prev, entry->next);
    entry->next = (imouos_list_node *)IMOUOS_LIST_POISON1;
    entry->prev = (imouos_list_node *)IMOUOS_LIST_POISON2;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline Int32 imouos_list_empty(const imouos_list_node *head)
{
    return READ_ONCE(head->next) == head;
}

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define imouos_list_entry(ptr, type, member) \
    imouos_containerOf(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define imouos_list_first_entry(ptr, type, member) \
    imouos_list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define imouos_list_last_entry(ptr, type, member) \
    imouos_list_entry((ptr)->prev, type, member)

/**
 * list_next_entry - get the next element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define imouos_list_next_entry(pos, member) \
    imouos_list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_prev_entry - get the prev element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) \
    imouos_list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define imouos_list_first_entry_or_null(ptr, type, member) ({ \
    struct list_head *head__ = (ptr); \
    struct list_head *pos__ = READ_ONCE(head__->next); \
    pos__ != head__ ? imouos_list_entry(pos__, type, member) : NULL; \
})

/**
 * list_for_each    -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define imouos_list_for_each(pos, head) \
            for (pos = (head)->next; pos != (head); pos = pos->next)


/**
* list_for_each_prev   -   iterate over a list backwards
* @pos:    the &struct list_head to use as a loop cursor.
* @head:   the head for your list.
*/
#define imouos_list_for_each_prev(pos, head) \
        for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
* list_for_each_safe - iterate over a list safe against removal of list entry
* @pos:    the &struct list_head to use as a loop cursor.
* @n:      another &struct list_head to use as temporary storage
* @head:   the head for your list.
*/
#define imouos_list_for_each_safe(pos, n, head) \
                for (pos = (head)->next, n = pos->next; pos != (head); \
                    pos = n, n = pos->next)


/**
* list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
* @pos:    the &struct list_head to use as a loop cursor.
* @n:      another &struct list_head to use as temporary storage
* @head:   the head for your list.
*/
#define imouos_list_for_each_prev_safe(pos, n, head) \
        for (pos = (head)->prev, n = pos->prev; \
             pos != (head); \
             pos = n, n = pos->prev)

/**
* list_for_each_entry  -   iterate over list of given type
* @pos:    the type * to use as a loop cursor.
* @head:   the head for your list.
* @member: the name of the list_head within the struct.
*/
#define imouos_list_for_each_entry(pos, head, member)              \
        for (pos = imouos_list_first_entry(head, typeof(*pos), member);    \
             &pos->member != (head);                    \
             pos = imouos_list_next_entry(pos, member))

/**
* list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
* @pos:    the type * to use as a loop cursor.
* @n:      another type * to use as temporary storage
* @head:   the head for your list.
* @member: the name of the list_head within the struct.
*/
#define imouos_list_for_each_entry_safe(pos, n, head, member)          \
        for (pos = imouos_list_first_entry(head, typeof(*pos), member),    \
            n = imouos_list_next_entry(pos, member);           \
             &pos->member != (head);                    \
             pos = n, n = imouos_list_next_entry(n, member))


/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int imouos_list_is_singular(const struct imouos_list_node *head)
{
    return !imouos_list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct imouos_list_node *list,
        struct imouos_list_node *head, struct imouos_list_node *entry)
{
    struct imouos_list_node *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

/**
 * list_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *  and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void imouos_list_cut_position(struct imouos_list_node *list,
        struct imouos_list_node *head, struct imouos_list_node *entry)
{
    if (imouos_list_empty(head))
        return;
    if (imouos_list_is_singular(head) &&
        (head->next != entry && head != entry))
        return;
    if (entry == head)
        IMOUOS_INIT_LIST_HEAD(list);
    else
        __list_cut_position(list, head, entry);
}

static inline void __imouos_list_splice(const struct imouos_list_node *list,
                 struct imouos_list_node *prev,
                 struct imouos_list_node *next)
{
    struct imouos_list_node *first = list->next;
    struct imouos_list_node *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void imouos_list_splice(const struct imouos_list_node *list,
                struct imouos_list_node *head)
{
    if (!imouos_list_empty(list))
        __imouos_list_splice(list, head, head->next);
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void imouos_list_splice_tail(struct imouos_list_node *list,
                struct imouos_list_node *head)
{
    if (!imouos_list_empty(list))
        __imouos_list_splice(list, head->prev, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void imouos_list_splice_init(struct imouos_list_node *list,
                    struct imouos_list_node *head)
{
    if (!imouos_list_empty(list)) {
        __imouos_list_splice(list, head, head->next);
        IMOUOS_INIT_LIST_HEAD(list);
    }
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void imouos_list_splice_tail_init(struct imouos_list_node *list,
                     struct imouos_list_node *head)
{
    if (!imouos_list_empty(list)) {
        __imouos_list_splice(list, head->prev, head);
        IMOUOS_INIT_LIST_HEAD(list);
    }
}

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */
#define IMOUOS_HLIST_HEAD_INIT { .first = NULL }
#define IMOUOS_HLIST_HEAD(name) struct imouos_hlist_head name = {  .first = NULL }
#define IMOUOS_INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
static inline void IMOUOS_INIT_HLIST_NODE(struct imouos_hlist_node *h)
{
    h->next = NULL;
    h->pprev = NULL;
}

static inline Int32 imouos_hlist_unhashed(const struct imouos_hlist_node *h)
{
    return !h->pprev;
}

static inline Int32 imouos_hlist_empty(const struct imouos_hlist_head *h)
{
    return !READ_ONCE(h->first);
}

static inline void __imouos_hlist_del(struct imouos_hlist_node *n)
{
    struct imouos_hlist_node *next = n->next;
    struct imouos_hlist_node **pprev = n->pprev;

    WRITE_ONCE(*pprev, next);
    if (next)
        next->pprev = pprev;
}

static inline void imouos_hlist_del(struct imouos_hlist_node *n)
{
    __imouos_hlist_del(n);
    n->next = IMOUOS_LIST_POISON1;
    n->pprev = IMOUOS_LIST_POISON2;
}

static inline void imouos_hlist_del_init(struct imouos_hlist_node *n)
{
    if (!imouos_hlist_unhashed(n)) {
        __imouos_hlist_del(n);
        IMOUOS_INIT_HLIST_NODE(n);
    }
}

static inline void imouos_hlist_add_head(struct imouos_hlist_node *n, struct imouos_hlist_head *h)
{
    struct imouos_hlist_node *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    WRITE_ONCE(h->first, n);
    n->pprev = &h->first;
}

/* next must be != NULL */
static inline void imouos_hlist_add_before(struct imouos_hlist_node *n,
                    struct imouos_hlist_node *next)
{
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    WRITE_ONCE(*(n->pprev), n);
}

static inline void imouos_hlist_add_behind(struct imouos_hlist_node *n,
                    struct imouos_hlist_node *prev)
{
    n->next = prev->next;
    WRITE_ONCE(prev->next, n);
    n->pprev = &prev->next;

    if (n->next)
        n->next->pprev  = &n->next;
}

/* after that we'll appear to be on some hlist and hlist_del will work */
static inline void imouos_hlist_add_fake(struct imouos_hlist_node *n)
{
    n->pprev = &n->next;
}

static inline Int32 imouos_hlist_fake(struct imouos_hlist_node *h)
{
    return h->pprev == &h->next;
}

/*
 * Check whether the node is the only node of the head without
 * accessing head:
 */
static inline Int32
imouos_hlist_is_singular_node(struct imouos_hlist_node *n, struct imouos_hlist_head *h)
{
    return !n->next && n->pprev == &h->first;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
static inline void imouos_hlist_move_list(struct imouos_hlist_head *old, struct imouos_hlist_head *newnode)
{
    newnode->first = old->first;
    if (newnode->first)
        newnode->first->pprev = &newnode->first;
    old->first = NULL;
}

#define imouos_hlist_entry(ptr, type, member) imouos_containerOf(ptr,type,member)

#define imouos_hlist_for_each(pos, head) \
        for (pos = (head)->first; pos ; pos = pos->next)

#define imouos_hlist_for_each_safe(pos, n, head) \
        for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
             pos = n)

#define imouos_hlist_entry_safe(ptr, type, member) \
        ({ typeof(ptr) ____ptr = (ptr); \
           ____ptr ? imouos_hlist_entry(____ptr, type, member) : NULL; \
        })

/**
 * hlist_for_each_entry - iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the hlist_node within the struct.
 */
#define imouos_hlist_for_each_entry(pos, head, member)               \
        for (pos = imouos_hlist_entry_safe((head)->first, typeof(*(pos)), member);\
             pos;                           \
             pos = imouos_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_from - iterate over a hlist continuing from current point
 * @pos:    the type * to use as a loop cursor.
 * @member: the name of the hlist_node within the struct.
 */
#define imouos_hlist_for_each_entry_from(pos, member)                \
        for (; pos;                         \
             pos = imouos_hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another &struct hlist_node to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the hlist_node within the struct.
 */
#define imouos_hlist_for_each_entry_safe(pos, n, head, member)       \
        for (pos = imouos_hlist_entry_safe((head)->first, typeof(*pos), member);\
             pos && ({ n = pos->member.next; 1; });         \
             pos = imouos_hlist_entry_safe(n, typeof(*pos), member))
#if 0
/* 为了快速获取list大小,封装IMOUOS_LIST_*系列宏
   使用场景:调用IMOUOS_LIST_*系列宏,宏内包含锁的定义
*/
typedef struct imouos_list
{
    imouos_list_node head;
    imouos_rwlock rwlock;
    Uint32 size;
} imouos_list_t;

/* 初始化链表头结点,创建锁 */
#define IMOUOS_LIST_CREATE(list) \
            {\
                imouos_list_t* impl = (imouos_list_t*)(list);\
                assert(impl);\
                impl->size = 0;\
                IMOUOS_INIT_LIST_HEAD(&impl->head);\
                imouos_rwlockCreate(&impl->rwlock, compat_rwlock_prefer_read);\
            }


/* 去始化链表头结点,销毁锁，不对链表结点元素处理, 链表结点内存需要用户自行管理*/
#define IMOUOS_LIST_DESTORY(list) \
            {\
                imouos_list_t* impl = (imouos_list_t *)(list);\
                if(impl)\
                {\
                    IMOUOS_INIT_LIST_HEAD(&impl->head);\
                    imouos_rwlockDestory(impl->rwlock);\
                    impl->rwlock = NULL;\
                    impl->size = 0;\
                }\
            }

/* 链表是否为空, 1:空  0:非空*/
#define IMOUOS_LIST_EMPTY(list) imouos_list_empty(&(list)->head)

/* 获取链表大小 */
#define IMOUOS_LIST_SIZE(list, size) \
            {\
                imouos_list_t* impl = (imouos_list_t *)(list);\
                assert(impl);\
                if(impl)\
                {\
                    size = impl->size;\
                }\
            }
/* 入链表头部 */
#define  IMOUOS_LIST_PUSH_FRONT(list, _newNode) \
            {\
                imouos_list_t* impl = (imouos_list_t*)(list);\
                typeof(*(_newNode)) *newNode = (_newNode);\
                if(impl && newNode)\
                {\
                    imouos_rwlockEnterW(impl->rwlock);\
                    imouos_list_add(newNode, &impl->head);\
                    impl->size++;\
                    imouos_rwlockLeaveW(impl->rwlock);\
                }\
            }

/* 入链表尾部 */
#define IMOUOS_LIST_PUSH_BACK(list, _newNode) \
            {\
                imouos_list_t* impl = (imouos_list_t*)(list);\
                typeof(*(_newNode)) *newNode = (_newNode);\
                if(impl && newNode)\
                {\
                    imouos_rwlockEnterW(impl->rwlock);\
                    imouos_list_add_tail(newNode, &impl->head);\
                    impl->size++;\
                    imouos_rwlockLeaveW(impl->rwlock);\
                }\
            }
/* 链表头部取出结点 */
#define IMOUOS_LIST_POP_FRONT(list, elem, member) \
            {\
                imouos_list_t* impl = (imouos_list_t*)(list);\
                if(impl)\
                {\
                    imouos_rwlockEnterW(impl->rwlock);\
                    if (!imouos_list_empty(&impl->head))\
                    {\
                        typeof(*(elem)) *tmpElem=imouos_list_first_entry(&impl->head, typeof(*(elem)), member);\
                        imouos_list_del(&tmpElem->member);\
                        impl->size--;\
                        imouos_rwlockLeaveW(impl->rwlock);\
                        (elem)=tmpElem;\
                    }else{imouos_rwlockLeaveW(impl->rwlock);(elem)=NULL;}\
                }else{(elem)=NULL;}\
            }

/* 链表尾部取出结点 */
#define IMOUOS_LIST_POP_BACK(list, elem, member) \
            {\
                imouos_list_t* impl = (imouos_list_t*)(list);\
                if(impl)\
                {\
                    imouos_rwlockEnterW(impl->rwlock);\
                    if (!imouos_list_empty(&impl->head))\
                    {\
                        typeof(*(elem)) *tmpElem=imouos_list_last_entry(&impl->head, typeof(*(elem)), member);\
                        imouos_list_del(&tmpElem->member);\
                        impl->size--;\
                        imouos_rwlockLeaveW(impl->rwlock);\
                        (elem)=tmpElem;\
                    }else{imouos_rwlockLeaveW(impl->rwlock);(elem)=NULL;}\
                }else{(elem)=NULL;}\
            }

/* 获取链表读锁 */
#define IMOUOS_LIST_LOCK_R(list) \
    {\
        imouos_list_t* impl = (imouos_list_t*)(list);\
        if(impl)\
        {\
            imouos_rwlockEnterR(impl->rwlock);\
        }\
    }

/* 释放链表读锁 */
#define IMOUOS_LIST_UNLOCK_R(list) \
    {\
        imouos_list_t* impl = (imouos_list_t*)(list);\
        if(impl)\
        {\
            imouos_rwlockLeaveR(impl->rwlock);\
        }\
    }

/* 遍历整个链表 ,需要手动获取读锁 */
#define IMOUOS_LIST_FOR_EACH_ENTRY_SAFE(list, data, member) \
                imouos_list_t* __impl##member = (imouos_list_t*)(list);\
                assert(__impl##member);\
                typeof(*(data)) *tmpdata= NULL;\
                imouos_list_for_each_entry_safe(data, tmpdata, &__impl##member->head, member)
#endif

#ifdef __cplusplus
    }
#endif


#endif /* __IMOUOS_UTILS_LIST_H__ */
