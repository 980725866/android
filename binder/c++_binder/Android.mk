LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	test_server.cpp \
	BnHelloService.cpp \
	BpHelloService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	liblog \
	libbinder \

LOCAL_MODULE:= test_server
LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	test_client.cpp \
	BnHelloService.cpp \
	BpHelloService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	liblog \
	libbinder \

LOCAL_MODULE:= test_client
LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)