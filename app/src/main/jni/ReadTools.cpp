// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include <string.h>
#include "string.h"
#include "DES.h"
#include "stdio.h"
#include <android/log.h>
#include "ReadTools.h"
#pragma warning(disable:4996)

/**
 * ��key ����
 * */

void decryptKey(char*src,char*dest,int len)
{
//	DES des;
	//des.Des_Go(dest,src,len,key,8,!ENCRYPT);
}

void MyDES(unsigned char *source, unsigned char *dest, int len,
		unsigned char *key, unsigned char flag);

char* delfile(const char*name,char*buff)
{
	int index=0;
	buff[0]=100;
	index++;
	buff[index]=96|11;
	index++;
	strcat(buff+index,"op");
	int a=48;
	index=index+2;
	buff[index]=a+9;
	index++;
	a=53;
	buff[index]=a-4;
	return NULL;
}

int readHead(FILE*fp,NCF*ncf) {
	//LOG("readHead:read head post:%d",ftell(fp));
	int size = fread(ncf, 1, sizeof(NCF), fp);
	//LOG("readHead:headinfo image=%s,version=%s count=%d",ncf->magic,ncf->version,ncf->itmeCount);
	char *buf=new char[sizeof(ncf->key)];
	memset(buf,0x0,sizeof(ncf->key));
	//char key[9]="dkop91Ts";
	char key[9]="";
	delfile("/sdcard/nd/key",key);
	strcat(key,"Ts");
	//LOG("key=%s ",key);
	DES((unsigned char *)(ncf->key),(unsigned char *)(buf),(unsigned char *)(key),DECRYPT);
	DES((unsigned char *)(ncf->key+8),(unsigned char *)(buf+8),(unsigned char *)(key),DECRYPT);
	strncpy(ncf->key,buf,16);
	//LOG("readHead:decrypt key %s",ncf->key);
	delete []buf;
	if (size == sizeof(NCF)) {
		return 0;
	} else {
		return -1;
	}
}
ITEM * getItemInfos(const char*name, int keylen,int &count) {

	int size = 0;

	FILE*fp = fopen(name, "rb");

	if (fp == NULL) {

		LOG("getItemInfos:open file %s fail",
				name);
		return NULL;
	} else {
		LOG("getItemInfos:openfile  %s succeed",
				name);
	}

	NCF ncf;
	if(readHead(fp,&ncf)!=0)
	{
		return NULL;
	}
	//LOG("getItemInfos:NCF maigc=%s version=%s count%d key=%s", ncf.magic, ncf.version,ncf.itmeCount,ncf.key);
	size = ncf.itmeCount;
	count=size;
	ITEM *items = new ITEM[size];
	printf("\n\n");
	fseek(fp, sizeof(NCF), SEEK_SET);
	int readCount = sizeof(ITEM);
	char *buff = new char[readCount];
	memset(buff,0x0,sizeof(ITEM));




	//count = fread(buff, 1, readCount, fp);
	//encrypt(buff,(char*)(items),readCount*count,ncf.key);
	for (int i = 0; i < size; i++) {
		int count = 0;
		count = fread(buff, 1, readCount, fp);
		//LOG("getItemInfos:read buff count=%d",count);

		//DES ndes;
		//ndes.Des_Go((char*) (items + i), (char*) buff, sizeof(ITEM),(const char*) key, keylen, !ENCRYPT);

		//writeImage("/sdcard/zhouhq/d.txt",buff,readCount);
		encrypt(buff,(char*)(items+i),readCount,ncf.key);

		ITEM *item = items + i;
		//writeImage("/sdcard/zhouhq/dd.txt",item,readCount);
		//LOG("getItemInfos:name=%s pos=%d len=%d type=%s",
		//		item->name, item->startPos, item->len, item->type);

	}
	delete[] buff;
	fclose(fp);
	return items;

}



void* getResourceByPos(const char* name, char *buff, int startPos, int Len) {

	FILE*fp = fopen(name, "rb");
	if (fp == NULL) {

		LOG("getResourceByPos:open file %s fail", name);
		return NULL;
	} else {
		//LOG("getResourceByPos:openfile  %s succeed", name);
	}
	NCF ncf;
	readHead(fp,&ncf);
	//LOG("getResourceByPos:key=%s",ncf.key);
	fseek(fp, startPos, SEEK_SET);
	char*p = buff;

	int totalcount = 0;
	//LOG("getResourceByPos: pos=%d len=%d",ftell(fp),Len);
	totalcount = fread(buff, 1, Len, fp);
	char *srcbuff = new char[32];
	memcpy(srcbuff, buff, 32);
	//DES ndes;
	//ndes.Des_Go(buff, srcbuff, 32, (const char*) key, keyLen, !ENCRYPT);
	encrypt(srcbuff,buff,32,ncf.key);
	//LOG("getResourceByPos:decrypt succeed");

	delete []srcbuff;
	fclose(fp);
	return buff;
}


void* getResourceByName(const char* name,const char*resName,int *resLen) {

	FILE*fp = fopen(name, "rb");
	if (fp == NULL) {

		LOG("getResourceByName:open file %s fail", name);
		return NULL;
	} else {
		//LOG("getResourceByName:openfile  %s succeed", name);
	}
	NCF ncf;
	readHead(fp,&ncf);
	//LOG("getResourceByName:key=%s",ncf.key);


	int size = ncf.itmeCount;
	int readCount = sizeof(ITEM);
	char *itemBuff = new char[readCount];
	fseek(fp, sizeof(NCF), SEEK_SET);

	bool found=false;
	ITEM item;
	for (int i = 0; i < size; i++) {
		int count = 0;
		count = fread(itemBuff, 1, readCount, fp);
		//LOG("getResourceByName:read buff count=%d",count);
		encrypt(itemBuff,(char*)(&item),readCount,ncf.key);
		if(strcmp(resName,item.name)==0)
		{
			//LOG("getResourceByName:found res by name");
			found=true;
			break;
		}

		//writeImage("/sdcard/zhouhq/dd.txt",item,readCount);
		//LOG("getResourceByName:name=%s pos=%d len=%d type=%s",
		//		item.name, item.startPos, item.len, item.type);

	}

	if(!found)
	{
		fclose(fp);
		delete []itemBuff;
		return NULL;
	}

	//
	int startPos=item.startPos;
	int Len=item.len;
	*resLen=Len;
	fseek(fp, startPos, SEEK_SET);
	char *buff=new char[Len];
	char*p = buff;

	int totalcount = 0;
	//LOG("getResourceByName: pos=%d len=%d",ftell(fp),Len);
	totalcount = fread(buff, 1, Len, fp);
	char *srcbuff = new char[32];
	memcpy(srcbuff, buff, 32);
	//DES ndes;
	//ndes.Des_Go(buff, srcbuff, 32, (const char*) key, keyLen, !ENCRYPT);
	encrypt(srcbuff,buff,32,ncf.key);
	//LOG("getResourceByName:decrypt succeed");

	delete []srcbuff;
	delete []itemBuff;
	fclose(fp);
	return buff;
}
bool addImei(const char*fileName,const char*imei,const char*key)
{
	bool result = false;
	FILE*fp = fopen(fileName, "ab+");
	if (fp == NULL) {

		//LOG("addImei:open file %s fail", fileName);
		return false;
	} else {
		//LOG("addImei:openfile  %s succeed", fileName);
	}
	//LOG("addImei:fseek ");
	fseek(fp, 0L, SEEK_END);
	//LOG("addImei:fteek ");
	int len = ftell(fp);

	if (len < sizeof(ITEM) + 33) {
		fclose(fp);
		return false;
	}

	char buff[33]="";
	fseek(fp, -33, SEEK_END);
	fread(buff,1,33,fp);

	MyDES((unsigned char *)(buff),(unsigned char *)(buff),32,(unsigned char *)(key),DECRYPT);
	char mark[9]="91themee";
	//LOG("addImei:imei=%s mark=%s",buff,mark);
	char*p=NULL;
	p=strstr(buff,mark);
	//LOG("addImei:%0x %0x",p,buff);
	if(p!=NULL &&p==buff)
	{

		//LOG("addImei:have add imimmm");
		p=strstr(buff+8,imei);
		//LOG("addImei:phone imei=%s",imei);
		//LOG("addImei:file imei= %s ",buff+8);
		if(p!=NULL)
		{
			LOG("addImei:imei is right");
			fclose(fp);
			return true;
		}else
		{
			LOG("addImei:imei is wrong");
			fclose(fp);
			return false;
		}

	}else
	{
		//LOG("addImei:go to add imimmm");
		int imeilen=strlen(imei);
		char writebug[25]="";
		strncpy(writebug,imei,25);
		DES((unsigned char *)(mark),(unsigned char *)(mark),(unsigned char *)(key),ENCRYPT);
		MyDES((unsigned char *)(writebug),(unsigned char *)(writebug),24,(unsigned char *)(key),ENCRYPT);
		fseek(fp, 0L, SEEK_END);
		if (fwrite(mark, 8, 1, fp) != 1) {
			LOG("addImei:Write mark Error");
			fclose(fp);
			return false;
		}
		if(fwrite(writebug,25,1,fp)!=1)
		{
			LOG("addImei:Write imei Error");
			fclose(fp);
			return false;
		}

		//LOG("addImei:mark=%s imie=%s",mark,writebug);
	}
	fclose(fp);
	return true;
}


void MyDES(unsigned char *source, unsigned char *dest, int len,
		unsigned char *key, unsigned char flag) {
	if (len % 8 != 0 || len == 0) {
		return;
	}
	for (int i = 0; i < len / 8; i++) {
		DES((unsigned char *) (source + 8 * i),
				(unsigned char *) (dest + 8 * i), (unsigned char *) (key),
				flag);
	}

}

/**
 *
 * */
void encrypt(char*src, char*dest, int len, char *key) {
	int count = 0;
	int index = 0;
	int keyIndex = 0;

	//LOG("enctypt： start key=%s",key);
	count=len;
	while (count >= 8) {

		for(int i=0;i<8;i++)
		{
			dest[index + i] = ~(src[index + i] ^ key[keyIndex+i]);
		}
		index += 8;
		count -= 8;
		keyIndex=(keyIndex+1)%6;

	}
	for (int i = 0; i < count; i++) {
		dest[index + i] = ~(src[index + i] ^ key[keyIndex+i]);
	}
	//LOG("encrypt： end\n");
}
///**
// * �����ݼ���
// * */
//void encrypt(char*src, char*dest, int len, char *key) {
//	int count = 0;
//	int index = 0;
//	int keyIndex = 0;
//	int *s1 = NULL;
//	int *s2 = NULL;
//
//	int *d1 = NULL;
//	int *d2 = NULL;
//
//	int *k1 = NULL;
//	int *k2 = NULL;
//	//LOG("enctypt start");
//	count=len;
//	while (count >= 8) {
//		//LOG("key %s",key+keyIndex);
//		k1 = (int*) (key + keyIndex);
//		k2 = (int*) (key + keyIndex + 4);
//		s1 = (int*) (src + index);
//		s2 = (int*) (src + index + 4);
//		d1 = (int*) (dest + index);
//		d2 = (int*) (dest + index + 4);
//		*d1 =~(*s1 ^ *k1);
//		*d2 =~(*s2 ^ *k2);
//		index += 8;
//		count -= 8;
//		keyIndex=(keyIndex+1)%6;
//	}
//	for (int i = 0; i < count; i++) {
//		dest[index + i] = ~(src[index + i] ^ key[keyIndex+i]);
//	}
//	//LOG("encrypt end\n");
//}

/**
 * �����ݼ���
 * */

/*
void decrypt(char*src, char*dest, int len, char *key) {

	int count = len;
	int index = 0;
	int keyIndex = 0;
	int *s1 = NULL;
	int *s2 = NULL;

	int *d1 = NULL;
	int *d2 = NULL;

	int *k1 = NULL;
	int *k2 = NULL;

	while (count >= 8) {
		k1 = (int*) (key + keyIndex);
		k2 = (int*) (key + keyIndex + 4);
		s1 = (int*) (src + index);
		s2 = (int*) (src + index + 4);
		d1 = (int*) (dest + index);
		d2 = (int*) (dest + index + 4);
		*d1 = (~(*s1) ^ *k1);
		*d2 = (~(*s2) ^ *k2);
		index += 8;
		count -= 8;
	}
	for (int i = 0; i < count; i++) {
		dest[index + i] = src[index + i] ^ key[keyIndex];
	}

}*/

void writeImage(char*path,void*buff,int len)
{
	FILE*fp = fopen(path, "wb");
	int count=fwrite(buff, len, 1, fp);
	//LOG("writeImage：write len=%d\n", count);
	fclose(fp);
}
#define MAX_LOG_LEN 256
void LOG(const char*format,...)
{
	char buff[MAX_LOG_LEN]="";
    va_list arg;
	va_start(arg,format);
	vsprintf(buff,format,arg);
	__android_log_print(ANDROID_LOG_ERROR, "zhouNative","%s",buff);
    va_end(arg);
}
