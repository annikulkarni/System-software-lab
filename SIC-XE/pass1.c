#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100

int locctr = 0;
char labels[10][20];
int label_count = 0;
int line_count = 0;
int startingAddress;
typedef struct op
{
    char opcode[MAXSIZE];
    char opnum[MAXSIZE];
} OPCODE;

typedef struct object
{
    char label[MAXSIZE];
    char opcode[MAXSIZE];
    char operand[MAXSIZE];
} OBJ;
OPCODE opcodes[MAXSIZE];
int op_count = 0;
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
        int j = 0;
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

OBJ insert_tokens(char *s)
{
    s[strcspn(s, "\n")] = 0;
    int spaces = count_spaces(s);
    char *token = strtok(s, " ");
    OBJ temp_obj;
    char tokens[3][20];
    strcpy(temp_obj.label, " ");
    strcpy(temp_obj.opcode, " ");
    strcpy(temp_obj.operand, " ");

    if (spaces == 0)
    {
        strcpy(temp_obj.opcode, s);
        return temp_obj;
    }

    else if (spaces == 1)
    {
        int j = 0;

        while (token != NULL)
        {
            strcpy(tokens[j], token);

            token = strtok(NULL, " ");
            j++;
        }

        strcpy(temp_obj.opcode, tokens[0]);
        strcpy(temp_obj.operand, tokens[1]);
        return temp_obj;
    }
    else if (spaces == 2)
    {
        int j = 0;
        while (token != NULL)
        {
            strcpy(tokens[j], token);
            token = strtok(NULL, " ");
            j++;
        }

        strcpy(temp_obj.label, tokens[0]);
        strcpy(temp_obj.opcode, tokens[1]);
        strcpy(temp_obj.operand, tokens[2]);
        return temp_obj;
    }
}
int main()
{
  read_opcodes();
  FILE *fp;
    fp = fopen("input.txt", "r+");
    if (!fp)
    {
        printf("\n Unable to open ");
        return -1;
    }

    char line[500];
    FILE *fp2 = fopen("intermediate.txt", "w+");
    // FILE *fp4 = fopen("intermediate_temp.txt", "w+");
    FILE *fp3 = fopen("symtab.txt", "w+");

    if (fp2 == NULL)
    {
        printf("Error in opening file");
        exit(-1);
    }
    if (fp3 == NULL)
    {
        printf("Error in opening file");
        exit(-1);
    }
    OBJ temp;
    while (fgets(line, sizeof(line), fp))
    {
        temp = insert_tokens(line);
        int valid = 0;
        if (temp.opcode[0] == '.')
        {
            printf("This is comment line : %s\n", temp.opcode);
            continue;
        }
        for (int k = 0; k < op_count; k++)
        {
          if(temp.opcode[0]=='+')
          {
            char *op = temp.opcode+1;
            if (strcmp(op, opcodes[k].opcode) == 0)
            {
                valid = 1;
            }
          }
          else{
              if (strcmp(temp.opcode, opcodes[k].opcode) == 0)
            {
                valid = 1;
            }
          }
            
        }
        if (!strcmp("END", temp.opcode) ||!strcmp("BASE", temp.opcode)|| !strcmp("START", temp.opcode) || !strcmp("BYTE", temp.opcode) || !strcmp("WORD", temp.opcode) || !strcmp("RESW", temp.opcode) || !strcmp("RESB", temp.opcode))
        {
            valid = 1;
        }
        if (!valid)
        {
            printf("\nopcode %s is invalid \n\n", temp.opcode);
            continue;
        }
        if (!strcmp(temp.opcode, "START"))
        {

            locctr = strtol(temp.operand, NULL, 16);
            startingAddress = strtol(temp.operand, NULL, 16);
            {
                fprintf(fp2, "%-10s %-10s %-10s %s\n", temp.operand, temp.label, temp.opcode, temp.operand);
                printf("%-10s %-10s %-10s\n", temp.label, temp.opcode, temp.operand);
            }
        }
        else{
          if (strcmp(temp.label, " ") == 1)
            {
                int flag = 0;
                for (int i = 0; i < label_count; i++)
                {
                    if (strcmp(temp.label, labels[i]) == 0)
                    {
                        flag = 1;
                    }
                }
                if (flag == 1)
                {
                    printf("\nlabel is duplicated at line %d\n\n", line_count + 1);
                    continue;
                }
                else
                {
                    fprintf(fp3, "%s %X\n", temp.label, locctr);
                    strcpy(labels[label_count], temp.label);
                    label_count++;
                }
            }
            if (strcmp(temp.opcode, "BASE")==0)
            {
              fprintf(fp2, "%-10s %-10s %-10s %s\n","", temp.label, temp.opcode, temp.operand);
              printf("%-10s %-10s %-10s\n", temp.label, temp.opcode, temp.operand);
              continue;
            }
            fprintf(fp2, "%-10X %-10s %-10s %s\n", locctr, temp.label, temp.opcode, temp.operand);
            printf("%-10s %-10s %-10s\n", temp.label, temp.opcode, temp.operand);
            
            if(temp.opcode[0]=='+')
            {
              locctr += 4;
            }
           
            else if(temp.operand[0]=='A'||temp.operand[0]=='S'||temp.operand[0]=='T'||strcmp(temp.operand,"X")==0)
            {
              locctr += 2;
            }
            else if (!strcmp(temp.opcode, "WORD"))
            {
                locctr += 3;
            }
            else if (!strcmp(temp.opcode, "RESW"))
            {
                locctr = locctr + 3 * strtol(temp.operand, NULL, 16);
            }
            else if (!strcmp(temp.opcode, "RESB"))
            {
                locctr = locctr + strtol(temp.operand, NULL, 10);
            }
            else if (!strcmp(temp.opcode, "BYTE"))
            {
                if (temp.operand[0] == 'X')
                {
                    locctr = locctr + ((strlen(temp.operand) - 3) / 2);
                }
                else if (temp.operand[0] == 'C')
                {
                    locctr = locctr + (strlen(temp.operand) - 3);
                }
            }
            else if (!strcmp(temp.opcode, "END"))
            {
            }
            else
            {
                locctr = locctr + 3;
            }
        }

    }
    fclose(fp3);
    fclose(fp2);
    printf("\n\nLength of the program = %X\n\n", locctr - startingAddress);
  return 0;
}