#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

char POS_CH[59][16] = { "X","A","Caa","Cab","Cba","Cbb","Da","Dfa","Dfb",
						"Di","Dk","D","Na","Nb","Nc","Ncd","Nd","Neu","Nes","Nep","Neqa","Neqb","Nf",
						"Ng","Nh","I","P","T","VA","VAC","VB_C","VC","VCL","VD","VE","VF","VG","VH","VHC","VI",
						"VJ","VK","VL","V_2","DE","SHI","FW","Nv","X","X","X","X","X","X","X","X","X","X","DM"};

/*typedef struct _symbol_item{
	char *bsym;
	char *asym;
	char btype;
	char atype;
}symbol_item;*/



typedef struct _transx_item {
	char ch[6];
	int code;
	int index;
	int pos;
	int bpindex;
	int bptype;
	int trash;
	char *bsym;
	char *asym;
	char btype;
	char atype;
	char ch_plus[20];
	int index_quotient;
	int	index_remainder;
} transx_item;

typedef struct _transx {
	int size1;
	transx_item *data;
} transx;

int transx_load_fn(char *fn, transx *transx_p)
{
	char ch[6];
	int code;
	int index;
	int pos;
	int bpindex;
	int bptype;
	int trash2;
	char *bsym;
	char *asym;
	char btype[3];
	char atype[3];

	//char ch_plus[20];
	int num;
	int index_quotient;
	int	index_remainder;
	int i,j;
	FILE *fp = fopen(fn, "r");
	if( !fp ) {
		fprintf(stderr, "Cannot open %s\n", fn);
		return 0;
	}
	transx_p->size1 = 0;
	while( fscanf(fp, "%s %d %d %d %d %d %d", ch, &code, &index, &pos, &bpindex, &bptype, &trash2) == 7 ) {
		transx_p->size1 ++;
	}
	transx_p->data = (transx_item *) calloc(transx_p->size1, sizeof(transx_item));
	if( transx_p->data==NULL ) {
		fprintf(stderr, "cannot locate memory\n");
	}
	rewind(fp);
	for(i=0;i<transx_p->size1;i++) {

		fscanf(fp, "%s %d %d %d %d %d %d", transx_p->data[i].ch, &(transx_p->data[i].code), &(transx_p->data[i].index), &(transx_p->data[i].pos), &(transx_p->data[i].bpindex), &(transx_p->data[i].bptype),&(transx_p->data[i].trash));
		transx_p->data[i].code = transx_p->data[i].code / 1000;
		transx_p->data[i].index_quotient = transx_p->data[i].index / 100;
		transx_p->data[i].index_remainder = transx_p->data[i].index % 100;
 	}
	fclose(fp);
	return 1;
}

void transx_free(transx *transx_p)
{
	if( transx_p->data ) {
		free(transx_p->data);
	}
}



typedef struct _label_item {
	int start;
	int end;
	char phoneme[10];
	char syl[10];
	int type;
	int scase;

	char *phoneme1;
	int i, num_token;
} label_item;

typedef struct _label {
	int size2;
	int size3;
	label_item *data;
} label;

void label_free(label *label_p)
{
	if( label_p->data ) {
		free(label_p->data);
	}
}


int label_load_fn(char *fn, label *label_p)
{
	int start;
	int end;
	char phoneme[20];
	char syl[20];
	int j;
	int k;
	int num_token =0 ;
	char* tmp = NULL;
	char tmp_str[100];
	char c[4][20];
	FILE *fp = fopen(fn, "r");
	if( !fp ) {
		fprintf(stderr, "Cannot open %s\n", fn);
		return 0;
	}
	label_p->size2 = 0;
	while((fgets(tmp_str, 500, fp)) != NULL ) {
		tmp = strtok(tmp_str ," ");
		while(tmp != NULL)
		{

			strcpy(c[num_token],tmp);
			tmp = strtok(NULL," ");
			num_token ++;
		}
		if (num_token == 3){
			label_p->size2 ++;
			num_token = 0;
		}
		else if (num_token == 4){
			label_p->size2 ++;
			num_token = 0;
		}

	}
	label_p->data = (label_item *) calloc(label_p->size2, sizeof(label_item));
	if( label_p->data==NULL ) {
		fprintf(stderr, "cannot locate memory\n");

	}
	rewind(fp);
	j = 0;
	k = 0;
	while((fgets(tmp_str, 500, fp)) != NULL ) {
		tmp = strtok(tmp_str ," ");
		while(tmp != NULL)
		{
			strcpy(c[num_token],tmp);
			tmp = strtok(NULL," ");
			num_token ++;
		}
		if (num_token == 3){
			label_p->data[j].start =atoi(c[0]);
			label_p->data[j].end =atoi(c[1]);
			strcpy(label_p->data[j].phoneme,c[2]);
			label_p->data[j].type = 0;
			j ++;
			num_token = 0;
		}
		else if (num_token == 4){
			label_p->data[j].start =atoi(c[0]);
			label_p->data[j].end =atoi(c[1]);
			strcpy(label_p->data[j].phoneme,c[2]);
			strcpy(label_p->data[j].syl,c[3]);
			label_p->data[j].type = 1;
			j ++;
			num_token = 0;
		}
	}
	fclose(fp);
	return 1;
}





int main(int argc,char *argv[])
{
	FILE *f_in, *f_out ,f_tra;

	int type = 0;
	char sym1[100];
	char sym2[100];
	int ph_index = 0, syl_index = 0, word_index = 0, phr_index = 0, sent_index = 0,s1 = 0, s2 = 0;
	int num_ph = 0, num_syl = 0, num_word = 0, num_phr = 0, num_sent = 0, scase = 0 ;
	char filename[20] = {};
	int i,k;
	int j,n;
	int z=0;
	int index = 0;
	int num = 0;
	int ph;
	//char fn[1024] = {"LSJstory01-01.transx"};
	//char transx_bdir[1024] = {"D:\\Users\\Dsplab\\Desktop\\labdata\\wordtransxbig5"};
	//char lab_bdir[1024] = {"D:\\Users\\Dsplab\\Desktop\\labdata\\newlab"};
	//char out_bdir[1024] = {"D:\\Users\\Dsplab\\Desktop\\labdata\\mul"};
	//char transx_fn[1024];
	//char lab_fn[1024];
	//char out_fn[1024];
	transx trx;
	label lab;


	//f_list = fopen("filename.txt", "rb");

	//while (fscanf(f_list, "%s", filename) != EOF)
	//{

			//sprintf(transx_fn, "%s\\[utf-8_to_big5]_%s.transx", transx_bdir, filename);
			//sprintf(lab_fn, "%s\\%s.lab", lab_bdir, filename);
			//sprintf(out_fn, "%s\\%s.mul", out_bdir, filename);
			f_out = fopen(argv[3], "wb");
			//symbol_item *sym = (symbol_item *)calloc(500, sizeof(symbol_item));
			if( !transx_load_fn(argv[1], &trx) ) {
			exit(1);
			}
			else if( !label_load_fn(argv[2], &lab) ) {
			exit(1);
			}
			/*for(j=0;j<lab.size2;j++) {
			fprintf(stdout, "%s \n", lab.data[j].phoneme);
		}*/


	/*for(j=0;j<lab.size2;j++)
	{
		if (!(strcmp(lab.data[j].phoneme, "sp"))){
				scase = 9998;
			}
		else if (!(strcmp(lab.data[j].phoneme, "sil"))){
				scase = 9999;
			}
		else
				scase = 0;
				//j++;
				//fprintf(f_out,"%d \n"  ,scase);
	}*/
		for(i=0;i<trx.size1;i++)
		{
				if(trx.data[i].code == 6)
					i;
				else
				{
					if(trx.data[i-1].code == 6 && trx.data[i+1].code != 6)
					{
						trx.data[i].bsym = trx.data[i-1].ch;
						trx.data[i].btype = 1;
						trx.data[i].asym = "";
						trx.data[i].atype = 0;
					}
					else if(trx.data[i-1].code != 6 && trx.data[i+1].code == 6)
					{
						trx.data[i].bsym = "";
						trx.data[i].btype = 0;
						trx.data[i].asym = trx.data[i+1].ch;
						trx.data[i].atype = 1;
					}
					else if(trx.data[i-1].code == 6 && trx.data[i+1].code == 6)
					{
						trx.data[i].bsym = trx.data[i-1].ch;
						trx.data[i].btype = 1;
						trx.data[i].asym = trx.data[i+1].ch;
						trx.data[i].atype = 1;
					}

					else
						{
						trx.data[i].bsym = "";
						trx.data[i].asym = "";
						trx.data[i].btype = 0;
						trx.data[i].atype = 0;
						}

					//fprintf(stdout,"%s %d %s %d %s\n", trx.data[i].ch,trx.data[i].btype,trx.data[i].bsym,trx.data[i].atype,trx.data[i].asym);
				}
		}


		for(i=0,j=0;j<lab.size2;j++)
		{
			if(lab.data[j].type == 0)
				{
					fprintf(f_out,"%d %d %s",lab.data[j].start,lab.data[j].end,lab.data[j].phoneme);

				}
			else
			{
				for(;;i++)
					if(trx.data[i].code !=6)
						break;
						if(trx.data[i].index_quotient != 1)
						{
							if(trx.data[i].index_remainder == 1 )
							{
							fprintf(f_out,"%d %d %s %d ", lab.data[j].start,lab.data[j].end,lab.data[j].phoneme,trx.data[i].code);
							for(n=0;n<trx.data[i].index_quotient;n++)
								fprintf(f_out,"%s",trx.data[i+n].ch);
							if(trx.data[i+2].code == 6)
								fprintf(f_out," 1 %s %d %s %d %s\n",POS_CH[trx.data[i].pos],trx.data[i+1].btype,trx.data[i+1].bsym,trx.data[i+1].atype,trx.data[i+1].asym);
							else
							fprintf(f_out," 1 %s %d %s %d %s\n",POS_CH[trx.data[i].pos],trx.data[i].btype,trx.data[i].bsym,trx.data[i].atype,trx.data[i].asym);
							}
							else
								fprintf(f_out,"%d %d %s %d\n", lab.data[j].start,lab.data[j].end,lab.data[j].phoneme,trx.data[i].code);
						}

						else
							fprintf(f_out,"%d %d %s %d %s 1 %s %d %s %d %s\n",lab.data[j].start,lab.data[j].end,lab.data[j].phoneme,trx.data[i].code,trx.data[i].ch,POS_CH[trx.data[i].pos],trx.data[i].btype,trx.data[i].bsym,trx.data[i].atype,trx.data[i].asym);

				i++;


		}

	}


		fclose(f_out);
		//free(sym);
	//}

		transx_free(&trx);
		label_free(&lab);
		//fclose(f_list);
		//system("pause");
		return 0;
}
	/*while (fscanf(f_list, "%s", filename) != EOF){

		sprintf(transx_fn, "%s\\%s.transx", transx_bdir, filename);

		if( !transx_load_fn(transx_fn, &trx) ) {
			exit(1);
		}

		for(i=0;i<trx.size1;i++) {
			fprintf(stdout, "%d\n", trx.data[i].code);
		}

	//}

		//f_list = fopen("list.txt", "rb");
	/*while (fscanf(f_list, "%s", filename) != EOF){

		sprintf(lab_fn, "%s\\%s.lab", lab_bdir, filename);

		if( !label_load_fn(lab_fn, &lab) ) {
			exit(1);
		}

		for(j=0;j<lab.size2;j++) {
			fprintf(stdout, "%s\n", lab.data[j].phoneme);
		}*/


