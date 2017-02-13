LOCAL_PATH := $(call my-dir)

$(call import-add-path,$(COCOS_FRAMEWORKS)/cocos2d-x-3.8/)
$(call import-add-path,$(COCOS_FRAMEWORKS)/cocos2d-x-3.8/cocos)
$(call import-add-path,$(COCOS_FRAMEWORKS)/cocos2d-x-3.8/external)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := hellocpp/main.cpp 
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,./prebuilt-mk)
