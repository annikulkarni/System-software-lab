#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100
typedef struct symtab
{
    char label[MAXSIZE];
    char addr[MAXSIZE];
}SYM;
typedef struct intermediate
{
    char loc[MAXSIZE];
    char label[MAXSIZE];
    char opcode[MAXSIZE];
    char operand[MAXSIZE];
}INTR;
typedef struct op
{
    char opcode[MAXSIZE];
    char opnum[MAXSIZE];
}OPCODE;
SYM symbols[MAXSIZE];
INTR instr[MAXSIZE];
OPCODE opcodes[MAXSIZE];
int op_count = 0;
int sym_count = 0;
int instr_count = 0;
void read_opcodes()
{
    FILE *fp4;
    fp4 = fopen("optab.txt", "r+");
    if (!fp4)
    {
        printf("\n Unable to open ");
        return -1;
    }
    char line[500];
    while (fgets(line, sizeof(line), fp4))
    {
        char tokens[2][20];

       char *token = strtok(line, " ");
      int j=0;
      while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }
        tokens[1][strcspn(tokens[1], "\n")] = 0;
        strcpy(opcodes[op_count].opcode, tokens[0]);
         strcpy(opcodes[op_count].opnum, tokens[1]);
        op_count++;

    }
    fclose(fp4);
}
void read_symtab()
{
    FILE *fp;
    fp = fopen("symtab.txt", "r");
    if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }
    char line[500];
    while (fgets(line, sizeof(line), fp))
    {
        char tokens[2][20];

       char *token = strtok(line, " ");
      int j=0;
      while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }

        tokens[1][strcspn(tokens[1], "\n")] = 0;
        strcpy(symbols[sym_count].label, tokens[0]);
        strcpy(symbols[sym_count].addr, tokens[1]);
        sym_count++;

    }
    fclose(fp);
}
int search_optab(char *s)
{
    for(int i=0;i<op_count;i++)
    {
        if(strcmp(opcodes[i].opcode,s)==0)
        {
            return 1;
        }
    }
    return 0;
}
char * search_symtab(char *s)
{
      for(int i=0;i<sym_count;i++)
      {
        if(strcmp(symbols[i].label,s)==0)
        {
            return symbols[i].addr;
        }

      }

}
char *give_opnum(char *s)
{
    for(int i=0;i<op_count;i++)
      {
        if(strcmp(opcodes[i].opcode,s)==0)
        {
            return opcodes[i].opnum;
        }

      }
}
int count_spaces(char *s)
{
    int i = 0;
    int spaces = 0;
    while (s[i] != '\0')
    {
        if (s[i] == ' ')
        {
            spaces++;
        }
        i++;
    }
    return spaces;
}
INTR insert_instr(char *s)
{
    s[strcspn(s, "\n")] = 0;
    //printf("line is %s\n",s);
    int spaces = count_spaces(s);
    //printf("spaces = %d\n",spaces);
    char *token = strtok(s," ");
    INTR temp_obj;
    char tokens[4][20];
    strcpy(temp_obj.loc," ");
    strcpy(temp_obj.label," ");
    strcpy(temp_obj.opcode," ");
    strcpy(temp_obj.operand," ");

    if (spaces == 5)
    {
        int j = 0;

        while (token != NULL)
        {
            strcpy(tokens[j], token);

            token = strtok(NULL, " ");
            j++;
        }

        strcpy(temp_obj.loc, tokens[0]);
        strcpy(temp_obj.opcode, tokens[1]);
        return temp_obj;
    }
    else if (spaces == 4)
    {
        int j = 0;
        while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }

        strcpy(temp_obj.loc, tokens[0]);
        strcpy(temp_obj.opcode, tokens[1]);
        strcpy(temp_obj.operand, tokens[2]);
        return temp_obj;
    }
    else if(spaces == 3)
    {
        int j = 0;
        while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }

        strcpy(temp_obj.loc, tokens[0]);
        strcpy(temp_obj.label, tokens[1]);
        strcpy(temp_obj.opcode, tokens[2]);
        strcpy(temp_obj.operand, tokens[3]);
        return temp_obj;
    }

}
long long convert_to_binary(int n) {
  long long bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

  return bin;
}
int main()
{
    read_symtab();
    read_opcodes();

    FILE * fp ;
    fp = fopen("intermediate_temp.txt", "r+");
    if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }

    char first_line[500];

    fgets(first_line,500,fp);


    char tokens[4][20];

    char *token = strtok(first_line," ");

    int j=0;

    while (token != NULL)
    {
        strcpy(tokens[j], token);
        token = strtok(NULL, " ");
        j++;
    }

    FILE * object_record;
    FILE * output;

    output = fopen("output.txt","w+");

    object_record = fopen("object_program.txt", "w+");

    fprintf(object_record,"H^  %s^  1000^  0000",tokens[1]);
    fprintf(object_record,"\n");
    printf("starting addr = %s\n",tokens[0]);
    fprintf(object_record,"T^%6s",tokens[0]);

    printf("program name  = %s\n",tokens[1]);

    char line[500];
    char program_start[100];
    char start_addr[100];
    char object_program_temp[100];
    memset(object_program_temp,0,sizeof(object_program_temp));
    int object_program_count = 0;
    int object_program_size = 0;
    int flag = 0;
    int flag2 = 0;
     memset(start_addr,0,sizeof(start_addr));
     strcpy(program_start,tokens[0]);
    while (fgets(line, sizeof(line), fp))
    {
        flag = 0;
        INTR temp_instr = insert_instr(line);
        char object_code[100] = "";

        //printf("%s %s %s %s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand);
        if(search_optab(temp_instr.opcode))
        {
            int test = strlen(temp_instr.operand);
            //printf("last character = %c\n",temp_instr.operand[test-1]);
            if(temp_instr.operand[test-1]=='X')
            {
                char str[2];
               char add[4];
               strcpy(add,search_symtab("BUFFER"));

               int a = add[0] - '0';
               itoa (a, str, 2);
               int temp = strtol(str,NULL,10) + 8;
               char s[1];
               itoa (temp, s, 16);
                strcat(object_code,give_opnum(temp_instr.opcode));
                strcat(object_code,s);
                strncat(object_code,&add[1],1);
                strncat(object_code,&add[2],1);
                strncat(object_code,&add[3],1);
                //printf("%s\n",object_code);
                fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            }
            else if(strcmp(temp_instr.operand," ")!=0)
            {
                strcat(object_code,give_opnum(temp_instr.opcode));
                strcat(object_code,search_symtab(temp_instr.operand));
                fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            }

            else if(strcmp(temp_instr.operand," ")==0)
            {
                strcat(object_code,give_opnum(temp_instr.opcode));
                strcat(object_code,"0000");
                fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            }
        }

        else if(strcmp(temp_instr.opcode,"BYTE")==0)
        {

            if(temp_instr.operand[0]=='C')
            {
                for(int i=2 ;i<strlen(temp_instr.operand)-1;i++)
                {
                    char temp[10];
                    int c = temp_instr.operand[i];
                    sprintf(temp,"%X",c);
                    strcat(object_code,temp);
                }
               fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
               //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);

            }
            if(temp_instr.operand[0]=='X')
            {
                for(int i=2 ;i<strlen(temp_instr.operand)-1;i++)
                {
                    strncat(object_code,&temp_instr.operand[i],1);

                }
                fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
               //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);

            }

        }
        else if(strcmp(temp_instr.opcode,"WORD")==0)
        {
            char test[6] = "";
            itoa (strtol(temp_instr.operand,NULL,10), object_code, 16);

            for(int i=0;i<6-strlen(object_code);i++)
            {
                test[i]='0';
            }
            strcat(test,object_code);
            strcpy(object_code,test);
            fprintf(output,"%-10s %-10s %-10s %-10s %s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            //printf("%-10s %-10s %-10s %-10s %s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
        }

        else
        {
                flag = 1;
                fprintf(output,"%-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand);
                //printf("%-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand);
        }

            memset(start_addr,0,sizeof(start_addr));
            strcpy(start_addr,temp_instr.loc);

            if(flag==1&&flag2==1)
            {
                continue;
            }
            if(flag == 1)
            {

                char size[100];
                sprintf(size,"%X",object_program_size);
                printf("line = %s\n",object_program_temp);
                fprintf(object_record,"^%2s%s",size,object_program_temp);
                printf("size = %s\n",size);

                printf("start = %6s\n",start_addr);
                 //printf("start addr = %s\n",temp_instr.loc);
                 memset(object_program_temp,0,sizeof(object_program_temp));
                 object_program_size  = 0;
                 flag2 = 1;


            }
            else if(object_program_size + (strlen(object_code)/2) > 30)
            {

                char size[100];
                sprintf(size,"%X",object_program_size);
                printf("line = %s\n",object_program_temp);
                fprintf(object_record,"^%2s%s",size,object_program_temp);
                 printf("size = %s\n",size);
                fprintf(object_record,"\nT^%6s",start_addr);
                printf("start = %s\n",start_addr);
                memset(object_program_temp,0,sizeof(object_program_temp));
                object_program_size  = 0;
                strcat(object_program_temp,"^");
                strcat(object_program_temp,object_code);
                object_program_size+= strlen(object_code)/2;
                flag2 = 0;

            }
            else
            {
                if(flag2==1)
                {
                    fprintf(object_record,"\nT^%6s",start_addr);
                }
                strcat(object_program_temp,"^");
                strcat(object_program_temp,object_code);
                object_program_size+= strlen(object_code)/2;
                flag2 = 0;

            }


    }
    fprintf(object_record,"\nE^%6s",program_start);
    int end_addr;
    int program_str;
    end_addr = strtol(start_addr,NULL,16);
    program_str = strtol(program_start,NULL,16);
    fseek(object_record, 0, SEEK_SET);
    fseek(object_record,18,SEEK_CUR);
    fprintf(object_record,"%X",end_addr-program_str);

    return 0;
}


