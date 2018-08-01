#include <jni.h>
#include "com_example_administrator_hellojni_ndkbuild_MainActivity.h"


JNIEXPORT jstring
JNICALL
Java_com_example_administrator_hellojni_1ndkbuild_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject thiz) {
    return (*env)->NewStringUTF(env,"Hello ====");
}
