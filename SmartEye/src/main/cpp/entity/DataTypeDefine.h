//
// Created by chuangbo.cheng on 2020/8/3.
//

#ifndef SMART_EYE_DATA_TYPE_DEFINE_H
#define SMART_EYE_DATA_TYPE_DEFINE_H

#include <map>
#include <string>
#include <vector>
#include <mutex>

typedef unsigned long long u8;
typedef unsigned char BYTE;

typedef unsigned char byte;

/** 用于表示时间戳的类型 */
typedef unsigned long long Timestamp;

/** 最大时间戳 */
#define MaxTimestamp ULLONG_MAX

/**
 * 用来表示无符号Long型
 */
typedef unsigned long long Long;

/**
 * 字符串类型
 */
typedef std::string String;

/**
 * 文件内容
 */
typedef char *Blob;

/**
 * 锁类型
 */
typedef std::mutex Mutex;


/** 用于数据库的主键 */
typedef unsigned long long PrimaryKey;

typedef std::string String;

typedef void *Any;

typedef std::map<String, Any> StringAnyMap;

typedef std::map<String, String> StringMap;

typedef std::vector<String> StringVector;

template<typename Key, typename Value>
using PairVector = std::vector<std::pair<Key, Value>>;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

/** 内存大小 */
typedef unsigned long MemorySize;

#endif //SMART_EYE_DATA_TYPE_DEFINE_H
