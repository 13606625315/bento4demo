#ifndef __IMOUOS_OSAPI_RBMAP_H__
#define __IMOUOS_OSAPI_RBMAP_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Rbtree.h"
#include "OSA/List.h"
#include "OSA/Compare.h"

/// key值的比较函数
/// \param [in] refkey 为树节点的key值, newkey为传入的key值
///  newkey < refkey 返回-1(小于0),下次遍历左节点;
///  newkey == refkey 返回0,表示查找到key;
///  newkey > refkey返回1(大于0),下次遍历右节点;
typedef Int32 (*imouos_rb_cmp_fn)(const unsigned long refkey, const unsigned long newkey);

/* rbmap节点 */
typedef struct imouos_rbmap_node
{
    unsigned long value;        /* map的数据,整数是值,指针表示地址 */
    unsigned long key;          /* map key值 */
    imouos_rb_node node;         /* 红黑树的节点 */
}imouos_rbmap_node;

/* rbmap根节点 */
typedef struct imouos_rbmap_root {
    Uint32 size;
    imouos_rb_root rb_root;
    imouos_rb_cmp_fn cmp_func;
    imouos_list_node list;
}imouos_rbmap_root;

#define IMOUOS_RBMMAP_INIT(root) {.size = 0, .rb_root = RB_ROOT, .cmp_func = imouos_compareU32, .list={&(root).list, &(root).list}}

/********************************************************************
*
*                            函数声明区
*
*******************************************************************/
/// 创建一个基于红黑树的key-value映射
/// \param [inout] imouos_rbmap_root **pprbmap_root  树的根节点句柄地址
/// \param [in] imouos_rb_cmp_fn cmp 树的key比较函数
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_rbMapCreate(imouos_rbmap_root **pprbmap_root, imouos_rb_cmp_fn cmp);

/// 创建一个基于红黑树的key-value映射
/// \param [inout] imouos_rbmap_root **pprbmap_root 树的根节点句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_rbMapDestory(imouos_rbmap_root *rbmap_root);

/// 搜索 imouos_rbmap_node
/// \param [inout] imouos_rbmap_root **pprbmap_root 树的根节点句柄
/// \param [in] key         索引
/// \return imouos_rbmap_node(成功:key对应的节点 失败:NULL)
IMOUOS_API imouos_rbmap_node *imouos_rbMapSearch(imouos_rbmap_root *rbmap_root, unsigned long key);

/// 插入一个 imouos_rbmap_node
/// \param [in] key         索引
/// \param [in] value       值
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_rbMapInsert(imouos_rbmap_root *rbmap_root, unsigned long key, void *value);

/// 删除指定的一个 imouos_rbmap_node
/// \param [in] rbmap_root  map的根节点
/// \param [in] rbmapNode   需要删除的节点
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_rbMapEarse(imouos_rbmap_root *rbmap_root, imouos_rbmap_node *rbmapNode);

/// 删除指定的一个 key的节点
/// \param [in] rbmap_root  map的根节点
/// \param [in] key 需要删除的节点key
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_rbMapEarseBykey(imouos_rbmap_root *rbmap_root, unsigned long key);

/// 返回中序遍历的第一个 imouos_rbmap_node
/// \param [in] rbmap_root  map的根节点
/// \return 返回中序遍历的第一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapFirst(imouos_rbmap_root *rbmap_root);

/// 返回中序遍历的最后一个 imouos_rbmap_node
/// \param [in] rbmap_root  map的根节点
/// \return 返回中序遍历的最后一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapLast(imouos_rbmap_root *rbmap_root);

/// 返回中序遍历的后一个 imouos_rbmap_node
/// \param [in] rbmapNode   当前节点
/// \return 返回中序遍历的后一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapNext(imouos_rbmap_node *rbmapNode);

/// 返回中序遍历的前一个 imouos_rbmap_node
/// \param [in] rbmapNode   当前节点
/// \return 返回中序遍历的前一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapPrev(imouos_rbmap_node *rbmapNode);

/// 返回后序遍历的第一个 imouos_rbmap_node
/// \param [in] rbmap_root  根节点
/// \return 返回后序遍历的第一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapFirstPostOrder(imouos_rbmap_root *rbmap_root);

/// 返回后序遍历的当前节点的后一个 imouos_rbmap_node
/// \param [in] rbmapNode   当前节点
/// \return 返回后序遍历的后一个节点
IMOUOS_API imouos_rbmap_node* imouos_rbMapNextPostOrder(imouos_rbmap_node *rbmapNode);

/// 返回 rbmap_root节点数量
/// \param [in] rbmap_root  map的根节点
/// \return size, 树节点数量；
IMOUOS_API Uint32 imouos_rbMapSize(imouos_rbmap_root *rbmap_root);

/// rbmap_root是否为空
/// \param [in] rbmap_root  map的根节点
/// \return 1,为空; return 0, 非空;小于0,为IMOUOS_ErrCode_e;
IMOUOS_API Int32 imouos_rbMapIsEmpty(imouos_rbmap_root *rbmap_root);

/* 中序遍历 */
#define rbmap_order_for_each_entry_safe(pos, n, root, field) \
    for (pos = imouos_rbMapFirst(root); \
         pos && ({ n = imouos_rbMapNext(pos); 1; }); \
         pos = n)


/* 后序遍历,内部不能调用imouos_rbMapEarse
* Note, however, that it cannot handle other modifications that re-order the
* rbtree it is iterating over. This includes calling rb_erase() on @pos, as
* rb_erase() may rebalance the tree, causing us to miss some nodes.
*/
#define rbmap_postorder_for_each_entry_safe(pos, n, root, field) \
        for (pos = imouos_rbMapFirstPostOrder(root); \
             pos && ({ n = imouos_rbMapNextPostOrder(pos);1;}); \
             pos = n)

#endif  /* __IMOUOS_OSAPI_RBMAP_H__ */
