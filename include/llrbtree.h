#ifndef LLRBTREE_H__
#define LLRBTREE_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct llrbtree_node {
	struct llrbtree_node *left, *right;
	int64_t key;
	int64_t value;
	bool color;
} llrbtree_node_t;

typedef struct llrbtree_array {
	llrbtree_node_t **buf;
	size_t length, size;
} llrbtree_array_t;

int32_t llrbtree_init(llrbtree_node_t **root, const int64_t key, const int64_t value);
int32_t llrbtree_insert(llrbtree_node_t **root, const int64_t key, const int64_t value);
int32_t llrbtree_search(llrbtree_node_t *const restrict node, const int64_t key, const llrbtree_node_t **restrict res);
int32_t llrbtree_delete(llrbtree_node_t **root, const int64_t key, llrbtree_node_t **deleted);
void llrbtree_dfs(const llrbtree_node_t *const node);
int32_t llrbtree_bfs(llrbtree_node_t *const node);
int32_t llrbtree_free(llrbtree_node_t *node);

#endif // LLRBTREE_H__