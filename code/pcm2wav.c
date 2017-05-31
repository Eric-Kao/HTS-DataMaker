#include <stdio.h>
#include <string.h>


typedef   struct   {
    char         fccID[4];
    unsigned   long       dwSize;
    char         fccType[4];
}HEADER;

typedef   struct   {
    char         fccID[4];
    unsigned   long       dwSize;
    unsigned   short     wFormatTag;
    unsigned   short     wChannels;
    unsigned   long       dwSamplesPerSec;
    unsigned   long       dwAvgBytesPerSec;
    unsigned   short     wBlockAlign;
    unsigned   short     uiBitsPerSample;
}FMT;

typedef   struct   {
    char         fccID[4];
    unsigned   long       dwSize;
}DATA;

void show_usage()
{
    printf("PCM 2 WAV usage\neg:pcm2wav pcm_file wav_file channels sample_rate bits\n");
}

int main(int argc, char **argv)
{
    char src_file[128] = {0};
    char dst_file[128] = {0};
    int channels = 1;
    int bits = 16;
    int sample_rate = 22050;

    HEADER   pcmHEADER;
    FMT   pcmFMT;
    DATA   pcmDATA;

    unsigned   short   m_pcmData;
    FILE   *fp,*fpCpy;

    if (argc < 5)
    {
        show_usage();
        return -1;
    }
    sscanf(argv[1], "%s", src_file);
    sscanf(argv[2], "%s", dst_file);
    sscanf(argv[3], "%d", &channels);
    sscanf(argv[4], "%d", &sample_rate);
    if (argc>6)
    {
        sscanf(argv[5], "%d", &bits);
    }

    printf("parameter analyse succeess\n");

    if((fp=fopen(src_file,   "rb"))   ==   NULL)
    {
        printf("open pcm file %s error\n", argv[1]);
        return -1;
    }

    if((fpCpy=fopen(dst_file,   "wb+"))   ==   NULL)
    {
        printf("create wav file error\n");
        return -1;
    }

    strcpy(pcmHEADER.fccID,"RIFF");
    strcpy(pcmHEADER.fccType,"WAVE");
    fseek(fpCpy,sizeof(HEADER),1);

    if(ferror(fpCpy))
    {
        printf("error\n");
    }

    pcmFMT.dwSamplesPerSec=sample_rate;
    pcmFMT.dwAvgBytesPerSec=pcmFMT.dwSamplesPerSec*sizeof(m_pcmData);
    pcmFMT.uiBitsPerSample=bits;

    strcpy(pcmFMT.fccID,"fmt   ");
    pcmFMT.dwSize=16;
    pcmFMT.wBlockAlign=2;
    pcmFMT.wChannels=channels;
    pcmFMT.wFormatTag=1;
    //以上是创建wav头的FMT;

    fwrite(&pcmFMT,sizeof(FMT),1,fpCpy);

    strcpy(pcmDATA.fccID,"data");

    pcmDATA.dwSize=0;
    fseek(fpCpy,sizeof(DATA),1);
    fread(&m_pcmData,sizeof(unsigned   short),1,fp);

    while(!feof(fp))
    {
        pcmDATA.dwSize+=2;
        fwrite(&m_pcmData,sizeof(unsigned   short),1,fpCpy);
        fread(&m_pcmData,sizeof(unsigned   short),1,fp);
    }

    fclose(fp);
    pcmHEADER.dwSize=44+pcmDATA.dwSize;
    rewind(fpCpy);
    fwrite(&pcmHEADER,sizeof(HEADER),1,fpCpy);
    fseek(fpCpy,sizeof(FMT),1);
    fwrite(&pcmDATA,sizeof(DATA),1,fpCpy);
    fclose(fpCpy);

    return 0;
}
