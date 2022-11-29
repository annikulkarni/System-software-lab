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

            fseek(fp2, line*50 + (offset*2)+temp_off*2 + 10 + 250, SEEK_SET);
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
            char temp_new_val_str [atoi(tokens[2])];
            memset(temp_new_val_str, 0, sizeof(temp_new_val_str));
            
            
            for(int i=0;i<atoi(tokens[2]);i++)
            {
                temp_new_val_str[i]='0';
            }

            temp_new_val_str[atoi(tokens[2])] = '\0'; 
            
            int k=0;
            
            for(int i=strlen(temp_new_val_str)-strlen(new_val_str);i<strlen(temp_new_val_str);i++)
            {
                temp_new_val_str[i] = new_val_str[k];
                k++;
            }
            
          
        
            count = 0;
            fseek(fp2, line*50 + (offset*2)+temp_off*2 + 10 + 250, SEEK_SET);
            for (int i = 0; i < strlen(temp_new_val_str); i++)
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
                    fprintf(fp2, "%c", temp_new_val_str[i]);
                    count++;
            }

        }
        