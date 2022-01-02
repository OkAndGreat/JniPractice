#include <jni.h>
#include <string>

using namespace std;

//java中调用C中的字符串
extern "C" JNIEXPORT jstring JNICALL
Java_com_redrock_jnipractice_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}