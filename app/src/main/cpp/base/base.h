//
// Created by 单继强 on 2020-04-29.
//

#ifndef LEARNNDK_BASE_H
#define LEARNNDK_BASE_H

#include <android/log.h>
#include <jni.h>
#define LOG_TAG "ALLEN_JNI"
#define LOGI(...)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


#endif //LEARNNDK_BASE_H
