#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

char garbage;
void printMap();

int dir_x[4] = {0,0,1,-1};
int dir_y[4] = {-1,1,0,0};

int item_x[5], item_y[5];
int itemFlag[5] = {0};
int itemCollected;
int gate_x = 73, gate_y = 15;
int gameWin = 0;

char map[75][75];
int visitedA[75][75] = {};
int visitedB[75][75] = {};
int visitedC[75][75] = {};
 
struct Node{
	int x;
	int y;
	Node *parent;
	Node *next;
	Node *dir[4];
};

Node *createNode(int y, int x){
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode->y = y;
	newNode->x = x;
	newNode->parent = newNode->next = NULL;
	for(int i=0;i<4;i++){
		newNode->dir[i] = NULL;
	}
	return newNode;
}

Node *rootA, *rootB, *rootC;
Node *leafA, *leafB, *leafC;
int flagA = 0, flagB = 0, flagC = 0;

//BFS YG BENER
struct Queue{
	Node *node;
	Queue *next;
	Queue *prev;
};

Queue *headA = NULL, *headB = NULL, *headC = NULL;
Queue *tailA = NULL, *tailB = NULL, *tailC = NULL;

Queue *createQueue(Node* node){
	Queue *newQueue = (Queue*) malloc(sizeof(Queue));
	newQueue->node = node;
	newQueue->next = newQueue->prev = NULL;
	return newQueue;
}

//queue - pushHead

void enqueueA(Node* node){
	Queue *newQueue = createQueue(node);
	if(headA == NULL){
		headA = tailA = newQueue;
	}else{
		newQueue->next = headA;
		headA->prev = newQueue;
		headA = newQueue;
	}
}
void enqueueB(Node* node){
	Queue *newQueue = createQueue(node);
	if(headB == NULL){
		headB = tailB = newQueue;
	}else{
		newQueue->next = headB;
		headB->prev = newQueue;
		headB = newQueue;
	}
}
void enqueueC(Node* node){
	Queue *newQueue = createQueue(node);
	if(headC == NULL){
		headC = tailC = newQueue;
	}else{
		newQueue->next = headC;
		headC->prev = newQueue;
		headC = newQueue;
	}
}

//dequeue - popTail

void dequeueA(){
	if(headA==tailA){
		headA=tailA=NULL;
		free(headA);
	}
	else{
		Queue *del = tailA;
		tailA = del->prev;
		tailA->next = NULL;
		del = NULL;
		free(del);
	}
}

void dequeueB(){
	if(headB==tailB){
		headB=tailB=NULL;
		free(headB);
	}
	else{
		Queue *del = tailB;
		tailB = del->prev;
		tailB->next = NULL;
		del = NULL;
		free(del);
	}
}

void dequeueC(){
	if(headC==tailC){
		headC=tailC=NULL;
		free(headC);
	}
	else{
		Queue *del = tailC;
		tailC = del->prev;
		tailC->next = NULL;
		del = NULL;
		free(del);
	}
}

//clear Queue and Flag

void clearQueueA(){
	Queue *curr = tailA;
	while(curr){
		dequeueA();
		curr = curr->prev;
	}
	if(headA == tailA){
		headA = NULL;
		free(headA);
	}
}

void clearQueueB(){
	Queue *curr = tailB;
	while(curr){
		dequeueB();
		curr = curr->prev;
	}
	if(headB == tailB){
		headB = NULL;
		free(headB);
	}
}

void clearQueueC(){
	Queue *curr = tailC;
	while(curr){
		dequeueC();
		curr = curr->prev;
	}
	if(headC == tailC){
		headC = NULL;
		free(headC);
	}
}

void clearFlagA(){
	for(int i=0;i<75;i++){
		for(int j=0;j<75;j++){
			visitedA[i][j] = 0;
		}
	}
	flagA = 0;
	leafA = NULL;
	free(leafA);
}

void clearFlagB(){
	for(int i=0;i<75;i++){
		for(int j=0;j<75;j++){
			visitedB[i][j] = 0;
		}
	}
	flagB = 0;
	leafB = NULL;
	free(leafB);
}

void clearFlagC(){
	for(int i=0;i<75;i++){
		for(int j=0;j<75;j++){
			visitedC[i][j] = 0;
		}
	}
	flagC = 0;
	leafC = NULL;
	free(leafC);
}

void deletePath(Node *curr){
	if(curr == NULL){
		return;
	}
	curr->parent = NULL;
	curr->next = NULL;
	for(int i=0;i<4;i++){
		deletePath(curr->dir[i]);
	}
	for(int i=0;i<4;i++){
		curr->dir[i] = NULL;
	}
	free(curr);
}

//void printQueue(char c, Queue* head){
//	Queue *curr = head;
//	while(curr){
//		printf("%c: %d %d, ", c, curr->node->y, curr->node->x);
//		curr = curr->next;
//	}
//	printf("\n");
//}

void BFS_A(int y, int x){
	clearFlagA();
	deletePath(rootA);
	rootA = createNode(y,x);
	if(map[y][x] == 'P'){
		leafA = rootA;
		for(int i=0;i<4;i++){
			rootA->dir[i] = rootA;
		}
		return;
	}
	Node *parentDir[4], *parent;
	enqueueA(rootA);
	parent = rootA;
	while(tailA!=NULL){
		parent = tailA->node;
		x = tailA->node->x;
		y = tailA->node->y;
		dequeueA();
		for(int i=0;i<4;i++){
			if(x+dir_x[i]>=0 && map[y+dir_y[i]][x+dir_x[i]] != '\0' && y+dir_y[i]>=0 && y+dir_y[i] < 23  &&
			 visitedA[y+dir_y[i]][x+dir_x[i]] == 0 && (map[y+dir_y[i]][x+dir_x[i]] == ' ' || map[y+dir_y[i]][x+dir_x[i]] == '.' || map[y+dir_y[i]][x+dir_x[i]] == 'E') &&  map[y+dir_y[i]][x+dir_x[i]] != '#' && map[y+dir_y[i]][x+dir_x[i]] != '|'){
			 	visitedA[y+dir_y[i]][x+dir_x[i]] = 1;
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				enqueueA(parentDir[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
			}
			else if(map[y+dir_y[i]][x+dir_x[i]] == 'P'){
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
				if(flagA == 0){
					flagA = 1;
					leafA = parentDir[i];
					clearQueueA();
					break;
				}
			}
		}
	}
}

void BFS_B(int y, int x){
	clearFlagB();
	deletePath(rootB);
	rootB = createNode(y,x);
	if(map[y][x] == 'P'){
		leafB = rootB;
		for(int i=0;i<4;i++){
			rootB->dir[i] = rootB;
		}
		return;
	}
	Node *parentDir[4], *parent;
	enqueueB(rootB);
	parent = rootB;
	while(tailB!=NULL){
		parent = tailB->node;
		x = tailB->node->x;
		y = tailB->node->y;
		dequeueB();
		for(int i=0;i<4;i++){
			if(x+dir_x[i]>=0 && map[y+dir_y[i]][x+dir_x[i]] != '\0' && y+dir_y[i]>=0 && y+dir_y[i] < 23  &&
			 visitedB[y+dir_y[i]][x+dir_x[i]] == 0 && (map[y+dir_y[i]][x+dir_x[i]] == ' ' || map[y+dir_y[i]][x+dir_x[i]] == '*' || map[y+dir_y[i]][x+dir_x[i]] == 'E') && map[y+dir_y[i]][x+dir_x[i]] != '#' && map[y+dir_y[i]][x+dir_x[i]] != '|'){
			 	visitedB[y+dir_y[i]][x+dir_x[i]] = 1;
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				enqueueB(parentDir[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
			}
			else if(map[y+dir_y[i]][x+dir_x[i]] == 'P'){
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
				if(flagB == 0){
					flagB = 1;
					leafB = parentDir[i];
					clearQueueB();
					break;
				}
			}
		}
	}
}

void BFS_C(int y, int x){
	clearFlagC();
	deletePath(rootC);
	rootC = createNode(y,x);
	if(map[y][x] == 'P'){
		leafC = rootC;
		for(int i=0;i<4;i++){
			rootC->dir[i] = rootC;
		}
		return;
	}
	Node *parentDir[4], *parent;
	enqueueC(rootC);
	parent = rootC;
	while(tailC!=NULL){
		parent = tailC->node;
		x = tailC->node->x;
		y = tailC->node->y;
		dequeueC();
		for(int i=0;i<4;i++){
			if(x+dir_x[i]>=0 && map[y+dir_y[i]][x+dir_x[i]] != '\0' && y+dir_y[i]>=0 && y+dir_y[i] < 23  &&
			 visitedC[y+dir_y[i]][x+dir_x[i]] == 0 && (map[y+dir_y[i]][x+dir_x[i]] == ' ' || map[y+dir_y[i]][x+dir_x[i]] == '*' || map[y+dir_y[i]][x+dir_x[i]] == 'E') && map[y+dir_y[i]][x+dir_x[i]] != '#' && map[y+dir_y[i]][x+dir_x[i]] != '|'){
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				enqueueC(parentDir[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
			 	visitedC[y+dir_y[i]][x+dir_x[i]] = 1;
			}
			else if(map[y+dir_y[i]][x+dir_x[i]] == 'P'){
				parentDir[i] = createNode(y+dir_y[i],x+dir_x[i]);
				parentDir[i]->parent = parent;
				parent->dir[i] = parentDir[i];
				if(flagC == 0){
					flagC = 1;
					leafC = parentDir[i];
					clearQueueC();
					break;
				}
			}
		}
	}
}

//NON DS CODES

int p_y, p_x;
int A_y = 2, A_x = 71;
int B_y = 19, B_x = 2;
int C_y = 19, C_x = 71;
char move;
int playerMove = 0, playerStart = 0;
int difConstant = 1;

void initItemPositions(){
	srand (time(NULL));
	int i=0;
	while(i<5){
		int rand_y = (rand() % 22);
		int rand_x = (rand() % 74);
		if(map[rand_y][rand_x] != '#' && map[rand_y][rand_x] != 'E' && map[rand_y][rand_x] != '|' && map[rand_y][rand_x] != '&' && map[rand_y][rand_x] != '*'){
			map[rand_y][rand_x] = '*';
			item_x[i] = rand_x;
			item_y[i] = rand_y;
			i++;		
		}
	}
}

void movePlayer(int y, int x, char m){
	switch(m){
		case 'w':
			y--;
			break;
		case 'a':
			x--;
			break;
		case 's':
			y++;
			break;
		case 'd':
			x++;
			break;
	}
	if(map[y][x] == '|' || map[y][x] == '#'){
		return;
	}
	else if(map[y][x] == '*'){
		for(int i=0;i<5;i++){
			if(y == item_y[i] && x == item_x[i] && itemFlag[i] == 0){
				itemFlag[i] = 1;
				itemCollected++;
			}
		}
		if(itemCollected >= 5){
			map[gate_y][gate_x] = ' ';
		}
	}
	else if(y == gate_y && x == gate_x){
		//game win
		gameWin = 1;
		return;
	}
	map[p_y][p_x] = ' ';
	switch(m){
		case 'w':
			p_y--;
			break;
		case 'a':
			p_x--;
			break;
		case 's':
			p_y++;
			break;
		case 'd':
			p_x++;
			break;
	}
	map[p_y][p_x] = 'P';
}

void loadMap(){
	FILE *level = fopen("level1.txt", "r");
	int idx=0;
	while(fscanf(level, "%[^\n]\n", map[idx]) != EOF){
		idx++;
	}
	fclose(level);
}

void printMap(){
	system("cls");
	printf("\n");
	for(int i=0;i<22;i++){
		printf("     %s\n",map[i]);
	}
	printf("\n     Key Fragments Collected : %d\n", itemCollected);

}

void initGOPositions(){
	itemCollected = 0;
	for(int i=0;i<5;i++){
		itemFlag[i] = 0;
	}
	p_y = 2;
	p_x = 2;
	gameWin = 0;
	A_y = 2, A_x = 71;
	B_y = 19, B_x = 2;
	C_y = 19, C_x = 71;
	map[gate_y][gate_x] = '|';
	map[p_y][p_x] = 'P';
	map[A_y][A_x] = 'E';
	map[B_y][B_x] = 'E';
	map[C_y][C_x] = 'E';
	initItemPositions();
	playerMove = 0, playerStart = 0;
}

void mainMenu();
void help();
void selectDif();
void play();

int main(){
	while(true){
		mainMenu();
	}
	return 0;
}

void mainMenu(){
	char choice;
	for(int i=0;i<1;i++){
		system("cls");
		printf("\n     _____                 ____\n");
		printf("    |  ___|   _ _ __   ___/ ___|  ___ __ _ _ __   ___\n");
		printf("    | |_ | | | | '_ \\ / _ \\___ \\ / __/ _` | '_ \\ / _ \\\n");
 		printf("    |  _|| |_| | | | |  __/___) | (_| (_| | |_) |  __/\n");
		printf("    |_|   \\__,_|_| |_|\\___|____/ \\___\\__,_| .__/ \\___|\n");
 		printf("                                          |_|\n");
		printf("\n");
		printf("    1. Play\n");
		printf("    2. How to play\n");
		printf("    3. Exit game\n\n    ");
		choice=getchar();
		getchar();
		switch(choice){
			case '1':
				selectDif();
				break;
			case '2':
				help();
				break;
			case '3':
				system("cls");
				exit(0);
			default:
				i--;
				break;		
		}
	}
}

void help(){
	system("cls");
	printf("\n\n");
	printf("    Welcome to FuneScape,\n");
	printf("\n");    
	printf("    You are now locked inside this maze by the evil wizard Efes,\n");
	printf("\n");    
	printf("    but there is no maze that has no end!\n");
	printf("    If you look carefully you could find the escape door symbolized by '|' on the map.\n");
	printf("\n");    
	printf("    You should find 5 key fragments scattered around the maze in order to unlock that door and escape.\n");
	printf("    The fragments are symbolized by '*' on the map, to get it you should travel the maze carefully!\n");
	printf("\n");   
	printf("    Use 'W' to move upwards, 'S' to move downwards, 'A' to move left and 'D' to move right.\n");
	printf("\n");   
	printf("    After you gather all of the fragments the door should be automatically opened.\n");
	printf("    Be quick before the monsters of the maze catch up on you!\n");
	printf("\n");    
	printf("    Press any key to continue . .");
	garbage=getch();
}

void selectDif(){
	system("cls");
	char choice;
	for(int i=0;i<1;i++){
		system("cls");
		printf("\n\n");
		printf("    Select Difficulty\n");
		printf("    1. Insane\n");
		printf("    2. Hard\n");
		printf("    3. Easy\n\n    ");
		choice=getchar();
		getchar();
		switch(choice){
			case '1':
				difConstant = 2;
				break;
			case '2':
				difConstant = 3;
				break;
			case '3':
				difConstant = 6;
				break;
			default:
				i--;
				break;		
		}
	}
	play();
}

void play(){
	loadMap(); 
	initGOPositions();
	printMap();
	int loop=1;
	//game loop
	while(loop++) { 
		//waiting for game input
        if (kbhit()) { 
        	playerStart = 1;
        	playerMove = 1;
            move = getch();
            switch(move){
				case 'w':
					movePlayer(p_y,p_x,move);
					break;
				case 'a':
					movePlayer(p_y,p_x,move);
					break;
				case 's':
					movePlayer(p_y,p_x,move);
					break;
				case 'd':
					movePlayer(p_y,p_x,move);
					break;		
			}
			//game won
			if(gameWin == 1){
				printMap();
				system("cls");
				printf("\n");									   
				printf("               __   _____  _   _  __        _____ _   _\n");
				printf("        __/\\__ \\ \\ / / _ \\| | | | \\ \\      / /_ _| \\ | | __/\\__\n");
				printf("        \\    /  \\ V / | | | | | |  \\ \\ /\\ / / | ||  \\| | \\    /\n");
				printf("        /_  _\\   | || |_| | |_| |   \\ V  V /  | || |\\  | /_  _\\\n");
				printf("          \\/     |_| \\___/ \\___/     \\_/\\_/  |___|_| \\_|   \\/\n");
				printf("\n");
				printf("                       Press any key to continue\n");
				garbage=getch();
				break;
			}
			printMap();
        } 
		
		//Condition if catched / game lost
		if((A_y == p_y && A_x == p_x) || (B_y == p_y && B_x == p_x) ||(C_y == p_y && C_x == p_x)){
			if((A_y == p_y && A_x == p_x)){
				map[p_x][p_y] = 'E';
			}
			else if((B_y == p_y && B_x == p_x)){
				map[p_x][p_y] = 'E';
			}
			else if((C_y == p_y && C_x == p_x)){
				map[p_x][p_y] = 'E';
			}
			printMap();
			system("cls");
			printf("\n");									   
			printf("         __   __   _____  _   _   _     ___  ____  _____    __\n");
			printf("         \\ \\  \\ \\ / / _ \\| | | | | |   / _ \\/ ___|| ____|  / /\n");
			printf("          \\ \\  \\ V / | | | | | | | |  | | | \\___ \\|  _|   / /\n");
			printf("          / /   | || |_| | |_| | | |__| |_| |___) | |___  \\ \\\n");
			printf("         /_/    |_| \\___/ \\___/  |_____\\___/|____/|_____|  \\_\\\n");
			printf("\n");
			printf("                       Press any key to continue\n");
			garbage=getch();
			break;
		}
		
		//if player move then get the path to the player and move enemies, and check + move every certain time
		//yang nge mod loop itu difficulty constant (ntar itu dif constant kali 5)
		if(playerMove == 1 && (loop % (difConstant*12000/6) == 0)){
			Node *lastA, *lastB, *lastC;
			if(playerStart == 1){
				playerStart = 0;
				BFS_A(A_y,A_x); BFS_B(B_y,B_x); BFS_C(C_y,C_x);
				lastA = leafA, lastB = leafB, lastC = leafC;
				while(lastC->parent != NULL){
					lastC->parent->next = lastC;
					lastC = lastC->parent;
				}
				while(lastB->parent != NULL){
					lastB->parent->next = lastB;
					lastB = lastB->parent;
				}
				while(lastA->parent != NULL){
					lastA->parent->next = lastA;
					lastA = lastA->parent;
				}
			}
			//check if previous path is key, nanti bakal jadi key lg selama belon di collect:
			for(int i=0;i<5;i++){
				if(lastA->y == item_y[i] && lastA->x == item_x[i] && itemFlag[i] == 0){
					map[lastA->y][lastA->x] = '*';
					break;
				}
				else{
					map[lastA->y][lastA->x] = ' ';
				}
			}
			for(int i=0;i<5;i++){
				if(lastB->y == item_y[i] && lastB->x == item_x[i] && itemFlag[i] == 0){
					map[lastB->y][lastB->x] = '*';
					break;
				}
				else{
					map[lastB->y][lastB->x] = ' ';
				}
			}
			for(int i=0;i<5;i++){
				if(lastC->y == item_y[i] && lastC->x == item_x[i] && itemFlag[i] == 0){
					map[lastC->y][lastC->x] = '*';
					break;
				}
				else{
					map[lastC->y][lastC->x] = ' ';
				}
			}
			lastA = lastA->next; lastB = lastB->next; lastC = lastC->next;
			map[lastA->y][lastA->x] = 'E'; map[lastB->y][lastB->x] = 'E'; map[lastC->y][lastC->x] = 'E';
			A_y = lastA->y; A_x = lastA->x; B_y = lastB->y; B_x = lastB->x; C_y = lastC->y; C_x = lastC->x;
			printMap();
		}
		if(loop>=(3*12000)){
			loop = 1;
		}
    } 
}
