#include "stdafx.h"

#include "JniMain.h"

JNIEXPORT jstring JNICALL Java_JniMain_getStringFromC
(JNIEnv * env, jclass jclz){
	return (*env)->NewStringUTF(env, "hello JNI");
}