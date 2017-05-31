#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct ALA{

    char ini_phoneme[20];
    char n_phoneme[20];
    char c_phoneme[20];
    char m_phoneme[20];

}ALA;

typedef struct TABLE{

    char word[20];
    char ini_phoneme[20];
    char n_phoneme[20];
    char c_phoneme[20];
    char m_phoneme[20];
    int phoneme_num;

}TABLE;

int main(){

    FILE *ala_file,*mlf_file,*list,*filename,* table_file;
    char _list[100],name[100],temp[100],temp2[100],*buffer,table_temp[100];
    int len=0,index=0,num=0;
    list=fopen("D:\\Users\\Dsplab\\Documents\\Chris_Kao\\TCC300forMotion\\ala\\list.txt","rb");
    table_file=fopen("D:\\Users\\Dsplab\\Documents\\Chris_Kao\\TCC300forMotion\\sillable_table.txt","rb");
    filename=fopen("D:\\Users\\Dsplab\\Documents\\Chris_Kao\\TCC300forMotion\\ala\\filename.txt","rb");
    memset(temp,0,100);
    memset(temp2,0,100);
    TABLE table[500];

    //initial table
    int k;
    for(k=0;k<500;k++){
        memset(table[k].word,0,20);
        memset(table[k].ini_phoneme,0,20);
        memset(table[k].m_phoneme,0,20);
        memset(table[k].n_phoneme,0,20);
        memset(table[k].c_phoneme,0,20);
    }

    //make table
    while(fgets(temp,100,table_file)!=NULL){
        sprintf(temp,"%s",strtok(temp,"\r\n"));
        buffer=strtok(temp,"    ");
        sprintf(table[index].word,"%s",buffer);

        while(buffer!=NULL){
            buffer=strtok(NULL,"    ");
            if(buffer==NULL) {break;}
            sprintf(temp2,"%s",buffer);
            switch(temp2[0]){
                case 'i':
                    sprintf(table[index].ini_phoneme,"%s",temp2);
                    memset(temp2,0,100);
                    num++;
                    break;
                case 'n':
                    sprintf(table[index].n_phoneme,"%s",temp2);
                    memset(temp2,0,100);
                    num++;
                    break;
                case 'c':
                    sprintf(table[index].c_phoneme,"%s",temp2);
                    memset(temp2,0,100);
                    num++;
                    break;
                case 'm':
                    sprintf(table[index].m_phoneme,"%s",temp2);
                    num++;
                    memset(temp2,0,100);
                    break;
            }
        }
        table[index].phoneme_num=num;
        num=0;
        index++;
    }
     fclose(table_file);
    //table done

    while(fscanf(list,"%s",&_list)!=EOF){

    	ala_file=fopen(_list,"rb");
    	fscanf(filename,"%s",&name);
    	char path[100];
    	memset(path,0,100);
        sprintf(path,"D:\\Users\\Dsplab\\Documents\\Chris_Kao\\TCC300forMotion\\mlf\\%s.mlf",name);
    	mlf_file=fopen(path,"wb");
    	fprintf(mlf_file,"#!MLF!#\n\"*/%s.lab\"\n",name);

    	ALA input;
    	//initialize
        memset(input.ini_phoneme,0,20);
        memset(input.m_phoneme,0,20);
        memset(input.n_phoneme,0,20);
        memset(input.c_phoneme,0,20);
        memset(temp,0,100);
        memset(temp2,0,100);
        int temp_num=1;
        while(fgets(temp,200,ala_file)!=NULL){
            if(strcmp(temp,"\r\n")==0){break;}
            else{}
            sprintf(temp,"%s",strtok(temp,"\r\n"));
            len=strlen(temp);

            if(!isalpha(temp[len-1])&&(temp[len-2]!='L')){
                //search
                strcat(temp2,input.ini_phoneme);
                strcat(temp2,input.m_phoneme);
                strcat(temp2,input.n_phoneme);
                strcat(temp2,input.c_phoneme);


                int j=0;
                for(j=0;j<index;j++){
                    memset(table_temp,0,100);
                    strcat(table_temp,table[j].ini_phoneme);
                    strcat(table_temp,table[j].m_phoneme);
                    strcat(table_temp,table[j].n_phoneme);
                    strcat(table_temp,table[j].c_phoneme);


                    if((table[j].phoneme_num<=temp_num)&&(strcmp(temp2,table_temp)==0)){
                        fprintf(mlf_file,"%s\n",table[j].word);break;
                    }

                    else{}
                    memset(table_temp,0,100);
                }
                if(j==index){
                    //printf("notfind\n");
                }

                //重新存取
                temp_num=1;
                sprintf(temp,"%s",strtok(temp," "));
                memset(input.ini_phoneme,0,20);
                memset(input.m_phoneme,0,20);
                memset(input.n_phoneme,0,20);
                memset(input.c_phoneme,0,20);
            }
            else{temp_num++;}
            switch(temp[0]){
                case 'i':
                    sprintf(input.ini_phoneme,"%s",temp);
                    memset(temp2,0,100);
                    break;
                case 'n':
                    sprintf(input.n_phoneme,"%s",temp);
                    memset(temp2,0,100);
                    break;
                case 'c':
                    sprintf(input.c_phoneme,"%s",temp);
                    memset(temp2,0,100);
                    break;
                case 'm':
                    sprintf(input.m_phoneme,"%s",temp);
                    memset(temp2,0,100);
                    break;
            }

        }



    fclose(ala_file);
    fclose(mlf_file);


    }

    fclose(list);
    fclose(filename);
    return 0;
}


