#pragma once
#include "NameCard.h"
#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__

typedef NameCard* LData;

typedef struct ArrayList {
	LData data[100];
	int numOfData;
	int curPosition;
}List;

typedef List* ListPtr;
void ListInit(ListPtr plist);
void LInsert(ListPtr plist, LData pdata);
int LFirst(ListPtr plist, LData* pdata);
int LNext(ListPtr plist, LData* pdata);
LData LRemove(ListPtr plist);
int LCount(ListPtr plist);

#endif
