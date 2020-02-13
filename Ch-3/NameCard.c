#include "NameCard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NameCard* MakeNameCard(char* name, char* phone){
	NameCard* newName = (NameCard*)malloc(sizeof(NameCard));
	strcpy(newName->name, name);
	strcpy(newName->phone, phone);
	return newName;
}

void ShowNameCardInfo(NameCard* pcard) {
	printf("Name: %s\n", pcard->name);
	printf("Phone: %s\n", pcard->phone);
}

int NameCompare(NameCard* pcard, char* name) {
	if (!strcmp(pcard->name, name)) {
		return 0;
	}
	return 1;
}

void ChangePhoneNum(NameCard* pcard, char* phone) {
	strcpy(pcard->phone, phone);
}