#ifndef logger
#define logger
#include <stdarg.h>
#include "../../common.h"

// 'modes' of the logger.
// TODO: Make these usable :)
enum LogModes {
    INFO,
    WARN,
    ERROR,
    SUGGEST
};

// Error codes
// TODO: Make use cases out of this enum
enum ErrorCodes {
    NoError,
    NoTokenValue,
    UnexpectedToken,
    InvalidOperandSizes,
};

// Ansi codes.
#define INFO_COLOR          (UNS8 *)"\033[1;37m"
#define WARN_COLOR          (UNS8 *)"\033[1;35m"
#define ERROR_COLOR         (UNS8 *)"\033[1;31m"
#define SUGG_COLOR          (UNS8 *)"\033[1;33m"
#define RESET               (UNS8 *)"\033[0m"

// "Extra"
#define UL                  (UNS8 *)"\033[4;37m"


// Logger macros.
#define LOG_Info(msg, ...)                                 \
    fprintf(stdout, "%s[INFO]%s:\t",INFO_COLOR, RESET);   \
    fprintf(stdout, msg, ##__VA_ARGS__);
#define LOG_Warn(msg, ...)                                  \
    fprintf(stdout, "%s[MocaAsm, WARN]%s:\t", WARN_COLOR, RESET);    \
    fprintf(stdout, msg, ##__VA_ARGS__);                    
#define LOG_Error(msg, ...)                                 \
    fprintf(stderr, "%s[ERROR]%s:\t", ERROR_COLOR, RESET);  \
    fprintf(stderr, msg, ##__VA_ARGS__);
#define LOG_Suggest(msg, ...)                               \
    fprintf(stdout, "%s[MocaAsm, SUGGEST]%s", SUGG_COLOR, RESET);    \
    fprintf(stdout, msg, ##__VA_ARGS__);

#endif