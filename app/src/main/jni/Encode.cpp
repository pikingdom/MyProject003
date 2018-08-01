#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include "ReadTools.h"
#include "string.h"
#include "WriteTools.h"


jbyteArray native_getItemInfo(JNIEnv *env, jobject* object,jstring name)
{
	jboolean isCopy=true;
	const char *PN=env->GetStringUTFChars(name,&isCopy);
	LOG("native_getItemInfo: filename%s",PN);
	ITEM*items=NULL;
	int count=0;
	items=getItemInfos(PN,8,count);
	jbyteArray result=env->NewByteArray(sizeof(ITEM)*count);
	env->SetByteArrayRegion(result,0,sizeof(ITEM)*count,(jbyte*)items);
	if(items!=NULL)
	{
		delete[] items;
	}
	env->ReleaseStringUTFChars(name,PN);
	//env->DeleteLocalRef(result);
	LOG("return");
	return result;


	//return NULL;
}


jbyteArray native_getResource(JNIEnv *env, jobject* object,jstring name,int startPos,int len)
{
	jboolean isCopy=true;
	const char *filename=env->GetStringUTFChars(name,&isCopy);
	LOG("native_getResource %s",filename);





	char*buff=new char[len];
	char *r=(char*)getResourceByPos(filename,(char*)buff,startPos,len);
	if(r!=NULL)
	{
		LOG("native_getResource: getResource succeed");
		jbyteArray result=env->NewByteArray(len);
		env->SetByteArrayRegion(result,0,len,(jbyte*)buff);
		env->ReleaseStringUTFChars(name,filename);
		//env->DeleteLocalRef(result);
		delete[]buff;
		return result;
	}else
	{
		env->ReleaseStringUTFChars(name,filename);
		delete[]buff;
		return NULL;
	}

}


jbyteArray native_getResourceByName(JNIEnv *env, jobject* object,jstring name,jstring resname)
{
	jboolean isCopy=true;
	const char *filename=env->GetStringUTFChars(name,&isCopy);
	const char *res=env->GetStringUTFChars(resname,&isCopy);
	LOG("native_getResourceByName: %s",filename);


	int len=0;
	char *r=(char*)getResourceByName(filename,res,&len);
	if (r != NULL) {
		jbyteArray result = env->NewByteArray(len);
		LOG("native_getResourceByName: getResource succeed");

		env->SetByteArrayRegion(result, 0, len, (jbyte*) r);
		env->ReleaseStringUTFChars(name, filename);
		env->ReleaseStringUTFChars(name, res);
		//env->DeleteLocalRef(result);
		delete[] r;
		return result;
	} else {
		env->ReleaseStringUTFChars(name, filename);
		env->ReleaseStringUTFChars(name, res);
		LOG("native_getResourceByName: getResource faile");
		return NULL;
	}

}

jboolean native_addImei(JNIEnv *env, jobject* object, jstring name,
		jstring imei) {
	const char*file = NULL;
	const char *imeiStr = NULL;

	jboolean isCopy = true;
	file = env->GetStringUTFChars(name, &isCopy);
	imeiStr = env->GetStringUTFChars(imei, &isCopy);
	char key[]="qweasdrt";
	bool r = addImei(file, imeiStr, key);
	return r;
}
static const JNINativeMethod method_table[] = {
		{ "getItemInfos",    "(Ljava/lang/String;)[B",(void*)native_getItemInfo} ,
		{ "getResourceByPos","(Ljava/lang/String;II)[B",(void*)native_getResource},
		{"getResourceByName","(Ljava/lang/String;Ljava/lang/String;)[B",(void*)native_getResourceByName},
		{"addImei","(Ljava/lang/String;Ljava/lang/String;)Z",(void*)native_addImei}
	};

void JNI_OnUnload(JavaVM* vm, void* reserved) {

}



jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//__android_log_print(ANDROID_LOG_ERROR, "zhou", "JNI_OnLoad");

	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}

	jclass imageutil = env->FindClass("com/nd/hilauncherdev/theme/parse/EncodeTools");
	if (imageutil == NULL) {
		LOG("JNI_OnLoad:no find class ");
	} else {
		LOG("JNI_OnLoad:find class ");
	}
	result = env->RegisterNatives(imageutil, method_table, 4);
	if (result == JNI_OK) {
		LOG("JNI_OnLoad:RegisterNatives succeed");
	} else {
		LOG("JNI_OnLoad:RegisterNatives fail R=%d", result);
	}
	return JNI_VERSION_1_4;
}
