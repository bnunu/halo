/*
VECTOR_TREE.H

header included in hcex build.
*/

#ifndef __VECTOR_TREE_H
#define __VECTOR_TREE_H
#pragma once

/* ---------- headers */

#include "memory/array.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct vector_tree_node
{
	long value;
	long lower_child_index;
	long equal_child_index;
	long upper_child_index;
};

typedef void *(*vector_tree_get_vector)(
	void *context,
	long value);
typedef long (*vector_tree_compare_component)(
	void *context,
	const void *vector,
	const void *other_vector,
	short component_index);

struct vector_tree
{
	long root_index;
	struct dynamic_array nodes;
	short component_count;
	short pad;
	void *context;
	vector_tree_get_vector get_vector;
	vector_tree_compare_component compare_component;
};

/* ---------- prototypes/VECTOR_TREE.C */

void vector_tree_new(
	struct vector_tree *tree,
	short component_count,
	void *context,
	vector_tree_get_vector get_vector,
	vector_tree_compare_component compare_component);
void vector_tree_delete(
	struct vector_tree *tree);
boolean vector_tree_find(
	struct vector_tree *tree,
	const void *vector,
	struct vector_tree_node **index_reference);

/* ---------- globals */

/* ---------- public code */

#endif // __VECTOR_TREE_H
