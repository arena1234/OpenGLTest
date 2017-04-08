LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE        := libplayer_jni
LOCAL_CFLAGS        := -Werror

LOCAL_C_INCLUDES    :=  $(LOCAL_PATH)
LOCAL_SRC_FILES     :=  file/file.cpp               \
                        transform/touch.cpp         \
                        transform/sensor.cpp        \
                        transform/transform.cpp     \
                        bean/float_buffer.cpp       \
                        gl/gl_matrix.cpp            \
                        gl/gl_base.cpp              \
                        gl/gl_fbo.cpp               \
                        gl/gl_picture.cpp           \
                        gl/gl_video.cpp             \
                        bean/bean.cpp               \
                        bean/region.cpp             \
                        jni_api.cpp

LOCAL_LDLIBS        := -llog -lGLESv3 -lEGL -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
