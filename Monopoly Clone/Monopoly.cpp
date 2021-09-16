#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

//prototypes
void mainMenu();
void playerSelect();
void highScores();
void howToPlay();
void playGame();
void initMap();
void printBoard();
void playerTurn(int n);
void selectionSort(struct playerHighScore highScore[], int highScoreCount);
void selectionSortLB(struct playerLB sortedPlayer[], int playerNumber);

//structs
struct blockInfo{
	int block_number; //block number
	char block_name[100];
	int island_price;
	char block_type[100];
	int multiplier; //festival effects, init nya 1
	int owner; //siapa yg punya
};

struct playerLB{
	char name[100];
	int money;
	int asset;
	int symbol;
};

struct playerData{
	char name[100]; //nama
	int money; //jumlah money
	int asset; //jumlah asset
	int player_position; //posisi player
	int angel_card; //bool buat cek apakah player punya angel card
	int jail; //bool buat cek apakah player dipenjara ato nggak
	int travel; //bool buat cek travel
};

struct playerHighScore{
	char name[100];
	int money;
};

//variables
int playerNumber; //player number
struct playerData player[4]; //player
struct playerLB sortedPlayer[4]; //sorted player for leaderboards
char map[45][150]; //map
int map_line, block_num; //counter map line buat print + block_num counter
struct blockInfo block[28]; // karena ada 28 block
int gameFinish = 0; //bool to check game finish
int festival;
int currentPlayer, turn;
int actionPhase, kho;
struct playerHighScore highScore[1000];
int highScoreCount;
int giveUpPlayer;

//void start kalau di C lel (sadly ga ada fixedupdate sama update disini)
int main(){
	mainMenu();
	return 0;
}

//main menu
void mainMenu(){
	system("cls");
	printf("\n");
	printf(" MARI MENJADI KAYA\n\n");
	printf(" 1. Play\n");
	printf(" 2. See all the winner\n");
	printf(" 3. Basic Information\n");
	printf(" 4. Exit\n");
	printf(" >>");
	char choice;
	for(int i=0; i<1; i++){
		choice=getch();
		switch(choice){
			case '1':
				playerSelect();
				break;
			case '2':
				highScores();
				break;
			case '3':
				howToPlay();
				break;
			case '4':
				exit(0);
				break;
			default:
				i--;
				break;		
		}
	}
}

//player select
void playerSelect(){
	system("cls");
	printf("\n");
	for(int i=0; i<1; i++){
		printf(" Choose how many player to precipitate in the game [2-4]: ");
		scanf("%d", &playerNumber);
		if(playerNumber < 2 || playerNumber > 4){
			i--;
		}
	}
	for(int i=0;i<playerNumber;i++){
		printf(" Input player %d name [ must be 3 or 8 characters ]: ", i+1);
		scanf("%s", player[i].name);
		getchar();
		int len = strlen(player[i].name);
		if(len < 3 || len > 8){
			i--;
		}
	}
	playGame();
}

//initializations:
void initMap(){
	//open file
	FILE *fmap = fopen("map.txt", "r");
	FILE *fblock = fopen("block_information.txt", "r");
	
	//scan map
	map_line = 0;
	while(fscanf(fmap, "%[^\n]\n", &map[map_line]) != EOF){
		map_line++;
	}
	
	//scan block + reset value juga
	block_num=0;
	while(fscanf(fblock, "%[^#]#%d#%[^\n]\n", &block[block_num].block_name, &block[block_num].island_price, &block[block_num].block_type) != EOF){
		block[block_num].block_number = block_num;
		block[block_num].multiplier = 1;
		block[block_num].owner = -1;
		block_num++;
	}
	//close file
	fclose(fmap);
	fclose(fblock);
	
	//player data init:
	for(int i=0;i<playerNumber;i++){
		player[i].money = 1000;
		player[i].player_position = 0;
		player[i].asset = 0;
		player[i].angel_card = 0;
		player[i].jail = 0;
		player[i].travel = 0;
	}
}

void printBoard(){
	system("cls");
	
	//copy playerStat struct into playerLB struct for sorting purposes
	for(int i=0;i<playerNumber;i++){
		strcpy(sortedPlayer[i].name, player[i].name);
		sortedPlayer[i].money = player[i].money;
		sortedPlayer[i].asset = player[i].asset;
		sortedPlayer[i].symbol = i+35;
	}
	selectionSortLB(sortedPlayer,playerNumber);
	
	//clear all player position:
	for(int i=0;i<4;i++){
		for(int j=0;j<8;j++){
			switch(i){
				case 0:
					map[37-(j*5)][5] = ' ';
					break;
				case 1:
					map[37-(j*5)][5+1] = ' ';
					break;
				case 2:
					map[37+1-(j*5)][5] = ' ';
					break;
				case 3:
					map[37+1-(j*5)][5+1] = ' ';
					break;
			}
		}
		for(int j=8;j<14;j++){
			switch(i){
				case 0:
					map[2][20+((j-8)*14)] = ' ';
					break;
				case 1:
					map[2][20+1+((j-8)*14)] = ' ';
					break;
				case 2:
					map[2+1][20+((j-8)*14)] = ' ';
					break;
				case 3:
					map[2+1][20+1+((j-8)*14)] = ' ';
					break;
			}
		}
		for(int j=14;j<22;j++){
			switch(i){
				case 0:
					map[2+((j-14)*5)][102] = ' ';
					break;
				case 1:
					map[2+((j-14)*5)][102+1] = ' ';
					break;
				case 2:
					map[2+1+((j-14)*5)][102] = ' ';
					break;
				case 3:
					map[2+1+((j-14)*5)][102+1] = ' ';
					break;
			}
		}
		for(int j=22;j<28;j++){
			switch(i){
				case 0:
					map[37][88-((j-22)*14)] = ' ';
					break;
				case 1:
					map[37][88+1-((j-22)*14)] = ' ';
					break;
				case 2:
					map[37+1][88-((j-22)*14)] = ' ';
					break;
				case 3:
					map[37+1][88+1-((j-22)*14)] = ' ';
					break;
			}
		}
	}
	
	//clear all block marks
	for(int i=0;i<8;i++){
		map[36-(i*5)][12] = ' '; map[36-(i*5)][13] = ' ';
	}
	for(int i=0;i<7;i++){
		map[36-(i*5)][110] = ' '; map[36-(i*5)][111] = ' ';
	}
	for(int i=1;i<7;i++){
		map[36][12+(i*14)] = ' '; map[36][13+(i*14)] = ' ';
		map[1][12+(i*14)] = ' '; map[1][13+(i*14)] = ' ';
	}
	
	//player position (gap atas-bawah = 5 | gap kiri-kanan = 14)
	for(int i=0;i<playerNumber;i++){
		//0-7 (0 ada di 37,5; 7 ada di 2,5)
		if(player[i].player_position >= 0 && player[i].player_position <= 7){
			switch(i){
				case 0:
					map[37-(player[i].player_position*5)][5] = i+35;
					break;
				case 1:
					map[37-(player[i].player_position*5)][5+1] = i+35;
					break;
				case 2:
					map[37+1-(player[i].player_position*5)][5] = i+35;
					break;
				case 3:
					map[37+1-(player[i].player_position*5)][5+1] = i+35;
					break;
			}
		}
		//8-13
		if(player[i].player_position >= 8 && player[i].player_position <= 13){
			switch(i){
				case 0:
					map[2][20+((player[i].player_position-8)*14)] = i+35;
					break;
				case 1:
					map[2][20+1+((player[i].player_position-8)*14)] = i+35;
					break;
				case 2:
					map[2+1][20+((player[i].player_position-8)*14)] = i+35;
					break;
				case 3:
					map[2+1][20+1+((player[i].player_position-8)*14)] = i+35;
					break;
			}
		}
		//14-21 (14 ada di 2,102, 21 ada di 37,102)
		if(player[i].player_position >= 14 && player[i].player_position <= 21){
			switch(i){
				case 0:
					map[2+((player[i].player_position-14)*5)][102] = i+35;
					break;
				case 1:
					map[2+((player[i].player_position-14)*5)][102+1] = i+35;
					break;
				case 2:
					map[2+1+((player[i].player_position-14)*5)][102] = i+35;
					break;
				case 3:
					map[2+1+((player[i].player_position-14)*5)][102+1] = i+35;
					break;
			}
		}
		//22-27
		if(player[i].player_position >= 22 && player[i].player_position <= 27){
			switch(i){
				case 0:
					map[37][88-((player[i].player_position-22)*14)] = i+35;
					break;
				case 1:
					map[37][88+1-((player[i].player_position-22)*14)] = i+35;
					break;
				case 2:
					map[37+1][88-((player[i].player_position-22)*14)] = i+35;
					break;
				case 3:
					map[37+1][88+1-((player[i].player_position-22)*14)] = i+35;
					break;
			}
		}
	}
	
	//define all block marks
	for(int i=0;i<=7;i++){
		for(int j=0;j<4;j++){
			if(block[i].owner == j){
				map[36-(i*5)][12] = 'P'; map[36-(i*5)][13] = j+'1';
			}
		}
	}
	for(int i=8;i<=13;i++){
		for(int j=0;j<4;j++){
			if(block[i].owner == j){
				map[1][12+((i-7)*14)] = 'P'; map[1][13+((i-7)*14)] = j+'1';
			}
		}	
	}	
	for(int i=15;i<=21;i++){
		for(int j=0;j<4;j++){
			if(block[i].owner == j){
				map[1+((i-14)*5)][110] = 'P'; map[1+((i-14)*5)][111] = j+'1';
			}
		}
	}
	for(int i=22;i<=27;i++){
		for(int j=0;j<4;j++){
			if(block[i].owner == j){
				map[36][96-((i-22)*14)] = 'P'; map[36][97-((i-22)*14)] = j+'1';
			}
		}
	}
	
	//print map
	printf("\n");
	for(int i=0;i<map_line;i++){
		int j;
		printf("  %s   ",map[i]);
		switch(i){
			//leaderboard
			case 1:
				printf("Leaderboard\n"); break;
			case 2:
				printf("===========\n"); break;
			case 4:
				j=0;
				printf("%d. (%c) %-8s  %d K (%d K + %d K)\n", j+1, sortedPlayer[j].symbol, sortedPlayer[j].name, sortedPlayer[j].money+sortedPlayer[j].asset, sortedPlayer[j].money, sortedPlayer[j].asset); 
				break;
			case 5:
				j=1;
				printf("%d. (%c) %-8s  %d K (%d K + %d K)\n", j+1, sortedPlayer[j].symbol, sortedPlayer[j].name, sortedPlayer[j].money+sortedPlayer[j].asset, sortedPlayer[j].money, sortedPlayer[j].asset); 
				break;
			case 6:
				if(playerNumber>2){
					j=2;
					printf("%d. (%c) %-8s  %d K (%d K + %d K)\n", j+1, sortedPlayer[j].symbol, sortedPlayer[j].name, sortedPlayer[j].money+sortedPlayer[j].asset, sortedPlayer[j].money, sortedPlayer[j].asset); 
					break;
				}
				else{
					printf("\n");
					break;
				}
			case 7:
				if(playerNumber == 4){
					j=3;
					printf("%d. (%c) %-8s  %d K (%d K + %d K)\n", j+1, sortedPlayer[j].symbol, sortedPlayer[j].name, sortedPlayer[j].money+sortedPlayer[j].asset, sortedPlayer[j].money, sortedPlayer[j].asset); 
					break;	
				}
				else {
				printf("\n"); break;
				}
			case 10:
				printf("Block Information\n"); break;
			case 11:
				printf("=================\n"); break;
			case 13:
				if(actionPhase == 1 && strcmp(block[player[currentPlayer].player_position].block_type,"island") == 0){
					printf("Island Name: %s\n", block[player[currentPlayer].player_position].block_name); break;	
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 14){
					printf("Welcome to Festival Block!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 21){
					printf("Welcome to the Travel Block!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 26){
					printf("Welcome to Tax Block!\n");break;
				}
				else if(actionPhase == 1 && (player[currentPlayer].player_position == 8 || player[currentPlayer].player_position == 16)){
					printf("Welcome to Chance Card Block!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 7){
					printf("You land on jail!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 0){
					printf("Welcome to Go Block~\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 2){
					printf("Welcome to Free Block! There's nothing here~\n");break;
				}
				else{
					printf("\n");
				}
				break;
			case 14:
				if(actionPhase == 1 && player[currentPlayer].player_position == 14){
					printf("Choose any of your island~\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 21){
					printf("You can choose anywhere you want to go\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 26){
					printf("You will be charged 20 percent of your total money.\n");break;
				}
				else if(actionPhase == 1 && (player[currentPlayer].player_position == 8 || player[currentPlayer].player_position == 16)){
					printf("If you're lucky, you will receive an angel card!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 7){
					printf("You lose your next turn.\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 0){
					printf("If you land or pass this block,\n");break;
				}
				else{
					printf("\n");
				}
				break;
			case 15: 
				if(actionPhase == 1 && strcmp(block[player[currentPlayer].player_position].block_type,"island") == 0){
					printf("Rent: %d K\n", block[player[currentPlayer].player_position].island_price); break;	
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 14){
					printf("The chosen island price will be doubled!\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 21){
					printf("in the next turn~\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 26){
					printf("(assets + current money) if you land on this block.\n");break;
				}
				else if(actionPhase == 1 && player[currentPlayer].player_position == 0){
					printf("you will be receiving 200 K money~\n");break;
				}
				else{
					printf("\n");
				}
				break;
			case 16: 
				if(actionPhase == 1 && strcmp(block[player[currentPlayer].player_position].block_type,"island") == 0){
					printf("Status: ");
					switch(block[player[currentPlayer].player_position].multiplier){
						case 1:
							printf("Normal\n");
							break;
						case 2:
							printf("Price x2\n");
							break;
						case 4:
							printf("Price x4\n");
							break;
						case 8:
							printf("Price x8\n");
							break;
					}
				}
				else{
					printf("\n");
				}
				break;
				
			//Current Player
			case 24:
				printf("Current Player\n"); break;
			case 25:
				printf("==============\n"); break;
			case 27:
				printf("[P%d (%c)] ", currentPlayer+1, currentPlayer+35); 
				player[currentPlayer].angel_card == 1 ? printf("(^v^)/\n") : printf("\n");
				break;
			case 28:
				printf("Name: %s\n", player[currentPlayer].name); break;
			case 30:
				printf("Money: %d K\n", player[currentPlayer].money); break;
			case 31:
				printf("Total: %d K\n", player[currentPlayer].money+player[currentPlayer].asset); break;
			default:
				printf("\n"); break;
		}
	}
	printf("\n");
}

//the real game
void playGame(){
	system("cls");
	srand (time(NULL)); //random seed per game
	initMap();
	printBoard();
	gameFinish = 0;
	currentPlayer = 0;
	turn = 0;
	festival = -1;
	while(gameFinish == 0){
		playerTurn(currentPlayer);
		currentPlayer++;
		if(currentPlayer > (playerNumber-1)){
			currentPlayer = 0;
			turn++;	
		}
	}
	printBoard();
	int winner = sortedPlayer[0].symbol-35;
	if(winner == giveUpPlayer){
		winner++;
	}
	FILE *highScoreEnter = fopen("winnerlist.txt", "a");
	fprintf(highScoreEnter, "\n%s %d", player[winner].name ,player[winner].money + player[winner].asset);
	fclose(highScoreEnter);
	
	printf("%s win the game! Congratulations~\n\n", player[winner].name);
	printf("List Assets:\n");
	for(int j=0;j<=27;j++){
		if(block[j].owner == winner){
			printf("%2d. %s - %d K\n", j, block[j].block_name, block[j].island_price);
		}
	}
	printf("Total Assets: %d K\n", player[winner].asset);
	printf("Money: %d K\n", player[winner].money);
	printf("Total (Money + Assets): %d K\n\n", player[winner].money + player[winner].asset);
	printf("Press any key to return to main menu...");
	char garbage=getch();
	mainMenu();
}

//turn
//terdiri atas roll dice, gerak, cek posisi skrg + action
void playerTurn(int n){
	int move;
	//--------------------------------------roll dice-----------------------------------------------
	char choice, garbage; //garbage basically get something but ga peduli buat apa
	int diceRolled;
	//jail = cannot move
	if(player[n].jail == 1){
		printBoard();
		printf("Player %d (%c)'s Turn!\n", n+1, n+35);
		printf("You are in the jail, you can move in the next turn.\n");
		printf("Press any key to continue...");
		garbage=getch();
		player[n].jail = 0;
		return;
	}
	//travel effects
	if(player[n].travel == 1){
		player[n].travel = 0;
		int travel_move;
		for(int i=0;i<1;i++){
			printBoard();
			printf("Player %d (%c)'s Turn!\n", n+1, n+35);
			printf("Choose any block that you want to go [0-27]:");
			scanf("%d", &travel_move);
			if(travel_move < 0 || travel_move > 27 || travel_move == 21){
				i--;
			}
		}
		travel_move < 21 ? travel_move+=28 : travel_move;
		move = travel_move - player[n].player_position;
		//moving process
		while(move>=0){
			printBoard();
			if(player[n].player_position == 0 && turn != 0 && move != diceRolled){
				printf("\nYou got 200 K money from Bank!\n");
				player[n].money += 200;
				printf("Press any key to continue...");
				garbage=getch();
			}
			if(move>0){
				player[n].player_position++;
				if(player[n].player_position > 27){
					player[n].player_position = 0;
				}
			}
			move--;
			//delay, teknik ko elvan ganteng dari bootcamp
			for(int i=0;i<200000000;i++){
				;
			}
		}
		return;
	}
	//move
	for(int i=0; i<1; i++){
		printBoard();
		printf("Player %d (%c)'s Turn!\n", n+1, n+35);
		printf("[Press Q if you want to give up]\n");
		printf("Press any key to roll dice...");
		choice=getch();
		switch(choice){
			case 'q':
				if(turn>=2){
					printBoard();
					printf("%s choose to give up!\n\n", player[n].name);
					garbage=getch();
					gameFinish = 1;
					return;
				}
				else{
					printBoard();
					printf("You can't give up on 1~2 round!\n");
					printf("Press any key to continue...");
					garbage=getch();
					i--;
				}
				break;
			case 'Q':
				if(turn>=2){
					printBoard();
					printf("%s choose to give up!\n\n", player[n].name);
					giveUpPlayer = n;
					garbage=getch();
					gameFinish = 1;
					return;
				}
				else{
					printBoard();
					printf("You can't give up on 1~2 round!\n");
					printf("Press any key to continue...");
					garbage=getch();
					i--;
				}
				break;
			default:
				//roll dice
				//scanf("%d", &diceRolled); getchar(); //debug mode
				diceRolled = (rand() % 11)+2;
				break;		
		}
	}
	//---------------------------------------moving process-------------------------------------------
	move = diceRolled;
	while(move>=0){
		printBoard();
		printf("Player %d (%c)'s Turn!\n", n+1, n+35);
		printf("Dice Rolled: %d\n", diceRolled);
		printf("Move Remaining: %d\n", move);
		if(player[n].player_position == 0 && turn != 0 && move != diceRolled){
			printBoard();
			printf("Player %d (%c)'s Turn!\n", n+1, n+35);
			printf("Dice Rolled: %d\n", diceRolled);
			printf("Move Remaining: %d\n", move);
			printf("\nYou got 200 K money from Bank!\n");
			player[n].money += 200;
			printf("Press any key to continue...");
			garbage=getch();
		}
		if(move>0){
			player[n].player_position++;
			if(player[n].player_position > 27){
				player[n].player_position = 0;
			}
		}
		move--;
		//delay, teknik ko elvan ganteng dari bootcamp
		for(int i=0;i<200000000;i++){
			;
		}
	}
	//-------------------------------------action process---------------------------------------------
	actionPhase = 1;
	//cek space:
	//jail block --> lose a turn (space 7)
	
	if(player[n].player_position == 0){
		printBoard();
		printf("Player %d (%c)'s Turn!\n", n+1, n+35);
		printf("Dice Rolled: %d\n", diceRolled);
		printf("Move Remaining: 0\n");
		printf("\nYou got 200 K money from Bank!\n");
		printf("Press any key to continue...");
		garbage=getch();
	}
	
	if(player[n].player_position == 7){
		printBoard();
		player[n].jail = 1;
		printf("You landed on jail, you cannot move in your next turn\n");
		printf("Press any key to continue...");
		garbage=getch();
	}
	
	//island block --> ya gitu
	//cek apakah island kosong ato nggak, kalau kosong tanya mau rent ato nggak
	//kalau ada isi kalau punya orang lain
	//cek mau pakai angel card ato nggak? 
	//kalau duit ga cukup cek apakah mau pake sell island ato bankrupt
	//kalau duit dan asset ga cukup auto game over (gameFinish = 1 dan return)
	if(strcmp(block[player[n].player_position].block_type,"island") == 0){
		char decide[10];
		if(block[player[n].player_position].owner == -1){
			while(true){
				if(block[player[n].player_position].island_price > player[n].money){
					printBoard();
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
					printf("You don't have enough money to purchase this island!\n", block[player[n].player_position].block_name);
					printf("Press any key to continue...");
					garbage=getch();
					break;
				}
				printBoard();
				printf("Player %d (%c)'s Turn!\n", n+1, n+35);
				printf("Do you want to buy %s island for %d K [ Yes | No ] ? ", block[player[n].player_position].block_name, block[player[n].player_position].island_price);
				scanf("%s", decide);
				getchar();
				if(strcmp("Yes", decide) == 0){
					block[player[n].player_position].owner = n;
					player[n].money -= block[player[n].player_position].island_price;
					player[n].asset += block[player[n].player_position].island_price;
					printBoard();
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
					printf("You have purchased and owned %s island\n", block[player[n].player_position].block_name);
					printf("Press any key to continue...");
					garbage=getch();
					break;
				}
				else if(strcmp("No", decide) == 0){
					break;
				}
			}
		}
		else if(block[player[n].player_position].owner != n){
			int angelCard = 0;
			if(player[n].angel_card == 1){
				printBoard();
				printf("Player %d (%c)'s Turn!\n", n+1, n+35);
				printf("Use Angel Card [ Yes | No ] ? ");
				scanf("%s", decide);
				getchar();
				if(strcmp("Yes", decide) == 0){
					player[n].angel_card = 0;
					angelCard = 1;
					printBoard();
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
					printf("You used your angel card!\n");
					printf("Press any key to continue...");
					garbage=getch();
				}
				if(strcmp("No", decide) == 0){
					printBoard();
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
				}
			}
			while(angelCard == 0){
				if((block[player[n].player_position].island_price*block[player[n].player_position].multiplier*3/2) <= player[n].money){
					printBoard();
					player[n].money -= (block[player[n].player_position].island_price*block[player[n].player_position].multiplier*3/2);
					player[block[player[n].player_position].owner].money += (block[player[n].player_position].island_price*block[player[n].player_position].multiplier*3/2);
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
					printf("Oops, you land on %s's island~ You need to pay a rent for %d K!\n", player[block[player[n].player_position].owner].name, (block[player[n].player_position].island_price*block[player[n].player_position].multiplier*3/2));
					printf("Press any key to continue...");
					garbage=getch();
					break;
				}
				else if((block[player[n].player_position].island_price*block[player[n].player_position].multiplier*3/2) > (player[n].money)){
					printBoard();
					printf("Player %d (%c)'s Turn!\n", n+1, n+35);
					printf("You don't have enough money to pay rent!\n");
					printf("1. Sell Island\n");
					printf("2. Bankrupt\n");
					printf("Choose [ 1 | 2 ]:");
					char rip;
					for(int i=0; i<1; i++){
						rip=getch();
						switch(rip){
							case '1':
								printBoard();
								printf("Player %d (%c)'s Turn!\n", n+1, n+35);
								printf("Choose island that you want to sell:\n");
								for(int j=0;j<=27;j++){
									if(block[j].owner == n){
										printf("%2d. %s - %d K\n", j, block[j].block_name, block[j].island_price);
									}
								}
								printf(" 0. Back\n");
								printf(">> ");
								int number;
								scanf("%d", &number);
								if(number == 0){
									break;
								}
								kho = 0;
								for(int j=0;j<=27;j++){
									if(block[j].owner == n && number == j){
										kho = 1;
										printBoard();
										printf("Player %d (%c)'s Turn!\n", n+1, n+35);
										printf("You have sold %s for %d\n", block[j].block_name, block[j].island_price);
										block[j].owner = -1;
										player[n].money += block[j].island_price;
										printf("Press any key to continue...");
										garbage=getch();
										break;
									}
								}
								if(kho == 0){
									i--;
									break;
								}
								break;
							case '2':
								printBoard();
								printf("Player %d has gone bankrupt! The game is over!\n", n+1);
								printf("Press any key to continue...");
								gameFinish = 1;
								garbage=getch();
								break;
							default:
								i--;
								break;		
						}
					}
				}
				else{
					break;
				}	
				if(gameFinish == 1){
					break;
				}
			}
		}
	}
	
	//festival block --> multiplier (space 14)
	//kalau belon ada yg pake festival (festival = -1) --> player yg dateng buat festival
	//kalau udh ada festival, kalau player yg sama lgsg mau mutiply yg mana, kalau ga ada langsung 
	if(player[n].player_position == 14){
		if(festival == -1){
			festival = n;
		}
		if(festival != n){
			for(int i=0;i<block_num;i++){
				block[i].multiplier = 1;
			}
			festival = n;
		}
		if(festival == n){
			for(int i=0;i<1;i++){
				printBoard();
				printf("Player %d (%c)'s Turn!\n", n+1, n+35);
				printf("Choose island that you want to hold festival:\n");
				for(int j=0;j<=27;j++){
					if(block[j].owner == n){
						printf("%2d. %s - %d K\n", j, block[j].block_name, block[j].island_price);
					}
				}
				printf(" 0. Skip festival\n");
				printf(">> ");
				int number;
				scanf("%d", &number);
				if(number == 0){
					continue;
				}
				kho = 0;
				for(int j=0;j<=27;j++){
					if(block[j].owner == n && number == j){
						kho = 1;
						printf("Yay! The chosen block rent price has been doubled!\n");
						switch(block[j].multiplier){
							case 4:
								block[j].multiplier = 8;
								break;
							case 2:
								block[j].multiplier = 4;
								break;
							case 1:
								block[j].multiplier = 2;
								break;
						}
						printf("Press any key to continue...");
						garbage=getch();
						continue;
					}
				}
				if(kho == 0){
					i--;
					continue;
				}
			}	
		}
	}
	
	//travel block --> basically repeat dice roll tapi bebas mau brp aja (space 21)
	if(player[n].player_position == 21){
		printBoard();
		player[n].travel = 1;
		printf("You landed on travel block, you can go anywhere in your next turn.\n");
		printf("Press any key to continue...");
		garbage=getch();
	}
	
	//chance card --> 20% chance dapet angel card (space 8 sama 16)
	if(player[n].player_position == 8 || player[n].player_position == 16){
		printBoard();
		int chance = (rand() % 5);
		if(chance == 0){
			player[n].angel_card = 1;
			printf("Player %d (%c)'s Turn!\n", n+1, n+35);
			printf("Congratulations! You got an angel card! (^v^)/\n");
			printf("Press any key to continue...");
			garbage=getch();
		}else{
			printf("Player %d (%c)'s Turn!\n", n+1, n+35);
			printf("Unlucky!\n");
			printf("Press any key to continue...");
			garbage=getch();
		}
	}
	
	//tax --> kurangin 20% dari money+asset (space 26)
	if(player[n].player_position == 26){
		int taxPay = (player[n].money+player[n].asset)/5;
		while(true){
			printBoard();
			if(player[n].money < taxPay){
				printBoard();
				printf("Player %d (%c)'s Turn!\n", n+1, n+35);
				printf("%d K will be taken for tax!\n", taxPay);
				printf("You don't have enough money to pay tax!\n");
				printf("1. Sell Island\n");
				printf("2. Bankrupt\n");
				printf("Choose [ 1 | 2 ]:");
				char rip;
				for(int i=0; i<1; i++){
					rip=getch();
					switch(rip){
						case '1':
							printBoard();
							printf("Player %d (%c)'s Turn!\n", n+1, n+35);
							printf("Choose island that you want to sell:\n");
							for(int j=0;j<=27;j++){
								if(block[j].owner == n){
									printf("%2d. %s - %d K\n", j, block[j].block_name, block[j].island_price);
								}
							}
							printf(" 0. Back\n");
							printf(">> ");
							int number;
							scanf("%d", &number);
							if(number == 0){
								break;
							}
							kho = 0;
							for(int j=0;j<=27;j++){
								if(block[j].owner == n && number == j){
									kho = 1;
									printBoard();
									printf("Player %d (%c)'s Turn!\n", n+1, n+35);
									printf("You have sold %s for %d\n", block[j].block_name, block[j].island_price);
									block[j].owner = -1;
									player[n].money += block[j].island_price;
									printf("Press any key to continue...");
									garbage=getch();
									break;
								}
							}
							if(kho == 0){
								i--;
								break;
							}
							break;
						case '2':
							printBoard();
							printf("Player %d has gone bankrupt! The game is over!\n", n+1);
							printf("Press any key to continue...");
							gameFinish = 1;
							garbage=getch();
							break;
						default:
							i--;
							break;		
					}
				}
			}
			if(gameFinish == 1){
				break;
			}
			printf("Player %d (%c)'s Turn!\n", n+1, n+35);
			printf("%d K will be taken for tax!\n", taxPay);
			player[n].money -= taxPay;
			printf("Press any key to continue...");
			garbage=getch();
			break;
		}	
	}
	
	//free space --> do nothing (space 2)
	if(player[n].player_position == 2){
		printBoard();
		printf("Player %d (%c)'s Turn!\n", n+1, n+35);
		printf("Nothing happens\n");
		printf("Press any key to continue...");
		garbage=getch();
	}
	//end of turn
	actionPhase = 0;
}

//---------------------------
//high scores and how to play
//---------------------------
void highScores(){
	FILE *playerHighScore = fopen("winnerlist.txt", "r");
	highScoreCount=0;
	while(fscanf(playerHighScore, "%s %d\n", &highScore[highScoreCount].name, &highScore[highScoreCount].money) != EOF){
		highScoreCount++;
	}
	fclose(playerHighScore);
	
	selectionSort(highScore, highScoreCount);
	
	system("cls");
	printf("\n List Winner!\n");
	printf(" ============\n\n");
	for(int i=0;i<highScoreCount;i++){
		printf(" %02d. %-9s %d K\n", i+1, highScore[i].name, highScore[i].money);
	}
	printf("\n Press [B] to return to main menu\n");
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
	printf("\n About this game :\n\n");
	printf(" 1. 1000 K money will be given out to all player at the first round.\n");
	printf(" 2. Whenever you land on a block that no one owns, you can build island on that block.\n");
	printf("    Once you own the block, other players must pay rent if they landed on your land.\n");
	printf(" 3. If you land on a Chance Card, you will get the chance to get angel card~\n");
	printf(" 4. When you pass Go, you will receive 200 K money from the bank.\n");
	printf(" 5. Bankruptcy. You are automatically lose if you can't pay the rent.\n");
	printf(" 6. If one player go bankrupt, the game is done\n");
	printf(" 7. The winner will be determinated from the leaderboard.\n\n");
	printf(" Enjoy and have fun :)\n\n");
	printf(" Press [B] to return to main menu\n");
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

//utilities (sorting alg)
void selectionSort(struct playerHighScore highScore[], int highScoreCount){
	int temp;
	int swap;
	char cswap[100];
	for(int i=0;i<highScoreCount-1;i++){
		temp = i;
		for(int j=i+1;j<highScoreCount;j++){
			if(highScore[temp].money < highScore[j].money){
				temp = j;
			}
		}
		swap = highScore[i].money; highScore[i].money = highScore[temp].money; highScore[temp].money = swap;
		strcpy(cswap,highScore[i].name); strcpy(highScore[i].name,highScore[temp].name); strcpy(highScore[temp].name,cswap);
	}
}

void selectionSortLB(struct playerLB sortedPlayer[], int playerNumber){
	int temp;
	int swap;
	char cswap[100];
	for(int i=0;i<playerNumber-1;i++){
		temp = i;
		for(int j=i+1;j<playerNumber;j++){
			if((sortedPlayer[temp].money+sortedPlayer[temp].asset)< (sortedPlayer[j].money+sortedPlayer[j].asset)){
				temp = j;
			}
		}
		swap = sortedPlayer[i].money; sortedPlayer[i].money = sortedPlayer[temp].money; sortedPlayer[temp].money = swap;
		swap = sortedPlayer[i].asset; sortedPlayer[i].asset = sortedPlayer[temp].asset; sortedPlayer[temp].asset = swap;
		strcpy(cswap,sortedPlayer[i].name); strcpy(sortedPlayer[i].name,sortedPlayer[temp].name); strcpy(sortedPlayer[temp].name,cswap);
		swap = sortedPlayer[i].symbol; sortedPlayer[i].symbol = sortedPlayer[temp].symbol; sortedPlayer[temp].symbol = swap;
	}
}
