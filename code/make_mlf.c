#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

typedef struct syllable_item{
    char syllable_phon[100];
    char syllable_alphabet[10];
}syllable_item;

typedef struct txt_item{
    char txt_chinese[100];
    char txt_phon[100];
}txt_item;

int main(int argc, char *argv[])
{
    FILE *syllable,*inputfile,*outputfile;
    int i=0,j=0,syllable_end=0,txt_end=0,_print=0;
    char temp_phon[100],temp_alphabet[10],temp[20],temp_txtchinese[100],temp_txtphon[10];
    char *name;

    syllable=fopen(argv[1],"rb");


    //count the syllable.txt endline
    while(fscanf(syllable,"%s %s",temp_phon,temp_alphabet)!=EOF){
            syllable_end++;
    }
    //go back the file top
    fseek(syllable,0,SEEK_SET);
    syllable_item syll_table[syllable_end];

    //input syll_table
    for(i=0;i<syllable_end;i++){
        fscanf(syllable,"%s %s",syll_table[i].syllable_phon,syll_table[i].syllable_alphabet);
    }
    //input txt
    //open .txt file
    inputfile=fopen(argv[2],"rb");
    //open .mlf file
    outputfile=fopen(argv[3],"wb");
    sprintf(temp,"%s",argv[2]);

    for(i=0;i<20;i++){
        if(temp[i]=='.'&&temp[i+1]=='t'){
            temp[i]='\0';
            break;
        }
    }

    name=strtok(temp,"/");
    name=strtok(NULL,"/");
    name=strtok(NULL,"/");
    name=strtok(NULL,"/");

    //write .mlf file
    fprintf(outputfile,"#!MLF!#\n\"*/%s.lab\"\n",name);

    //compare
    txt_end=0;
    while(fscanf(inputfile,"%s %s",temp_txtchinese,temp_txtphon)!=EOF){
        txt_end++;
    }
    fseek(inputfile,0,SEEK_SET);
    txt_item txt_item1[txt_end];
    for(i=0;i<txt_end;i++){
        fscanf(inputfile,"%s %s",txt_item1[i].txt_chinese,txt_item1[i].txt_phon);
    }

    for(i=0;i<txt_end;i++){
        for(j=0;j<syllable_end;j++){
            if(strcmp(txt_item1[i].txt_phon,syll_table[j].syllable_phon)==0){
                fprintf(outputfile,"%s\n",syll_table[j].syllable_alphabet);
                break;
            }
            else if(strlen(txt_item1[i].txt_phon)==4&&j==syllable_end-1){
                txt_item1[i].txt_phon[2]='\0';
                j=0;
            }
            else if(strlen(txt_item1[i].txt_phon)==6&&j==syllable_end-1){
                txt_item1[i].txt_phon[4]='\0';
                j=0;
            }
            else if(strlen(txt_item1[i].txt_phon)==8){
                txt_item1[i].txt_phon[6]='\0';
                j=0;
            }

            else{}
        }
    }
    return 0;
}
