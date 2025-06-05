#include "../include/llrbtree.h"

int32_t llrbtree_node_init(llrbtree_node_t **const restrict node, const int64_t key, const int64_t value, bool color) {
	if (!node) return -1;
	*node = (llrbtree_node_t*)malloc(sizeof(llrbtree_node_t));
	if (!*node) return -2;
	**node = (llrbtree_node_t){
		.value = value,
		.key = key,
		.color = color
	};
	return 0;
}

bool llrbtree_node_is_red(llrbtree_node_t *node) {
	return node && node->color;
}

int32_t llrbtree_init(llrbtree_node_t **root, const int64_t key, const int64_t value) {
	return llrbtree_node_init(root, key, value, 0);
}

void llrbtree_node_rotate_left(llrbtree_node_t **const node) {
	llrbtree_node_t *A = *node;
	if (!A) return;
	llrbtree_node_t *B  = A->right;
	if (!B) return;
	A->right = B->left;
	B->left = A;
	B->color = B->left->color;
	B->left->color = true;
	*node = B;
}

void llrbtree_node_rotate_right(llrbtree_node_t **const node) {
	llrbtree_node_t *A = *node;
	if (!A) return;
	llrbtree_node_t *B = A->left;
	if (!B) return;
	A->left = B->right;
	B->right = A;
	B->color = B->right->color;
	B->right->color = true;
	*node = B;
}

void llrbtree_node_flip_colors(llrbtree_node_t *const node) {
	node->color = !node->color;
	if (node->left) node->left->color = !node->left->color;
	if (node->right) node->right->color = !node->right->color;
}

void llrbtree_balance(llrbtree_node_t **node) {
	if (llrbtree_node_is_red((*node)->right) && !llrbtree_node_is_red((*node)->left)) {
		llrbtree_node_rotate_left(node);
		(*node)->color = false;
		if ((*node)->left) (*node)->left->color = true;
	}
	if (llrbtree_node_is_red((*node)->left) && llrbtree_node_is_red((*node)->left->left)) {
		llrbtree_node_rotate_right(node);
		(*node)->color = false;
		if ((*node)->right) (*node)->right->color = true;
	}
	if (llrbtree_node_is_red((*node)->right) && llrbtree_node_is_red((*node)->left)) {
		llrbtree_node_flip_colors(*node);
	}
}

int32_t llrbtree_node_insert(llrbtree_node_t **const restrict node, const int64_t key, const int64_t value) {
	int res = 0;
	if (!(*node)) {
		return llrbtree_node_init(node, key, value, true);
	} 
	if (llrbtree_node_is_red((*node)->right) && llrbtree_node_is_red((*node)->left)) {
		llrbtree_node_flip_colors(*node);
	}
	if (key < (*node)->key) {
		res = llrbtree_node_insert(&((*node)->left), key, value);
	} else if (key > (*node)->key) {
		res = llrbtree_node_insert(&((*node)->right), key, value);
	} else {
		(*node)->value = value;
	}
	if (llrbtree_node_is_red((*node)->right) && !llrbtree_node_is_red((*node)->left)) {
		llrbtree_node_rotate_left(node);
	}
	if (llrbtree_node_is_red((*node)->left) && llrbtree_node_is_red((*node)->left->left)) {
		llrbtree_node_rotate_right(node);
	}
	
	return res;
}

int32_t llrbtree_insert(llrbtree_node_t **root, const int64_t key, const int64_t value) {
	if (!root) return -1;
	int32_t res = llrbtree_node_insert(root, key, value);
	if (res) return res;
	(*root)->color = 0;
	return 0;
}

int32_t llrbtree_array_push(llrbtree_array_t *array, llrbtree_node_t *node) {
	if (array->length == array->size) {
		size_t nsz = array->size * 2;
		llrbtree_node_t **tmp = (llrbtree_node_t**)realloc(array->buf, nsz * sizeof(llrbtree_node_t*));
		if (!tmp) return -2;
		array->buf = tmp;
		array->size = nsz;
	}
	array->buf[array->length++] = node;
	return 0;
}

int32_t llrbtree_array_pop(llrbtree_array_t *array, llrbtree_node_t **node) {
	if (array->length == 0) {
		*node = NULL;
		return -10;
	}
	*node = array->buf[--array->length];
	return 0;
}

void llrbtree_dfs(const llrbtree_node_t *const node) {
	if (!node) return;
	llrbtree_dfs(node->left);
	printf("[%lld, %lld] ", node->key, node->value);
	llrbtree_dfs(node->right);
}

int32_t llrbtree_bfs(llrbtree_node_t *const node) {
	if (!node) return -1;
	llrbtree_array_t array1 = {
		.size = 16,
		.buf = (llrbtree_node_t**)malloc(16 * sizeof(llrbtree_node_t*))
	};
	if (!array1.buf) {
		return -2;
	}
	llrbtree_array_t array2 = {
		.size = 16,
		.buf = (llrbtree_node_t**)malloc(16 * sizeof(llrbtree_node_t*))
	};
	if (!array2.buf) {
		free(array1.buf);
		return -2;
	}
	llrbtree_array_push(&array1, node);
	while (array1.length) {
		for (size_t i = 0; i < array1.length; ++i) {
			printf("[%llu, %llu] ", array1.buf[i]->key, array1.buf[i]->value);
			int res = 0;
			if (array1.buf[i]->left) {
				res = llrbtree_array_push(&array2, array1.buf[i]->left);
				if (res) {
					printf("res: %d\n", res);
					free(array1.buf);
					free(array2.buf);
					return res;
				}
			}
			if (array1.buf[i]->right) {
				res = llrbtree_array_push(&array2, array1.buf[i]->right);
				if (res) {
					printf("res: %d\n", res);
					free(array1.buf);
					free(array2.buf);
					return res;
				}
			}
		}
		printf("\n");
		llrbtree_node_t **tmp_ptr = array1.buf;
		array1.buf = array2.buf;
		array2.buf = tmp_ptr;
		array1.length = array2.length;
		array2.length = 0;
		size_t tmp = array1.size;
		array1.size = array2.size;
		array2.size = tmp;
	}
	free(array1.buf);
	free(array2.buf);
	return 0;
}

int32_t llrbtree_search(llrbtree_node_t *const restrict node, const int64_t key, const llrbtree_node_t **restrict res) {
	if (!node) return -1;
	llrbtree_node_t *curr = node;
	do {
		if (key == curr->key) {
			if (res) *res = curr;
			return 0;
		}
		if (key < curr->key) {
			curr = curr->left;
		}
		if (key > curr->key) {
			curr = curr->right;
		}
	} while (curr);
	return -6;
}

int32_t llrbtree_free(llrbtree_node_t *node) {
	if (!node) return -1;
	llrbtree_array_t array1 = {
		.size = 16,
		.buf = (llrbtree_node_t**)malloc(16 * sizeof(llrbtree_node_t*))
	};
	if (!array1.buf) {
		return -2;
	}
	llrbtree_array_t array2 = {
		.size = 16,
		.buf = (llrbtree_node_t**)malloc(16 * sizeof(llrbtree_node_t*))
	};
	if (!array2.buf) {
		free(array1.buf);
		return -2;
	}
	llrbtree_array_push(&array1, node);
	while (array1.length) {
		for (size_t i = 0; i < array1.length; ++i) {
			// printf("i: %llu, key: %llu, value: %llu\n", i, array1.buf[i]->key, array1.buf[i]->value);
			int res = 0;
			if (array1.buf[i]->left) {
				res = llrbtree_array_push(&array2, array1.buf[i]->left);
				if (res) {
					printf("res: %d\n", res);
					free(array1.buf);
					free(array2.buf);
					return res;
				}
			}
			if (array1.buf[i]->right) {
				res = llrbtree_array_push(&array2, array1.buf[i]->right);
				if (res) {
					printf("res: %d\n", res);
					free(array1.buf);
					free(array2.buf);
					return res;
				}
			}
			free(array1.buf[i]);
		}
		llrbtree_node_t **tmp_ptr = array1.buf;
		array1.buf = array2.buf;
		array2.buf = tmp_ptr;
		array1.length = array2.length;
		array2.length = 0;
		size_t tmp = array1.size;
		array1.size = array2.size;
		array2.size = tmp;
	}
	free(array1.buf);
	free(array2.buf);
	return 0;
}

void llrbtree_node_move_red_left(llrbtree_node_t **node) {
	llrbtree_node_t *r = *node;
	if (!r) return;
	r->color = false;
	r->left->color = true;
	if ((*node)->right) { 
		if (llrbtree_node_is_red((*node)->right->left)) {
			llrbtree_node_rotate_right(&(*node)->right);
			llrbtree_node_rotate_left(node);
		} else {
			if ((*node)->right) r->right->color = true;
		}
	}
}

void llrbtree_node_move_red_right(llrbtree_node_t **node) {
	llrbtree_node_t *r = *node;
	if (!r) return;
	llrbtree_node_flip_colors(r);
	if (r->left && llrbtree_node_is_red(r->left->left)) {
		llrbtree_node_rotate_right(node);
		r = *node;
		llrbtree_node_flip_colors(r);
	}
}

int32_t llrbtree_node_delete(llrbtree_node_t **node, const int64_t key, llrbtree_node_t **deleted) {
	int res = 0;
	if (!(*node)) {
		printf("Cannot find key %lld\n", key);
		return -6;
	}
	if (key < (*node)->key) {
		if ((*node)->left && !llrbtree_node_is_red((*node)->left) && !llrbtree_node_is_red((*node)->left->left)) {
			llrbtree_node_move_red_left(node);
		}
		res = llrbtree_node_delete(&(*node)->left, key, deleted);
	} else {
		if (llrbtree_node_is_red((*node)->left)) {
			llrbtree_node_rotate_right(node);
		}
		if (key == (*node)->key) {
			if (!(*node)->right) {
				llrbtree_node_t *r = *node;
				(*node) = (*node)->left;
				if (deleted) *deleted = r;
				else {
					r->left = NULL;
					r->right = NULL;
					free(r);
				}
				return 0;
			} else {
				llrbtree_node_t *r = *node;
				llrbtree_node_t *leftmost = r->right;
				while (leftmost->left) leftmost = leftmost->left;
				int64_t tmp = r->key;
				r->key = leftmost->key;
				leftmost->key = tmp;
				tmp = r->value;
				r->value = leftmost->value;
				leftmost->value = tmp;
				res = llrbtree_node_delete(&(*node)->right, key, deleted);
			}
		} else {
			if ((*node)->right && !llrbtree_node_is_red((*node)->right) && !llrbtree_node_is_red((*node)->right->left)) {
				llrbtree_node_move_red_right(node);	
			}
			res = llrbtree_node_delete(&(*node)->right, key, deleted);
		}
	}
	llrbtree_balance(node);
	return res;
}

int32_t llrbtree_delete(llrbtree_node_t **root, const int64_t key, llrbtree_node_t **deleted) {
	if (!root) return -1;
	if (!(*root)) return -6;
	if (!(*root)->color && !llrbtree_node_is_red((*root)->left) && !llrbtree_node_is_red((*root)->right)) {
        (*root)->color = true; // Make root red (will be flipped back to black at the end)
    }
	int res = llrbtree_node_delete(root, key, deleted);
	if (*root) (*root)->color = false;
	if (res) return res;
	return 0;
}

int32_t main(void) {
	llrbtree_node_t *root = NULL;
	for (size_t i = 0; i < 10000; ++i) {
		llrbtree_insert(&root, i, i);
	}
	int count = 0;
	int res = llrbtree_search(root, 1, NULL);
	if (!res) {
		printf("Found\n");
	}
	llrbtree_node_t *deleted = NULL;
	res = llrbtree_delete(&root, 0, &deleted);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) {
			printf("result: %lld\n", deleted->value);
			free(deleted);
			deleted = NULL;
		}
	}
	res = llrbtree_delete(&root, 500, &deleted);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) {
			printf("result: %lld\n", deleted->value);
			free(deleted);
			deleted = NULL;
		}
	}
	res = llrbtree_delete(&root, 500, NULL);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) printf("%lld\n", deleted->value);
	}
	res = llrbtree_delete(&root, 500, NULL);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) printf("%lld\n", deleted->value);
	}
	res = llrbtree_delete(&root, 500, NULL);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) printf("%lld\n", deleted->value);
	}
	res = llrbtree_delete(&root, 631, NULL);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) {
			printf("%lld\n", deleted->value);
		}
	}
	res = llrbtree_delete(&root, 631, NULL);
	printf("res: %d\n", res);
	if (!res) {
		count++;
		if (deleted) printf("%lld\n", deleted->value);
	}
	for (int64_t i = 0; i < 10000; ++i) {
		res = llrbtree_delete(&root, i, &deleted);
		printf("res: %d\n", res);
		if (!res) {
			count++;
			if (deleted) {
				printf("result: %lld\n", deleted->value);
				free(deleted);
			}
		}
		deleted = NULL;
	}
	llrbtree_dfs(root);
	printf("\n");
	llrbtree_free(root);
	printf("%d\n", count);
	printf("DONE\n");
	return 0;
}