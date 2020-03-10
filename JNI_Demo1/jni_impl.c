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

	
//JNI访问java中的非静态方法
JNIEXPORT void JNICALL Java_JniMain_accessMethod
(JNIEnv *env , jobject jobj){
	//得到jclass,即JniMain.class
	jclass jclz = (*env)->GetObjectClass(env, jobj);

	//得到jmethodID,方法的名字getRandomInt，方法的签名
	jmethodID mid = (*env)->GetMethodID(env,jclz,"getRandomInt","(I)I");
	
	//调用该非静态方法
	jint random = (*env)->CallIntMethod(env,jobj,mid,200);

	printf("C random:%d \n",random);

}


//JNI访问java中的静态方法
JNIEXPORT void JNICALL Java_JniMain_accessStaticMethod
(JNIEnv *env, jobject jobj){

	//得到jclass,即JniMain.class
	jclass clz = (*env)->GetObjectClass(env,jobj);

	//得到jmethodID,方法的名字getRandomUUId，方法签名
	jmethodID jmid = (*env)->GetStaticMethodID(env, clz, "getRandomUUId", "()Ljava/lang/String;");

	//调用该静态方法
	jstring uuid = (*env)->CallStaticObjectMethod(env,clz,jmid);

	//jstring -> char *
	char * uuid_c = (*env)->GetStringUTFChars(env,uuid,NULL);

	char filename[100];
	sprintf(filename,"D://%s.txt",uuid_c);
	FILE *fp = fopen(filename,"w+");
	fputs("I love timor",fp);
	fclose(fp);
	printf("文件写入成功\n");
}

//访问java的构造方法
JNIEXPORT jobject JNICALL Java_JniMain_accessConstructor
(JNIEnv * env, jobject jobj){
	//通过类的路径来从JVM里面找到对应的类
	jclass jclz = (*env)->FindClass(env,"java/util/Date");

	//获取构造方法的jmothodID
	jmethodID jmid = (*env)->GetMethodID(env,jclz,"<init>","()V");

	//实例化一个Date对象，返回值是一个jobject
	jobject dateObj = (*env)->NewObject(env,jclz,jmid);

	//获取getTime()的jmethodID,前提是我们访问了相关对象的构造函数创建了这个对象
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
	
	//iscp代表的是string转char的时候是否从新开辟的内存copy的一份
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
	//windows提供的字符串转码的操作
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
		(*env)->ReleaseStringChars(env, in, c_str);// JVM 使用。通知JVM c_str 所指的空间可以释放了
	}
	
	char *c_str2 = "诺克萨斯";
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID jmid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");
	
	//jstring -> jbyteArray
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(c_str2));
	// 将Char * 赋值到 bytes
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(c_str2), c_str2);
	jstring charsetName = (*env)->NewStringUTF(env, "GB2312");

	return (*env)->NewObject(env, str_cls, jmid, bytes, charsetName);


}
