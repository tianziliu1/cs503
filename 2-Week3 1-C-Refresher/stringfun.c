#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *);
void word_print(char *, int, int);
void replace_word(char *, int, char *, char *);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int user_str_len = strlen(user_str);

    if(user_str_len >= len){
        strncpy(buff, user_str, len - 1);
        buff[len - 1] = '\0';
        return len - 1;
    }
    else{
        strcpy(buff, user_str);
        return user_str_len;
    }

    //return 0; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int wc = 0;
    bool word_start = false;

    for(int i = 0; i < str_len; i++){
        char current_char = buff[i];

        if(!word_start){
            if(current_char != ' '){
                wc++;
                word_start = true;
            }
        }
        else{
            if(current_char == ' '){
                word_start = false;
            }
        }
    }
    return wc;
}

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len/2; i++){
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int word_print(char *buff, int buff_len, int str_len){
    if (str_len > buff_len)
        return -1;
    if (str_len == 0)
        return 0;

    int word_count = 0;
    int char_ctr = 0;
    bool at_start = true;

    for (int i = 0; i < str_len; i++){
        char c = buff[i];

        if(at_start == true){
            word_count++;
            at_start = false;
        }
        if (c == ' '){
            printf("%d ", char_ctr);
            char_ctr = 0;
            at_start = true;
        }
        else{
            putchar(c);
            char_ctr++;
        }
    }
    printf("%d\n", char_ctr);
    return word_count;
}

// To replce the first occurrence of a word in the string
int replace_word(char *buff, int len, char *str, char *newstr, int newstr_len){
    char *pos = strstr(buff, str);
    if(pos ==NULL){
        return 0;
    }
    int idx = pos - buff;
    int str_len = strlen(str);
    int newstr = strlen(newstr);
    int newstr_len = strlen(buff) - str_len + newstr_len;

    if(newstr_len > buff_len){
        printf("Buffer overflow error.\n");
        return -1;
    }
    else{
        char new_buff[buff_len];
        strncpy(new_buff, buff, idx);
        strcpy(new_buff + idx, newstr);
        strcpy(new_buff + idx + newstr_len, buff + idx + str_len);
        strncpy(buff, new_buff, buff_len);
        return 0;
    }
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // argc is the number of command line arguments. argv is an array of strings
    // representing the command line arguments passed to the program.
    // The if statement below provided two arguments. if argc is less than 2
    // or the first command line argument does not start with a hyphen, the usage message
    // is displayed and the program exists with an error.

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // The purpose of the if statement below is to check if the user
    // provided fewer than three arguments. If so, the usage message is 
    // displayed and the program exits with an error.

    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*) malloc(BUFFER_SZ *sizeof(char));
    if(buff == NULL){
        printf("Error: Failed to allocate memory for buffer\n");
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff);
            printf("Reversed string: %s\n", buff);
            break;
        case 'w':
            printf("Word Print\n----------\n");
            word_print(buff);
            break;
        case 'x':
            printf("Word Replace");
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          The reason for providing both the pointer and the length even
//          though the buff variable is set for 50 bytes is to prevent buffer
//          overflow vulnerabilities. It's a good practice to maintain bounds. 