/* This program has a buffer overflow vulnerability. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int vulnerable_function(char *str){
    char buffer[100];

    /* The following statement has a buffer overflow problem */
    strcpy(buffer, str);

    /* The following statement returns the address of str*/
    printf("The base address of str is 0x%x \n", (unsigned int) &str);
    
    return 1;
}

int main(int argc, char **argv){
    char str[400];
    FILE *malicious_file;

    malicious_file= fopen("malicious_file", "r");
    fread(str, sizeof(char), 400, malicious_file);
    vulnerable_function(str);

    printf("Returned Properly\n");
    return 1;
}
