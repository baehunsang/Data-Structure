#include <stdio.h>
#include "ArrayList.h"
#include "NameCard.h"
void ListInit(ListPtr plist) {
	plist->curPosition = -1;
	plist->numOfData = 0;
}

void LInsert(ListPtr plist, LData data) {
	if (plist ->numOfData >=100) {
		printf("Exception: OverFlow!\n");
		return;
	}
	
	plist->data[plist->numOfData] = data;
	plist->numOfData++;
}

int LFirst(ListPtr plist, LData* pdata){
	if (plist->numOfData ==0) {
		printf("Exception: No Data Exist");
		return 0;
	}
	
	plist->curPosition = 0;
	*pdata = plist->data[0];
	return 1;
}

int LNext(ListPtr plist, LData* pdata) {
	if (plist->curPosition >= plist->numOfData-1) {
		return 0;
	}

	(plist->curPosition)++;
	*pdata = plist->data[plist->curPosition];
	return 1;
}

LData LRemove(ListPtr plist) {
	LData data = plist->data[plist->curPosition];
	int i;
	for (i = plist->curPosition; i < (plist->numOfData) - 1; i++) {
		plist->data[i] = plist->data[i + 1];
	}
	(plist->curPosition)--;
	(plist->numOfData)--;
	return data;
}

int LCount(ListPtr plist) {
	return plist->numOfData;
}
