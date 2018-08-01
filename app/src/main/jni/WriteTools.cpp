
#include <string.h>
#include "DES.h"
#include "stdio.h"
#include "ReadTools.h"



/**
 * ��key ����
 * */
const char key[16]="12345678";
char RandomKey[16]="0123456789abcd";
void encryptKey(char*src,char*dest,int len)
{

}



void* WriteResource(FILE*fp, void *buff, int startPos, int Len) {
	fseek(fp, startPos, SEEK_SET);
	//LOG("WriteResource: currentPos=%d len=%d",ftell(fp),Len);
	char*srcbuff = new char[32];
	memcpy(srcbuff, buff, 32);
	//DES des;
	//des.Des_Go((char*)buff,(char*)srcbuff,32,"qweasdrt",8,ENCRYPT);

	encrypt((char*)buff,(char*)srcbuff,32,RandomKey);
	int count = fwrite(buff, 1, Len, fp);
	//printf("len=%d\n", count);
	if (count < 0) {
		//LOG("setResource fail");
		return NULL;
	} else {
		//LOG("setResource succeed\n");
		return buff;
	}
}


/**
fp:�ļ�ָ��
itemArray:��Ŀ����
size:��Ŀ����
*/

int WriteItemInfo(FILE*fp, ITEM *itemArray, int size,char*key)
{
	int itemSize = sizeof(ITEM);
	fseek(fp, sizeof(NCF), SEEK_SET);
	//LOG("WriteItemInfo");
	for (int i = 0; i<size; i++)
	{
		int count = 0;
		ITEM *item = itemArray + i;
		char *buff = new char[itemSize];
		memset(buff,0x0,itemSize);
		//DES des;
		//des.Des_Go(buff,(char*)item,itemSize,"qweasdrt",8,ENCRYPT);
		encrypt((char*)item,buff,itemSize,key);
		//writeImage("/sdcard/zhouhq/s.txt",buff,itemSize);
		//LOG("WriteItemInfo: currentPos=%d len=%d",ftell(fp),itemSize);
		count = fwrite(buff, itemSize, 1, fp);
		//LOG("WriteItemInfo:%s %d %d %s",item->name,item->startPos,item->len,item->type);
	}
	return 0;
}

int WriteHead(FILE*fp, NCF*ncf) {
	//LOG("WriteHead: currentPos=%d len=%d",ftell(fp),sizeof(NCF));

	char *buf=new char[sizeof(ncf->key)];
	encryptKey(ncf->key,buf,sizeof(ncf->key));
	strcpy(ncf->key,buf);
	delete []buf;
	int size = fwrite(ncf, sizeof(NCF), 1, fp);

	if (size == sizeof(NCF)) {
		return 0;
	} else {
		return -1;
	}
}


void testWriteFile(char *fileName)
{
	FILE *fp = fopen(fileName, "wb");
	if (fp == NULL)
	{
		//LOG("open file faile  %s",fileName);
		return;
	}
	//LOG("open file succeed %s",fileName);
	//LOG("sizeof(NFC)=%d sizeof(ITEM)=%d",sizeof(NCF),sizeof(ITEM));
	NCF* ncf = new NCF();
	memset(ncf,0x0,sizeof(NCF));
	sprintf(ncf->magic, "%s", "NDCF");
	sprintf(ncf->version, "%s", "11");
	sprintf(ncf->key,"%s",RandomKey);
	ncf->itmeCount = 2;
	WriteHead(fp, ncf);

	int startPost=sizeof(NCF)+sizeof(ITEM)*ncf->itmeCount;




	ITEM *items=new ITEM[ncf->itmeCount];

	char name[2][32]={"/sdcard/zhouhq/girl1.png","/sdcard/zhouhq/girl2.png"};

	for (int i = 0; i < ncf->itmeCount; i++) {
		FILE*png=NULL;
		ITEM *item=NULL;
		item = items+i;


		memset(item, 0x0, sizeof(ITEM));
		strcpy(item->name,name[i]);
		png = fopen(item->name, "rb");
		fseek(png, 0L, SEEK_END);
		item->len = ftell(png);
		item->startPos = startPost;
		strcpy(item->type, "png");
		fclose(png);
		startPost = startPost + item->len;
	}
	WriteItemInfo(fp,items,ncf->itmeCount,RandomKey);


	for(int i=0;i<ncf->itmeCount;i++)
	{
		ITEM *im=items+i;
		FILE* wfp=fopen(im->name,"rb");
		char *buff=new char[im->len];
		memset(buff,0x0,im->len);
		fseek(wfp, 0L, SEEK_SET);
		fread(buff,1,im->len,wfp);
		WriteResource(fp,buff,im->startPos,im->len);
		fclose(wfp);
		delete buff;
	}

	fclose(fp);
}

/*
void writeImage(char*path,void*buff,int len)
{
	char*p = strstr(path, ".");
	char newPath[64] = "";
	strncpy(newPath, path, p - path);
	strcat(newPath, "_bak.jpg");
	FILE*fp = fopen(newPath, "wb");
	printf("opne file %s\n", newPath);
	int count=fwrite(buff, len, 1, fp);
	printf("write len=%d\n", count);
	fclose(fp);

}*/
