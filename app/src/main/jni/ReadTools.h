//#ifndef _ReadTool_H_
//#define _ReadTool_H_

typedef struct MyStruct {
	char magic[4];
	char version[2];
	short itmeCount;
	char key[16];
} NCF;

typedef struct ITEM_INFO
{
	char name[128];
	int startPos;
	int len;
	char type[8];


}ITEM;
int test(char*name);
ITEM * getItemInfos(const char*name,int keylen,int &count);
void* getResourceByPos(const char* name, char *buff, int startPos, int Len);
void* getResourceByName(const char* name,const char*resName,int *resLen);
bool addImei(const char*fileName,const char*imei,const char*key);
void writeImage(char*path,void*buff,int len);
void encrypt(char*src, char*dest, int len, char *key);
void LOG(const char*format,...);
//void decrypt(char*src, char*dest, int len, char *key);
//#endif
