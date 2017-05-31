#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main()
{
	FILE *f_list, *f_in, *f_out, *f_name;
	char rename[200];
	char filename[200];
	f_list = fopen("D:\\Users\\Dsplab\\Desktop\\list.txt", "rb");
	f_out = fopen("out.bat","wb");
	int motion=1,speaker=1;

    while(fscanf(f_list,"%s",filename)!=EOF){

        sprintf(rename,"D:\\Users\\Dsplab\\Documents\\Chris_Kao\\TranningData\\MotionData\\sad\\full\\MOT_spk_%d%02d.lab",motion,speaker);
        fprintf(f_out,"copy %s %s\n",filename,rename);
        speaker++;
    }

    fclose(f_list);
    fclose(f_out);
    return 0;
}
