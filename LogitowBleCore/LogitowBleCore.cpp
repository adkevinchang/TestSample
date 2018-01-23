#include <jni.h>
#include"com_logitow_bleapi_LogitowBleControl.h"
#include <stdio.h>
#include "LogitowBleControler.h"
#include "BleEvent.hpp"
#include "event\EventBus.hpp"

using namespace LogitowApi;

static JavaVM *bleenv = NULL; // java�������������  
static jobject bleobj = NULL; // �������� 

/**
*������������
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

		// ע�⣡����Ҫ������  
		JNIEnv *env;
		// ��ȡ��ǰ�̵߳� env   
		bleenv->AttachCurrentThread((void **)&env, NULL);
		// ���classĬ���ǳ�ʼ��gs_objectʱ�����õ�Java ��  
		jclass cls = env->GetObjectClass(bleobj);
		// ��ʼ�Խ���Ϣ------  
		jmethodID mid = env->GetMethodID(cls, "sendBleNotification", "(Ljava/lang/String;)V");
		if (mid == NULL) {
			return;
		}
		jstring data_str = env->NewStringUTF(e->getMessage().c_str());
		// ����  
		env->CallVoidMethod(bleobj, mid,data_str);
		// ����֮��һ��Ҫ  DetachCurrentThread ȡ��������Ҫ��Ȼ�����˳������쳣  
		(bleenv)->DetachCurrentThread();
	}

private:
	//static const int BORDER_SIZE = 500;

};


//java��������sdk��ʼ������
// bleInit
// ��ȡjava�������java����

JNIEXPORT jint JNICALL
Java_com_logitow_bleapi_LogitowBleControl_bleInit(JNIEnv *env, jobject obj)
{

	// ע���ˣ��ڵ�һ�ν�����ʱ���Ҿͱ��������ˣ�Ҫ�죡������  
	env->GetJavaVM(&bleenv);    
	//���浽ȫ�ֱ�����JVM  
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
	//���ȵõ�jclass�����Ǻ���õ����Ժͷ���ID�Ļ���
	jclass clazz_TestNative = env->GetObjectClass(object);
	jmethodID max_id = env->GetMethodID(clazz_TestNative, "max", "(DD)D");
	jdouble maxvalue = env->CallDoubleMethod(object, max_id, 3.14, 3.15);
	cout << maxvalue << endl;
}
*/
