//
// Created by lpf on 2025/3/26.
//

#ifndef BEDEMO2_COMMONMACRO_H
#define BEDEMO2_COMMONMACRO_H

/**
 * 获取单例
 * TYPE:接口类名
 */
#define GET_INSTANCE(TYPE)                  \
 TYPE::getInstance()                        \

// TYPE:接口类名
#define DEF_INSTANCE(TYPE)                  \
static TYPE * _Nonnull getInstance();                \

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
}

#endif //BEDEMO2_COMMONMACRO_H
