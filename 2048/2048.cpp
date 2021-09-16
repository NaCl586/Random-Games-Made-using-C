#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

char input;
int score;
int hiscore;
bool wasValidMove;
int lastScore;

struct Block{
	int _value;
	Block* blockToMerge;
	int _isMerging;
};

struct Node{
	Block* _block;
};

char _board[20][20];
int _width = 4;
int _height = 4;
Node* _node[4][4];

int getRandom(int min, int max){
	return (rand() % (max-min + 1)) + min;
}

char toChar(int value){
	switch(value){
		case 1: return '1'; //2
		case 2: return '2'; //4
		case 3: return '3'; //8
		case 4: return '4'; //16
		case 5: return '5'; //32
		case 6: return '6'; //64
		case 7: return '7'; //128
		case 8: return '8'; //256
		case 9: return '9'; //512
		case 10: return 'A'; //1024
		case 11: return 'B'; //2048
	}
}

int powerOfTwo(int power){
	switch(power){
		case 1: return 2;
		case 2: return 4;
		case 3: return 8;
		case 4: return 16;
		case 5: return 32;
		case 6: return 64;
		case 7: return 128;
		case 8: return 256;
		case 9: return 512;
		case 10: return 1024;
		case 11: return 2048;
	}
}

void initiateBoard(){
	srand (time(NULL));
	for(int i=0;i<_height;i++){
		for(int j=0;j<_width;j++){
			_node[i][j] = (Node*) malloc(sizeof(Node));
			_node[i][j]->_block = NULL;
		}
	}
}

Block* spawnSingleBlock(int value){
	Block* newBlock = (Block*) malloc(sizeof(Block));
	newBlock->_value = value;
	return newBlock;
}

void printBoard(){
	for(int i=0;i<_height*2;i++){
		for(int j=0;j<_width*2+1;j++){
			_board[i][j] = ' ';
		}
	}
	for(int i=0;i<_height+1;i++){
		for(int j=0;j<_width+1;j++){
			_board[i*2][j*2] = '*';
			_board[i*2][j*2 + 1] = ' ';
		}
	}
	for(int i=0;i<_height;i++){
		for(int j=0;j<_width;j++){
			int exist = 0;
			if(j<_width && _node[i][j]->_block != NULL){
				exist = 1;
				_board[i*2 + 1][j*2 + 1] = toChar(_node[i][j]->_block->_value);
			}
			if(exist == 1){
				_board[i*2][j*2 + 1] = _board[i*2 + 2][j*2 + 1] = '=';
				_board[i*2 + 1][j*2] = _board[i*2 + 1][j*2 + 2] = '|';	
			}
		}
	}
	printf("=====2048=====\n\n");
	for(int i=0; i <= _height*2+1; i++){
		printf("  ");
		for(int j=0; j <= _width*2 ;j++){
			printf("%c", _board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Score  : %d\n", score);
	printf("Hiscore: %d\n", hiscore);
}

bool boardIsFull(){
	for(int i=0;i<_height;i++){
		for(int j=0;j<_width;j++){
			if(_node[i][j]->_block == NULL) return false;
		}
	}
	return true;
}

void spawnBlocks(int amount){
	if(boardIsFull() || !wasValidMove) return;
	
	for(int h=0;h<amount;h++){
		int spawned = 0;
		while(spawned == 0){
			int x = getRandom(0,3);
			int y = getRandom(0,3);
			if(_node[x][y]->_block == NULL){
				int spawn = getRandom(0,9) >= 8 ? 2 : 1;
				_node[x][y]->_block = spawnSingleBlock(spawn);
				spawned = 1;
			}
		}
	}
}

void shift(char dir){
	int done, max;
	wasValidMove = false;
	//kiri
	if(dir == 'a'){
		for(int i=0;i<_height;i++){
			done = 0;
			while(!done){
				for(int j=0;j<_width;j++){
					done = 1;
					if(_node[i][j]->_block != NULL){
						done = 0;
						break;
					}
				}
				if(done) break;
				for(int j=0;j<_width;j++){
					if(_node[i][j]->_block != NULL){
						if(j-1 < 0){
							done = 1;
						}
						else if(_node[i][j-1]->_block != NULL){
							if(_node[i][j-1]->_block->_value == _node[i][j]->_block->_value){
								_node[i][j]->_block = NULL;	
								_node[i][j-1]->_block->_value++;
								lastScore = powerOfTwo(_node[i][j-1]->_block->_value);
								score += lastScore;
								
								done = 0;
								wasValidMove = true;
							}
							else{
								done = 1;
							}
						}
						else{
							_node[i][j-1]->_block = _node[i][j]->_block;
							_node[i][j]->_block = NULL;	
							done = 0;
							wasValidMove = true;
						}			
					}
				}
			}
		}
	}
	//kanan
	if(dir == 'd'){
		for(int i=0;i<_height;i++){
			done = 0;
			while(!done){
				for(int j=0;j<_width;j++){
					done = 1;
					if(_node[i][j]->_block != NULL){
						done = 0;
						break;
					}
				}
				if(done) break;
				for(int j=_width-1;j>=0;j--){
					if(_node[i][j]->_block != NULL){
						if(j+1 >= _width){
							done = 1;
						}
						else if(_node[i][j+1]->_block != NULL){
							if(_node[i][j+1]->_block->_value == _node[i][j]->_block->_value){
								_node[i][j]->_block = NULL;	
								_node[i][j+1]->_block->_value++;
								lastScore = powerOfTwo(_node[i][j+1]->_block->_value);
								score += lastScore;
								done = 0;
								wasValidMove = true;
							}
							else{
								done = 1;
							}
						}
						else{
							_node[i][j+1]->_block = _node[i][j]->_block;
							_node[i][j]->_block = NULL;	
							done = 0;
							wasValidMove = true;
						}			
					}
				}
			}
		}
	}
	//atas
	if(dir == 'w'){
		for(int j=0;j<_width;j++){
			done = 0;
			while(!done){
				for(int i=0;i<_height;i++){
					done = 1;
					if(_node[i][j]->_block != NULL){
						done = 0;
						break;
					}
				}
				if(done) break;
				for(int i=0;i<_height;i++){
					if(_node[i][j]->_block != NULL){				
						if(i-1 < 0){
							done = 1;
						}
						else if(_node[i-1][j]->_block != NULL){
							if(_node[i-1][j]->_block->_value == _node[i][j]->_block->_value){
								_node[i][j]->_block = NULL;	
								_node[i-1][j]->_block->_value++;
								lastScore = powerOfTwo(_node[i-1][j]->_block->_value);
								score += lastScore;
								done = 0;
								wasValidMove = true;
							}
							else{
								done = 1;
							}
						}
						else{
							_node[i-1][j]->_block = _node[i][j]->_block;
							_node[i][j]->_block = NULL;	
							done = 0;
							wasValidMove = true;
						}		
					}
				}
			}
		}
	}
	//bawah
	if(dir == 's'){
		for(int j=0;j<_width;j++){
			done = 0;
			while(!done){
				for(int i=0;i<_height;i++){
					done = 1;
					if(_node[i][j]->_block != NULL){
						done = 0;
						break;
					}
				}
				if(done) break;
				for(int i=_height-1;i>=0;i--){
					if(_node[i][j]->_block != NULL){
						if(i+1 >= _width){
							done = 1;
						}
						else if(_node[i+1][j]->_block != NULL){
							if(_node[i+1][j]->_block->_value == _node[i][j]->_block->_value){
								_node[i][j]->_block = NULL;	
								_node[i+1][j]->_block->_value++;
								lastScore = powerOfTwo(_node[i+1][j]->_block->_value);
								score += lastScore;
								done = 0;
								wasValidMove = true;
							}
							else{
								done = 1;
							}
						}
						else{
							_node[i+1][j]->_block = _node[i][j]->_block;
							_node[i][j]->_block = NULL;	
							done = 0;
							wasValidMove = true;
						}			
					}
				}
			}
		}
	}
}
void game(){
	score = 0;
	wasValidMove = true;
	initiateBoard();
	int _round = 0;
	spawnBlocks(2 - _round);
	_round++;
	while(true){
		system("cls");
		printBoard();	
		input = getch();
		shift(input);
		
		if(lastScore != 2048) spawnBlocks(2 - _round);
		
		if(lastScore == 2048){
			system("cls");
			printBoard();	
			printf("You win!\n");
			return;
		}
		else if(boardIsFull() && !wasValidMove){
			system("cls");
			printBoard();	
			printf("You lose!\n");
			return;
		}

	}
}

int main(){
	hiscore = 0;
	while(true){
		game();
		char c;
		int playAgain = 0;
		for(int i=0;i<1;i++){
			hiscore = score;
			printf("Play again? (Y/N)\n");
			scanf("%c", &c);
			if(c == 'y' || c == 'Y') playAgain = 1;
			else if(c == 'n' || c == 'N') playAgain = 0;
			else i--;
		}
		if(playAgain == 0) break;
		else continue;
		
	}
	return 0;
}
