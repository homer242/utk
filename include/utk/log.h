/*
 * Copyright (c) 2011-2012 Anthony Viallard
 *
 *    This file is part of Utk.
 *
 * Utk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Utk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Utk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UTK_LOG_H_
#define _UTK_LOG_H_

/*
 * log.h
 *
 * - The pretty things here are first utk_log_* macros (utk_log_info, utk_log_debug, ...).
 *   Macros utk_log_open, utk_log_close and utk_log_write are just to harmonize
 *   use of log interface;
 * - You can enable utk_log_debug() by defining ENABLE_UTK_LOG_DEBUG macro.
 * - You can enable color logs by defining ENABLE_UTK_VT102_COLOR macro
 *   (before any utk includes).
 */

#include <syslog.h>

#include <utk/vt102.h>

#define utk_log_info(fmt, ...)                                          \
	utk_log_write(LOG_INFO, fmt, ##__VA_ARGS__)

#define utk_log_notice(fmt, ...)                                        \
        utk_log_write(LOG_NOTICE, UTK_VT102_COLOR_PURPLE(fmt), ##__VA_ARGS__)

#define utk_log_warn(fmt, ...)                                         \
        utk_log_write(LOG_ERR, UTK_VT102_COLOR_YELLOW(fmt), ##__VA_ARGS__)

#define utk_log_error(fmt, ...)                                         \
        utk_log_write(LOG_ERR, UTK_VT102_COLOR_RED(fmt), ##__VA_ARGS__)

#if defined(ENABLE_UTK_LOG_DEBUG)
#define utk_log_debug(fmt, ...)                                         \
        utk_log_write(LOG_DEBUG, "in %s:%04d - " fmt,                   \
                  __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define utk_log_debug(fmt, ...) do {;} while(0)
#endif

/*
 * utk_log_open - wrapper macro to openlog
 *
 *  Open a connection to the system logger.
 *
 * (view syslog man page for more documentation)
 */
#define utk_log_open(ident, opt, facility) \
        openlog(ident, opt, facility)

/*
 * utk_log_close - wrapper macro to closelog
 *
 *  Close the connection used for the system logger.
 *
 * (view syslog man page for more documentation)
 */
#define utk_log_close() \
        closelog()

/*
 * utk_log_write - wrapper macro to syslog
 *
 *  Write message to the system logger.
 *
 * (view syslog man page for more documentation)
 */
#define utk_log_write(priority, fmt, ...) \
        syslog(priority, fmt, ##__VA_ARGS__)

#endif
