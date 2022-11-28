#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE 100
typedef struct mod_rec
{
    char label[10];
    char addr[10];
} MOD_REC;

MOD_REC mod_records[MAXSIZE];
int m_count = 0;
char program_addr[10];
int read_estab()
{
    FILE *fp;
    fp = fopen("estab.txt", "r+");
    if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }
    char line[500];

    char first_line[500];

    fgets(first_line, 500, fp);
    first_line[strcspn(first_line, "\n")] = 0;

    char tokens_f[3][20];

    char *token_f = strtok(first_line, " ");

    int j = 0;
    while (token_f != NULL)
    {
        strcpy(tokens_f[j], token_f);
        token_f = strtok(NULL, " ");
        j++;
    }
    strcpy(program_addr, tokens_f[1]);
    strcpy(mod_records[m_count].label, tokens_f[0]);
    strcpy(mod_records[m_count].addr, tokens_f[1]);
    m_count++;
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;
        char tokens[3][20];
        strcpy(tokens[0], "");
        strcpy(tokens[1], "");
        strcpy(tokens[2], "");

        char *token = strtok(line, " ");
        int j = 0;
        while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }
        tokens[1][strcspn(tokens[1], "\n")] = 0;
        tokens[2][strcspn(tokens[2], "\n")] = 0;
       
            strcpy(mod_records[m_count].label, tokens[0]);
            strcpy(mod_records[m_count].addr, tokens[1]);
            m_count++;
        
    }
    fclose(fp);
}
char * search_estab(char *s)
{
    for(int i=0;i<m_count;i++)
    {
        if(strcmp(mod_records[i].label,s)==0)
        {
            return mod_records[i].addr;
        }
    }
    return 'undefined';
}
void print_estab()
{
     for(int i=0;i<m_count;i++)
    {
        printf("es : %s %s\n",mod_records[i].label,mod_records[i].addr);
    }
}
int main()
{
    char csaddr[10];
    char execaddr[10];
    char cslth[10];
    read_estab();
    
    strcpy(csaddr, program_addr);
    strcpy(execaddr, program_addr);
    

    FILE *fp;
    FILE *fp2;

    fp = fopen("input.txt", "r+");
    fp2 = fopen("memory.txt", "w+");

    if (!fp)
    {
        printf("Unable to open file\n");
        return -1;
    }
    char line[500];
   
    int start = strtol(program_addr, NULL, 16);
    int progad = strtol(program_addr, NULL, 16);

    for (int i = 0; i < 25; i++)
    {
        fprintf(fp2, "%-10X********  ********  ********  ********\n", start); // 50 spaces
        start = start + 16;
    }
    fseek(fp2, 0, SEEK_SET);
    int csad = strtol(csaddr, NULL, 16);
    int cslt;
    
    while (fgets(line, sizeof(line), fp))
    {
        if(line[0]=='E')
        {
            csad = csad + cslt;
           
        }
        if(line[0]=='H')
        {
            char tokens[4][20];

            char *token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                strcpy(tokens[j], token);
                token = strtok(NULL, " ");
                j++;
            }
            strcpy(cslth,tokens[3]);
            cslt = strtol(cslth, NULL, 16);
        }
       
        if (line[0] == 'T')
        {
            line[strcspn(line, "\n")] = 0;
            
            char tokens[7][20];

            char *token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                strcpy(tokens[j], token);
                token = strtok(NULL, " ");
                j++;
            }

            char record[100];
            memset(record, 0, sizeof(record));
            for (int i = 2; i < 7; i++)
            {
                strcat(record, tokens[i]);
            }
            
           
            int temp_addr;
            temp_addr = csad + strtol(tokens[1], NULL, 16);
           
            int line_finder = temp_addr - progad;
            
            int line = line_finder / 16;
            int offset = line_finder % 16;
           
            int temp_off = offset / 4;
            
            
           
            fseek(fp2, line*50 + (offset*2)+temp_off*2 + 10 , SEEK_SET);
            
            int count = 0;

            for (int i = 0; i < strlen(record); i ++)
            {

                    if (count >= 30)
                    {
                        break;
                    }
                    if(count!=0 && (offset*2 + count)%8==0)
                    {
                        fseek(fp2, ftell(fp2) + 2, SEEK_SET);
                    }
                    
                    if(ftell(fp2)%50==0)
                    {
                        fseek(fp2,ftell(fp2) + 10, SEEK_SET);
                       
                    }
                    fprintf(fp2, "%c", record[i]);
                    count++;
                
            }

        }
        if(line[0]=='M')
        {
           line[strcspn(line, "\n")] = 0;
            
            char tokens[4][20];

            char * token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                strcpy(tokens[j], token);
                token = strtok(NULL, " ");
                j++;
            }
            char * temp_label = tokens[3] + 1;
            char m_addr[10];
            char prev_val_str[10];
            int prev_val;
            memset(prev_val_str, 0, sizeof(prev_val_str));
            memset(m_addr, 0, sizeof(m_addr));
            strcpy(m_addr , search_estab(temp_label));
            int temp_addr;
            temp_addr = csad + strtol(tokens[1], NULL, 16);
            int line_finder = temp_addr - progad;
            int adder  = strtol(m_addr, NULL, 16);
            
            int line = line_finder / 16;
            int offset = line_finder % 16;
            int temp_off = offset / 4;

            fseek(fp2, line*50 + (offset*2)+temp_off*2 + 10 , SEEK_SET);
            int count = 0;
            
            for(int i=0;i<atoi(tokens[2]);i++)
            {
                if(count!=0 && (offset*2 + count)%8==0)
                {
                    fseek(fp2, ftell(fp2) + 2, SEEK_SET);
                }
                if(ftell(fp2)%50==0)
                {
                    fseek(fp2,ftell(fp2) + 10, SEEK_SET);
                }
                char c = fgetc(fp2);
                strncat(prev_val_str,&c,1);
                count++;
            }
            prev_val = strtol(prev_val_str, NULL, 16);
            printf("prev value = %X at line %d , offset : %d\n",prev_val,line,offset);
            int new_val;
            if(tokens[3][0]=='+')
            {
               new_val = prev_val + adder;
            }
            if(tokens[3][0]=='-')
            {
               new_val = prev_val - adder;
            }
            char new_val_str[10];
            memset(new_val_str, 0, sizeof(new_val_str));

            sprintf(new_val_str, "%X", new_val);
            count = 0;
            fseek(fp2, line*50 + (offset*2)+temp_off*2 + 10 , SEEK_SET);
            for (int i = 0; i < strlen(new_val_str); i++)
            {
                
                    if (count >= 30)
                    {
                        break;
                    }
                    if(count!=0 && (offset*2 + count)%8==0)
                    {
                        fseek(fp2, ftell(fp2) + 2, SEEK_SET);
                    }
                    if(ftell(fp2)%50==0)
                    {
                        fseek(fp2,ftell(fp2) + 10, SEEK_SET);
                        printf("here it is : %d\n",ftell(fp2));
                    }
                    fprintf(fp2, "%c", new_val_str[i]);
                    count++;
            }

        }
        
        
    }

    return 0;
}