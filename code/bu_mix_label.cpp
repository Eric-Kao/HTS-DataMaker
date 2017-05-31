#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <io.h>

#define PHONE_SIZE 5
#define POS_SIZE 5
#define TIME_SIZE 12



typedef struct _sentence
{
	int sent_phr_index;
	int num_syl;
	int num_word;
	int num_phr;

}SENTENCE;

typedef struct _phrase
{
	int phr_word_index;
	int belong_sent;
	int bef_num_syl;
	int bef_num_word;
	int cur_num_syl;
	int cur_num_word;
	int fd_pos_in_utter;
	int bd_pos_in_utter;
	int next_num_syl;
	int next_num_word;

}PHRASE;

typedef struct _word
{
	int word_syl_index;
	int belong_phr;
	char *bef_word_POS1;
	char *bef_word_POS2;
	int bef_num_syl;
	char *cur_word_POS1;
	char *cur_word_POS2;
	int cur_num_syl;
	int fd_pos_in_phr;
	int bd_pos_in_phr;
	char *next_word_POS1;
	char *next_word_POS2;
	int next_num_syl;

}WORD;

typedef struct _syllable
{
	int syl_ph_index;
	char ch_or_en;
	int belong_word;
	char bef_stress;
	int bef_num_phone;//cur_syl_ph_index - bef_syl_ph_index
	char cur_stress;
	int cur_num_phone;//next_syl_ph_index(num_ph) - cur_syl_ph_index
	int fd_pos_in_word;//f_pos = cur_syl_index - cur_word_syl_index + 1
	int bd_pos_in_word;//b_pos = next_word_syl_index(num_syl) - cur_syl_index
	int fd_pos_in_phr;//f_pos = cur_syl_index - word[cur_phr_word_index].word_syl_index + 1
	int bd_pos_in_phr;//b_pos = word[next_phr_word_index].word_syl_index - cur_syl_index
	int num_stress_bef_in_phr;
	int num_stress_aft_in_phr;
	int num_syl_bef2_stress;
	int num_syl_aft2_stress;
	char *name_vowel;
	char next_stress;
	int next_num_phone;//next_next_syl_ph_index(num_ph) - next_syl_ph_index

}SYLLABLE;

typedef struct _phone
{
	int belong_syl;
	char *cur_phone;
	char *bef_bef_phone;
	char *bef_phone;
	char *next_phone;
	char *next_next_phone;
	int fd_pos_in_syl;//f_pos = cur_phone_index - cur_syl_ph_index + 1
	int bd_pos_in_syl;//b_pos = next_syl_ph_index(num_ph) - cur_phone_index
	char *dur_start;
	char *dur_end;

}PHONE;

int StringType(char tmp_str[100]);
void InitialPhone(int ph_index, PHONE *ph);
void InitialSyllable(int syl_index, SYLLABLE *syl);
void InitialWord(int word_index, WORD *word);
int GetPhone(char tmp_str[100], int ph_index, PHONE *ph, int cur_syl_index);
void GetSyllable(char tmp_str[100], int ph_index, int syl_index, SYLLABLE *syl, int cur_word_index,PHONE *ph);
void GetWord(char tmp_str[100], int syl_index, int word_index, WORD *word, int cur_phr_index);
int GetPhrase(char tmp_str[100], int word_index, int phr_index, PHRASE *phr, int cur_sent_index);
int GetSentence(char tmp_str[100], int phr_index, int sent_index, SENTENCE *sent);

void PhoneProcess(int ph_index, int num_ph, int num_syl, PHONE *ph, SYLLABLE *syl);

void SyllableProcess(int syl_index, int num_ph, int num_syl, int num_word, int num_phr, PHONE *ph, SYLLABLE *syl, WORD *word, PHRASE *phr);
void CntNumPhone(int syl_index, int num_ph, int num_syl, SYLLABLE *syl);
int CntStressBefSyl(int cur_phr_word_syl_index, int cur_syl_index, SYLLABLE *syl);
int CntStressAftSyl(int next_phr_word_syl_index, int cur_syl_index, SYLLABLE *syl);
int CntSylBefToStress(int cur_syl_index, SYLLABLE *syl);
int CntSylAftToStress(int cur_syl_index, int max_syl, SYLLABLE *syl);
int FindVowel(int cur_syl_index, PHONE *ph, SYLLABLE *syl);

void WordProcess(int word_index, int num_syl, int num_word, int num_phr, WORD *word, PHRASE *phr);
void CntNumSylInWord(int word_index, int num_syl, int num_word, WORD *word);
void POSBefWord(int word_index, WORD *word);
void POSNextWord(int word_index, int num_word, WORD *word);

void PhraseProcess(int phr_index, int num_syl, int num_word, int num_phr, WORD *word, PHRASE *phr);
void CntNumSylInPhr(int phr_index, int num_syl, int num_phr, WORD *word, PHRASE *phr);
void CntNumWordInPhr(int phr_index, int num_word, int num_phr, PHRASE *phr);

void SentenceProcess(int sent_index, int num_syl, int num_word, int num_phr, int num_sent, WORD *word, PHRASE *phr, SENTENCE *sent);
void CntNumSylInSent(int sent_index, int num_syl, int num_sent, WORD *word, PHRASE *phr, SENTENCE *sent);
void CntNumWordInSent(int sent_index, int num_word, int num_phr, int num_sent, PHRASE *phr, SENTENCE *sent);
void CntNumPhr(int sent_index, int num_phr, int num_sent, SENTENCE *sent);

void OutputLabel(int num_ph, PHONE *ph, SYLLABLE *syl, WORD *word, PHRASE *phr, SENTENCE *sent);

char pun_mark[4][4]={
	{-17, -68, -116, },/**1  ¡A**/
	{-29, -128, -126, },/**3  ¡C**/
	{-17, -68, -97, },/**8  ¡H**/
	{-17, -68, -127, },/**9  ¡I**/
};



int main()
{
	
	PHONE *ph = (PHONE *)calloc(10000, sizeof(PHONE));
	SYLLABLE *syl = (SYLLABLE *)calloc(10000, sizeof(SYLLABLE));
	WORD *word = (WORD *)calloc(10000, sizeof(WORD));
	PHRASE *phr = (PHRASE *)calloc(1000, sizeof(PHRASE));
	SENTENCE *sent = (SENTENCE *)calloc(1000, sizeof(SENTENCE));

	int type = 0;
	int ph_index = 0, syl_index = 0, word_index = 0, phr_index = 0, sent_index = 0;
	int num_ph = 0, num_syl = 0, num_word = 0, num_phr = 0, num_sent = 0;
	char tmp_str[1000] = {};


	//printf("beginning make label...\n");

	/*initial*/
	ph_index = 0, syl_index = 0, word_index = 0, phr_index = 0, sent_index = 0;
	num_ph = 0, num_syl = 0, num_word = 0, num_phr = 0, num_sent = 0;
	phr[phr_index].phr_word_index = 0;
	phr_index++;
	sent[sent_index].sent_phr_index = 0;
	sent_index++;
	setmode(fileno(stdout), O_BINARY);
	while ((fgets(tmp_str, 1024, stdin)) != NULL){
		type = StringType(tmp_str);
		switch (type){
		case 0:
			InitialPhone(ph_index, ph);
			ph_index = GetPhone(tmp_str, ph_index, ph, syl_index - 1);
			break;
		case 1:
			InitialPhone(ph_index, ph);
			ph_index = GetPhone(tmp_str, ph_index, ph, syl_index);
			
			InitialSyllable(syl_index, syl); 
			GetSyllable(tmp_str, ph_index, syl_index, syl, word_index - 1,ph);
			syl_index = syl_index + 1;

			break;
		case 2:
			InitialPhone(ph_index, ph);
			ph_index = GetPhone(tmp_str, ph_index, ph, syl_index);

			InitialSyllable(syl_index, syl);
			GetSyllable(tmp_str, ph_index, syl_index, syl, word_index,ph);
			syl_index = syl_index + 1;

			InitialWord(word_index, word);
			GetWord(tmp_str, syl_index, word_index, word, phr_index - 1);
			word_index = word_index + 1;

			phr_index = GetPhrase(tmp_str, word_index, phr_index, phr, sent_index-1);

			sent_index = GetSentence(tmp_str, phr_index, sent_index, sent);
			break;
		case 3:
			break;
		}
	}
	
	/*check error*/
	//for(i = 0; i < ph_index; i++){
	//    printf("ph_index:%d start:%s end:%s\n", i, ph[i].dur_start, ph[i].dur_end);
	//}
	/*check syl_ph_index*/
	//for(i = 0; i < syl_index; i++){
	//    printf("syl_index:%d syl_ph_index:%d\n", i, syl[i].syl_ph_index);
	//}
	/*check word_syl_index*/
	//for(i = 0; i < word_index; i++){
	//    printf("word_syl_index\n");
	//    printf("word_index:%d word_syl_index:%d\n", i, word[i].word_syl_index);
	//}
	/*check phr_word_index*/
	//for(i = 0; i < phr_index; i++){
	//    printf("phr_word_index\n");
	//    printf("phr_index:%d phr_word_index:%d\n", i, phr[i].phr_word_index);
	//}
	/*check phr_word_index*/
	//for(i = 0; i < sent_index; i++){
	//    printf("sent_phr_index\n");
	//    printf("sent_index:%d sent_phr_index:%d\n", i, sent[i].sent_phr_index);
	//}

	num_ph = ph_index;
	ph_index = 0;
	num_syl = syl_index;
	syl_index = 0;
	num_word = word_index;
	word_index = 0;
	num_phr = phr_index - 1;
	phr_index = 0;
	num_sent = sent_index - 1;
	sent_index = 0;

	/*for (i = 0; i < num_word; i++){
		printf("word[%d]_belong_phr:%d\n", i, word[i].belong_phr);
	}

	for (i = 0; i < num_phr; i++){
		printf("phr[%d]_belong_sent:%d\n", i, phr[i].belong_sent);
	}*/

	//printf("num_ph:%d num_syl:%d num_word:%d num_phr:%d num_sent:%d\n", num_ph, num_syl, num_word, num_phr, num_sent);

	/*phone process*/
	while (ph_index < num_ph){
		PhoneProcess(ph_index, num_ph, num_syl, ph, syl);
		ph_index++;
	}

	/*syllable process*/
	while (syl_index < num_syl){
		SyllableProcess(syl_index, num_ph, num_syl, num_word, num_phr, ph, syl, word, phr);
		syl_index++;
	}

	/*word process*/
	while (word_index < num_word){
		WordProcess(word_index, num_syl, num_word, num_phr, word, phr);
		word_index++;
	}

	/*phrase process*/
	while (phr_index < num_phr){
		PhraseProcess(phr_index, num_syl, num_word, num_phr, word, phr);
		phr_index++;
	}

	/*sentence process*/
	while (sent_index < num_sent){
		SentenceProcess(sent_index, num_syl, num_word, num_phr, num_sent, word, phr, sent);
		sent_index++;
	}

	OutputLabel(num_ph, ph, syl, word, phr, sent);

	free(ph);
	free(syl);
	free(word);
	free(phr);
	free(sent);

	//system("pause");

	return 0;
}

void InitialPhone(int ph_index, PHONE *ph)
{
	ph[ph_index].dur_start = (char *)calloc(20, sizeof(char));
	ph[ph_index].dur_end = (char *)calloc(20, sizeof(char));
	ph[ph_index].cur_phone = (char *)calloc(20, sizeof(char));

}

void InitialSyllable(int syl_index, SYLLABLE *syl)
{
	syl[syl_index].name_vowel = (char *)calloc(20, sizeof(char));
}

void InitialWord(int word_index, WORD *word)
{

	word[word_index].cur_word_POS1 = (char *)calloc(20, sizeof(char));
	word[word_index].cur_word_POS2 = (char *)calloc(20, sizeof(char));

}

int StringType(char tmp_str[100])
{
	int i, segment = 0;
	//if(tmp_str[0]=='\n')
	if(strcmp(tmp_str,"\n")==0)
		return 3;

	for (i = 0; i < strlen(tmp_str); i++){
		if (tmp_str[i] == ' ')
			segment++;
	}
	if (segment == 2)
		return 0;
	else if (segment == 3)
		return 1;
	else
		return 2;
}

int GetPhone(char str[100], int ph_index, PHONE *ph, int cur_syl_index)
{
	char tmp_str[100];
	char *sub_str;
	int p;


	strcpy(tmp_str, str);
	sub_str = strtok(tmp_str, " \n\r");
	strcpy(ph[ph_index].dur_start, sub_str);
	sub_str = strtok(NULL, " \n\r");
	strcpy(ph[ph_index].dur_end, sub_str);

	if (!strcmp(ph[ph_index].dur_start, ph[ph_index].dur_end)){
		return ph_index;
	}

	sub_str = strtok(NULL, " \n\r");
	//printf("sub_str:%s\n",sub_str);
	strcpy(ph[ph_index].cur_phone, sub_str);
	if (strcmp(ph[ph_index].cur_phone, "sil") && strcmp(ph[ph_index].cur_phone, "sp")){
		ph[ph_index].belong_syl = cur_syl_index;
	}
	else if (strcmp(ph[ph_index].cur_phone, "sil")){
		ph[ph_index].belong_syl = 9998;
	}
	else
		ph[ph_index].belong_syl = 9999;
	ph_index++;
	return ph_index;


}

void GetSyllable(char str[100], int ph_index, int syl_index, SYLLABLE *syl, int cur_word_index,PHONE *ph)
{
	char tmp_str[100];
	char *sub_str;
	int p,i;

	syl[syl_index].belong_word = cur_word_index;
	strcpy(tmp_str, str);
	sub_str = strtok(tmp_str, " \n\r");
	for (i = 0; i < 3; i++)
	sub_str = strtok(NULL, " \n\r");
	syl[syl_index].cur_stress = sub_str[0];
	syl[syl_index].syl_ph_index = ph_index - 1;

	for(p=0;p < sizeof(ph[syl[syl_index].syl_ph_index]);p++)
	{
		if(ph[syl[syl_index].syl_ph_index].cur_phone[p] == '_')
		{
			syl[syl_index].ch_or_en = 'c';
			break;
		}
		else
			syl[syl_index].ch_or_en = 'e';
	}
}

void GetWord(char str[100], int syl_index, int word_index, WORD *word, int cur_phr_index)
{
	int i;
	char tmp_str[100];
	char *sub_str;

	word[word_index].belong_phr = cur_phr_index;

	strcpy(tmp_str, str);
	sub_str = strtok(tmp_str, " \n\r");
	for (i = 0; i < 5; i++){
		sub_str = strtok(NULL, " \n\r");
	}
	if (sub_str[0] == '1'){
		sub_str = strtok(NULL, " \n\r");
		strcpy(word[word_index].cur_word_POS1, sub_str);
		strcpy(word[word_index].cur_word_POS2, "x");
	}
	else if (sub_str[0] == '2')
	{
		sub_str = strtok(NULL, " \n\r");
		strcpy(word[word_index].cur_word_POS1, sub_str);
		sub_str = strtok(NULL, " \n\r");
		strcpy(word[word_index].cur_word_POS2, sub_str);
	}
	word[word_index].word_syl_index = syl_index - 1;

}

int GetPhrase(char str[1000], int word_index, int phr_index, PHRASE *phr, int cur_sent_index)
{
	int i;
	char tmp_str[1000];
	char *sub_str;


	strcpy(tmp_str, str);
	sub_str = strtok(tmp_str, " \n\r");
	for (i = 0; i < 7; i++){
		sub_str = strtok(NULL, " \n\r");
	}
	if (sub_str[0] == '0'){
		sub_str = strtok(NULL, " \n\r");
		if (sub_str[0] == '1'){
			sub_str = strtok(NULL, " \n\r");
			if (!(strcmp(sub_str , "¡A"))|| !(strcmp(sub_str , "¡C"))||!(strcmp(sub_str , "¡I"))||!(strcmp(sub_str , "¡H"))){
				phr[phr_index].belong_sent = cur_sent_index;
				phr[phr_index].phr_word_index = word_index;
				phr_index++;
			}
		}
	}
	else if (sub_str[0] == '1')
	{
		for (i = 0; i < 2; i++){
			sub_str = strtok(NULL, " \n\r");
		}
		if (sub_str[0] == '1'){
			sub_str = strtok(NULL, " \n\r");
			if (!(strcmp(sub_str , "¡A"))|| !(strcmp(sub_str , "¡C"))||!(strcmp(sub_str , "¡I"))||!(strcmp(sub_str , "¡H"))){
				phr[phr_index].belong_sent = cur_sent_index;
				phr[phr_index].phr_word_index = word_index;
				phr_index++;
			}
		}
	}
	return phr_index;
}

int GetSentence(char str[100], int phr_index, int sent_index, SENTENCE *sent)
{
	int i;
	char tmp_str[100];
	char *sub_str;

	strcpy(tmp_str, str);
	sub_str = strtok(tmp_str, " \n\r");
	for (i = 0; i < 7; i++){
		sub_str = strtok(NULL, " \n\r");
	}
	if (sub_str[0] == '0'){
		sub_str = strtok(NULL, " \n\r");
		if (sub_str[0] == '1'){
			sub_str = strtok(NULL, " \n\r");
			if (!(strcmp(sub_str , "¡C"))||!(strcmp(sub_str , "¡I"))||!(strcmp(sub_str , "¡H"))){
				sent[sent_index].sent_phr_index = phr_index;
				sent_index++;
			}
		}
	}
	else if (sub_str[0] == '1')
	{
		for (i = 0; i < 2; i++){
			sub_str = strtok(NULL, " \n\r");
		}
		if (sub_str[0] == '1'){
			sub_str = strtok(NULL, " \n\r");
			if (!(strcmp(sub_str , "¡C"))||!(strcmp(sub_str , "¡I"))||!(strcmp(sub_str , "¡H"))){
				sent[sent_index].sent_phr_index = phr_index;
				sent_index++;
			}
		}
	}
	return sent_index;
}

void PhoneProcess(int ph_index, int num_ph, int num_syl, PHONE *ph, SYLLABLE *syl)
{
	int cur_syl_index = 0;
	int next_syl_index = 0;

	cur_syl_index = ph[ph_index].belong_syl;
	next_syl_index = cur_syl_index + 1;

	/*when phone is "sil"*/


	/*bef_bef_phone*/
	if ((ph_index - 2) < 0)
		ph[ph_index].bef_bef_phone = "x";
	else
		ph[ph_index].bef_bef_phone = ph[ph_index - 2].cur_phone;
	/*bef_phone*/
	if ((ph_index - 1) < 0)
		ph[ph_index].bef_phone = "x";
	else
		ph[ph_index].bef_phone = ph[ph_index - 1].cur_phone;
	/*next_phone*/
	if ((ph_index + 1) >= num_ph)
		ph[ph_index].next_phone = "x";
	else
		ph[ph_index].next_phone = ph[ph_index + 1].cur_phone;
	/*next_next_phone*/
	if ((ph_index + 2) >= num_ph)
		ph[ph_index].next_next_phone = "x";
	else
		ph[ph_index].next_next_phone = ph[ph_index + 2].cur_phone;

	if (cur_syl_index == 9999|| cur_syl_index == 9998){
		ph[ph_index].fd_pos_in_syl = 0;
		ph[ph_index].bd_pos_in_syl = 0;
	}
	else{
		/*fd_pos_in_syl*/
		ph[ph_index].fd_pos_in_syl = ph_index - (syl[cur_syl_index].syl_ph_index) + 1;
		/*bd_pos_in_syl*/
		if ((next_syl_index) < num_syl){
			if (strcmp(ph[syl[next_syl_index].syl_ph_index - 1].cur_phone, "sil") && strcmp(ph[syl[next_syl_index].syl_ph_index - 1].cur_phone, "sp"))
				ph[ph_index].bd_pos_in_syl = (syl[next_syl_index].syl_ph_index) - ph_index;
			else
				ph[ph_index].bd_pos_in_syl = (syl[next_syl_index].syl_ph_index) - ph_index - 1;
		}
		else if (strcmp(ph[num_ph - 1].cur_phone, "sil") && strcmp(ph[num_ph - 1].cur_phone, "sp"))
			ph[ph_index].bd_pos_in_syl = num_ph - ph_index;
		else
			ph[ph_index].bd_pos_in_syl = num_ph - ph_index - 1;
	}



}

void SyllableProcess(int syl_index, int num_ph, int num_syl, int num_word, int num_phr, PHONE *ph, SYLLABLE *syl, WORD *word, PHRASE *phr)
{
	int cur_word_index = 0;
	int cur_phr_index = 0;
	int tmp_vowel_index = 0;

	cur_word_index = syl[syl_index].belong_word;
	cur_phr_index = word[cur_word_index].belong_phr;

	/*num_phone*/
	CntNumPhone(syl_index, num_ph, num_syl, syl);

	/*bef_stress*/
	if ((syl_index - 1) < 0)
		syl[syl_index].bef_stress = 'x';
	else
		syl[syl_index].bef_stress = syl[syl_index - 1].cur_stress;

	/*next_stress*/
	if ((syl_index + 1) >= num_syl)
		syl[syl_index].next_stress = 'x';
	else
		syl[syl_index].next_stress = syl[syl_index + 1].cur_stress;

	/*bef_num_phone*/
	if ((syl_index - 1) < 0)
		syl[syl_index].bef_num_phone = 0;
	else
		syl[syl_index].bef_num_phone = syl[syl_index - 1].cur_num_phone;

	/*next_num_phone*/
	if ((syl_index + 1) >= num_syl)
		syl[syl_index].next_num_phone = 0;
	else
		syl[syl_index].next_num_phone = syl[syl_index + 1].cur_num_phone;

	/*fd_pos_in_word*/
	syl[syl_index].fd_pos_in_word = syl_index - (word[cur_word_index].word_syl_index) + 1;

	/*bd_pos_in_word*/
	if ((cur_word_index + 1) < num_word){
		syl[syl_index].bd_pos_in_word = (word[cur_word_index + 1].word_syl_index) - syl_index;
	}
	else{
		syl[syl_index].bd_pos_in_word = num_syl - syl_index;
	}

	/*fd_pos_in_phr*/
	syl[syl_index].fd_pos_in_phr = syl_index - word[phr[cur_phr_index].phr_word_index].word_syl_index + 1;

	/*bd_pos_in_phr*/
	if ((cur_phr_index + 1) < num_phr)
		syl[syl_index].bd_pos_in_phr = (word[phr[cur_phr_index + 1].phr_word_index].word_syl_index) - syl_index;
	else
		syl[syl_index].bd_pos_in_phr = num_syl - syl_index;

	/*num_stress_bef_in_phr*/
	syl[syl_index].num_stress_bef_in_phr = CntStressBefSyl(word[phr[cur_phr_index].phr_word_index].word_syl_index, syl_index, syl);

	/*num_stress_aft_in_phr*/
	if ((cur_phr_index + 1) < num_phr)
		syl[syl_index].num_stress_aft_in_phr = CntStressAftSyl(word[phr[cur_phr_index + 1].phr_word_index].word_syl_index, syl_index, syl);
	else
		syl[syl_index].num_stress_aft_in_phr = CntStressAftSyl(num_syl, syl_index, syl);

	/*num_syl_bef2_stress*/
	syl[syl_index].num_syl_bef2_stress = CntSylBefToStress(syl_index, syl);

	/*num_syl_aft2_stress*/
	syl[syl_index].num_syl_aft2_stress = CntSylAftToStress(syl_index, num_syl, syl);

	/*name_vowel*/
	tmp_vowel_index = FindVowel(syl_index, ph, syl);
	if (tmp_vowel_index != -1)
		strcpy(syl[syl_index].name_vowel, ph[tmp_vowel_index].cur_phone);
	else
		strcpy(syl[syl_index].name_vowel, "novowel");

}

void CntNumPhone(int syl_index, int num_ph, int num_syl, SYLLABLE *syl)
{
	int i;

	if (syl_index != 0)
		return;

	for (i = 0; i < num_syl; i++){
		if ((i + 1) == num_syl){
			syl[i].cur_num_phone = num_ph - syl[i].syl_ph_index;
		}
		else
			syl[i].cur_num_phone = syl[i + 1].syl_ph_index - syl[i].syl_ph_index;
	}


}

int CntStressBefSyl(int cur_phr_word_syl_index, int cur_syl_index, SYLLABLE *syl)
{
	int num_stress = 0;
	int i;

	for (i = cur_phr_word_syl_index; i < cur_syl_index; i++){
		if (syl[i].cur_stress != '6' && syl[i].cur_stress == '1')
			num_stress++;
	}

	return num_stress;
}

int CntStressAftSyl(int next_phr_word_syl_index, int cur_syl_index, SYLLABLE *syl)
{
	int num_stress = 0;
	int i;

	for (i = cur_syl_index + 1; i < next_phr_word_syl_index; i++){
		if (syl[i].cur_stress != '6' && syl[i].cur_stress == '1')
			num_stress++;
	}

	return num_stress;
}

int CntSylBefToStress(int cur_syl_index, SYLLABLE *syl)
{
	int num_syl = 0;//if check_stress = 0 mean before syllable no stressed syllable, otherwise check_stress = 1

	if (cur_syl_index == 0 && syl[cur_syl_index].ch_or_en == 'e')
		return 0;
	else if (cur_syl_index == 0 && syl[cur_syl_index].ch_or_en == 'c')
		return 'x';
	else
		cur_syl_index -= 1;

	while (cur_syl_index >= 0){
		num_syl++;

		if (syl[cur_syl_index].cur_stress == '7')
			return num_syl;

		cur_syl_index--;
	}
	return 0;
}

int CntSylAftToStress(int cur_syl_index, int max_syl, SYLLABLE *syl)
{
	int num_syl = 0;

	if (cur_syl_index == max_syl - 1 && syl[cur_syl_index].ch_or_en == 'c')
		return 'x';
	else if (cur_syl_index == max_syl - 1 && syl[cur_syl_index].ch_or_en == 'e')
		return 0;
	else
		cur_syl_index += 1;

	while (cur_syl_index < max_syl){
		num_syl++;
		if (syl[cur_syl_index].cur_stress == '7')
			return num_syl;
		cur_syl_index++;
	}
	return 0;
}

int FindVowel(int cur_syl_index, PHONE *ph, SYLLABLE *syl)
{
	int syl_ph_index = 0;
	int num_ph = 0;
	char vowel_list[] = "ae aa ah ao aw ay eh ey er iy ih ow oy uh uw n_a n_ai n_ao n_e n_eh n_ei n_er n_FNULL1 n_FNULL2 n_o n_ou n_wu n_yi n_yu";
	char tmp_list[300] = {};
	char *tmp_vowel = NULL;

	syl_ph_index = syl[cur_syl_index].syl_ph_index;//the start phone in syllable
	num_ph = syl[cur_syl_index].cur_num_phone;//number of phone in syllable
	//printf("syl_ph_index:%d num_ph:%d\n", syl_ph_index, num_ph);
	while (num_ph > 0){
		strcpy(tmp_list, vowel_list);
		tmp_vowel = strtok(tmp_list, " ");
		while (tmp_vowel != NULL){
			//printf("cur_ph:%s tmp_vowel:%s\n", ph[syl_ph_index].cur_phone, tmp_vowel);
			if (!strcmp(ph[syl_ph_index].cur_phone, tmp_vowel))
				return syl_ph_index;
			tmp_vowel = strtok(NULL, " ");
		}
		syl_ph_index++;
		num_ph--;
	}

	return -1;
}

void WordProcess(int word_index, int num_syl, int num_word, int num_phr, WORD *word, PHRASE *phr)
{
	int cur_phr_index = 0;

	cur_phr_index = word[word_index].belong_phr;

	/*POS bef_word*/
	POSBefWord(word_index, word);

	/*POS next_word*/
	POSNextWord(word_index, num_word, word);

	/*count number of syllable in word*/
	CntNumSylInWord(word_index, num_syl, num_word, word);

	/*count num_syl_bef_word*/
	if ((word_index - 1) < 0)
		word[word_index].bef_num_syl = 0;
	else{
		word[word_index].bef_num_syl = word[word_index - 1].cur_num_syl;
	}


	/*count num_syl_next_word*/
	if ((word_index + 1) >= num_word)
		word[word_index].next_num_syl = 0;
	else{
		word[word_index].next_num_syl = word[word_index + 1].cur_num_syl;
	}


	/*fd_pos_in_phr*/
	word[word_index].fd_pos_in_phr = word_index - (phr[cur_phr_index].phr_word_index) + 1;

	/*bd_pos_in_phr*/
	if ((cur_phr_index + 1) < num_phr){
		word[word_index].bd_pos_in_phr = (phr[cur_phr_index + 1].phr_word_index) - word_index;
	}
	else{
		word[word_index].bd_pos_in_phr = num_word - word_index;
	}


}

void CntNumSylInWord(int word_index, int num_syl, int num_word, WORD *word)
{
	int i;

	if (word_index != 0)
		return;

	for (i = 0; i < num_word; i++){
		if ((i + 1) == num_word){
			word[i].cur_num_syl = num_syl - word[i].word_syl_index;
		}
		else{
			word[i].cur_num_syl = word[i + 1].word_syl_index - word[i].word_syl_index;
		}
	}




}

void POSBefWord(int word_index, WORD *word)
{
	/*POS1 bef_word*/
	if ((word_index - 1) < 0)
		word[word_index].bef_word_POS1 = "x";
	else
		word[word_index].bef_word_POS1 = word[word_index - 1].cur_word_POS1;

	/*POS2 bef_word*/
	if ((word_index - 1) < 0)
		word[word_index].bef_word_POS2 = "x";
	else
		word[word_index].bef_word_POS2 = word[word_index - 1].cur_word_POS2;
}

void POSNextWord(int word_index, int num_word, WORD *word)
{
	/*POS1 next_word*/
	if ((word_index + 1) >= num_word)
		word[word_index].next_word_POS1 = "x";
	else
		word[word_index].next_word_POS1 = word[word_index + 1].cur_word_POS1;

	/*POS2 next_word*/
	if ((word_index + 1) >= num_word)
		word[word_index].next_word_POS2 = "x";
	else
		word[word_index].next_word_POS2 = word[word_index + 1].cur_word_POS2;
}

void PhraseProcess(int phr_index, int num_syl, int num_word, int num_phr, WORD *word, PHRASE *phr)
{
	/*num_syl_cur_phr*/
	CntNumSylInPhr(phr_index, num_syl, num_phr, word, phr);

	/*num_syl_bef_phr*/
	if ((phr_index - 1) < 0)
		phr[phr_index].bef_num_syl = 0;
	else{
		phr[phr_index].bef_num_syl = phr[phr_index - 1].cur_num_syl;
	}

	/*num_syl_next_phr*/
	if ((phr_index + 1) >= num_phr)
		phr[phr_index].next_num_syl = 0;
	else{
		phr[phr_index].next_num_syl = phr[phr_index + 1].cur_num_syl;
	}

	/*num_word_cur_phr*/
	CntNumWordInPhr(phr_index, num_word, num_phr, phr);

	/*num_word_bef_phr*/
	if ((phr_index - 1) < 0)
		phr[phr_index].bef_num_word = 0;
	else{
		phr[phr_index].bef_num_word = phr[phr_index - 1].cur_num_word;
	}

	/*num_word_next_phr*/
	if ((phr_index + 1) >= num_phr)
		phr[phr_index].next_num_word = 0;
	else{
		phr[phr_index].next_num_word = phr[phr_index + 1].cur_num_word;
	}

	/*fd_pos_in_utter*/
	phr[phr_index].fd_pos_in_utter = phr_index + 1;

	/*bd_pos_in_utter*/
	phr[phr_index].bd_pos_in_utter = num_phr - phr_index;

}

void CntNumSylInPhr(int phr_index, int num_syl, int num_phr, WORD *word, PHRASE *phr)
{
	int i;
	int cur_phr_word_index = 0, next_phr_word_index = 0;

	if (phr_index != 0)
		return;

	for (i = 0; i < num_phr; i++){
		if ((i + 1) == num_phr){
			cur_phr_word_index = phr[i].phr_word_index;
			phr[i].cur_num_syl = num_syl - word[cur_phr_word_index].word_syl_index;
		}
		else{
			cur_phr_word_index = phr[i].phr_word_index;
			next_phr_word_index = phr[i + 1].phr_word_index;
			phr[i].cur_num_syl = word[next_phr_word_index].word_syl_index - word[cur_phr_word_index].word_syl_index;
		}
	}
}

void CntNumWordInPhr(int phr_index, int num_word, int num_phr, PHRASE *phr)
{
	int i;

	if (phr_index != 0)
		return;

	for (i = 0; i < num_phr; i++){
		if ((i + 1) == num_phr){
			phr[i].cur_num_word = num_word - phr[i].phr_word_index;
		}
		else{
			phr[i].cur_num_word = phr[i + 1].phr_word_index - phr[i].phr_word_index;
		}
	}
}

void SentenceProcess(int sent_index, int num_syl, int num_word, int num_phr, int num_sent, WORD *word, PHRASE *phr, SENTENCE *sent)
{
	/*num_syl*/
	CntNumSylInSent(sent_index, num_syl, num_sent, word, phr, sent);

	/*num_word*/
	CntNumWordInSent(sent_index, num_word, num_phr, num_sent, phr, sent);

	/*num_phr*/
	CntNumPhr(sent_index, num_phr, num_sent, sent);
}

void CntNumSylInSent(int sent_index, int num_syl, int num_sent, WORD *word, PHRASE *phr, SENTENCE *sent)
{
	int i;
	int cur_sent_phr_word_index = 0, next_sent_phr_word_index = 0;

	if (sent_index != 0)
		return;

	for (i = 0; i < num_sent; i++){
		if ((i + 1) == num_sent){
			cur_sent_phr_word_index = phr[sent[i].sent_phr_index].phr_word_index;
			sent[i].num_syl = num_syl - word[cur_sent_phr_word_index].word_syl_index;
		}
		else{
			cur_sent_phr_word_index = phr[sent[i].sent_phr_index].phr_word_index;
			next_sent_phr_word_index = phr[sent[i + 1].sent_phr_index].phr_word_index;
			sent[i].num_syl = word[next_sent_phr_word_index].word_syl_index - word[cur_sent_phr_word_index].word_syl_index;
		}
	}
}

void CntNumWordInSent(int sent_index, int num_word, int num_phr, int num_sent, PHRASE *phr, SENTENCE *sent)
{
	int i;
	int cur_sent_phr_index = 0, next_sent_phr_index = 0;

	if (sent_index != 0)
		return;

	for (i = 0; i < num_sent; i++){
		if ((i + 1) == num_sent){
			cur_sent_phr_index = sent[i].sent_phr_index;
			sent[i].num_word = num_word - phr[cur_sent_phr_index].phr_word_index;
		}
		else{
			cur_sent_phr_index = sent[i].sent_phr_index;
			next_sent_phr_index = sent[i + 1].sent_phr_index;
			sent[i].num_word = phr[next_sent_phr_index].phr_word_index - phr[cur_sent_phr_index].phr_word_index;
		}
	}
}

void CntNumPhr(int sent_index, int num_phr, int num_sent, SENTENCE *sent)
{
	int i;

	if (sent_index != 0)
		return;

	for (i = 0; i < num_sent; i++){
		if ((i + 1) == num_sent){
			sent[i].num_phr = num_phr - sent[i].sent_phr_index;
		}
		else{
			sent[i].num_phr = sent[i + 1].sent_phr_index - sent[i].sent_phr_index;
		}
	}
}

void OutputLabel(int num_ph, PHONE *ph, SYLLABLE *syl, WORD *word, PHRASE *phr, SENTENCE *sent)
{
	int i, cur_syl = 0, cur_word = 0, cur_phr = 0, cur_sent = 0;
	int bef_syl = 0, bef_word = 0;
 	for (i = 0; i < num_ph; i++){
		fprintf(stdout, "%*s %*s ", 10, ph[i].dur_start, 10, ph[i].dur_end);
		//fprintf(stdout, "%s\n", ph[i].cur_phone);
		fprintf(stdout, "%s^%s-%s+%s=%s", ph[i].bef_bef_phone, ph[i].bef_phone, ph[i].cur_phone, ph[i].next_phone, ph[i].next_next_phone);
		cur_syl = ph[i].belong_syl;

		if (cur_syl == 9999){
			fprintf(stdout, "@0_0");
			fprintf(stdout, "/A:x_0");
			fprintf(stdout, "/B:x-0@0-0&0-0#0-0!0-0|x");
			fprintf(stdout, "/C:x+0");
			fprintf(stdout, "/D:x_x-0/E:x_x+0@0+0/F:x_x=0");
			fprintf(stdout, "/G:0_0/H:0=0@0=0/I:0_0");
			fprintf(stdout, "/J:0+0-0\n");
		}
		else if (cur_syl == 9998){
			bef_syl = ph[i-1].belong_syl;
			bef_word = syl[bef_syl].belong_word;
			fprintf(stdout, "@0_0");
			fprintf(stdout, "/A:x_0");
			fprintf(stdout, "/B:x-0@0-0&0-0#0-0!0-0|x");
			fprintf(stdout, "/C:x+0");
			fprintf(stdout, "/D:%s_x-%d", word[bef_word].bef_word_POS1, word[bef_word].bef_num_syl);
			fprintf(stdout, "/E:%s_x+%d@%d+%d", word[bef_word].cur_word_POS1, word[bef_word].cur_num_syl, word[bef_word].fd_pos_in_phr, word[bef_word].bd_pos_in_phr);
			fprintf(stdout, "/F:%s_x=%d", word[bef_word].next_word_POS1, word[bef_word].next_num_syl);
			fprintf(stdout, "/G:0_0/H:0=0@0=0/I:0_0");
			fprintf(stdout, "/J:0+0-0\n");
		}
		else{
			cur_word = syl[cur_syl].belong_word;
			cur_phr = word[cur_word].belong_phr;
			cur_sent = phr[cur_phr].belong_sent;
			fprintf(stdout, "@%d_%d", ph[i].fd_pos_in_syl, ph[i].bd_pos_in_syl);
			fprintf(stdout, "/A:%c_%d", syl[cur_syl].bef_stress, syl[cur_syl].bef_num_phone);
			fprintf(stdout, "/B:%c-%d@%d-%d&%d", syl[cur_syl].cur_stress, syl[cur_syl].cur_num_phone, syl[cur_syl].fd_pos_in_word, syl[cur_syl].bd_pos_in_word, syl[cur_syl].fd_pos_in_phr);
			if( syl[cur_syl].ch_or_en == 'c')
				fprintf(stdout, "-%d#%d-%d!x-x|%s", syl[cur_syl].bd_pos_in_phr, syl[cur_syl].num_stress_bef_in_phr, syl[cur_syl].num_stress_aft_in_phr, syl[cur_syl].name_vowel);
			else
				fprintf(stdout, "-%d#%d-%d!%d-%d|%s", syl[cur_syl].bd_pos_in_phr, syl[cur_syl].num_stress_bef_in_phr, syl[cur_syl].num_stress_aft_in_phr, syl[cur_syl].num_syl_bef2_stress, syl[cur_syl].num_syl_aft2_stress, syl[cur_syl].name_vowel);
			fprintf(stdout, "/C:%c+%d", syl[cur_syl].next_stress, syl[cur_syl].next_num_phone);
			fprintf(stdout, "/D:%s_%s-%d", word[cur_word].bef_word_POS1, word[cur_word].bef_word_POS2, word[cur_word].bef_num_syl);
			fprintf(stdout, "/E:%s_%s+%d@%d+%d", word[cur_word].cur_word_POS1, word[cur_word].cur_word_POS2, word[cur_word].cur_num_syl, word[cur_word].fd_pos_in_phr, word[cur_word].bd_pos_in_phr);
			fprintf(stdout, "/F:%s_%s=%d", word[cur_word].next_word_POS1, word[cur_word].next_word_POS2, word[cur_word].next_num_syl);
			fprintf(stdout, "/G:%d_%d", phr[cur_phr].bef_num_syl, phr[cur_phr].bef_num_word);
			fprintf(stdout, "/H:%d=%d@%d=%d", phr[cur_phr].cur_num_syl, phr[cur_phr].cur_num_word, phr[cur_phr].fd_pos_in_utter, phr[cur_phr].bd_pos_in_utter);
			fprintf(stdout, "/I:%d_%d", phr[cur_phr].next_num_syl, phr[cur_phr].next_num_word);
			fprintf(stdout, "/J:%d+%d-%d\n", sent[cur_sent].num_syl, sent[cur_sent].num_word, sent[cur_sent].num_phr);
		}
		/*else if (cur_syl == 9998){
			bef_syl = ph[i-1].belong_syl;
			bef_word = syl[bef_syl].belong_word;
			fprintf(stdout, "@0_0");
			fprintf(stdout, "/A:x_0");
			fprintf(stdout, "/B:x-0@0-0&0-0#0-0!0-0|x");
			fprintf(stdout, "/C:x+0");
			fprintf(stdout, "/D:%s_x-%d", word[bef_word].bef_word_POS1, word[bef_word].bef_num_syl);
			fprintf(stdout, "/E:%s_x+%d@%d+%d", word[bef_word].cur_word_POS1, word[bef_word].cur_num_syl, word[bef_word].fd_pos_in_phr, word[bef_word].bd_pos_in_phr);
			fprintf(stdout, "/F:%s_x=%d", word[bef_word].next_word_POS1, word[bef_word].next_num_syl);
			fprintf(stdout, "/G:0_0/H:0=0@0=0/I:0_0");
			fprintf(stdout, "/J:0+0-0\n");
		}
		else{
			fprintf(stdout, "@0_0");
			fprintf(stdout, "/A:x_0");
			fprintf(stdout, "/B:x-0@0-0&0-0#0-0!0-0|x");
			fprintf(stdout, "/C:x+0");
			fprintf(stdout, "/D:x_x-0/E:x_x+0@0+0/F:x_x=0");
			fprintf(stdout, "/G:0_0/H:0=0@0=0/I:0_0");
			fprintf(stdout, "/J:0+0-0\n");
		}*/

	}

}


