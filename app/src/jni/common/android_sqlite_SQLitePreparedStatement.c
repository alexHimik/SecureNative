#include <jni.h>
#include "sqlite3.h"

#include "android_sqlite.h"

jfieldID queryArgsCountField;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = 0;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	jclass class  = (*env)->FindClass(env, "ua/secure/sqlite/SQLitePreparedStatement");

	queryArgsCountField = (*env)->GetFieldID(env, class, "queryArgsCount", "I");

	return JNI_VERSION_1_4;
}

int Java_ua_secure_sqlite_SQLitePreparedStatement_prepare(JNIEnv* env, jobject object, int sqliteHandle, jstring sql) {
	sqlite3* handle = (sqlite3*) sqliteHandle;

    char const * sqlStr = (*env)->GetStringUTFChars(env, sql, 0);
	//LOGI("SQL");
	//LOGI(sqlStr);
    sqlite3_stmt* stmt_handle;

    int errcode = sqlite3_prepare_v2(handle, sqlStr, -1, &stmt_handle, 0);
    //LOGI("ERRCODE");
    const char * error = sqlite3_errmsg(handle);
    //LOGI(error);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, handle, errcode);
    } else {
    	int argsCount = sqlite3_bind_parameter_count(stmt_handle);

    	(*env)->SetIntField(env, object, queryArgsCountField, argsCount);
    }

    if (sqlStr != 0) (*env)->ReleaseStringUTFChars(env, sql, sqlStr);

    return (int)stmt_handle;
}

jobjectArray Java_ua_secure_sqlite_SQLitePreparedStatement_reset(JNIEnv* env, jobject object, int statementHandle) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	int errcode = sqlite3_reset (handle);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }

    int stmtColumnCount = sqlite3_column_count(handle);
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray strArray = (*env)->NewObjectArray(env, stmtColumnCount, stringClass, 0);
    jsize i = 0;

    for (i=0; i<stmtColumnCount; i++) {
		const char* name = sqlite3_column_name(handle, i);

    	jstring nameString = (*env)->NewStringUTF(env, name);

    	(*env)->SetObjectArrayElement(env, strArray, i, nameString);

    	(*env)->DeleteLocalRef(env, nameString);
    }

    return strArray;
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_finalize(JNIEnv* env, jobject object, int statementHandle) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	int errcode = sqlite3_finalize (handle);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_bindByteBuffer(JNIEnv* env, jobject object, int statementHandle, int index, jobject value) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	const void* buf = (*env)->GetDirectBufferAddress(env, value);
	int length = (*env)->GetDirectBufferCapacity(env, value);

	int errcode = sqlite3_bind_blob(handle, index, buf, length, SQLITE_STATIC);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_bindString(JNIEnv* env, jobject object, int statementHandle, int index, jstring value) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	char const * valueStr = (*env)->GetStringUTFChars(env, value, 0);

	char * encoded = encodeStringData(valueStr);
	int errcode = sqlite3_bind_text(handle, index, encoded, -1, SQLITE_TRANSIENT);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }

	if (valueStr != 0) (*env)->ReleaseStringUTFChars(env, value, valueStr);
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_bindInt(JNIEnv* env, jobject object, int statementHandle, int index, int value) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	int encoded = encodeIntData(value);
	int errcode = sqlite3_bind_int(handle, index, encoded);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_bindDouble(JNIEnv* env, jobject object, int statementHandle, int index, double value) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	double encoded = encodeDoubleData(value);
	int errcode = sqlite3_bind_double(handle, index, encoded);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }
}

void Java_ua_secure_sqlite_SQLitePreparedStatement_bindNull(JNIEnv* env, jobject object, int statementHandle, int index) {
	sqlite3_stmt* handle = (sqlite3_stmt*)statementHandle;

	int errcode = sqlite3_bind_null(handle, index);
    if (SQLITE_OK != errcode) {
    	throw_sqlite3_exception(env, sqlite3_db_handle(handle), errcode);
    }
}

int encodeIntData(int value) {

    return value;
}

double encodeDoubleData(double value) {

    return value;
}

char * encodeStringData(char * value) {

    return value;
}

