//
// Created by lpf on 2025/3/25.
//

#ifndef BEDEMO2_LOG_H
#define BEDEMO2_LOG_H

#include <android/log.h>

#ifndef LOG_TEXT_MAX_LEN
#define LOG_TEXT_MAX_LEN  (1024 * 2)  // 2KB
#endif

class Log {
public:
    static void info(const char* tag, const char* format, ...) {

        char logText[LOG_TEXT_MAX_LEN] = {0};

        va_list arg_list;
        va_start(arg_list, format);
        vsnprintf(logText, LOG_TEXT_MAX_LEN, format, arg_list);
        va_end(arg_list);

        __android_log_print(ANDROID_LOG_INFO, tag, "%s", logText );
    }
};


#endif //BEDEMO2_LOG_H
