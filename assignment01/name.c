#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도-
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가
void load_names( FILE *fp, int year_index, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// qsort를 위한 비교 함수
int compare( const void *n1, const void *n2);

// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}	
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	
	int num = 0;
	FILE *fp;
	int num_year = 0;
	
	if (argc == 1) return 0;

	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi( &argv[1][3]);
	
	for (int i = 1; i < argc; i++)
	{
		num_year++;
		fp = fopen( argv[i], "r");
		assert( fp != NULL);
		
		int year = atoi( &argv[i][3]); // ex) "yob2009.txt" -> 2009
		
		fprintf( stderr, "Processing [%s]..\n", argv[i]);
		
		// 연도별 입력 파일(이름 정보)을 구조체에 저장
		load_names( fp, year-start_year, names);
		
		fclose( fp);
	}
	
	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort( names->data, names->len, sizeof(tName), compare);
	
	// 이름 구조체를 화면에 출력
	print_names( names, num_year);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 1;
}

void load_names(FILE* fp, int year_index, tNames* names) {
	char buffer[100];
	char* strPtr;
	if (year_index == 0) { //처음
		char buffer[100];
		fgets(buffer, sizeof(buffer), fp);
		char* strPtr = strtok(buffer, ",");
		memset(&(names->data[0]), 0, sizeof(tName));
		strcpy((names->data[0]).name, strPtr);
		strPtr = strtok(NULL, ",");
		(names->data)[0].sex = *strPtr;
		strPtr = strtok(NULL, ",");
		(names->data)[0].freq[year_index] = atoi(strPtr);
		names->len++;
		names->capacity *= 2;
	}                              //처음 동적할당
	while (!feof(fp)) {
		fgets(buffer, sizeof(buffer), fp);
		strPtr = strtok(buffer, ",");
		for (int i = 0; i <= (names->len); i++) {
			if (i < (names->len) && !strncmp(strPtr, (names->data)[i].name, 20)){  //이름이 같을때
				strPtr = strtok(NULL, ",");
				if (*strPtr == (names->data)[i].sex) {    //이름, 성별이 같을때
					strPtr = strtok(NULL, ",");
					(names->data[i]).freq[year_index] = atoi(strPtr);
					break;
				}
				else {
					strPtr[1] = ',';               //이름은 같으나 성별이 다를때
					*(strPtr - 1) = ',';
					strPtr = strtok(buffer, ",");

				}
			}
			if (i==(names->len)) {       //이름이 다를때, 동적할당
				if (names -> len == names ->capacity) {
					names->capacity *= 2;
				}
				names->data = (tName*)realloc(names->data, (names->capacity) * sizeof(tName));
				memset(&(names->data[i]), 0, sizeof(tName));
				strcpy((names->data[i]).name , strPtr);
				strPtr = strtok(NULL, ",");
				(names->data[i]).sex = *strPtr;
				strPtr = strtok(NULL, ",");
				(names->data[i]).freq[year_index] = atoi(strPtr);
				names->len++;
				break;
			}
			continue;
		}
	}
}

void print_names(tNames* names, int num_year) {
	for (int i = 0; i < names->len; i++) {
		printf("%s %c ", (names->data)[i].name, (names->data)[i].sex);
		for(int j=0; j<num_year; j++){
			printf("%d ", (names->data)[i].freq[j]);
		}
		printf("\n");
	}
}

int compare(const void* n1, const void* n2) {
	const tName* input1 = n1;
	const tName* input2 = n2;
	if (strcmp(input1->name, input2 ->name ) > 0) {
		return 1;
	}
	else if (strcmp(input1->name, input2->name) == 0) {
		if (input1->sex > input2->sex) {
			return 1;
		}
		else if (input1->sex == input2->sex) {
			return 0;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}
