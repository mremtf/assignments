#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PLAYERS 9
#define POS_SIZE 3
#define MAX_STR_LEN 25

typedef struct Player_{
	char name[MAX_STR_LEN];
	int number_of_hits;
	int number_of_strikes;
	struct Player_ *next;
}Player;

int batting(Player *player); 
int pitch_result(void); 
Player* add_player(Player *first_player, const char *name);
Player* pull_player(Player *first_player);
void destroy_team(Player *first_player, Player* next); 
void display_line_up(Player* first);
void display_players_stats(Player* first);
void readFile(Player** lineup, const char* file);

int main (int argc, char *argv[]) {
	if (argc < 2){
		exit(1);	
	}
	srand(time(NULL));
	int strikes = 0;
	int hits = 0;
	Player *lineup = NULL;
	readFile(&lineup,argv[1]);
	printf("\nLine up for this team is: ");
	display_line_up(lineup);


	printf("\n\n%s is batting\n", lineup->name);

	for (; strikes < 3; ) {
				
		int result = pitch_result();
		if (result) {
			++hits;
		}
		else
			++strikes;
	}
	destroy_team(lineup,lineup);
	printf("\nScore of game was: %d \n", hits/4);
	return 0;
}

void readFile(Player** lineup, const char* file) {
		FILE* fp = fopen(file,"r");
		char buffer[MAX_STR_LEN] = {0};
		int i = 0;
		for ( ;i < MAX_PLAYERS; ++i) {
			fscanf(fp,"%s", buffer);
			*lineup = add_player(*lineup,buffer);
		}
		fclose(fp);
		
}

void display_line_up(Player* first) {
	Player* curr = first;
	for (; curr->next != first; curr = curr->next) {
		printf("\n%s",curr->name);
	}
	printf("\n%s", curr->name);
}


int batting(Player *player) {
	
	printf("\n%s is now batting\n",player->name);
	int strikes = 0;
	int result = 0;
	int walks = 0;
	for ( ;strikes < 3;) {
		result = pitch_result();
		if (result == 0)
			++strikes;
		else if (result == 1) {
			++walks;
		}
		else if (walks == 4 || result == 2)
			return 1;
	}
	return 0;
}

int pitch_result(void) {
	int result = rand() % 4;
	if (result >= 2){
		printf("Strike!\n");
		return 0;
	}
	else if (result >= 1) {
		printf("hit!\n");
		return 1;
	}
	printf("hit!\n");
	return 2;

}

// ENQUEUE PLAYER
Player* add_player(Player *first_player, const char *player_name){
	

	
	if (first_player == NULL) {
	Player* new_node = malloc(sizeof(Player));
	strcpy(new_node->name,player_name);
	new_node->next = new_node;
		return new_node;
	}
	Player* new_node = malloc(sizeof(Player));
	strcpy(new_node->name,player_name);
	
	new_node->next = first_player;
	Player *current = first_player;
	while (current->next != first_player){
		current = current->next;
	}
	current->next = new_node;
	return first_player;
} 
// FREE PLAYERS
void destroy_team(Player *first, Player *next) {
   Player* current = first->next;
	Player* destroy;
	while(current != first) {
		destroy = current;
		current = current->next;
		free(destroy);
	}
	free(first);
}
