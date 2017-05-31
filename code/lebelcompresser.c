#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getout[100]={};

typedef struct data_item{
    char start_time[100];
    char end_time[100];
    char phoneme[100];
    char word[50];
}data_item;

int main(int argc, char *argv[]){

    FILE *inputfile,*outputfile,*list,*filename;

    char temp_list[100],temp_filename[100],trash[100],temp_phon[100],temp_word[100],temp_start_time[100],temp_end_time[100];
    int i=0,j=0,k=0,end=0;
    //list=fopen("C:\\Users\\user\\Desktop\\labdata\\lab\\list.txt","rb");
    //filename=fopen("C:\\Users\\user\\Desktop\\labdata\\lab\\filename.txt","rb");

    //while(fscanf(list,"%s",temp_list)!=EOF){

        //open .lab file
        //fscanf(filename,"%s",temp_filename);
        inputfile=fopen(argv[1],"rb");

        //open new.lab file
        /*char path[100]="C:\\Users\\user\\Desktop\\labdata\\newlab\\";
        strcat(path,temp_filename);
        strcat(path,"new.lab");*/
        outputfile=fopen(argv[2],"wb");

        //read first two line
        fscanf(inputfile,"%s",trash);
        fscanf(inputfile,"%s",trash);
        fscanf(inputfile,"%s %s %s %s",temp_start_time,temp_end_time,temp_phon,temp_word);//read first sil line
        end=1;
        while(fscanf(inputfile,"%s %s %s %s",temp_start_time,temp_end_time,temp_phon,temp_word)!=EOF){

            fscanf(inputfile,"%s %s %s",temp_start_time,temp_end_time,temp_phon);

            if(strcmp(temp_phon,"sil")==0){
                fscanf(inputfile,"%s",temp_word);
                end++;
            }
            else{end++;}

            while(strcmp(temp_phon,"sp")!=0){
                memset(temp_phon,0,strlen(temp_phon));
                fscanf(inputfile,"%s %s %s",temp_start_time,temp_end_time,temp_phon);
                if(strcmp(temp_start_time,".")==0){break;}
                else{end++;}
            }
            if(strcmp(temp_start_time,".")==0){break;}
            else {end++;}
        }
        fseek(inputfile,0,SEEK_SET);
        data_item data_item1[end];
        fscanf(inputfile,"%s",trash);
        fscanf(inputfile,"%s",trash);
        i=0;
        fscanf(inputfile,"%s %s %s %s",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme,data_item1[i].word);
        //fprintf(outputfile,"%s %s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme,data_item1[i].word);
        for(i=1;i<end;i++){
            fscanf(inputfile,"%s %s %s %s",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme,data_item1[i].word);
            //fprintf(outputfile,"%s %s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme,data_item1[i].word);
            while(strcmp(data_item1[i].phoneme,"sp")!=0){
                i++;
                fscanf(inputfile,"%s %s %s",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme);
                if(strcmp(data_item1[i].start_time,".")==0){break;}
                else{
                    //fprintf(outputfile,"%s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme);
                }
            }
        }
        int index=0;
        //delete
        for(i=0;i<end;i++){
            if(strcmp(data_item1[i].phoneme,"sil")!=0&&strcmp(data_item1[i].phoneme,"sp")!=0){
                for(j=0;j<strlen(data_item1[i].phoneme);j++){
                    if(data_item1[i].phoneme[j]=='+'||data_item1[i].phoneme[j]=='-'){
                        for(k=0;k<20;k++){
                            data_item1[i].phoneme[k]=data_item1[i].phoneme[j+k+1];
                            if(data_item1[i].phoneme[j+k+1]=='+'||data_item1[i].phoneme[j+k+1]=='-'||data_item1[i].phoneme[j+k+1]=='\0'){
                                data_item1[i].phoneme[k]='\0';
                                break;
                            }
                        }
                    }
                }
            }
            else if(strcmp(data_item1[i].word,"sil")==0){
                memset(data_item1[i].word,0,strlen(data_item1[i].word));

            }
            else if(strcmp(data_item1[i].phoneme,"sp")==0){
                if(strcmp(data_item1[i].start_time,data_item1[i].end_time)==0){
                    getout[index++]=i;
                }
                else{
                    int temp1,temp2;
                    temp1= atoi(data_item1[i].start_time);
                    temp2= atoi(data_item1[i].end_time);
                    if(temp2-temp1<2500000){
                        strcpy(data_item1[i-1].end_time,data_item1[i].end_time);
                        getout[index++]=i;
                    }
                    else{}
                }
            }
            else{}
        }

        //output
        i=0;
        fprintf(outputfile,"%s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme);
        for(i=1;i<end;i++){
            if(test_if_need2pass(i,index)==1){
                goto HERE;
            }
            else{};
            if((strcmp(data_item1[i].phoneme,"sil")==0)&&(strcmp(data_item1[i].word,"")==0)){
                fprintf(outputfile,"%s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme);
            }
            else{
                fprintf(outputfile,"%s %s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme,data_item1[i].word);
            }
            while(strcmp(data_item1[i].phoneme,"sp")!=0){
                i++;
                if(test_if_need2pass(i,index)==1){
                    goto HERE;
                }
                else{}
                if(strcmp(data_item1[i].start_time,".")==0){break;}
                else{
                    fprintf(outputfile,"%s %s %s\n",data_item1[i].start_time,data_item1[i].end_time,data_item1[i].phoneme);
                }
            }
            HERE: i=i;
        }

    //}
    return 0;
}

int test_if_need2pass(int x,int index){
    int i=0;
    for(i=0;i<index;i++){
        if(x==getout[i]){
            return 1;
        }
    }
    return 0;
}


