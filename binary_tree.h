// operator # is stringification, it makes "identifier"
// ## operator concatinates two tokens, a##b = ab in code

#ifndef F_BINARY_TREE_HEADER
#define F_BINARY_TREE_HEADER

#include <stdlib.h>
#define F_COMBINE(pre, word) F_COMBINE2(pre, word)
#define F_COMBINE2(pre, word) pre##word

#endif // F_BINARY_TREE_HEADER

#ifndef F_BT_TYPE
#	error "F_BINARY_TREE_TYPE must be defined"
#endif

#ifndef F_BT_CMP
#	error "F_BT_CMP must be defiened"
#endif

// names for data structures
#define F_BT_SNAME F_COMBINE(Fbt_, F_BT_TYPE)
#define F_BT_NODE F_COMBINE(F_COMBINE(Fbt_, F_BT_TYPE), _NODE)


// default name is in form 'Fbt_type'
#ifndef F_BT_NAME
#define F_BT_NAME F_COMBINE(Fbt_, F_BT_TYPE)
#endif

// default prefix for functions
#ifndef F_BT_PREFIX
#define F_BT_PREFIX F_COMBINE(F_BT_NAME, _)
#endif

// default linkage and storage options
#ifndef F_BT_LINKAGE
#define F_BT_LINKAGE static inline
#endif

typedef struct F_BT_NODE F_BT_NODE;
struct F_BT_NODE {
	F_BT_TYPE value;
	F_BT_NODE* left;
	F_BT_NODE* right;
	F_BT_NODE* parent;
};

typedef struct F_BT_SNAME {
	F_BT_NODE* root;
	size_t count;
	unsigned int deep;
} F_BT_SNAME;


// creating one instance with given/default name
// F_BT_SNAME F_BT_NAME = {};

F_BT_SNAME F_COMBINE(F_BT_PREFIX, create)(void) {
	F_BT_SNAME x = {};
	return x;
}

bool F_COMBINE(F_BT_PREFIX, push)(F_BT_SNAME* bt, F_BT_TYPE value) {
	F_BT_NODE** ptr = &bt->root;
	while (*ptr) {
		if (F_BT_CMP(value, (*ptr)->value))
			ptr = &(*ptr)->left;
		else
			ptr = &(*ptr)->right;
	}

	F_BT_NODE df = {};
	*ptr = malloc(sizeof(F_BT_NODE));
	if (!*ptr)
		return 1;
	**ptr = df;
	(*ptr)->value = value;

	return 0;
}

F_BT_TYPE F_COMBINE(F_BT_PREFIX, find)(F_BT_SNAME* bt, F_BT_TYPE value) {
	F_BT_NODE** ptr = &bt->root;
	while (*ptr && ) {
		if (F_BT_CMP(value, (*ptr)->value))
			ptr = &(*ptr)->left;
		else
			ptr = &(*ptr)->right;
	}

	if (*ptr)
		return 
	F_BT_NODE df = {};
	*ptr = malloc(sizeof(F_BT_NODE));
	if (!*ptr)
		return 1;
	**ptr = df;
	(*ptr)->value = value;

	return 0;
}

// undefine all so they can be used later
#undef F_BT_TYPE
#undef F_BT_CMP
#undef F_BT_NAME
#undef F_BT_SNAME
#undef F_BT_NODE
#undef F_BT_PREFIX
#undef F_BT_LINKAGE