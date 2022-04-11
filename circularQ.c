/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 최대 큐 크기 4

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue(); // 큐 생성, 동적할당
int freeQueue(QueueType *cQ); // 큐 동적할당 해제
int isEmpty(QueueType *cQ); // 큐 비었는지 여부
int isFull(QueueType *cQ); // 큐 꽉 찼는지 여부
void enQueue(QueueType *cQ, element item); // 큐 요소 추가
void deQueue(QueueType *cQ, element* item); // 큐 요소 제거
void printQ(QueueType *cQ); // 큐 출력
void debugQ(QueueType *cQ); // 큐 요소, front rear 확인
element getElement(); // 큐 요소 입력


int main(void)
{
	QueueType *cQ = createQueue(); // createQueue 함수 이용 큐 생성
	element data; // 큐 요소 값 변수

	char command; // 메뉴 변수

	do{ // 메뉴 구현
		printf("\n[----- [Kim Hyeong Jin]  [2021041079] -----]\n");
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I': // 메뉴 i : 큐 요소 추가, getElement, enQueue 함수 이용
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D': // 메뉴 d : 큐 요소 제거, deQueue 함수 이용
			deQueue(cQ, &data);
			break;
		case 'p': case 'P': // 메뉴 p : 큐 출력, printQ 함수 이용
			printQ(cQ);
			break;
		case 'b': case 'B': // 메뉴 b : 큐 요소, front rear 확인, debugQ 함수 이용
			debugQ(cQ);
			break;
		case 'q': case 'Q': // 메뉴 q : 종료
			break;
		default: // 이외의 경우 : 에러 메시지 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 에러 메시지
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 큐 동적 할당
	cQ->front = 0; // front 0 초기값
	cQ->rear = 0; // rear 0 초기값
	return cQ; // 변수 리턴
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // NULL이면 종료
    free(cQ); // 동적 할당 해제
    return 1;
}

element getElement()
{
	element item; 
	printf("Input element = ");
	scanf(" %c", &item); // 요소 입력 받아 item에 저장
	return item; // item 리턴
}


int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear){ // front와 rear가 같으면 (큐가 빈 경우)
		printf("Circular Queue is empty!\n"); // 큐가 비었다는 메시지 출력
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ)
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // front와 (rear+1)%(최대 큐 크기) 가 같은 경우 (큐가 꽉 찬 경우)
		printf(" Circular Queue is full!\n"); // 큐가 꽉 찼다는 메시지 출력
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return; // 큐가 꽉 찼을 경우 종료
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 다음 위치 이동
		cQ->queue[cQ->rear] = item; // 다음 위치 이동된 rear 인덱스인 큐에 data 값 (입력값) 저장
	}
}

void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)) return; // 큐가 비었으면 종료
	else {
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front를 다음 위치 이동
		*item = cQ->queue[cQ->front]; // 다음 위치 이동된 front 인덱스인 큐 값 (제거하려는 값)을 data 주소가 가리키는 값에 저장 (실제 값 삭제 x)
		return;
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // 초기값 (front + 1) % (최대 큐 크기)
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // 나중값 (rear + 1) % (최대 큐 크기)

	printf("Circular Queue : [");

	i = first; // 초기값
	while(i != last){ // 나중값까지 반복
		printf("%3c", cQ->queue[i]); // 큐 값 출력
		i = (i+1)%MAX_QUEUE_SIZE; // 인덱스 증가

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) { // 인덱스와 front 일치할 때
			printf("  [%d] = front\n", i); // front 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // 큐 값 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front와 rear 출력
}