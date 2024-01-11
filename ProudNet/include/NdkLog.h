/*
 * log.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: kjsa
 */

#ifdef _NDK_LOG_ENABLE_
#include <android/log.h>

#define LOGV(...)   __android_log_print(ANDROID_LOG_VERBOSE, "ProudNet_VERBOSE", __VA_ARGS__)
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG, "ProudNet_DEBUG", __VA_ARGS__)
#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO, "ProudNet_INFO", __VA_ARGS__)
#define LOGW(...)   __android_log_print(ANDROID_LOG_WARN, "ProudNet_WARN", __VA_ARGS__)
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR, "ProudNet_ERROR", __VA_ARGS__)

#else
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#endif /* LOG_H_ */
