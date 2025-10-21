/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_HASHTABLE_H_
#define _NIAL_HASHTABLE_H_

#include "nial_list.h"

/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define NIAL_GOLDEN_RATIO_PRIME_32 0x9e370001UL

#define NIAL_GOLDEN_RATIO_PRIME   NIAL_GOLDEN_RATIO_PRIME_32
#define nial_hash_long(val, bits) nial_hash_32(val, bits)

static inline u32 nial_hash_32(u32 val, unsigned int bits)
{
    /* On some cpus multiply is faster, on others gcc will do shifts */
    u32 hash = val * NIAL_GOLDEN_RATIO_PRIME_32;

    /* High bits are more random, so use them. */
    return hash >> (32 - bits);
}

static inline unsigned long nial_hash_ptr(const void *ptr, unsigned int bits)
{
    return nial_hash_long((unsigned long)ptr, bits);
}

static inline unsigned long nial_hash32_ptr(const void *ptr)
{
    unsigned long val = (unsigned long)ptr;

    return val;
}

#define NIAL_DEFINE_HASHTABLE(name, bits) \
    struct nial_hlist_head name[1 << (bits)] = {[0 ...((1 << (bits)) - 1)] = HLIST_HEAD_INIT}

#define NIAL_DECLARE_HASHTABLE(name, bits) struct nial_hlist_head name[1 << (bits)]

#define NIAL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define NIAL_HASH_SIZE(name) (NIAL_ARRAY_SIZE(name))
#define NIAL_HASH_BITS(name) ilog2(NIAL_HASH_SIZE(name))

/* Use hash_32 when possible to allow for fast 32bit hashing in 64bit kernels. */
#define nial_hash_min(val, bits) (sizeof(val) <= 4 ? nial_hash_32(val, bits) : nial_hash_long(val, bits))

static inline void __nial_hash_init(struct nial_hlist_head *ht, unsigned int sz)
{
    unsigned int i;

    for (i = 0; i < sz; i++)
        NIAL_INIT_HLIST_HEAD(&ht[i]);
}

/**
 * __nial_hash_init - initialize a hash table
 * @hashtable: hashtable to be initialized
 *
 * Calculates the size of the hashtable from the given parameter, otherwise
 * same as nial_hash_init_size.
 *
 * This has to be a macro since HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
#define nial_hash_init(hashtable) __nial_hash_init(hashtable, NIAL_HASH_SIZE(hashtable))

/**
 * nial_hlist_add_head - add an object to a hashtable
 * @hashtable: hashtable to add to
 * @node: the &struct nial_hlist_node of the object to be added
 * @key: the key of the object to be added
 */
#define nial_hash_add(hashtable, node, key) \
    nial_hlist_add_head(node, &hashtable[nial_hash_min(key, NIAL_HASH_BITS(hashtable))])

/**
 * nial_hlist_add_head_rcu - add an object to a rcu enabled hashtable
 * @hashtable: hashtable to add to
 * @node: the &struct nial_hlist_node of the object to be added
 * @key: the key of the object to be added
 */
#define nial_hash_add_rcu(hashtable, node, key) \
    nial_hlist_add_head_rcu(node, &hashtable[nial_hash_min(key, NIAL_HASH_BITS(hashtable))])

/**
 * nial_hash_hashed - check whether an object is in any hashtable
 * @node: the &struct nial_hlist_node of the object to be checked
 */
static inline int nial_hash_hashed(struct nial_hlist_node *node)
{
    return !nial_hlist_unhashed(node);
}

static inline int __nial_hash_empty(struct nial_hlist_head *ht, unsigned int sz)
{
    unsigned int i;

    for (i = 0; i < sz; i++)
        if (!nial_hlist_empty(&ht[i]))
            return 0;

    return 1;
}

/**
 * __nial_hash_empty - check whether a hashtable is empty
 * @hashtable: hashtable to check
 *
 * This has to be a macro since HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
#define nial_hash_empty(hashtable) __nial_hash_empty(hashtable, NIAL_HASH_SIZE(hashtable))

/**
 * nial_hash_del - remove an object from a hashtable
 * @node: &struct nial_hlist_node of the object to remove
 */
static inline void nial_hash_del(struct nial_hlist_node *node)
{
    nial_hlist_del_init(node);
}

/**
 * nial_hash_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the nial_hlist_node within the struct
 */
#define nial_hash_for_each(name, bkt, obj, member)                                    \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < NIAL_HASH_SIZE(name); (bkt)++) \
    nial_hlist_for_each_entry(obj, &name[bkt], member)

/**
 * nial_hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the nial_hlist_node within the struct
 */
#define nial_hash_for_each_safe(name, bkt, tmp, obj, member)                          \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < NIAL_HASH_SIZE(name); (bkt)++) \
    nial_hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/**
 * nial_hash_for_each_possible - iterate over all possible objects hashing to the
 * same bucket
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the nial_hlist_node within the struct
 * @key: the key of the objects to iterate over
 */
#define nial_hash_for_each_possible(name, obj, member, key) \
    nial_hlist_for_each_entry(obj, &name[nial_hash_min(key, NIAL_HASH_BITS(name))], member)

/**
 * nial_hash_for_each_possible_safe - iterate over all possible objects hashing to the
 * same bucket safe against removals
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @tmp: a &struct used for temporary storage
 * @member: the name of the nial_hlist_node within the struct
 * @key: the key of the objects to iterate over
 */
#define nial_hash_for_each_possible_safe(name, obj, tmp, member, key) \
    nial_hlist_for_each_entry_safe(obj, tmp, &name[nial_hash_min(key, NIAL_HASH_BITS(name))], member)

#endif /* _NIAL_HASHTABLE_H_ */
