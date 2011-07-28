/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_openhome_net_device_DvDeviceStandard */

#ifndef _Included_org_openhome_net_device_DvDeviceStandard
#define _Included_org_openhome_net_device_DvDeviceStandard
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_openhome_net_device_DvDeviceStandard
 * Method:    DvDeviceStandardCreateNoResources
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_openhome_net_device_DvDeviceStandard_DvDeviceStandardCreateNoResources
  (JNIEnv *, jclass, jstring);

/*
 * Class:     org_openhome_net_device_DvDeviceStandard
 * Method:    DvDeviceStandardCreate
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_openhome_net_device_DvDeviceStandard_DvDeviceStandardCreate
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_openhome_net_device_DvDeviceStandard
 * Method:    DvDeviceDestroy
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_openhome_net_device_DvDeviceStandard_DvDeviceDestroy
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     org_openhome_net_device_DvDeviceStandard
 * Method:    DvResourceWriterLanguageCount
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_openhome_net_device_DvDeviceStandard_DvResourceWriterLanguageCount
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_openhome_net_device_DvDeviceStandard
 * Method:    DvResourceWriterLanguage
 * Signature: (JI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_openhome_net_device_DvDeviceStandard_DvResourceWriterLanguage
  (JNIEnv *, jclass, jlong, jint);

#ifdef __cplusplus
}
#endif
#endif
