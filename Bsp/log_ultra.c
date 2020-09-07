
/* segger rtt includes. */
#include <SEGGER_RTT.h>

/* lib includes. */
#include <stdarg.h>
#include <stdint.h>
#include "log_ultra.h"


const char * g_log_hex_digits = "0123456789ABCDEF";

uint8_t        g_segger_init_flag = 0;
uint32_t       g_log_dbg_msk = LOG_MSK_DEFAULT;
int32_t        g_log_dbg_lvl = LOG_LEVEL_DEFAULT;
log_callback_t m_log_callback = LOG_CALLBACK_DEFAULT;


/**
 * Gets a timestamp to use with the log functions.
 *
 * @return A timestamp to use with the log functions.
 */
__weak uint32_t log_timestamp_get(void)
{
    // Overwrite with your own time function
    return 0;
}

void log_callback_rtt(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, va_list arguments)
{
    SEGGER_RTT_printf(0, "<t: %10u>, %s, %4d, ",timestamp, p_filename, line);
    SEGGER_RTT_vprintf(0, format, &arguments);
}

//void log_callback_logview(uint32_t dbg_level, const char * p_filename, uint16_t line,
//    uint32_t timestamp, const char * format, va_list arguments)
//{
//    SEGGER_RTT_printf(0, "%u;%u;%s;%u;", dbg_level, timestamp, p_filename, line);
//    SEGGER_RTT_vprintf(0, format, &arguments);
//    SEGGER_RTT_Write(0, "$", 1);
//}

/**
 * Initializes the logging module.
 *
 * @param[in] mask     Mask specifying which modules to log information from.
 * @param[in] level    Maximum log level to print messages from.
 * @param[in] callback Callback function for printing log strings.
 */
void log_init(uint32_t mask, uint32_t level, log_callback_t callback)
{
    if(g_segger_init_flag == 0)
    {
        SEGGER_RTT_Init();
        g_segger_init_flag = 1;
    }
    
    g_log_dbg_msk = mask;
    g_log_dbg_lvl = level;

    m_log_callback = callback;
}

/**
 * Sets the log callback function.
 *
 * The callback function is called to print strings from the logging module.
 * An application that is interested in internal logging can set this function
 * in order to print the log information in an application-specific way.
 *
 * @param[in] callback The callback function to use for printing log information.
 */
void log_set_callback(log_callback_t callback)
{
    m_log_callback = callback;
}

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
void log_printf(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, ...)
{
    va_list arguments; /*lint -save -esym(530,arguments) Symbol arguments not initialized. */
    va_start(arguments, format);
    log_vprintf(dbg_level, p_filename, line, timestamp, format, arguments);
    va_end(arguments); /*lint -restore */
}


void log_vprintf(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, va_list arguments)
{
    if (m_log_callback != NULL)
    {
        m_log_callback(dbg_level, p_filename, line, timestamp, format, arguments);
    }
}

