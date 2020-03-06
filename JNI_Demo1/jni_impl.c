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


//����java�зǾ�̬ȫ�ֱ���key�����޸�ֵ
JNIEXPORT jstring JNICALL Java_JniMain_accessFieldModify
(JNIEnv * env, jobject jobj){

	//�õ�jclass,��JniMain.class
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//��FieldId, "key" �������ƣ�"Ljava/lang/String;"����ǩ��
	jfieldID fid = (*env)->GetFieldID(env,jclz, "key", "Ljava/lang/String;");

	//�õ�key��Ӧ��ֵ
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);

	//��jstring����ת��Ϊc���Ե�char�ַ�����
	char * c_str = (*env)->GetStringUTFChars(env,jstr,NULL);

	//���ַ���"key"�ĳ�"hello key"
	char text[20] = "hello ";
	strcat(text, c_str);

	jstring new_str = (*env)->NewStringUTF(env,text);

	//��jobj��key��Ա���������µ�ֵ
	(*env)->SetObjectField(env,jobj,fid,new_str);

	//�ͷ��ڴ�
	(*env)->ReleaseStringUTFChars(env,new_str,c_str);

	return new_str;
}


JNIEXPORT void JNICALL Java_JniMain_accessStaticFieldModify
(JNIEnv * env, jobject jobj){

	//�õ�jclass,��JniMain.class
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//��FieldId, "count" �������ƣ�"I"����ǩ��
	jfieldID fid = (*env)->GetStaticFieldID(env, jclz, "count", "I");

	//��ȡcount��ֵ
	jint count = (*env)->GetStaticIntField(env,jclz,fid);

	count += 5;

	//��jobj��count��̬��Ա���������µ�ֵ
	(*env)->SetStaticIntField(env, jclz, fid, count);

}

	