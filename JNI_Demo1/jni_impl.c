#include "stdafx.h"

#include "JniMain.h"
#include <string.h>

JNIEXPORT jstring JNICALL Java_JniMain_getStringFromC
(JNIEnv * env, jclass jclz){
	return (*env)->NewStringUTF(env, "hello JNI");
}

JNIEXPORT jstring JNICALL Java_JniMain_getStringFromC2
(JNIEnv * env, jobject jobj){
	return (*env)->NewStringUTF(env, "I'm stonger");
}


//访问java中非静态全局变量key，并修改值
JNIEXPORT jstring JNICALL Java_JniMain_accessFieldModify
(JNIEnv * env, jobject jobj){

	//得到jclass,即JniMain.class
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//得FieldId, "key" 属性名称，"Ljava/lang/String;"属性签名
	jfieldID fid = (*env)->GetFieldID(env,jclz, "key", "Ljava/lang/String;");

	//得到key对应的值
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);

	//将jstring类型转化为c语言的char字符数组
	char * c_str = (*env)->GetStringUTFChars(env,jstr,NULL);

	//将字符串"key"改成"hello key"
	char text[20] = "hello ";
	strcat(text, c_str);

	jstring new_str = (*env)->NewStringUTF(env,text);

	//给jobj的key成员变量设置新的值
	(*env)->SetObjectField(env,jobj,fid,new_str);

	//释放内存
	(*env)->ReleaseStringUTFChars(env,new_str,c_str);

	return new_str;
}


JNIEXPORT void JNICALL Java_JniMain_accessStaticFieldModify
(JNIEnv * env, jobject jobj){

	//得到jclass,即JniMain.class
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//得FieldId, "count" 属性名称，"I"属性签名
	jfieldID fid = (*env)->GetStaticFieldID(env, jclz, "count", "I");

	//获取count的值
	jint count = (*env)->GetStaticIntField(env,jclz,fid);

	count += 5;

	//给jobj的count静态成员变量设置新的值
	(*env)->SetStaticIntField(env, jclz, fid, count);

}

	