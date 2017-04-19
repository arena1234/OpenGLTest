LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

#opencv
include $(LOCAL_PATH)/native/jni/OpenCV.mk

LOCAL_MODULE        := libplayer_jni
LOCAL_CFLAGS        := -Werror

LOCAL_C_INCLUDES    :=  $(LOCAL_PATH)               \
                        $(LOCAL_PATH)/open_cv_include
LOCAL_SRC_FILES     :=  file/file.cpp               \
                        transform/touch.cpp         \
                        transform/sensor.cpp        \
                        transform/transform.cpp     \
                        bean/float_buffer.cpp       \
                        gl/gl_matrix.cpp            \
                        gl/gl_shape.cpp             \
                        gl/gl_base.cpp              \
                        gl/gl_renderer.cpp          \
                        gl/gl_fbo.cpp               \
                        gl/gl_picture.cpp           \
                        gl/gl_video.cpp             \
                        bean/bean.cpp               \
                        bean/region.cpp             \
                        jni_api.cpp

LOCAL_LDLIBS        += -llog -lGLESv3 -lEGL -ljnigraphics -lm

include $(BUILD_SHARED_LIBRARY)
