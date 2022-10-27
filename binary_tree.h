// operator # is stringification, it makes "identifier"
// ## operator concatinates two tokens, a##b = ab in code

#ifndef F_BINARY_TREE_HEADER
#define F_BINARY_TREE_HEADER

#include <stdlib.h>
#define F_COMBINE(pre, word) pre##word
 
#endif // F_BINARY_TREE_HEADER

#ifndef bt_TYPE
#	error "F_BINARY_TREE_TYPE must be defined"
#endif

#ifndef bt_NAME
#define bt_NAME F_COMBINE(fbt_, bt_TYPE)
#define bt_NODE F_COMBINE(bt_PREFIX, _NODE)
#endif

#ifndef bt_PREFIX
#define bt_PREFIX F_COMBINE(bt_NAME, _)
#endif

#ifndef bt_LINKAGE
#define bt_LINKAGE static inline
#endif

typedef struct bt_NODE{
	bt_TYPE* value;
	bt_NODE* left;
	bt_NODE* right;
	bt_NODE* parent;
} bt_NODE;

typedef struct bt_NAME {
	bt_NODE* root;
	size_t count;
	unsigned int deep;
} bt_NAME;


