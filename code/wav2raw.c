#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

FILE *wavFile, *pcmFile, *f1, *f2;

int main (int argc, char *argv[])
{
	//f1 = fopen("arg[1]", "rb");
	//f2 = fopen("filename.txt", "rb");
	//char wavlist[100], pcm_name[100];
	int i, size;
	short informationMemory;
	/*while (fscanf(f1, "%s" , wavlist) != EOF){
		fscanf(f2, "%s" , pcm_name);
        printf("%s.raw",pcm_name);

		char path[70] = "D:\\Users\\Dsplab\\Desktop\\motion\\raw\\";
		strcat(path,pcm_name);
		strcat(path,".raw");*/



		wavFile = fopen(argv[1], "rb");
		pcmFile = fopen(argv[2], "wb");


		fseek(wavFile,44,SEEK_END);
		size = ftell(wavFile);
		fseek(wavFile,44,SEEK_SET);

		for(i=0;i<size;i++){
			fread(&informationMemory,1,1,wavFile);
			fwrite(&informationMemory,1,1,pcmFile);
		}
		//printf(" done!\n");
		fclose(wavFile);
		fclose(pcmFile);
	/*}
	fclose(f1);
	fclose(f2);*/

	return 0;
}
