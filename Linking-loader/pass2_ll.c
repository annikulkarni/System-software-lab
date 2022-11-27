#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20
struct estab
{
   char csname[10];
   char extsym[10];
   int address;
   int length;
} es[MAX];
struct memory
{
   int loca;
   char addr[10];
} mem[100];
int main()
{
   char input[10], name[10], symbol[10], addr[10], op[5], ad[10];
   int count = 0, progaddr, csaddr, add, len, i, loc, j, l = 0, adr, k;
   FILE *fp1, *fp2;
   fp1 = fopen("input.txt", "r");
   fp2 = fopen("estab.txt", "r");
   while (!feof(fp2))
   {
      fscanf(fp2, "%s\t%s\t\t%d\t%d\n", es[count].csname, es[count].extsym, &es[count].address, &es[count].length);
      count++;
   }
   printf("\nRecord written to strucuture");
   fscanf(fp1, "%s", input);
   while (strcmp(input, "END") != 0)
   {
      if (strcmp(input, "H") == 0)
      {
         fscanf(fp1, "%s %s %s", name, symbol, addr);
         for (i = 0; i < count; i++)
         {
            if (strcmp(name, es[i].csname) == 0)
            {
               printf("\nRecord Found");
               loc = es[i].address;
               printf("\n%s:%d", es[i].csname, loc);
               break;
            }
         }
         progaddr = loc;
      }
      else if (strcmp(input, "T") == 0)
      {
         fscanf(fp1, "%s %s", addr, name);
         loc = progaddr + atoi(addr);
         len = atoi(name);
         j = len / 3;
         for (i = 0; i < j; i++)
         {
            fscanf(fp1, "%s", symbol);
            mem[l].loca = loc;
            strcpy(mem[l].addr, symbol);
            printf("\n%d: %s", loc, symbol);
            loc = loc + 3;
            l++;
         }
         printf("\n%d", l);
      }
      else if (strcmp(input, "M") == 0)
      {
         fscanf(fp1, "%s %s", addr, name);
         adr = progaddr + atoi(addr);
         fscanf(fp1, "%s", symbol);
         printf("\n%s", symbol);
         len = strlen(symbol);
         for (i = 0, j = 1; i < len; i++, j++)
            name[i] = symbol[j];
         printf("\n%s", name);
         for (i = 0; i < count; i++)
         {
            if ((strcmp(name, es[i].extsym) == 0) || (strcmp(name, es[i].csname) == 0))
            {
               printf("\nSymbol Found: %s", es[i].extsym);
               add = es[i].address;
               printf("\nSymbol Address: %d", add);
               break;
            }
         }
         for (j = 0; j < l; j++)
         {
            if (mem[j].loca == adr)
            {
               printf("\nModifying Memory: %d with value: %s", adr, mem[j].addr);
               if (symbol[0] == '+')
               {
                  len = strlen(mem[j].addr);
                  for (i = 0; i < 2; i++)
                     op[i] = mem[j].addr[i];
                  op[i] = '\0';
                  for (k = 0, i = 2; i < len; i++, k++)
                     ad[k] = mem[j].addr[i];
                  ad[k] = '\0';
                  printf("\nOpcode: %s", op);
                  printf("\nAddress: %2s", ad);
                  k = atoi(ad) + add;
                  sprintf(ad, "%d", k);
                  for (i = 0; i < 2; i++)
                     mem[j].addr[i] = op[i];
                  mem[j].addr[i] = '\0';
                  printf("\nModified Address at location %d: %d", mem[j].loca, k);
                  strcat(mem[j].addr, ad);
                  printf("\n%s", mem[j].addr);
                  printf("\nModified Address: %d", k);
               }
               else if (symbol[0] == '-')
               {
                  len = strlen(mem[j].addr);
                  for (i = 0; i < 2; i++)
                     op[i] = mem[j].addr[i];
                  op[i] = '\0';
                  for (k = 0, i = 2; i < len; i++, k++)
                     ad[k] = mem[j].addr[i];
                  ad[k] = '\0';
                  printf("\nOpcode: %s", op);
                  printf("\nAddress: %s", ad);
                  k = atoi(ad) - add;
                  sprintf(ad, "%d", k);
                  for (i = 0; i < 2; i++)
                     mem[j].addr[i] = op[i];
                  mem[j].addr[i] = '\0';
                  printf("\nModified Address at location %d: %d", mem[j].loca, k);

                  strcat(mem[j].addr, ad);
                  printf("\n%s", mem[j].addr);
               }
               break;
            }
         }
      }
      fscanf(fp1, "%s", input);
   }
   printf("\n");
   for (i = 0; i < l; i++)
   {
      printf("%d: ", mem[i].loca);
      for (j = 0; j < 2; j++)
         printf("%c", mem[i].addr[j]);
      printf("  %d: ", mem[i].loca + 1);
      for (j = 2; j < 4; j++)
         printf("%c", mem[i].addr[j]);
      printf("  %d: ", mem[i].loca + 2);
      for (j = 4; j < strlen(mem[j].addr); j++)
         printf("%c", mem[i].addr[j]);
      printf("\t");
   }
}