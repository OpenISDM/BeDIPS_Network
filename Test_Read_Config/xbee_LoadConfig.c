#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch, file_name[25];
    FILE *fp;

    fp = fopen("Config", "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    printf("The contents of %s file are:\n", file_name);

    while((ch = fgetc(fp)) != EOF){
        if(ch == '\n'){
            printf("N\n");
        }
        else
            printf("%c", ch);
    }
   fclose(fp);
   return 0;
}
