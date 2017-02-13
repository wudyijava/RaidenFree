APP_STL := gnustl_static

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif

#ע�⣺���������armeabi-v7a���룬����ʹ��armeabi��������ĳЩ������(�ر�����boss��ʱ��)��ܿ�
#APP_ABI=all32 is equivalent to APP_ABI=armeabi,armeabi-v7a,x86,mips.
#APP_ABI=all64 is equivalent to APP_ABI=arm64-v8a,x86_64,mips64.
APP_ABI := armeabi-v7a