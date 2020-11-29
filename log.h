#ifndef __Log_H__
#define __Log_H__
//******************************************************************************
#include <cstring>
#include "utils/time_helper.h"

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define NO_LOG          0x00
#define ERROR_LEVEL     0x01
#define WARNING_LEVEL   0x02
#define INFO_LEVEL      0x03
#define DEBUG_LEVEL     0x04

#ifndef LOG_LEVEL
#define LOG_LEVEL   DEBUG_LEVEL
#endif

#define PRINT_FUNCTION(format, ...)      fprintf(stderr, format, __VA_ARGS__)

#define LOG_FMT             "%s | %-5s | %20s:%-4d] "
#define LOG_ARGS(LOG_TAG)   CTimeHelper::getTimeString().c_str(), LOG_TAG, _FILE, __LINE__

#define NEWLINE     "\n"

#define ERROR_TAG   "ERROR"
#define WARNING_TAG "WARN"
#define INFO_TAG    "INFO"
#define DEBUG_TAG   "DEBUG"

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(message, args...)     PRINT_FUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(DEBUG_TAG), ## args)
#else
#define LOG_DEBUG(message, args...)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define LOG_INFO(message, args...)      PRINT_FUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(INFO_TAG), ## args)
#else
#define LOG_INFO(message, args...)
#endif

#if LOG_LEVEL >= WARNING_LEVEL
#define LOG_WARNING(message, args...)   PRINT_FUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(WARNING_TAG), ## args)
#else
#define LOG_WARNING(message, args...)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define LOG_ERROR(message, args...)     PRINT_FUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define LOG_ERROR(message, args...)
#endif

#if LOG_LEVEL >= NO_LOGS
#define LOG_IF_ERROR(condition, message, args...) if (condition) PRINT_FUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define LOG_IF_ERROR(condition, message, args...)
#endif
//******************************************************************************
#endif