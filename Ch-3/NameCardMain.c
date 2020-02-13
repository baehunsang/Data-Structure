#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "NameCard.h"

void printBody();
void InsertMember(List* plist);
void SearchMember(List* plist);
void ChangePhoneNumber(List* plist);
void DeletInfo(List* plist);
void printAllMember(List* plist);

void main() {
	int choice;
	List member;
	ListInit(&member);
	
	while (1) {
		printBody();
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			InsertMember(&member);
			break;
		case 2:
			SearchMember(&member);
			break;
		case 3:
			ChangePhoneNumber(&member);
			break;
		case 4:
			DeletInfo(&member);
			break;
		case 5:
			printAllMember(&member);
			break;
		case 6:
			system("cls");
			break;
		 case 7:
			 return;
			break;
		 default:
			 break;
		}
	}
}

void printBody() {
	printf("\n");
	printf("1. Insert member\n");
	printf("2. Search a member\n");
	printf("3. Change a phone number\n");
	printf("4. Delet a member\n");
	printf("5. print all member\n");
	printf("6. Clear\n");
	printf("7. Exit\n");
}
void InsertMember(List* plist) {
	printf("\n");
	char name[30];
	char phoneNumber[30];
	NameCard* newCard;
	printf("Enter a name\n");
	scanf("%s", name);
	printf("Enter a phone number\n");
	scanf("%s", phoneNumber);
	newCard = MakeNameCard(name, phoneNumber);
	LInsert(plist, newCard);
}

void SearchMember(List* plist) {
	printf("\n");
	char name[30];
	printf("Enter a name\n");
	scanf("%s", name);
	LData data;
	if (LFirst(plist, &data)) {
		if (!strcmp(data->name, name)){
			ShowNameCardInfo(data);
		}
		while (LNext(plist, &data)) {
			if (!strcmp(data->name, name)) {
				ShowNameCardInfo(data);
			}
		}
	}
}

void ChangePhoneNumber(List* plist) {
	printf("\n");
	char name[30];
	char phoneNumber[30];
	printf("Enter a name\n");
	scanf("%s", name);
	LData data;
	if (LFirst(plist, &data)) {
		if (!strcmp(data->name, name)) {
			printf("Enter a phone number\n");
			scanf("%s", phoneNumber);
			ChangePhoneNum(data, phoneNumber);
		}
		while (LNext(plist, &data)) {
			if (!strcmp(data->name, name)) {
				printf("Enter a phone number\n");
				scanf("%s", phoneNumber);
				ChangePhoneNum(data, phoneNumber);
			}
		}
	}
}

void DeletInfo(List* plist) {
	printf("\n");
	char name[30];
	printf("Enter a name\n");
	scanf("%s", name);
	LData data;
	if (LFirst(plist, &data)) {
		if (!strcmp(data->name, name)) {
			LRemove(plist);
			free(data);
		}
		while (LNext(plist, &data)) {
			if (!strcmp(data->name, name)) {
				LRemove(plist);
				free(data);
			}
		}
	}
}

void printAllMember(List* plist) {
	printf("\n");
	LData data;
	if (LFirst(plist, &data)) {
		ShowNameCardInfo(data);
		while (LNext(plist, &data)) {
			ShowNameCardInfo(data);
		}
	}
}


