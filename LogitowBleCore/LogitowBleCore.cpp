#include <jni.h>
#include"com_logitow_bleapi_LogitowBleControl.h"
#include <stdio.h>
#include "LogitowBleControler.h"
#include "BleEvent.hpp"
#include "event\EventBus.hpp"

using namespace LogitowApi;

static JavaVM *bleenv = NULL; // java虚拟机保存起来  
static jobject bleobj = NULL; // 保存起来 

/**
*蓝牙监听处理
*/
class BleListener : public EventHandler<BleEvent>
{
public:
	BleListener() { }

	virtual ~BleListener() { }


	/**
	* 
	*
	* @param e The BleEvent event
	*/
	virtual void onEvent(BleEvent * e) override {

		// Ignore the event if it's already been canceled
		if (e->getCanceled()) {
			return;
		}

		//NPLInterface::NPLObjectProxy output_msg;
		//output_msg["succeed"] = "true";
		//output_msg["sample_number_output"] = (double)(1234567);
		//output_msg["result"] = e->getMessage();

		//std::string output;
		//NPLInterface::NPLHelper::NPLTableToString("msg", output_msg, output);
		//pState->activate("script/test/echo.lua", output.c_str(), output.size());

		//jclass clazz_TestNative = bleenv->GetObjectClass(bleobj);
		//jmethodID max_id = bleenv->GetMethodID(clazz_TestNative, "sendBleNotification", "(Ljava/lang/String;)V");
		//jdouble maxvalue = bleenv->CallDoubleMethod(bleobj, max_id,e->getMessage().c_str());
	//	jstring data_str = bleenv->NewStringUTF(e->getMessage().c_str());
		//bleenv->CallVoidMethod(bleobj, max_id,data_str);

		// 注意！！我要调用了  
		JNIEnv *env;
		// 获取当前线程的 env   
		bleenv->AttachCurrentThread((void **)&env, NULL);
		// 这个class默认是初始化gs_object时所调用的Java 类  
		jclass cls = env->GetObjectClass(bleobj);
		// 开始对讲消息------  
		jmethodID mid = env->GetMethodID(cls, "sendBleNotification", "(Ljava/lang/String;)V");
		if (mid == NULL) {
			return;
		}
		jstring data_str = env->NewStringUTF(e->getMessage().c_str());
		// 调用  
		env->CallVoidMethod(bleobj, mid,data_str);
		// 用完之后一定要  DetachCurrentThread 取消关联，要不然程序退出会有异常  
		(bleenv)->DetachCurrentThread();
	}

private:
	//static const int BORDER_SIZE = 500;

};


//java调用蓝牙sdk初始化方法
// bleInit
// 获取java虚拟机和java对象

JNIEXPORT jint JNICALL
Java_com_logitow_bleapi_LogitowBleControl_bleInit(JNIEnv *env, jobject obj)
{

	// 注意了，在第一次进来的时候，我就保存他们了，要快！！！！  
	env->GetJavaVM(&bleenv);    
	//保存到全局变量中JVM  
	bleobj = env->NewGlobalRef(obj);
	printf("bleInit!/n");
	BleListener* blehand = new BleListener();
	EventBus::AddHandler<BleEvent>(blehand);

	return 1;
}

JNIEXPORT void JNICALL
Java_com_logitow_bleapi_LogitowBleControl_startSearchBle(JNIEnv *env, jobject obj)
{
	printf("startSearchBle!/n");
	LogitowApi::LogitowBleControler::GetInstance()->ConnectDevice();
	return;
}

JNIEXPORT jint JNICALL
Java_com_logitow_bleapi_LogitowBleControl_getBleStatus(JNIEnv *env, jobject obj)
{
	printf("getBleStatus!/n");
	if (LogitowApi::LogitowBleControler::GetInstance()->connected)
	{
		return 1;
	}
	return 0;
}

JNIEXPORT void JNICALL
Java_com_logitow_bleapi_LogitowBleControl_connectDevice(JNIEnv *env, jobject obj)
{
	printf("connectDevice!/n");
	LogitowApi::LogitowBleControler::GetInstance()->ConnectDevice();
	return;
}

/*
JNIEXPORT void JNICALL Java_jni_TestNative_sayhello(JNIEnv *env, jobject object) {
	//首先得到jclass对象，是后面得到属性和方法ID的基础
	jclass clazz_TestNative = env->GetObjectClass(object);
	jmethodID max_id = env->GetMethodID(clazz_TestNative, "max", "(DD)D");
	jdouble maxvalue = env->CallDoubleMethod(object, max_id, 3.14, 3.15);
	cout << maxvalue << endl;
}
*/
