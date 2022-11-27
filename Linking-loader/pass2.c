#include<stdio.h>
#include<string.h>
#define MAXSIZE 100
typedef struct mod_rec {
    char label[10];
    char addr[10];
}MOD_REC;

MOD_REC mod_records[MAXSIZE];
int m_count = 0;
char program_addr[10];
int read_estab()
{
   FILE * fp ;
   fp = fopen("estab.txt","r+");
   if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }
    char line[500];

    char first_line[500];  

    fgets(first_line,500,fp);

    char tokens_f[3][20];

    char *token_f = strtok(first_line," ");

    int j=0;
    while (token_f != NULL)
    {
        strcpy(tokens_f[j], token_f);
        token_f = strtok(NULL, " ");
        j++;
    }   
    strcpy(program_addr, tokens_f[1]);

    while (fgets(line, sizeof(line), fp))
    {
        char tokens[3][20];
        strcpy(tokens[0],"");
        strcpy(tokens[1],"");
        strcpy(tokens[2],"");
        
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
        if(strcmp(tokens[2],""))
        {
            continue;
        }
        else{
            
        strcpy(mod_records[m_count].label, tokens[0]);
        strcpy(mod_records[m_count].addr, tokens[1]);
        m_count++;

        }

        
    }
        fclose(fp);     
}
int main()
{
    char csaddr[10];
    char execaddr[10];
    read_estab();
    printf("%s\n",program_addr);
    strcpy(csaddr,program_addr);
    strcpy(execaddr,program_addr);

    FILE *fp ;
    FILE *fp2;

    fp = fopen("input.txt","r+");
    fp2 = fopen("memory.txt","w+");

    if(!fp)
    {
        printf("Unable to open file\n");
        return -1;
    }
    char line[500];
    while(fgets(line,sizeof(line),fp))
    {
       
    }

    
    return 0;
}