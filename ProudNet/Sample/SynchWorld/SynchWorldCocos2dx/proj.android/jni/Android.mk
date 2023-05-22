LOCAL_SHORT_COMMANDS := true

LOCAL_ABSOLUTE_PATH := $(call my-dir)

# ProudNetClient Prebuilt

include $(CLEAR_VARS)

LOCAL_PATH := $(LOCAL_ABSOLUTE_PATH)

LOCAL_MODULE := ProudNetClient

LOCAL_SRC_FILES := ../../../../../lib/NDK/r8b/4.6/Debug/libProudNetClient.a

include $(PREBUILT_STATIC_LIBRARY)

# App Build

include $(CLEAR_VARS)

LOCAL_PATH := $(LOCAL_ABSOLUTE_PATH)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Client.cpp \
                   ../../Classes/ConnectLayer.cpp \
                   ../../Classes/CustomTableViewDataSource.cpp \
                   ../../Classes/ErrorLayer.cpp \
                   ../../Classes/LocalHero.cpp \
                   ../../Classes/MainGUILayer.cpp \
                   ../../Classes/MainLayer.cpp \
                   ../../Classes/RemoteHero_C.cpp \
                   ../../Classes/World.cpp \
                   ../../Classes/stdafx.cpp \
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Common \
					$(LOCAL_PATH)/../../../../../include \

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += ProudNetClient

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
