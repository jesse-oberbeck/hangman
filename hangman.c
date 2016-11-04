#include <stdio.h>
#include <string.h>





void clear_buffer(void)
{
    while(getchar() != '\n');
}

int print_puzzle(char *word_buf, char *all_guesses, int word_len)
{
    int printed = 0;
    printf("allguess: %s\n", all_guesses);
    for(int i = 0; i < word_len; i++){
        for(int i2 = 0; all_guesses[i2] != '\0'; i2++){
            if(word_buf[i] == all_guesses[i2]){
                printf("%c", word_buf[i]);
                printed = 1;
                break;
            }
        }
        if(!printed){
            printf("-");
            printed = 0;
        }
        printed = 0;
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
        if(strcmp(user_guess, &all_guesses[i]) == 0){
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
    int output = 0;
    printf("%d\n", word_len);
    while(1){
        output = print_puzzle(&word_buf, &all_guesses, word_len);
        if(output){
            wrong++;
        }
        collect_input(&user_guess, all_guesses);
        printf("guess in main: %c\n",user_guess);
    }
}
