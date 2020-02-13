#include <stdio.h>
#include "ArrayList.h"

int main(void)
{
	List list;
	int i, sum = 0, data;
	ListInit(&list);
	for (i = 0; i < 9; i++) {
		LInsert(&list, i+1);
	}

	if (LFirst(&list, &data)) {
		sum += data;
		while (LNext(&list, &data)) {
			sum += data;
		}
	}
	printf("%d\n", sum);

	if (LFirst(&list, &data)) {
		if (data % 2 == 0 || data % 3 == 0) {
			LRemove(&list);
		}
		while (LNext(&list, &data)) {
			if (data % 2 == 0 || data % 3 == 0) {
				LRemove(&list);
			}
		}
	}

	if (LFirst(&list, &data)) {
		printf("%d ", data);
		while (LNext(&list, &data)) {
			printf("%d ", data);
		}
	}
	printf("\n");
	return 0;
}