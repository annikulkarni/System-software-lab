#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char start[10];
    printf("Enter the starting address :");
    scanf("%s", start);
    FILE *fp;
    FILE *fp2;
    fp = fopen("input.txt", "r+");
    fp2 = fopen("output.txt", "w+");
    if (!fp2)
    {
        printf("\n Unable to open ");
        return -1;
    }
    if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }
    char line[500];
    char cur_length[100];
    int curr_addr = (int)strtol(start, NULL, 16); 
    printf("%X\n",curr_addr);
    int addr = (int)strtol(start, NULL, 16); 
    int flag = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char tokens[4][20];
        if (line[0] == 'H')
        {
            char *token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                strcpy(tokens[j], token);
                token = strtok(NULL, " ");
                j++;
            }
            if(flag == 1)
              {
                printf("before : %X\n",curr_addr);
                curr_addr += (int)strtol(cur_length, NULL, 16);
                printf("after : %X\n",curr_addr);
                flag = 0;
              }
            fprintf(fp2, "%-10s %-10s %-10X %-10s\n", tokens[1], " ", curr_addr, tokens[3]);
            strcpy(cur_length,tokens[3]);
              
                
            
        }
        else if (line[0] == 'D')
        {
            char tokens[20][20];
            char *token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                strcpy(tokens[j], token);
                token = strtok(NULL, " ");
                j++;
            }
            for (int i = 1; i < j; i+=2)
            {
                fprintf(fp2,"%-10s %-10s %-10X %-10s\n"," ", tokens[i], curr_addr + strtol(tokens[i + 1], NULL, 16)," ");
                flag = 1;
            }
            fprintf(fp2,"\n");
          
        }
    }
    fclose(fp);
    return 0;
}
