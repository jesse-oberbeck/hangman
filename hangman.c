#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

/*Defines size which allows for
supercalifragilisticexpialidocious*/
enum{
    MAX_WORD_SIZE = 36
}; 


/*Functions prototyped so main can be at the top 
the way Liam wants it.*/

void clear_buffer(void);

void print_man(int *wrongs);

void read_stats(char *line1, char *line2, char *line3, char *line4, char *line5, char *line6, char *line7);

void write_stats(int *winlose, char *line1, char *line2, char *line3, int *time_played, char *line6);

void read_file(char *word_buf, char *arg);

int print_puzzle(char *word_buf, char *all_guesses, int word_len);

void count_wrong(char *word_buf, char *all_guesses, int word_len, int *wrongs);

void collect_input(char *user_guess, char *all_guesses);




int main(int argc, char *argv[])
{
    int wrongs = 0;
    char word_buf[128];
    char user_guess;
    char all_guesses[64] = {'\0'};
    char line1[64];
    char line2[64];
    char line3[64];
    char line4[64];
    char line5[64];
    char line6[64];
    char line7[64];
    int winlose = 0;
    int start_time = time(NULL);

    //This is just here to get rid of argc unused warning.
    if(argc == 0){
        argc = 0;
    }
    
    read_stats(line1, line2, line3, line4, line5, line6, line7);

    if(access("~/.words", F_OK)){
        puts("File found.");
        read_file(word_buf, argv[1]);
    }
        
    //}
    int word_len = strlen(word_buf);
    
    while((wrongs < 6) && (winlose == 0)){
        winlose = print_puzzle(word_buf, all_guesses, word_len);
        if(!winlose){
            collect_input(&user_guess, all_guesses);
        }
        count_wrong(word_buf, all_guesses, word_len, &wrongs);
        print_man(&wrongs);
    }
    int time_played = time(NULL) - start_time;
    printf("\nTime Played: %d seconds.", time_played);
    write_stats(&winlose, line1, line2, line3, &time_played, line6);
}


/*Clears the buffer, just like it says.*/
void clear_buffer(void)
{
    while(getchar() != '\n');
}


/*Builds a stick man based on number of wrong
answers.*/
void print_man(int *wrongs)
{
    const char *head = "  O\n";
    const char *body = "  |  \n";
    const char *onearm = " /|  \n";
    const char *twoarms = " /|\\ \n";
    const char *oneleg = " /\n";
    const char *twolegs = " / \\\n";
    switch(*wrongs){
        case 1:
            printf("%s", head);
            break;
        case 2:
            printf("%s%s", head, body);
            break;
        case 3:
            printf("%s%s", head, onearm);
            break;
        case 4:
            printf("%s%s", head, twoarms);
            break;
        case 5:
            printf("%s%s%s", head, twoarms, oneleg);
            break;
        case 6:
            printf("%s%s%s", head, twoarms, twolegs);
    }
}


/*Reads statistics in from file, or if file is not found,
makes the file and fills it with base values.*/
void read_stats(char *line1, char *line2, char *line3, char *line4, char *line5, char *line6, char *line7)
{
    char linecounter;
    int lines = 0;
    char filename[32];
    char *path = getenv("HOME");
    snprintf(filename, sizeof(filename), "%s/.hangman", path);

    FILE *stats = fopen(filename, "a+");
    if(!stats){
        perror("Stats file failed to open.");
	exit(1);
    }
    while((linecounter = fgetc(stats)) != EOF){
        if(linecounter == '\n'){
            lines++;
        }
    }

    /*Prints values read in at the beginning
    of program's run.*/
    fseek(stats, 0, SEEK_SET);
    if(lines > 2){
        fgets(line1, 32, stats);
        line1[strlen(line1) - 1] = '\0';
	printf("%s ", line1);

        fgets(line2, 32, stats);
        line2[strlen(line2) - 1] = '\0';
	printf("%s ", line2);

        fgets(line3, 32, stats);
        line3[strlen(line3) - 1] = '\0';
	printf("%s ", line3);

        fgets(line4, 32, stats);
        line4[strlen(line4) - 1] = '\0';
	printf("%s\n", line4);

        fgets(line5, 32, stats);
        line5[strlen(line5) - 1] = '\0';
	printf("%s\n", line5);

        fgets(line6, 32, stats);
        line6[strlen(line6) - 1] = '\0';
	printf("%s\n", line6);

        fgets(line7, 32, stats);
        line6[strlen(line7) - 1] = '\0';
	printf("%s\n", line7);

    /*Fills the stats file with default values
    if it was just made.*/
    }else{
        fputs("0 Games\n", stats);
        fputs("0 Wins\n", stats);
        fputs("0 Losses\n", stats);
        fputs("0 Average\n", stats);
        fputs("0 seconds played previously.\n", stats);
        fputs("0 seconds total.", stats);
        fputs("0 seconds average.", stats);
    }
    fclose(stats);
}


/*Writes adjusted statistical values at the end
of program's run.*/
void write_stats(int *winlose, char *line1, char *line2, char *line3, int *time_played, char *line6)
{
    char filename[32];
    char *path = getenv("HOME");

    //Builds "relative" path and reads file.
    snprintf(filename, sizeof(filename), "%s/.hangman", path);
    FILE *stats = fopen(filename, "w");

    if(!stats){
        perror("Stats file failed to open.");
	exit(1);
    }

    fseek(stats, 0, SEEK_SET);

    //Adjust and print values to screen and file.
    long int num1 = strtol(line1, NULL, 10);
    num1++;
    fprintf(stats, "%li Games\n", num1);
    printf("\nTotal games: %li\n", num1);

    long int num2 = strtol(line2, NULL, 10);
    if(*winlose == 1){
        num2++;
    }
    fprintf(stats, "%li Wins\n", num2);
    printf("Number of wins: %li\n", num2);

    long int num3 = strtol(line3, NULL, 10);
    if(*winlose == 0){
        num3++;
    }
    printf("Number of losses: %li\n", num3);
    fprintf(stats, "%li Loss\n", num3);

    double num4 = (num2 - num3) / (double)num1;
    fprintf(stats, "%f Average score\n", num4);
    printf("Average score: %f\n", num4);

    fprintf(stats, "%d Seconds played previously.\n", *time_played);
    printf("Seconds played this game: %d\n", *time_played);

    double num5 = strtol(line6, NULL, 10);
    double total_time = (double)num5 + (double)(*time_played);
    fprintf(stats, "%f Total time played\n", total_time);
    printf("Total time played: %f\n", total_time);

    double avg_time = (double)num5 / (double)num1;
    fprintf(stats, "%f Average time played\n", avg_time);
    printf("Average time played: %f\n", avg_time);


    fclose(stats);
}


/*Reads in answer list, setting the 
goal word to a random word from the file.*/
void read_file(char *word_buf, char *arg)
{
    srand(time(NULL) + clock());
    int lines = 0;
    int randomcompare = 0;
    char linecounter;
    char *path = getenv("HOME");
    char filename[32];

    //Concatenates user home to file name. If present, uses arg as filename.
    if(arg != NULL){
        snprintf(filename, sizeof(filename), "%s/%s", path, arg);
    }else{
        snprintf(filename, sizeof(filename), "%s/.words", path);
    }

    FILE *word_list = fopen(filename, "r");
    if(!word_list){
        perror("File failed to open.");
        return;
    }

    /*Counts number of lines via newline character.
    init value is 1 to account for last line.*/
    while((linecounter = fgetc(word_list)) != EOF){
        if(linecounter == '\n'){
            lines++;
        }
    }

    int answer_num = (rand() % lines) + 1;

    //Returns to top of file after line count.
    fseek(word_list, 0, SEEK_SET);
    while(answer_num != randomcompare){
        fgets(word_buf, MAX_WORD_SIZE, word_list);
        randomcompare++;
    }
    
    word_buf[strlen(word_buf) - 1] = '\0';
    fclose(word_list);
}


/*On each turn, prints correctly guessed letters, and
lines where the letter has not yet been guessed.*/
int print_puzzle(char *word_buf, char *all_guesses, int word_len)
{
    int dash_count = 0;
    int printed = 0;
    int correct = 0;

    //On first run, prints lines and punctuation.
    if(all_guesses[0] == '\0'){
        for(int i = 0; i < word_len; i++){
            if(isalpha(word_buf[i]) == 0){
                printf("%c", word_buf[i]);
            }else{
                printf("_");
            }
        }
    return(0);
    }

    //Handle letters.
    for(int i = 0; i < word_len; i++){
        for(int i2 = 0; all_guesses[i2] != '\0'; i2++){
            if((word_buf[i] == all_guesses[i2]) || (isalpha(word_buf[i]) == 0)){
                printf("%c", word_buf[i]);
                printed = 1;
                correct++;
                break;
            }
        }
    //Handle lines/dashes.
        if(!printed){
            printf("_");
            dash_count++;
            printed = 0;
        }
        printed = 0;
    }
    if(dash_count == 0){
        puts("\nYOU WIN!");
        return(1);
    }else{
        return(0);
    }
}


/*Assesses the number of incorrect guesses*/
void count_wrong(char *word_buf, char *all_guesses, int word_len, int *wrongs)
{
    int wrong = 0;
    int flag = 0;
    for(int i = 0; all_guesses[i] != '\0'; i++){
        for(int i2 = 0; i2 < word_len; i2++){
            if(word_buf[i2] == all_guesses[i]){
                flag = 1;
            }
        }
        if(flag != 1){
            wrong++;
        }
        flag = 0;
        
    }
    int correct_guesses = strlen(all_guesses) - wrong;
    *wrongs = strlen(all_guesses) - correct_guesses;
    printf("\nguesses: %s\n", all_guesses);
}


/*Takes guess from player. Takes ONLY the
first character from entry.*/
void collect_input(char *user_guess, char *all_guesses)
{
    puts("\n\nGuess a letter.");
    *user_guess = getchar();
    clear_buffer();
    int i = 0;
    while(all_guesses[i] != '\0'){
        if(*user_guess == all_guesses[i]){
            return;
        }
        i++;
    }
    all_guesses[i] = *user_guess;
    return;
}
