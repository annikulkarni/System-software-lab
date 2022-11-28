#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char start[10];
    char csName[20][50];
    int csCount = 0;

    char dfrecords[100][20];
    int dfCount = 0;

    printf("Enter the starting address : ");
    scanf("%s", start);
    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("input.txt", "r+");
    fp2 = fopen("estab.txt", "w+");
    if (fp1 == NULL)
    {
        printf("\n Unable to open input.txt");
        return -1;
    }
    if (fp2 == NULL)
    {
        printf("\n Unable to open estab.txt");
        return -1;
    }
    char line[500];
    char cur_length[100];
    int curr_addr = (int)strtol(start, NULL, 16); 
    int addr = (int)strtol(start, NULL, 16); 
    int flag = 0;
    int  duplicate_csname = 0;
    while (fgets(line, sizeof(line), fp1))
    {
        if(duplicate_csname==1)
        {
            if(line[0]=='E')
            {
                duplicate_csname = 0;
            }
            else{
                continue;
            }
        }

        line[strcspn(line, "\n")] = 0;
        if(strcmp(line, "END") == 0) {
            break;
        }
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
            for(int k=0;k<csCount;k++) {
                if(strcmp(tokens[1], csName[k]) == 0) {
                    printf("Duplicate CSNAME : %s\n", tokens[1]);
                    duplicate_csname = 1;
                }
            }
            if(duplicate_csname == 1)
            {
                continue;
            }

            if(flag == 1)
            {
                curr_addr += (int)strtol(cur_length, NULL, 16);
                flag = 0;
            }
            fprintf(fp2, "%-10s %-10s %-10X %-10s\n", tokens[1], " ", curr_addr, tokens[3]);
            printf("%-10s %-10s %-10X %-10s\n", tokens[1], " ", curr_addr, tokens[3]);
            strcpy(csName[csCount++], tokens[1]);
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
                int flag2 = 0;
                for(int k=0; k<dfCount; k++) {
                    if(strcmp(tokens[i], dfrecords[k]) == 0) {
                        flag2 = 1;
                        printf("\n==> Duplicate define variable : %s\n\n", tokens[i]);
                        break;
                    }
                }
                if(flag2 == 0) {
                    strcpy(dfrecords[dfCount++], tokens[i]);

                    fprintf(fp2,"%-10s %-10s %-10X %-10s\n"," ", tokens[i], curr_addr + strtol(tokens[i + 1], NULL, 16)," ");
                    printf("%-10s %-10s %-10X %-10s\n"," ", tokens[i], curr_addr + strtol(tokens[i + 1], NULL, 16)," ");
                    flag = 1;
                }   
            }
        }
    }
    fclose(fp1);
    return 0;
}
