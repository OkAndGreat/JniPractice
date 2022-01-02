#include <jni.h>
#include <string>

using namespace std;

//java中调用C中的字符串
extern "C" JNIEXPORT jstring JNICALL
Java_com_redrock_jnipractice_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//俩数相加
extern "C"
JNIEXPORT jint JNICALL
Java_com_redrock_jnipractice_MainActivity_sum(JNIEnv *env, jobject thiz, jint a, jint b) {
    return a+b;
}

//修改java代码里的值
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_changeHello(JNIEnv *env, jobject thiz) {
    // 自己得到 MainActivity.class
    jclass jClass = env->GetObjectClass(thiz);
    jfieldID j_fid = env->GetFieldID(jClass,"hello", "Ljava/lang/String;");
    jstring jHello = env->NewStringUTF("Hello From C++");
    env->SetObjectField(thiz,j_fid,jHello);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_redrock_jnipractice_MainActivity_minus(JNIEnv *env, jobject thiz, jint a, jint b) {
    jclass mainClass = env->GetObjectClass(thiz);

    jmethodID jMethodId = env->GetMethodID(mainClass,"minusInJava","(II)I");

    jint result = env->CallIntMethod(thiz,jMethodId,a,b);

    return result;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_changeAge(JNIEnv *env, jclass thiz) {
    jfieldID id = env->GetStaticFieldID(thiz,"age","I");
    jint age = env->GetStaticIntField(thiz,id);
    age = 666;
    env->SetStaticIntField(thiz,id,age);
}

/*
   签名规则 大写

   javap -s -p MainActivity     必须是.class

    Java的boolean  --- Z
    Java的byte  --- B
    Java的char  --- C
    Java的short  --- S
    Java的int  --- I
    Java的long  --- J
    Java的float  --- F
    Java的double  --- D
    Java的void  --- V
    Java的引用类型  --- Lxxx/xxx/xx/类名;
    Java的String  --- Ljava/lang/String;
    Java的array  int[]  --- [I         double[][][][]  --- [[[D
    int add(char c1, char c2) ---- (CC)I
    void a()     ===  ()V

    javap -s -p xxx.class    -s 输出xxxx.class的所有属性和方法的签名，   -p 忽略私有公开的所有属性方法全部输出
 */