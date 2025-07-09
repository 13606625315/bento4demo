/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  linux/include/linux/rbtree.h

  To use rbtrees you'll have to implement your own insert and search cores.
  This will avoid us to use callbacks and to drop drammatically performances.
  I know it's not the cleaner way,  but in C (not in C++) to get
  performances and genericity...

  See Documentation/rbtree.txt for documentation and samples.
*/
#ifndef __IMOUOS_OSAPI_RBTREE_H__
#define __IMOUOS_OSAPI_RBTREE_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Compiler.h"

typedef struct rb_node {
    unsigned long  __rb_parent_color;
    struct rb_node *rb_right;
    struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long)))) imouos_rb_node;
    /* The alignment might seem pointless, but allegedly CRIS needs it */

typedef struct rb_root {
    struct rb_node *rb_node;
}imouos_rb_root;


#define rb_parent(r)   ((struct rb_node *)((r)->__rb_parent_color & ~3))

#define RB_ROOT (struct rb_root) { NULL, }
#define rb_entry(ptr, type, member) CONTAINER_FOR(ptr, type, member)

#define RB_EMPTY_ROOT(root)  (READ_ONCE((root)->rb_node) == NULL)

/* 'empty' nodes are nodes that are known not to be inserted in an rbtree */
#define RB_EMPTY_NODE(node)  \
    ((node)->__rb_parent_color == (unsigned long)(node))
#define RB_CLEAR_NODE(node)  \
    ((node)->__rb_parent_color = (unsigned long)(node))


static inline void rb_link_node(struct rb_node *node, struct rb_node *parent,
                struct rb_node **rb_link)
{
    node->__rb_parent_color = (unsigned long)parent;
    node->rb_left = node->rb_right = NULL;

    *rb_link = node;
}
#if 0
static inline void rb_link_node_rcu(struct rb_node *node, struct rb_node *parent,
                    struct rb_node **rb_link)
{
    node->__rb_parent_color = (unsigned long)parent;
    node->rb_left = node->rb_right = NULL;

    rcu_assign_pointer(*rb_link, node);
}
#endif
#define rb_entry_safe(ptr, type, member) \
    ({ typeof(ptr) ____ptr = (ptr); \
       ____ptr ? rb_entry(____ptr, type, member) : NULL; \
    })

/** 后续遍历
 * rbtree_postorder_for_each_entry_safe - iterate in post-order over rb_root of
 * given type allowing the backing memory of @pos to be invalidated
 *
 * @pos:    the 'type *' to use as a loop cursor.
 * @n:      another 'type *' to use as temporary storage
 * @root:   'rb_root *' of the rbtree.
 * @field:  the name of the rb_node field within 'type'.
 *
 * rbtree_postorder_for_each_entry_safe() provides a similar guarantee as
 * list_for_each_entry_safe() and allows the iteration to continue independent
 * of changes to @pos by the body of the loop.
 *
 * Note, however, that it cannot handle other modifications that re-order the
 * rbtree it is iterating over. This includes calling rb_erase() on @pos, as
 * rb_erase() may rebalance the tree, causing us to miss some nodes.
 */
#define rbtree_postorder_for_each_entry_safe(pos, n, root, field) \
    for (pos = rb_entry_safe(rb_first_postorder(root), typeof(*pos), field); \
         pos && ({ n = rb_entry_safe(rb_next_postorder(&pos->field), \
            typeof(*pos), field); 1; }); \
         pos = n)

/* 中序遍历 */
#define rbtree_order_for_each_entry_safe(pos, n, root, field) \
    for (pos = rb_entry_safe(rb_first(root), typeof(*pos), field); \
         pos && ({ n = rb_entry_safe(rb_next(&pos->field), \
            typeof(*pos), field); 1; }); \
         pos = n)


/* 插入一个 rb_node */
IMOUOS_API void rb_insert_color(imouos_rb_node *node, imouos_rb_root *root);

/* 删除一个 rb_node */
IMOUOS_API void rb_erase(imouos_rb_node *node, imouos_rb_root *root);

/* Find logical next and previous nodes in a tree */
/* 函数用于获得中序遍历的下一个 rb_node */
IMOUOS_API imouos_rb_node *rb_next(const imouos_rb_node *node);

/* 函数用于获得中序遍历的前一个 rb_node */
IMOUOS_API imouos_rb_node *rb_prev(const imouos_rb_node *node);

/* 函数用于获得中序遍历的第一个 rb_node */
IMOUOS_API imouos_rb_node *rb_first(const imouos_rb_root *root);

/* 函数用于获得中序遍历的最后一个 rb_node */
IMOUOS_API imouos_rb_node *rb_last(const imouos_rb_root *root);

/* 函数用于获得后序遍历的第一个 rb_node */
IMOUOS_API imouos_rb_node *rb_first_postorder(const imouos_rb_root *root);

/* 函数用于获得后序遍历的下一个 rb_node */
IMOUOS_API imouos_rb_node *rb_next_postorder(const imouos_rb_node *node);

/* 替换一个rb_node without remove/rebalance/add/rebalance */
IMOUOS_API void rb_replace_node(imouos_rb_node *victim, imouos_rb_node *newnode, imouos_rb_root *root);


#endif  /* __IMOUOS_OSAPI_RBTREE_H__ */
