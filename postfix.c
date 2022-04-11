/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 // 최대 스택 크기 10
#define MAX_EXPRESSION_SIZE 20 // 최대 expression 크기 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; // Infix expression
char postfixExp[MAX_EXPRESSION_SIZE]; // Postfix expression
char postfixStack[MAX_STACK_SIZE]; // Postfix 스택
int evalStack[MAX_STACK_SIZE];

int postfixStackTop = -1;
int evalStackTop = -1;

int evalResult = 0;

void postfixPush(char x); // Postfix 스택 값 Push
char postfixPop(); // Postfix 스택 값 Pop
void evalPush(int x); // 계산 스택 값 Push
int evalPop(); // 계산 스택 값 Pop
void getInfix(); // Infix 입력 받아 저장
precedence getToken(char symbol);
precedence getPriority(char x); // 우선순위 확인
void charCat(char* c); // 문자열 복사, 연결
void toPostfix(); // Postifix로 변환
void debug(); // 결과 확인 (디버그)
void reset(); // 초기화
void evaluation(); // 값 계산

int main()
{
	char command; // 메뉴 변수

	do{
		printf("\n[----- [Kim Hyeong Jin]  [2021041079] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { // 메뉴
		case 'i': case 'I': // 메뉴 i : Infix 입력 받아 저장, getInfix 함수 이용
			getInfix();
			break;
		case 'p': case 'P': // 메뉴 p : Postfix로 변환,  toPostfix 함수 이용
			toPostfix();
			break;
		case 'e': case 'E': // 메뉴 e : 값 계산, evaluation 함수 이용
			evaluation();
			break;
		case 'd': case 'D': // 메뉴 d : 결과 확인 (디버그), debug 함수 이용
			debug();
			break;
		case 'r': case 'R': // 메뉴 r : 초기화, reset 함수 이용
			reset();
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

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x; // Postfix 스택에 값 Push
}

char postfixPop()
{
	char x;
    if(postfixStackTop == -1) // top이 -1이면 (값 X)
        return '\0'; // NULL 리턴
    else { // 이외의 경우
    	x = postfixStack[postfixStackTop--]; // x == top의 값, top - 1
    }
    return x; // x 리턴
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // 계산 스택에 값 Push
}

int evalPop()
{
    if(evalStackTop == -1) // top이 -1이면 (값 X)
        return -1; // NULL 리턴
    else
        return evalStack[evalStackTop--]; // top의 값 리턴
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) { // 문자에 따라 우선순위 리턴
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x) // 우선순위 확인
{
	return getToken(x); // getToken(x) 리턴
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') // NULL일 경우 (처음)
		strncpy(postfixExp, c, 1); // 문자열 복사
	else // 이외의 경우
		strncat(postfixExp, c, 1); // 문자열 연결
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') // 문자열 끝까지 반복
	{
		if(getPriority(*exp) == operand) // 리턴값 피연산자
		{
			x = *exp;
        	charCat(&x); // charCat 함수
		}
        else if(getPriority(*exp) == lparen) { // 리턴값 왼쪽 괄호

        	postfixPush(*exp); // postfixpush 함수
        }
        else if(getPriority(*exp) == rparen) // 리턴값 오른쪽 괄호
        {
        	while((x = postfixPop()) != '(') { // PostfixPop 함수, 스택에서 왼쪽 괄호가 나올때 까지 반복
        		charCat(&x); // charCat 함수
        	}
        }
        else // 이외의 경우
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) // 스택의 top의 값의 우선순위 >= 읽은 문자의 우선순위 일때 반복
            {
            	x = postfixPop(); // Postfix 스택 값 Pop
            	charCat(&x); // 문자열 복사, 연결
            }
            postfixPush(*exp); // Postfix 스택 값 Push
        }
        exp++;
	}

    while(postfixStackTop != -1) // top이 -1이 될때 까지
    {
    	x = postfixPop(); // Postfix 스택 값 Pop
    	charCat(&x); // 문자열 복사, 연결
    }

}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); // Infix expression
	printf("postExp =  %s\n", postfixExp); // Postfix expression
	printf("eval result = %d\n", evalResult); // 계산값

	printf("postfixStack : "); // Postfix 스택 출력
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() // 초기화
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++) // 스택 초기화
		postfixStack[i] = '\0';
         
	postfixStackTop = -1; 
	evalStackTop = -1;
	evalResult = 0;
}
void evaluation() // 계산
{
	int opr1, opr2, i;

	int length = strlen(postfixExp); // 길이, strlen 이용
	char symbol; // 문자
	evalStackTop = -1; // 계산 스택 top 초기값

	for(i = 0; i < length; i++) // 길이만큼 반복
	{
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) { // 피연산자일 때
			evalPush(symbol - '0'); // 계산 스택 값 Push
		}
		else { // 이외의 경우
			opr2 = evalPop(); // 계산 스택 값 Pop 후 opr2에 저장
			opr1 = evalPop(); // 계산 스택 값 Pop 후 opr1에 저장
			switch(getToken(symbol)) { // 기호에 따라 계산 후 계산 스택에 Push
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop(); // evalPop 함수로 결과값 저장
}