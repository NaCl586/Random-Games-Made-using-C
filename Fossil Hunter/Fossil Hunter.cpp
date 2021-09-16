#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

void mainMenu();
void howToPlay();
void selectDif();
void playGame();
void winGame();
void loseGame();

char c[50][50]; //playfield
int a[50][50] = {}; //fossil location
char dif;
int chance;
int fossil;
int size;

int main(){
	mainMenu();
	return 0;
}

void mainMenu(){
	system("cls");
	printf("\n");
	printf(" POSSIL HUNTER\n\n");
	printf(" 1. Play\n");
	printf(" 2. Help\n");
	printf(" 3. Exit\n");
	printf(" >>");
	char choice;
	for(int i=0; i<1; i++){
		choice=getch();
		switch(choice){
			case '1':
				selectDif();
				break;
			case '2':
				howToPlay();
				break;
			case '3':
				exit(0);
				break;
			default:
				i--;
				break;		
		}
	}
}

void selectDif(){
	system("cls");
	printf("\n");
	printf("  === Choose your difficulty ===\n");
	printf("\n");
	printf(" 1. Easy ( 10 x 10 Dirt Lot, 10 fossil & 50 dig chance)\n");
	printf(" 2. Medium ( 20 x 20 Dirt Lot, 20 fossil & 150 dig chance)\n");
	printf(" 3. Hard ( 30 x 30 Dirt Lot, 30 fossil & 200 dig chance)\n");
	printf(" >>");
	for(int i=0; i<1; i++){
		dif=getch();
		switch(dif){
			case '1':
				size = 10;
				fossil = 10;
				chance = 50;
				break;
			case '2':
				size = 20;
				fossil = 20;
				chance = 150;
				break;
			case '3':
				size = 30;
				fossil = 30;
				chance = 200;
				break;
			default:
				i--;
				break;		
		}
	}
	playGame();
}

void playGame(){
	system("cls");
	
	//init blank map
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			c[i][j] = 'O';
		}
	}
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			a[i][j] = 0;
		}
	}
	
	int fossilRand = fossil;
	int fossilType;
	int rand_x, rand_y;
	
	//generate fossil location
	srand (time(NULL));
	while(fossilRand>0){
		fossilType = (rand() % 6)+1;
		switch(fossilType){
			case 1: // #
				if(fossilRand - 1 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					a[rand_y][rand_x] = 1;
					fossilRand--;
				}
				break;
			case 2: //T piece
 				if(fossilRand - 4 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					//cek apakah sebelahnya kosong atau oob
					if((rand_x+1 < size && rand_y+1 < size && rand_x-1 >= 0) && (a[rand_y][rand_x+1] != 1 && a[rand_y+1][rand_x] != 1 && a[rand_y][rand_x-1] != 1)){
						a[rand_y][rand_x] = 1;
						a[rand_y][rand_x+1] = 1;
						a[rand_y+1][rand_x] = 1;
						a[rand_y][rand_x-1] = 1;
						fossilRand-=4;
					}	
				}
				break;
			case 3: // ##
				if(fossilRand - 2 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					//cek apakah sebelahnya kosong atau oob
					if(rand_x+1 < size && a[rand_y][rand_x+1] != 1){
						a[rand_y][rand_x] = 1;
						a[rand_y][rand_x+1] = 1;
						fossilRand-=2;
					}	
				}
				break;
			case 4: //T piece kebalik
				if(fossilRand - 4 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					//cek apakah sebelahnya kosong atau oob
					if((rand_x+1 < size && rand_y-1 >= 0 && rand_x-1 >= 0) && (a[rand_y][rand_x+1] != 1 && a[rand_y-1][rand_x] != 1 && a[rand_y][rand_x-1] != 1)){
						a[rand_y][rand_x] = 1;
						a[rand_y][rand_x+1] = 1;
						a[rand_y-1][rand_x] = 1;
						a[rand_y][rand_x-1] = 1;
						fossilRand-=4;
					}	
				}
				break;
			case 5: //o piece
				if(fossilRand - 4 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					//cek apakah sebelahnya kosong atau oob
					if((rand_x+1 < size && rand_y+1 < size) && (a[rand_y][rand_x+1] != 1 && a[rand_y+1][rand_x] != 1 && a[rand_y+1][rand_x+1] != 1)){
						a[rand_y][rand_x] = 1;
						a[rand_y][rand_x+1] = 1;
						a[rand_y+1][rand_x] = 1;
						a[rand_y+1][rand_x+1] = 1;
						fossilRand-=4;
					}	
				}
				break;
			case 6: //#####
				if(fossilRand - 5 < 0){
					break;
				}
				rand_y = (rand() % size);
				rand_x = (rand() % size);
				if(a[rand_y][rand_x] != 1){
					//cek apakah kira2 piece nya bakal overlap atau oob
					if((rand_x+1 < size && rand_x+2 < size && rand_x-1 >= 0 && rand_x-2 >= 0) && (a[rand_y][rand_x-2] != 1 && a[rand_y][rand_x-1] != 1 && a[rand_y][rand_x+1] != 1 && a[rand_y][rand_x+2] != 1)){
						for(int i=0;i<5;i++){
							a[rand_y][rand_x-2+i] = 1;
						}				
						fossilRand-=5;
					}	
				}
				break;
		}
	}
		
	//print map + play game
	while(chance>=fossil && fossil>0){
		system("cls");
		printf("\n");
		switch(dif){
			case '1':
				printf("   ");
				break;
			case '2':
				printf("             ");
				break;
			case '3':
				printf("                       ");
				break;	
		}
		printf("=== Possil Hunter ===\n");
		printf("\n");
		for(int i=-1;i<size;i++){
			printf("  ");
			i == -1 ? printf("  ") : printf("%d ", (i+1)%10);
			for(int j=0;j<size;j++){
				i == -1 ? printf("%d ", (j+1)%10) : printf("%c ", c[i][j]);
			}
			printf("\n");
		}
		printf("\n  Remaining fossil lot to found : %d\n", fossil);
		printf("  Remaining dig chance : %d\n", chance);
		printf("\n");
		printf("  Input coordinate [Format : X-Y] : ");
		int x,y;
		scanf("%d%*c%d", &x,&y);
		if(x-1 >= 0 && x-1 < size && y-1 >= 0 && y-1 < size){
			if(a[y-1][x-1] == 1){
				fossil--;
				chance--;
				c[y-1][x-1] = '#';
				a[y-1][x-1] = 5;
			}
			else if(a[y-1][x-1] == 0){
				chance--;
				c[y-1][x-1] = '.';
				a[y-1][x-1] = 5;
			}
		}
	}
	
	//win
	if(fossil<=0){
		winGame();
	}
	
	//lose
	if(chance<fossil){
		loseGame();
	}
}

void winGame(){
	system("cls");
	printf("\n You Win!\n\n");
	printf(" You have found all of the fossils!\n");
	printf(" Press [B] to return to the main menu\n");
	printf(" >>");
	char choice;
	for(int i=0; i<1; i++){
		choice=getch();
		switch(choice){
			case 'b':
				mainMenu();
				break;
			case 'B':
				mainMenu();
				break;
			default:
				i--;
				break;		
		}
	}
}

void loseGame(){
	system("cls");
	printf("\n Game Over!\n\n");
	printf(" You have ran out of chances!\n");
	printf(" Press [B] to return to the main menu\n");
	printf(" >>");
	char choice;
	for(int i=0; i<1; i++){
		choice=getch();
		switch(choice){
			case 'b':
				mainMenu();
				break;
			case 'B':
				mainMenu();
				break;
			default:
				i--;
				break;		
		}
	}
}

void howToPlay(){
	system("cls");
	printf("\n");
	printf("  === HELP ===\n");
	printf("\n");
	printf("  1. You are a fossil hunter, your job is to find fossils hidden somewhere in the dirt lot\n");
	printf("  2. You will need to find a certain number of fossil to win the game\n");
	printf("  3. You also have a limited number of chance to dig\n");
	printf("  4. If your chance to dig is smaller than the remaining fossil lot, you will lose\n");
	printf("  5. To dig a dirt lot, you will need to input the coordinate of that dirt lot\n");
	printf("  6. You can't dig a dirt lot that you have dug\n");
	printf("  7. These are some symbols that you need to know\n");
	printf("      O : Dirt Lot\n");
	printf("      # : Fossil\n");
	printf("      . : Dug Dirt\n");
	printf("  8. Fossil will have various shape just like shown below\n");
	printf("      +=======+=========+========+=========+========+===========+\n");
	printf("      |  - #  |  - ###  |  - ##  |  -  #   |  - ##  |  - #####  |\n");
	printf("      |       |      #  |        |    ###  |    ##  |           |\n");
	printf("      +=======+=========+========+=========+========+===========+\n");
	printf("  9. You will have different amount of chance to dig a dirt lot depends on the difficulty\n");
	printf(" 10. Each difficulty will have its own number of fossil that you need to find\n\n");
	printf(" Press [B] to continue\n");
	printf(" >>");
	char choice;
	for(int i=0; i<1; i++){
		choice=getch();
		switch(choice){
			case 'b':
				mainMenu();
				break;
			case 'B':
				mainMenu();
				break;
			default:
				i--;
				break;		
		}
	}
}

