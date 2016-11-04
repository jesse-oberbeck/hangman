#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

enum{
    MAX_WORD_SIZE = 36
}; 

void clear_buffer(void)
{
    while(getchar() != '\n');
}

void read_stats(char *line1, char *line2, char *line3, char *line4)
{
    char linecounter;
    int lines = 0;
//////
    char filename[32];
    char *path = getenv("HOME");
    snprintf(filename, sizeof(filename), "%s/.hangman", path);

    FILE *stats = fopen(filename, "ab+");
    if(!stats){
        perror("Stats file failed to open.");
	exit(1);
    }
    while((linecounter = fgetc(stats)) != EOF){
        if(linecounter == '\n'){
            lines++;
        }
    }
    fseek(stats, 0, SEEK_SET);
    if(lines == 4){
        fgets(line1, sizeof(line1), stats);
        line1[strlen(line1) - 1] = '\0';
	printf("%s ", line1);

        fgets(line2, sizeof(line2), stats);
        line2[strlen(line2) - 1] = '\0';
	printf("%s ", line2);

        fgets(line3, sizeof(line3), stats);
        line3[strlen(line3) - 1] = '\0';
	printf("%s ", line3);

        fgets(line4, sizeof(line4), stats);
        line4[strlen(line4) - 1] = '\0';
	printf("%s\n", line4);

    }else{
        fputs("0 Games\n", stats);
        fputs("0 Wins\n", stats);
        fputs("0 Losses\n", stats);
        fputs("0 Average\n", stats);
    }
    fclose(stats);
}

void write_stats(int *winlose, char *line1, char *line2, char *line3, char *line4)
{
    char *line1w;
    char *line2w;
    char *line3w;
    char *line4w;
    char filename[32];
    char *path = getenv("HOME");

    snprintf(filename, sizeof(filename), "%s/.hangman", path);
    FILE *stats = fopen(filename, "ab+");

    if(!stats){
        perror("Stats file failed to open.");
	exit(1);
    }

    fseek(stats, 0, SEEK_SET);

    fgets(line1, sizeof(line1), stats);
    printf("LINE 1: %s\n", line1);
    line1[strlen(line1) - 1] = '\0';
    long int num1 = strtol(line1, &line1w, 10);
    num1++;
    fprintf(stats, "%li Games\n", num1);


    fgets(line2, sizeof(line2), stats);
    line2[strlen(line2) - 1] = '\0';
    long int num2 = strtol(line2, &line2w, 10);
    if(*winlose == 1){
        num2++;
        fprintf(stats, "%li Wins\n", num2);
    }

    fgets(line3, sizeof(line3), stats);
    line3[strlen(line3) - 1] = '\0';
    long int num3 = strtol(line3, &line3w, 10);

    fgets(line4, sizeof(line4), stats);
    line4[strlen(line4) - 1] = '\0';
    long int num4 = strtol(line4, &line4w, 10);

    //fputs("0 Games\n", stats);
    //fputs("0 Wins\n", stats);
    //fputs("0 Losses\n", stats);
    //fputs("0 Average\n", stats);

    fclose(stats);
}

void read_file(char *word_buf)
{
    srand(time(NULL) + clock());
    int lines = 0;
    int randomcompare = 0;
    char linecounter;
    char *path = getenv("HOME");
    char filename[32];

    //Concatenates user home to file name.
    snprintf(filename, sizeof(filename), "%s/.words", path);

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

int print_puzzle(char *word_buf, char *all_guesses, int word_len)
{
    int dash_count = 0;
    int printed = 0;
    int correct = 0;
    for(int i = 0; i < word_len; i++){
        for(int i2 = 0; all_guesses[i2] != '\0'; i2++){
            if(word_buf[i] == all_guesses[i2]){
                printf("%c", word_buf[i]);
                printed = 1;
                correct++;
                break;
            }
        }
        if(!printed){
            printf("-");
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
    printf("guesses: %s\n", all_guesses);
}

////////////////////////////////////////
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

//////////////////////////////////////////
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
    int winlose = 0;
    
    read_stats(line1, line2, line3, line4);

    if(argc == 2){
        strncpy(word_buf, argv[1], sizeof(word_buf));
    }else{
        if(access("~/.words", F_OK)){
            puts("File found.");
            read_file(word_buf);
        }
        
    }
    //printf("WB: %s\n", word_buf);
    int word_len = strlen(word_buf);
    
    while((wrongs < 6) && (winlose == 0)){
        winlose = print_puzzle(word_buf, all_guesses, word_len);
        //printf("winlose: %d\n",winlose);
        if(!winlose){
            collect_input(&user_guess, all_guesses);
        }
        count_wrong(word_buf, all_guesses, word_len, &wrongs);
    }
    //puts("You lose...");
    write_stats(&winlose, line1, line2, line3, line4);
}
