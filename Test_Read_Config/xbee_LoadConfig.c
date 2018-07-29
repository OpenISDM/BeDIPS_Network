#include <stdio.h>
#include <stdlib.h>

// API FRAME
// AP ARG

int main(){
    char ch, AT_Command[30];
    FILE *cfg;

    cfg = fopen(" xbee_Beacon.conf", "r"); // read mode

    if (cfg == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    //printf("The contents of %s file are:\n", file_name);

    while((ch = fgetc(cfg)) != EOF){
        if(ch == '\n'){
            printf("\\r\n");
        }
        else{
            printf("%c", ch);
        }
    }

    fclose(cfg);
    return 0;
}
