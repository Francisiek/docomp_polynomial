#include <stdio.h>
#include <stdbool.h>

bool cmp(int x, int y) {
	return x < y;
}

#define F_BT_TYPE int
#define F_BT_CMP cmp
#include "binary_tree.h"

main() {
	Fbt_int x = Fbt_int_create();
	Fbt_int_push(&x, 5);
	printf("%d\n", x.root->value);
}