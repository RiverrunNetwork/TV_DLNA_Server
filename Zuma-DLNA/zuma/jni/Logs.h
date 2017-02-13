#ifndef LOGS_H_
#define LOGS_H_

#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "MediaRender", __VA_ARGS__) 
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "MediaRender", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "MediaRender", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , "MediaRender", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "MediaRender", __VA_ARGS__)

#endif //LOGS_H_
