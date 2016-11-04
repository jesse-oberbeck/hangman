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
    printf("wordbuf post file: %s\n", word_buf);
    fclose(word_list);
}

void print_puzzle(char *word_buf, char *all_guesses, int word_len)
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
        exit(0);
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
    
    if(argc == 2){
        strncpy(word_buf, argv[1], sizeof(word_buf));
    }else{
        if(access("~/.words", F_OK)){
            puts("File found.");
            read_file(word_buf);
        }
        
    }
    printf("WB: %s\n", word_buf);
    int word_len = strlen(word_buf);
    
    while(wrongs < 6){
        print_puzzle(word_buf, all_guesses, word_len);
        collect_input(&user_guess, all_guesses);
        count_wrong(word_buf, all_guesses, word_len, &wrongs);
    }
    puts("You lose...");
}
