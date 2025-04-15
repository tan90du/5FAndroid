//
// Created by jianxi.zjx on 2018/10/9.
//

#ifndef SOCOL_MODULE_COMMONMACRO_H
#define SOCOL_MODULE_COMMONMACRO_H
#include <pthread.h>


/**
 * 创建对象，释放对象
 */
#if 1
/**
 * 释放对象
 */
#define DELETE(OBJ)                                                 \
DELETE_(OBJ,__FILE__, __LINE__)                                     \

/**
 * 释放对象数组
 */
#define DELETEARRAY(OBJ)                                            \
DELETEARRAY_(OBJ,__FILE__, __LINE__)                                \

/**
 * 创建对象
 */
#define NEW                                                         \
new                                             \

extern void * operator new(size_t size, const char *file, int line);
extern void * operator new[](size_t size, const char *file, int line);
extern void operator delete(void * p);
extern void operator delete [] (void * p);
extern void operator delete(void * p, const char *file, int line);
extern void operator delete [] (void * p, const char *file, int line);
#define DELETE_(OBJ, FILE, LINE)                                    \
do{                                                                 \
    printf("Del point:%p,file:%s,line:%d\n",OBJ, FILE, LINE);       \
    if(OBJ){                                                        \
        delete OBJ;                                                 \
        OBJ = NULL;                                                 \
    }                                                               \
}while(0);                                                          \

#define DELETEARRAY_(OBJ, FILE, LINE)                               \
do{                                                                 \
    printf("Del[] point:%p,file:%s,line:%d\n",OBJ, FILE, LINE);     \
    if(OBJ){                                                        \
        delete[] OBJ;                                               \
        OBJ = NULL;                                                 \
    }                                                               \
}while(0);                                                          \

#define __DUMP_MEMORY__ 1
#else
#define NEW                                     new
#define DELETE                                  delete
#define DELETEARRAY                             delete[]
#endif
/**
 * 获取单例
 * TYPE:接口类名
 */
#define GET_INSTANCE(TYPE)                  \
 TYPE::getInstance()                        \

// TYPE:接口类名
#define DEF_INSTANCE(TYPE)                  \
static TYPE * getInstance();                \

// TYPE:接口类名,IMPL:实现类名
#define IMPL_INSTANCE(TYPE,IMPL)            \
static InsMutex sLocker;                    \
static TYPE* sInstance = NULL;              \
TYPE* TYPE::getInstance(){                  \
    TYPE* tmp = sInstance;                  \
    if(NULL == tmp){                        \
        InsLockWrap lock(sLocker);          \
        tmp = sInstance;                    \
        if(NULL == tmp){                    \
            tmp = new IMPL();               \
            sInstance = tmp;                \
        }                                   \
    }                                       \
    return tmp;                             \
}                                           \

/**
 * 创建对象,创建的对象需自己释放
 * TYPE:接口类名
 * CREATE获取的对象需自行释放
 */
#define CREATE(TYPE)                        \
 TYPE::create()                             \

// TYPE:接口类名
#define DEF_CREATE(TYPE)                    \
static TYPE* create();                      \

// TYPE:接口类名,IMPL:实现类名
#define IMPL_CREATE(TYPE,IMPL)              \
TYPE* TYPE::create(){                       \
    return new IMPL();                      \
}                                           \

/**
 * 辅助类
 */
class InsMutex{
public:
    InsMutex(){
        pthread_mutex_init(&mutex, NULL);
    }
    ~InsMutex(){
        pthread_mutex_destroy(&mutex);
    }
    void lock(){
        pthread_mutex_lock(&mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
private:
    pthread_mutex_t mutex;
};
class  InsLockWrap{
public:
    InsLockWrap(InsMutex &Lock):
            m_Lock(&Lock) {
        m_Lock->lock();
    }
    ~InsLockWrap(){
        m_Lock->unlock();
    }
private:
    InsMutex * m_Lock;
};


#endif //SOCOL_MODULE_COMMONMACRO_H
