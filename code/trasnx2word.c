#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Transx{
    char word[5];
    char trash[100];
}Transx;


int main(){

    FILE *list,*output,*input;
    char _list[100];
    int sex=0,speaker=1,sentence_num=1,i=0;
    list=fopen("D:\\Users\\Dsplab\\Desktop\\TCC300\\transx\\file.txt","rb");

    while(fscanf(list,"%s",&_list)!=EOF){
        Transx transx;
        if(strcmp(_list,"Q")==0){
             speaker++;
             sentence_num=1;
             fscanf(list,"%s",&_list);
        }
        if(strcmp(_list,"W")==0){sex=1;speaker=1;fscanf(list,"%s",&_list);}
        char path[100];
    	input=fopen(_list,"rb");


    	sprintf(path,"D:\\Users\\Dsplab\\Desktop\\TCC300\\worddata\\MOT_slt_%d%03d%04d.txt",sex,speaker,sentence_num++);
    	output=fopen(path,"wb");


        while(fscanf(input,"%s",&transx.word)!=EOF){
            for(i=0;i<5;i++){
                fscanf(input,"%s",&transx.trash);
            }
            fprintf(output,"%s",transx.word);
        }

        fclose(input);
        fclose(output);


    }

    return 0;
}
