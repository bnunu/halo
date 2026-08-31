/*
STACK_MEMORY_POOL.H

header included in hcex build.
*/

#ifndef __STACK_MEMORY_POOL_H
#define __STACK_MEMORY_POOL_H
#pragma once

/* ---------- constants */

enum
{
	/* stamped into memory_block::header_signature by memory_block_set_size_and_index */
	k_memory_block_header_signature = 'fryd',

	/* stamped into the last long of every block, checked by memory_block_valid */
	k_memory_block_footer_signature = 'chkn'
};

/* ---------- macros */

/* memory_block::size_and_flags packs the real block size with a lock bit in the sign */
#define MEMORY_BLOCK_LOCKED_BIT 31
#define MEMORY_BLOCK_SIZE_MASK (~(unsigned long)FLAG(MEMORY_BLOCK_LOCKED_BIT))

/* ---------- structures */

struct memory_block
{
	unsigned long size_and_flags;
	long slot_index;
	struct memory_block *previous;
	struct memory_block *next;
	void **handle;
	long lock_count;
	unsigned long header_signature;
	byte data[1];
};

struct stack_memory_pool
{
	char const *name;
	byte *base_address;
	long size;
	long maximum_block_count;
	long next_block_index;
	long bytes_used;
	long maximum_bytes_used;
	unsigned long block_count;
	unsigned long maximum_block_count_used;
	long largest_block_size;
	boolean disable_compaction;
	byte unused29[3];
	struct memory_block *first_block;
	struct memory_block *last_block;
	struct memory_block *blocks[1];
};

/* ---------- prototypes/STACK_MEMORY_POOL.C */

void stack_memory_pool_reset(
	struct stack_memory_pool *pool);

void dispose_handle(
	struct stack_memory_pool *pool,
	struct memory_block *block);

void lock_handle(
	struct stack_memory_pool *pool,
	void **h);

void unlock_handle(
	struct stack_memory_pool *pool,
	void *h);

void dispose_pointer(
	struct stack_memory_pool *pool,
	void *p);

struct memory_block *pool_new_handle(
	struct stack_memory_pool *pool,
	long allocation_size,
	char const *file,
	unsigned long line);

struct memory_block *pool_new_handle_clear(
	struct stack_memory_pool *pool,
	long allocation_size,
	char const *file,
	unsigned long line);

boolean pool_resize_handle(
	struct stack_memory_pool *pool,
	struct memory_block **h,
	long allocation_size,
	char const *file,
	unsigned long line);

void *pool_new_pointer(
	struct stack_memory_pool *pool,
	long allocation_size,
	char const *file,
	unsigned long line);

void *pool_new_pointer_clear(
	struct stack_memory_pool *pool,
	long allocation_size,
	char const *file,
	unsigned long line);

void *pool_resize_pointer(
	struct stack_memory_pool *pool,
	void *pointer,
	long allocation_size,
	char const *file,
	unsigned long line);

/* ---------- globals */

/* ---------- public code */

#endif // __STACK_MEMORY_POOL_H
