#include "stdafx.h"

#include "JniMain.h"
#include <string.h>
#include <Windows.h>

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

	
//JNI����java�еķǾ�̬����
JNIEXPORT void JNICALL Java_JniMain_accessMethod
(JNIEnv *env , jobject jobj){
	//�õ�jclass,��JniMain.class
	jclass jclz = (*env)->GetObjectClass(env, jobj);

	//�õ�jmethodID,����������getRandomInt��������ǩ��
	jmethodID mid = (*env)->GetMethodID(env,jclz,"getRandomInt","(I)I");
	
	//���ø÷Ǿ�̬����
	jint random = (*env)->CallIntMethod(env,jobj,mid,200);

	printf("C random:%d \n",random);

}


//JNI����java�еľ�̬����
JNIEXPORT void JNICALL Java_JniMain_accessStaticMethod
(JNIEnv *env, jobject jobj){

	//�õ�jclass,��JniMain.class
	jclass clz = (*env)->GetObjectClass(env,jobj);

	//�õ�jmethodID,����������getRandomUUId������ǩ��
	jmethodID jmid = (*env)->GetStaticMethodID(env, clz, "getRandomUUId", "()Ljava/lang/String;");

	//���øþ�̬����
	jstring uuid = (*env)->CallStaticObjectMethod(env,clz,jmid);

	//jstring -> char *
	char * uuid_c = (*env)->GetStringUTFChars(env,uuid,NULL);

	char filename[100];
	sprintf(filename,"D://%s.txt",uuid_c);
	FILE *fp = fopen(filename,"w+");
	fputs("I love timor",fp);
	fclose(fp);
	printf("�ļ�д��ɹ�\n");
}

//����java�Ĺ��췽��
JNIEXPORT jobject JNICALL Java_JniMain_accessConstructor
(JNIEnv * env, jobject jobj){
	//ͨ�����·������JVM�����ҵ���Ӧ����
	jclass jclz = (*env)->FindClass(env,"java/util/Date");

	//��ȡ���췽����jmothodID
	jmethodID jmid = (*env)->GetMethodID(env,jclz,"<init>","()V");

	//ʵ����һ��Date���󣬷���ֵ��һ��jobject
	jobject dateObj = (*env)->NewObject(env,jclz,jmid);

	//��ȡgetTime()��jmethodID,ǰ�������Ƿ�������ض���Ĺ��캯���������������
	jmethodID mid = (*env)->GetMethodID(env,jclz,"getTime","()J");

	jlong time = (*env)->CallLongMethod(env, dateObj, mid);

	printf("time: %lld \n",time);

	return dateObj;
}

/*
* Class:     JniMain
* Method:    chineseChars
* Signature: (Ljava/lang/String;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_JniMain_chineseChars
(JNIEnv * env, jobject jobj, jstring in){
	
	//iscp�������stringתchar��ʱ���Ƿ���¿��ٵ��ڴ�copy��һ��
	jboolean iscp;
	char * c_str = (*env)->GetStringChars(env, in, &iscp);
	if (iscp == JNI_TRUE)
	{
		printf("is copy: JNI_TRUE\n");
	}
	else if (iscp == JNI_FALSE)
	{
		printf("is copy: JNI_FALSE\n");
	}

	int length = (*env)->GetStringLength(env, in);
	const jchar * jcstr = (*env)->GetStringChars(env, in, NULL);
	if (jcstr == NULL) {
		return NULL;
	}
	//jchar -> char
	char * rtn = (char *)malloc(sizeof(char)* 2 * (length + 1));
	memset(rtn, 0, sizeof(char)* 2 * (length + 1));
	int size = 0;
	//windows�ṩ���ַ���ת��Ĳ���
	size = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)jcstr, length, rtn, sizeof(char)* 2 * (length + 1), NULL, NULL);
	/*if (size <= 0)
	{
	printf("size: 0 \n", rtn);
	return NULL;
	}*/
	printf("jni string: %s\n", rtn);
	if (rtn != NULL) {
		free(rtn);
		rtn = NULL;
	}
	if (iscp == JNI_TRUE)
	{
		(*env)->ReleaseStringChars(env, in, c_str);// JVM ʹ�á�֪ͨJVM c_str ��ָ�Ŀռ�����ͷ���
	}
	
	char *c_str2 = "ŵ����˹";
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID jmid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");
	
	//jstring -> jbyteArray
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(c_str2));
	// ��Char * ��ֵ�� bytes
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(c_str2), c_str2);
	jstring charsetName = (*env)->NewStringUTF(env, "GB2312");

	return (*env)->NewObject(env, str_cls, jmid, bytes, charsetName);


}
