
//SIC / XE PROGRAM HANDLING ONLY INSTRUCTIONS OF FORMAT 1 AND FORMAT 2

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
int read_opcodes()
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
int read_symtab()
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
int give_hex(char s)
{
    if(s=='4')
    {
        return 4;
    }
    else if(s=='C')
    {
        return 12;
    }
    else if(s=='8')
    {
        return 8;
    }
    else if(s=='0')
    {
        return 0;
    }
}
char give_reg(char s)
{
    if(s=='A')
    {
        return '0';
    }
    else if(s=='X')
    {
        return '1';
    }
    else if(s=='T')
    {
        return '5';
    }
    else if(s=='S')
    {
        return '4';
    }
}
int main()
{
   
    read_symtab();
    read_opcodes();
     FILE * fp ;
    fp = fopen("test.txt", "r+");
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
    char opcode[4];
     memset(start_addr,0,sizeof(start_addr));
     strcpy(program_start,tokens[0]);
     while (fgets(line, sizeof(line), fp))
    {
        printf("%s\n",line);
        flag = 0;
        INTR temp_instr = insert_instr(line);
        char object_code[100] = "";
        
        //printf("%s %s %s %s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand);
        if(search_optab(temp_instr.opcode))
        {
            int test = strlen(temp_instr.operand);
            //printf("last character = %c\n",temp_instr.operand[test-1]);
            
            
            if(temp_instr.operand[0]=='L'||temp_instr.operand[0]=='A'||temp_instr.operand[0]=='S'||temp_instr.operand[0]=='T'||strcmp(temp_instr.operand,"X")==0)
            {
               
                strcpy(opcode,give_opnum(temp_instr.opcode));
                strcat(object_code,opcode);
                char reg_num;
                if(temp_instr.operand[1]==',')
                {
                    char reg_num1 = give_reg(temp_instr.operand[0]);
                    char reg_num2 = give_reg(temp_instr.operand[2]);
                    strncat(object_code,&reg_num1,1);
                    strncat(object_code,&reg_num2,1);
                    printf("%s\n",object_code);
                    fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);

                }
                else{
                        reg_num = give_reg(temp_instr.operand[0]);
                        strncat(object_code,&reg_num,1);
                        char zero = '0';
                        strncat(object_code,&zero,1);
                        printf("%s\n",object_code);
                        fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);

                }
                
                //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            }

            else if(strcmp(temp_instr.operand," ")==0)
            {
                if(strcmp(temp_instr.opcode, "RSUB")==0) {
                    strcpy(object_code,"4F0000");
                    printf("%s\n",object_code);
                    fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                    //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                }
                else {
                    strcpy(object_code,give_opnum(temp_instr.opcode));
                    printf("%s\n",object_code);
                    fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                    //printf("%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
                }
                
                
            }
            else 
            {
                flag = 1;
                fprintf(output,"%-10s %-10s %-10s %-10s %-10s\n",temp_instr.loc,temp_instr.label,temp_instr.opcode,temp_instr.operand,object_code);
            }
        


        }
           

    }



  return 0;
}