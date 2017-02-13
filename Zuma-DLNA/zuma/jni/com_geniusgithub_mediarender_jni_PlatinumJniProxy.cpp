#include <assert.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>

#include "com_geniusgithub_mediarender_jni_PlatinumJniProxy.h"
#include "Platinum.h"

#include "Logs.h"

#include "DMRCenter.h"

static const char *classPathName = "com/bftv/service/dlna/jni/PlatinumReflection";

DMRCenter *g_DMRCenter = NULL;

JavaVM* g_vm = NULL;

jclass  g_inflectClass = NULL;

jmethodID g_methodID = NULL;

char* ConvertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray, jbyte *&bytes)
{
    char *chars = NULL;
    bytes = env->GetByteArrayElements(bytearray, 0);
    int chars_len = env->GetArrayLength(bytearray);
    chars = new char[chars_len + 1];
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;

    return chars;
}

void ActionInflect(int cmd, const char* value, const char* data)
{
    if (g_vm == NULL)
    {
        LOGW("g_vm = NULL!!!");
        return ;
    }

    int status;
    JNIEnv *env = NULL;
    bool isAttach = false;
    status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if(status != JNI_OK)
    {
        status = g_vm->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOGW("callback_handler: failed to attach , current thread, status = %d", status);
            return;
        }
        isAttach = true;
    }

    jstring valueString = NULL;
    jstring dataString = NULL;
    jclass inflectClass = g_inflectClass;
    jmethodID inflectMethod = g_methodID;

    if (inflectClass == NULL || inflectMethod == NULL)
    {
        goto end;
    }

    LOGW("CMD = %d\nVALUE = %s\nDATA = %s",cmd, value, data);

    valueString = env->NewStringUTF(value);
    dataString = env->NewStringUTF(data);

    env->CallStaticVoidMethod(inflectClass, inflectMethod, cmd, valueString, dataString);

    env->DeleteLocalRef(valueString);
    env->DeleteLocalRef(dataString);

end:
    if (env->ExceptionOccurred())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if (isAttach)
    {
        g_vm->DetachCurrentThread();
    }
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGW("JNI_OnLoad!");

    g_vm = vm;

    g_DMRCenter = new DMRCenter();

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOGW("JNI_OnUnload!");
}

/*
 * Class:     com_geniusgithub_mediarender_jni_PlatinumJniProxy
 * Method:    startMediaRender
 * Signature: ([B[B)I
 */
JNIEXPORT jint JNICALL Java_com_bftv_service_dlna_jni_PlatinumJniProxy_startMediaRender
  (JNIEnv *env, jclass, jbyteArray name, jbyteArray showVR, jbyteArray version, jbyteArray uuid)
{
    LOGW("startMediaRender!");

    jclass myclass = env->FindClass(classPathName);
    g_inflectClass = (jclass)env->NewGlobalRef(myclass);
    g_methodID = env->GetStaticMethodID(g_inflectClass, "onActionReflection","(ILjava/lang/String;Ljava/lang/String;)V");

    jbyte *bytes;
    const char *c_name = (const char*)ConvertJByteaArrayToChars(env, name, bytes);

    jbyte *bytes0;
    const char *c_showVR = (const char*)ConvertJByteaArrayToChars(env, showVR, bytes0);

    jbyte *bytes1;
    const char *c_version = (const char*)ConvertJByteaArrayToChars(env, version, bytes1);
    
    jbyte *bytes2;
    const char *c_uuid = (const char *)ConvertJByteaArrayToChars(env, uuid, bytes2);

    int ret = g_DMRCenter->Start(c_name, c_showVR, c_version, c_uuid);

    env->ReleaseByteArrayElements(name,    bytes,  JNI_ABORT);
    env->ReleaseByteArrayElements(showVR,  bytes0, JNI_ABORT);
    env->ReleaseByteArrayElements(version, bytes1, JNI_ABORT);
    env->ReleaseByteArrayElements(uuid,    bytes2, JNI_ABORT);

    delete c_name;
    delete c_showVR;
    delete c_version;
    delete c_uuid;

    return ret;
}

/*
 * Class:     com_geniusgithub_mediarender_jni_PlatinumJniProxy
 * Method:    stopMediaRender
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_bftv_service_dlna_jni_PlatinumJniProxy_stopMediaRender
  (JNIEnv *env, jclass)
{
    LOGW("stopMediaRender!");

    g_DMRCenter->Stop();

    if (g_inflectClass != NULL) {
        env->DeleteGlobalRef(g_inflectClass);
        g_inflectClass = NULL;
    }
}

/*
 * Class:     com_geniusgithub_mediarender_jni_PlatinumJniProxy
 * Method:    responseGenaEvent
 * Signature: (I[B[B)Z
 */
JNIEXPORT jboolean JNICALL Java_com_bftv_service_dlna_jni_PlatinumJniProxy_responseGenaEvent
  (JNIEnv *env, jclass, jint cmd, jbyteArray value, jbyteArray data)
{
    LOGW("responseGenaEvent!");

    jbyte *bytes;
    const char *c_value = (const char*)ConvertJByteaArrayToChars(env, value, bytes);

    jbyte *bytes2;
    const char *c_data = (const char *)ConvertJByteaArrayToChars(env, data, bytes2);

    int ret = g_DMRCenter->responseGenaEvent(cmd, c_value, c_data);

    env->ReleaseByteArrayElements(value, bytes, JNI_ABORT);
    env->ReleaseByteArrayElements(data, bytes2, JNI_ABORT);

    delete c_value;
    delete c_data;
}

/*
 * Class:     com_geniusgithub_mediarender_jni_PlatinumJniProxy
 * Method:    enableLogPrint
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_bftv_service_dlna_jni_PlatinumJniProxy_enableLogPrint
  (JNIEnv *, jclass, jboolean flag)
{
    LOGW("enableLogPrint");
}
