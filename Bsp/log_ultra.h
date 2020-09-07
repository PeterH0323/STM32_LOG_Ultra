
#ifndef __LOG_ULTRA_H
#define __LOG_ULTRA_H

/*-----------------------------------------------------------
 * Includes files
 *----------------------------------------------------------*/
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*-----------------------------------------------------------
 * Exported constants
 *----------------------------------------------------------*/


/*-----------------------------------------------------------
 * Exported macro
 *----------------------------------------------------------*/
 
/**
 * @defgroup Log module configuration
 * @{
 */

/** Enable logging module. */
#define LOG_ENABLE 1

/** Default log level. Messages with lower criticality is filtered. */
#define LOG_LEVEL_DEFAULT LOG_LEVEL_WARN

/** Default log mask. Messages with other sources are filtered. */
#define LOG_MSK_DEFAULT LOG_GROUP_STACK

/** Enable logging with RTT callback. */
#define LOG_ENABLE_RTT 1

/** The default callback function to use. */
#define LOG_CALLBACK_DEFAULT log_callback_rtt

/** @} end of MESH_CONFIG_LOG */


/** Maximum number of array elements to print in the @ref __LOG_XB macro. */
#define LOG_ARRAY_LEN_MAX   128

/**
 * @defgroup LOG_SOURCES Log sources
 * Defines various sources for logging messages. This can be used in __LOG_INIT() to
 * filter events from different modules.
 * @{
 */

#define LOG_SRC_APP_1          	(1 <<  0) /**< Receive logs from the app 1. */
#define LOG_SRC_APP_2          	(1 <<  1) /**< Receive logs from the app 2. */
#define LOG_SRC_APP_3          	(1 <<  2) /**< Receive logs from the app 3. */

/** Group for receiving logs from the core stack. */
#define LOG_GROUP_STACK         (LOG_SRC_APP_1 | LOG_SRC_APP_2)

/** @} */

/**
 * @defgroup LOG_LEVELS Log levels
 * Defines possible criticality levels for logged messages. This can be used in
 * __LOG_INIT() to filter events by criticality.
 * @{
 */

#define LOG_LEVEL_ASSERT ( 0) /**< Log level for assertions */
#define LOG_LEVEL_ERROR  ( 1) /**< Log level for error messages. */
#define LOG_LEVEL_WARN   ( 2) /**< Log level for warning messages. */
#define LOG_LEVEL_REPORT ( 3) /**< Log level for report messages. */
#define LOG_LEVEL_INFO   ( 4) /**< Log level for information messages. */
#define LOG_LEVEL_DBG1   ( 5) /**< Log level for debug messages (debug level 1). */
#define LOG_LEVEL_DBG2   ( 4) /**< Log level for debug messages (debug level 2). */
#define LOG_LEVEL_DBG3   ( 7) /**< Log level for debug messages (debug level 3). */
#define EVT_LEVEL_BASE   ( 8) /**< Base level for event logging. For internal use only. */
#define EVT_LEVEL_ERROR  ( 9) /**< Critical error event logging level. For internal use only. */
#define EVT_LEVEL_INFO   (10) /**< Normal event logging level. For internal use only. */
#define EVT_LEVEL_DATA   (11) /**< Event data logging level. For internal use only. */

/** @} */

/** Filename macro used when printing. Provides the filename of the input file without any directory prefix. */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)


/*-----------------------------------------------------------
 * Exported function
 *----------------------------------------------------------*/


#if LOG_ENABLE

/** Global debug mask. The value of this variable is used to filter the log messages being printed. */
extern uint32_t g_log_dbg_msk;
/** Global log level. The value of this variable is used to filter the log messages being printed. */
extern int32_t g_log_dbg_lvl;

/** Callback function used for printing log strings. */
typedef void (*log_callback_t)(uint32_t dbg_level, const char * p_filename, uint16_t line,
                                   uint32_t timestamp, const char * format, va_list arguments);

/** Callback function for printing debug information over RTT. */
void log_callback_rtt(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, va_list arguments);

///** Callback function for printing debug information over RTT in the format required by LogViewer. */
//void log_callback_logview(uint32_t dbg_level, const char * p_filename, uint16_t line,
//    uint32_t timestamp, const char * format, va_list arguments);

/**
 * Initializes the logging module.
 *
 * @param[in] mask     Mask specifying which modules to log information from.
 * @param[in] level    Maximum log level to print messages from.
 * @param[in] callback Callback function for printing log strings.
 */
void log_init(uint32_t mask, uint32_t level, log_callback_t callback);

/**
 * Sets the log callback function.
 *
 * The callback function is called to print strings from the logging module.
 * An application that is interested in internal logging can set this function
 * in order to print the log information in an application-specific way.
 *
 * @param[in] callback The callback function to use for printing log information.
 */
void log_set_callback(log_callback_t callback);

/**
 * Gets a timestamp to use with the log functions.
 *
 * @return A timestamp to use with the log functions.
 */
uint32_t log_timestamp_get(void);

/**
 * Prints log data.
 * This function is used by the logging macros, but can also be called directly
 * if desired.
 *
 * @param[in] dbg_level    The debugging level to print the message as.
 * @param[in] p_filename   Name of the file in which the log call originated.
 * @param[in] line         Line number where the function was called.
 * @param[in] timestamp    Timestamp for when the log function was called.
 * @param[in] format       Format string, printf()-compatible.
 *
 * @see log_vprintf()
 */
void __attribute((format(printf, 5, 6))) log_printf(
    uint32_t dbg_level, const char * p_filename, uint16_t line, uint32_t timestamp, const char * format, ...);

/**
 * Prints log data.
 * This function is used by the logging macros, but can also be called directly
 * if desired.
 *
 * @param[in] dbg_level    The debugging level to print the message as.
 * @param[in] p_filename   Name of the file in which the log call originated.
 * @param[in] line         Line number for where the log function was called.
 * @param[in] timestamp    Timestamp for when the log function was called.
 * @param[in] format       Format string, printf()-compatible.
 * @param[in] arguments    Arguments according to the @c format string.
 *
 * @see log_printf()
 */
void log_vprintf(uint32_t dbg_level, const char * p_filename, uint16_t line, uint32_t timestamp,
    const char * format, va_list arguments);



/*-----------------------------------------------------------
 * Exported function macro
 *----------------------------------------------------------*/
/*lint -emacro(506, MIN) */ /* Suppress "Constant value Boolean */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * Initializes the logging framework.
 * @param[in] msk      Log mask
 * @param[in] level    Log level
 * @param[in] callback Log callback
 */
#define __LOG_INIT(msk, level, callback) log_init(msk, level, callback)

/**
 * Prints a log message.
 * @param[in] source Log source
 * @param[in] level  Log level
 * @param[in] ...    Arguments passed on to the callback (similar to @c printf)
 */
#define __LOG(source, level, ...)                                       \
    if ((source & g_log_dbg_msk) && level <= g_log_dbg_lvl)             \
    {                                                                   \
        log_printf(level, __FILENAME__, __LINE__, log_timestamp_get(), __VA_ARGS__); \
    }

/**
 * Prints an array with a message.
 * @param[in] source Log source
 * @param[in] level  Log level
 * @param[in] msg    Message string
 * @param[in] array  Pointer to array
 * @param[in] len    Length of array (in bytes)
 */
#define __LOG_XB(source, level, msg, array, array_len)                      \
    if ((source & g_log_dbg_msk) && (level <= g_log_dbg_lvl))               \
    {                                                                       \
        unsigned _array_len = array_len;                                    \
        _array_len = MIN(_array_len, LOG_ARRAY_LEN_MAX);                    \
        char array_text[LOG_ARRAY_LEN_MAX * 2 + 1];                         \
        for(unsigned _i = 0; _i < _array_len; ++_i)                         \
        {                                                                   \
            extern const char * g_log_hex_digits;                           \
            uint8_t array_elem = array[_i];                                 \
            array_text[_i * 2] = g_log_hex_digits[(array_elem >> 4) & 0xf]; \
            array_text[_i * 2 + 1] = g_log_hex_digits[array_elem & 0xf];    \
        }                                                                   \
        array_text[_array_len * 2] = 0;                                     \
        log_printf(level, __FILENAME__, __LINE__, log_timestamp_get(), "%s: %s\n", msg, array_text); \
    }

#else  /* NRF_MESH_LOG_ENABLE == 0*/
#define __LOG_INIT(...)
#define __LOG(...)
#define __LOG_XB(...)

#ifndef LOG_CALLBACK_DEFAULT
/** Default log callback. */
#define LOG_CALLBACK_DEFAULT NULL
#endif 

#endif  /* LOG_ENABLE */

/** @} */


#endif /* __LOG_ULTRA_H */
