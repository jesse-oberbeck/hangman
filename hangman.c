#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clear_buffer(void)
{
    while(getchar() != '\n');
}

void print_puzzle(char *word_buf, char *all_guesses, int word_len, int *prev_correct, int *wrong)
{
    int printed = 0;
    int correct = 0;
    printf("allguess: %s\n", all_guesses);
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
            printed = 0;
        }
        printed = 0;
    }
    /*
     if(*prev_correct == correct){
        *wrong += 1;
    }
    */
    //printf("correct: %d\n", correct);
    int wrongs = (strlen(all_guesses) - correct);
    if(wrongs == 6){
        puts("you lose...");
        exit();
    }else{
        printf("Wrong: %d\n", wrongs);
    }
    puts("");
    return;
}

////////////////////////////////////////
void collect_input(char *user_guess, char *all_guesses)
{
    puts("Guess a letter.");
    *user_guess = getchar();
    clear_buffer();
    int i = 0;
    while(all_guesses[i] != '\0'){
        puts("comparing");
        if(*user_guess == all_guesses[i]){
            return;
        }
        i++;
    }
    all_guesses[i] = *user_guess;
    //printf("In collect guess: %s\n", user_guess);
    return;
}

//////////////////////////////////////////
int main(int argc, char *argv[])
{
    char word_buf[128] = {"testing"};
    int word_len = strlen(word_buf);
    char user_guess;
    char all_guesses[32] = {'\0'};
    int wrong = 0;
    int correct = 0;
    printf("%d\n", word_len);
    while(1){
        print_puzzle(&word_buf, &all_guesses, word_len, &correct, &wrong);

        collect_input(&user_guess, all_guesses);
        printf("guess in main: %c\n",user_guess);
        //printf("wrong: %d\n", wrong);
    }
}
