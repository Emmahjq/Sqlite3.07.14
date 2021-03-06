/*
** 2001 September 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This is the header file for the generic hash-table implemenation
** used in SQLite.
*/

/*
在SQLite里这是哈希表使用的头文件
*/
#ifndef _SQLITE_HASH_H_
#define _SQLITE_HASH_H_

/* Forward declarations of structures. */
/*
结构体的声明
*/
typedef struct Hash Hash;
typedef struct HashElem HashElem;

/* A complete hash table is an instance of the following structure.
** The internals of this structure are intended to be opaque -- client
** code should not attempt to access or modify the fields of this structure
** directly.  Change this structure only by using the routines below.
** However, some of the "procedures" and "functions" for modifying and
** accessing this structure are really macros, so we can't really make
** this structure opaque.
**
** All elements of the hash table are on a single doubly-linked list.
** Hash.first points to the head of this list.
**
** There are Hash.htsize buckets.  Each bucket points to a spot in
** the global doubly-linked list.  The contents of the bucket are the
** element pointed to plus the next _ht.count-1 elements in the list.
**
** Hash.htsize and Hash.ht may be zero.  In that case lookup is done
** by a linear search of the global list.  For small tables, the 
** Hash.ht table is never allocated because if there are few elements
** in the table, it is faster to do a linear search than to manage
** the hash table.
*/

/*
完整的哈希表是下面结构体的实例化。

哈希表中的元素在一个双联表中。哈希表的头指针指向标中的头元素。


*/
struct Hash {
  unsigned int htsize;      /* Number of buckets in the hash table *///无符号短整型，当使用哈希表存储时，哈希表中桶的数量
  unsigned int count;       /* Number of entries in this table *///无符号短整型，哈希表中入口项的个数(记录的个数)
  HashElem *first;          /* The first element of the array *///HashElem 类型，哈希元素指针，指向入口项双向链表的表头
  struct _ht {              /* the hash table */
    int count;                 /* Number of entries with this hash *//*其中 count成员变量记录本桶中记录的数量*/
    HashElem *chain;           /* Pointer to first entry with this hash *//* chain指向桶内记录链表的表头，哈希表使用的冲突消解策略是链表*/
  } *ht; /*_ht 结构指针,哈希表存储结构，当使用哈希表存储时，将表现为一个桶的数组,含有两个成员变量*/
};

/* Each element in the hash table is an instance of the following 
** structure.  All elements are stored on a single doubly-linked list.
**
** Again, this structure is intended to be opaque, but it can't really
** be opaque because it is used by macros.
*/
/*
哈希表中的每个元素是一个实例的结构。所有元素都存储在一个双链接列表。
结构是不透明的，但是又不是真正不透明因为它使用了宏。

*/

struct HashElem {
  HashElem *next, *prev;       /* Next and previous elements in the table *//*指向哈希表的下一个和前一个元素*/
  void *data;                  /* Data associated with this element *//*和元素相关的数据*/
  const char *pKey; int nKey;  /* Key associated with this element *//*元素的键值和长度*/
};

/*
** Access routines.  To delete, insert a NULL pointer.
*/
void sqlite3HashInit(Hash*);/*哈希表的初始化函数，将各个成员变量初始化为 0 */
void *sqlite3HashInsert(Hash*, const char *pKey, int nKey, void *pData);/*向哈希表中插入元素*/
void *sqlite3HashFind(const Hash*, const char *pKey, int nKey);/*查找哈希表中的元素*/
void sqlite3HashClear(Hash*);/*将哈希表中所有的入口项删除，就是将 ht 的内存释放，将 first 
链表中所有的元素释放；将各个成员变量置 0 */

/*
** Macros for looping over all elements of a hash table.  The idiom is
** like this:
**
**   Hash h;
**   HashElem *p;
**   ...
**   for(p=sqliteHashFirst(&h); p; p=sqliteHashNext(p)){
**     SomeStructure *pData = sqliteHashData(p);
**     // do something with pData
**   }
*/

/*
哈希表的所有宏定义，定义是这样的：
*/
#define sqliteHashFirst(H)  ((H)->first)
#define sqliteHashNext(E)   ((E)->next)
#define sqliteHashData(E)   ((E)->data)
/* #define sqliteHashKey(E)    ((E)->pKey) // NOT USED */
/* #define sqliteHashKeysize(E) ((E)->nKey)  // NOT USED */

/*
** Number of entries in a hash table
*/
/* #define sqliteHashCount(H)  ((H)->count) // NOT USED */

#endif /* _SQLITE_HASH_H_ */
