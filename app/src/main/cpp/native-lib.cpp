#include <jni.h>
#include <string>

using namespace std;

// 日志输出
#include <android/log.h>

#define TAG "native-lib"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

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
Java_com_redrock_jnipractice_MainActivity_sum(__unused JNIEnv *env, __unused jobject thiz, jint a,
                                              jint b) {
    return a + b;
}

//修改java代码里的值
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_changeHello(JNIEnv *env, jobject thiz) {
    // 自己得到 MainActivity.class
    jclass jClass = env->GetObjectClass(thiz);
    jfieldID j_fid = env->GetFieldID(jClass, "hello", "Ljava/lang/String;");
    jstring jHello = env->NewStringUTF("Hello From C++");
    env->SetObjectField(thiz, j_fid, jHello);
}

//调用java里的方法
extern "C"
JNIEXPORT jint JNICALL
Java_com_redrock_jnipractice_MainActivity_minus(JNIEnv *env, jobject thiz, jint a, jint b) {
    jclass mainClass = env->GetObjectClass(thiz);

    jmethodID jMethodId = env->GetMethodID(mainClass, "minusInJava", "(II)I");

    jint result = env->CallIntMethod(thiz, jMethodId, a, b);

    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_changeAge(JNIEnv *env, jclass thiz) {
    jfieldID id = env->GetStaticFieldID(thiz, "age", "I");
    jint age = 666;
    env->SetStaticIntField(thiz, id, age);
}

//C++层修改java基本类型数组和String数组
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_changeArray(JNIEnv *env, jobject thiz,
                                                      jintArray int_array, jobjectArray str_array) {
    //    jsize GetArrayLength(jarray array)
    jsize intArrSize = env->GetArrayLength(int_array);
    jsize strArrSize = env->GetArrayLength(str_array);

    //拿到java基本类型的数组的头指针
    // jint* GetIntArrayElements(jintArray array, boolean* isCopy)
    int *jintArray = env->GetIntArrayElements(int_array, nullptr);
    //==================接着可以拿着这个指针进行修改===========================
    for(int i = 0;i < intArrSize;i++){
        *(jintArray + i) = 100 + i;
    }
    //修改完后记得通知java层代码 !!important
    /**
     * 0:           刷新Java数组，并 释放C++层数组
     * JNI_COMMIT:  只提交 只刷新Java数组，不释放C++层数组
     * JNI_ABORT:   只释放C++层数组
     */
    env->ReleaseIntArrayElements(int_array, jintArray, 0);


    for (int i = 0; i < strArrSize; i++) {
        jstring jString = static_cast<jstring>(env->GetObjectArrayElement(str_array, i));
        const char *jChar = env->GetStringUTFChars(jString, nullptr);
        LOGD("%s", jChar)
        //记得释放
        env->ReleaseStringUTFChars(jString, jChar);
    }
}


//传递引用类型对象并在C++层调用其方法
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_putObject(JNIEnv *env, jobject thiz, jobject student,
                                                    jstring str) {
    // 1.得到引用类型的class类型
    // 方法一：通过包名
    // jclass studentClass = env->FindClass("com/redrock/jnipractice/student");
    jclass studentClass = env->GetObjectClass(student); // 第二种

    // 2.得到引用类型对象里的方法ID
    jmethodID setName = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jmethodID getName = env->GetMethodID(studentClass, "getName", "()Ljava/lang/String;");
    jmethodID showInfo = env->GetStaticMethodID(studentClass, "showInfo", "(Ljava/lang/String;)V");

    // 3.调用 setName
    jstring value = env->NewStringUTF("AAAA");
    env->CallVoidMethod(student, setName, value);

    // 4.调用 getName
    jstring getNameResult = static_cast<jstring>(env->CallObjectMethod(student, getName));
    const char *getNameValue = env->GetStringUTFChars(getNameResult, NULL);
    LOGE("调用到getName方法，值是:%s\n", getNameValue);

    // 5.调用静态showInfo
    jstring jstringValue = env->NewStringUTF("静态方法你好，我是C++");
    env->CallStaticVoidMethod(studentClass, showInfo, jstringValue);

}

//在C++层创建java的对象
extern "C"
JNIEXPORT void JNICALL
Java_com_redrock_jnipractice_MainActivity_newObject(JNIEnv *env, jobject thiz) {
    // 1.通过包名+类名的方式 拿到 class
    const char *studentstr = "com/redrock/jnipractice/Student";
    jclass studentClass = env->FindClass(studentstr);

    // 2.通过student的class实例化此Student对象
    // AllocObject 只实例化对象，不调用对象的构造函数
    jobject studentObj = env->AllocObject(studentClass);

    //获得类中的方法ID
    jmethodID setName = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jmethodID setAge = env->GetMethodID(studentClass, "setAge", "(I)V");

    // 接下来就可以通过这个实例化的对象和方法ID调用方法
    jstring strValue = env->NewStringUTF("Test");
    env->CallVoidMethod(studentObj, setName, strValue);
    env->CallVoidMethod(studentObj, setAge, 99);


    // ==================== 下面演示调用构造方法来实例化对象 通过env->NewObject()
    jmethodID jStudentConstuctorMethod1Id = env->GetMethodID(studentClass,"<init>", "()V");
    jmethodID jStudentConstuctorMethod2Id = env->GetMethodID(studentClass,"<init>", "(Ljava/lang/String;)V");
    jmethodID jStudentConstuctorMethod3Id = env->GetMethodID(studentClass,"<init>", "(Ljava/lang/String;I)V");

    jobject studentObj1 = env->NewObject(studentClass,jStudentConstuctorMethod1Id);

    jstring studentJstr = env->NewStringUTF("123");
    jobject studentObj2 = env->NewObject(studentClass,jStudentConstuctorMethod2Id,studentJstr);

    jobject studentObj3 = env->NewObject(studentClass,jStudentConstuctorMethod3Id,studentJstr,3);

    // 释放局部引用
    env->DeleteLocalRef(studentClass);
    env->DeleteLocalRef(studentObj);
    env->DeleteLocalRef(studentObj1);
    env->DeleteLocalRef(studentObj2);
    env->DeleteLocalRef(studentObj3);
}