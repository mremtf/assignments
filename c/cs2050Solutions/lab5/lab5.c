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
	int number_of_outs;
	struct Player_ *next;
}Player;

int batting(Player *player); 
int pitch_result(void); 
Player* add_player(Player *first_player, const char *name);
Player* pull_player(Player *first_player);
void destroy_team(Player *first_player,Player* next); 
void display_lineup(Player* first, Player* next);
void readFile(Player** lineup, const char* file);
void update_player_stats(Player *lineup, Player *target); 
void display_player_stats(Player* first); 

int main (int argc, char *argv[]) {
	if (argc < 2){
		exit(1);	
	}
	srand(time(NULL));
	int outs = 0;
	int hits = 0;
	Player *lineup = NULL;
	readFile(&lineup,argv[1]);
	printf("\nLine up for this team is: ");
	display_lineup(lineup,lineup);
	
	for (; outs < 27; ) {
			
		Player * player = pull_player(lineup);
		int result = batting(player);
		if (result) {
			player->number_of_hits++;
			++hits;
		}
		else
			player->number_of_outs++;
			++outs;
		update_player_stats(lineup,player); //REQUIRED FOR BONUS
		free(player);
	}
	display_player_stats(lineup);
	printf("\nScore of game was: %d \n", hits/4);
	destroy_team(lineup,lineup);
	return 0;
}


/*BONUS PART*/
void update_player_stats(Player *lineup, Player *target) {
	Player* current = lineup;
	while (current->next != lineup) {
		if (strcmp(target->name,current->name) == 0) {
			current->number_of_outs = target->number_of_outs;
			current->number_of_hits = target->number_of_hits;
		}
		current = current->next;
	}

}

void display_player_stats(Player* first) {
	Player* curr = first;
	
	for (; curr->next != first; curr = curr->next) {
		printf("\n%s went %d/%d with a OBP of %.3f",curr->name, curr->number_of_hits,curr->number_of_outs+curr->number_of_hits,(float)curr->number_of_hits/(curr->number_of_outs+curr->number_of_hits));
	}
	printf("\n");

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

void display_lineup(Player* first,Player* next) {
		
	if(next->next != first) {
		printf("\n%s", next->name);
		display_lineup(first,next->next);
		
	}
	else {
		return;
		//printf("\n%s\n", next->name);
	}

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
		else if (result == 2) {
			return 1;
		}
		else if (result == 1 && walks == 3){
			printf("Walked!\n");
			return 1;
		}
		else if (result == 1) {
			++walks;
		}

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
		printf("Ball!\n");
		return 1;
	}
	printf("hit!\n");
	return 2;

}

// ENQUEUE PLAYER
Player* add_player(Player *first_player, const char *player_name){
	
	Player* new_node = malloc(sizeof(Player));
	strcpy(new_node->name,player_name);
	new_node->number_of_hits = 0;
	new_node->number_of_outs = 0;
	new_node->next = new_node;
	
	if (first_player == NULL) {
		return new_node;
	}
	new_node->next = first_player;
	Player *current = first_player;
	while (current->next != first_player){
		current = current->next;
	}
	current->next = new_node;
	return first_player;
} 
//front PLAYER
Player* pull_player(Player *first_player){
	Player* curr = first_player;
	static int on_deck = 0;
	int i = 0;
	for(;i < on_deck; ++i) {
		curr = curr->next;
	}
	Player* batting = malloc(sizeof(Player));	
	memcpy(batting,curr,sizeof(Player));
	on_deck++;
	return batting;
}
// FREE PLAYERS
void destroy_team(Player *first, Player* next) {	
	if(next->next == first) {
		free(next);
		return;
	}
	else {	
		destroy_team(first, next->next);
		free(next);
	}
}
