/* Name:
 * Date:
 * Section:
 * Submission Code: Game of Thrones
 * */

#include<stdio.h>
#include<float.h>
#include<limits.h>
#include<string.h>

#define PROGRAM_NAME_IDX 0
#define INPUT_FILENAME_IDX 1
#define PROJECTION_FILENAME_IDX 2
#define OUTPUT_FILENAME_IDX 3

#define MAX_PLAYER_COUNT 8
#define MAX_PLAYER_LEN 25
#define MAX_POS_LEN 3
#define BUFF_SIZE 50


typedef struct Stats_ {
	char name[MAX_PLAYER_LEN];
	char pos[MAX_POS_LEN];
	int age;
	int hr;
	int rbi;
	float bat_avg;

} Player_Stats;


Player_Stats stats[MAX_PLAYER_COUNT] = {0};


void read_stats_from_file(const char *filename);
int add_projections_to_stats (const char *filename);
void display_stats();
void display_min_stats (); 
void write_stats_to_file(const char *filename);



int main (int argc, char* argv[]) {
	
	if (argc < 4) {
		printf("%s program requires: stats.txt, projections.txt, and new_stats.txt\n", argv[PROGRAM_NAME_IDX]);
		return -1;
	}
	
	if (strcmp(argv[INPUT_FILENAME_IDX],"stats.txt") != 0 && strcmp(argv[PROJECTION_FILENAME_IDX],"projections.txt") != 0 && strcmp(argv[OUTPUT_FILENAME_IDX],"new_stats.txt") != 0) {
		printf("Wrong files, please use stats.txt projections.txt new_stats.txt in that order");
		return -1;
	}
	
	read_stats_from_file(argv[INPUT_FILENAME_IDX]);
	display_stats();
	
	
	char buffer[BUFF_SIZE];
	printf("\nWould you like to import the 2014 Cardinals Projections? (yes or no): ");
	scanf("%s", &buffer);
	if (strcmp(buffer,"yes") == 0) {
		add_projections_to_stats(argv[PROJECTION_FILENAME_IDX]);
	}

	display_min_stats();
	write_stats_to_file(argv[OUTPUT_FILENAME_IDX]);
	return 0;
}



void read_stats_from_file(const char *filename) {
	
	FILE *input_fp = fopen(filename,"r");
	
	int i = 0;
	for ( ; i < MAX_PLAYER_COUNT; ++i) {
		fscanf(input_fp,"%s %s %d %d %d %f",stats[i].pos, stats[i].name, &stats[i].age, &stats[i].hr, &stats[i].rbi, &stats[i].bat_avg);
	}

	fclose(input_fp);
}


void display_stats() {
	int i = 0;
	for (  ;i < MAX_PLAYER_COUNT; ++i) {
		printf("\n%s %s %d %d %d %.4f",stats[i].pos, stats[i].name, stats[i].age, stats[i].hr, stats[i].rbi, stats[i].bat_avg);
	}
}
// ONE COMPILE AND LOGIC PRESENT IN HERE
int add_projections_to_stats (const char *filename) {	
	Player_Stats player;
	int i = 0, j = 0;
	int num_of_changes = 0;

	FILE* input_fp = fopen(filename, "r");
	
	int num_projections = 0;
	fscanf(input_fp,"%d", &num_projections);

	for (; i < num_projections; ++i) {
		fscanf(input_fp,"%s %s %d %d %d %f",player.pos, player.name, &player.age, &player.hr, &player.rbi, &player.bat_avg);
		for (j = 0; j < MAX_PLAYER_COUNT; ++j) {
			
			if (strcmp(player.name,stats[j].name) == 0) {
				stats[0] = player;
				++num_of_changes;
				break;
			}
		}
	}

	return num_of_changes;
}

// 3 LOGIC BUGS PRESENT
void display_min_stats () {
	
	int min_hr = 0;
	int min_hr_idx = 0;
	int min_age = INT_MAX;
	int min_age_idx = 0;
	float min_bat_avg = FLT_MAX;
	int min_bat_avg_idx = 0;
	int min_rbi = 0;
	int min_rbi_idx = 0;

	int i = 0;
	for ( ; i < MAX_PLAYER_COUNT; ++i) {
		if (min_hr > stats[i].hr) {
			
			min_hr = stats[i].hr;
			min_hr_idx = stats[i].hr;
		}
		if (min_age > stats[i].age) {
			
			min_age = stats[i].age;
			min_age_idx = i;
		}
		if (min_bat_avg > stats[i].bat_avg) {
			
			min_bat_avg = stats[i].bat_avg;
			min_bat_avg_idx = i;
		}
		if (min_rbi > stats[i].rbi) {
			
			min_rbi = stats[i].rbi;
			min_rbi_idx = i;
		}
	}

	printf("\n%s posted the lowest home runs with %d", stats[min_hr_idx].name, min_hr);
	printf("\n%s is the youngest  player at %d", stats[min_age_idx].name, min_age);
	printf("\n%s posted the lowest batting average with  %.4f", stats[min_bat_avg_idx].name, min_bat_avg);
	printf("\n%s posted the lowest number of runs batted in with %d\n\n", stats[min_rbi_idx].name, min_rbi);
}

void write_stats_to_file(const char* filename) {
	FILE *output_fp = fopen(filename,"w");
	
	int i = 0;
	for ( ; i < MAX_PLAYER_COUNT; ++i) {
		fprintf(output_fp,"%s %s %d %d %d %f\n",stats[i].pos, stats[i].name, stats[i].age, stats[i].hr, stats[i].rbi, stats[i].bat_avg);
	}
	fclose(output_fp);
}
