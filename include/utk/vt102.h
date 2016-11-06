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

#ifndef _UTK_VT102_H_
#define _UTK_VT102_H_

/**
 * vt102.h
 *
 * - You can enable color in vt102 module by defining ENABLE_UTK_VT102_COLOR macro.
 */

#define UTK_VT102_ESCAPE                  "\033"

#if defined(ENABLE_UTK_VT102_COLOR)
 #define UTK_VT102_COLOR_RESET            UTK_VT102_ESCAPE "[0m"

 #define UTK_VT102_COLOR_BLACK(txt)       UTK_VT102_ESCAPE "[0;30m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_RED(txt)         UTK_VT102_ESCAPE "[0;31m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_GREEN(txt)       UTK_VT102_ESCAPE "[0;32m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_BROWN(txt)       UTK_VT102_ESCAPE "[0;33m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_BLUE(txt)        UTK_VT102_ESCAPE "[0;34m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_PURPLE(txt)      UTK_VT102_ESCAPE "[0;35m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_CYAN(txt)        UTK_VT102_ESCAPE "[0;36m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_GRAY(txt)        UTK_VT102_ESCAPE "[0;37m" txt UTK_VT102_COLOR_RESET

 #define UTK_VT102_COLOR_DARK_GRAY(txt)   UTK_VT102_ESCAPE "[1;30m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_LIGHT_RED(txt)   UTK_VT102_ESCAPE "[1;31m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_LIGHT_GREEN(txt) UTK_VT102_ESCAPE "[1;32m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_YELLOW(txt)      UTK_VT102_ESCAPE "[1;33m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_LIGHT_BLUE(txt)  UTK_VT102_ESCAPE "[1;34m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_LIGHT_PURPLE(txt)UTK_VT102_ESCAPE "[1;35m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_LIGHT_CYAN(txt)  UTK_VT102_ESCAPE "[1;36m" txt UTK_VT102_COLOR_RESET
 #define UTK_VT102_COLOR_WHITE(txt)       UTK_VT102_ESCAPE "[1;37m" txt UTK_VT102_COLOR_RESET
#else
 #define UTK_VT102_COLOR_BLACK(txt)       txt
 #define UTK_VT102_COLOR_RED(txt)         txt
 #define UTK_VT102_COLOR_GREEN(txt)       txt
 #define UTK_VT102_COLOR_BROWN(txt)       txt
 #define UTK_VT102_COLOR_BLUE(txt)        txt
 #define UTK_VT102_COLOR_PURPLE(txt)      txt
 #define UTK_VT102_COLOR_CYAN(txt)        txt
 #define UTK_VT102_COLOR_GRAY(txt)        txt

 #define UTK_VT102_COLOR_DARK_GRAY(txt)   txt
 #define UTK_VT102_COLOR_LIGHT_RED(txt)   txt
 #define UTK_VT102_COLOR_LIGHT_GREEN(txt) txt
 #define UTK_VT102_COLOR_YELLOW(txt)      txt
 #define UTK_VT102_COLOR_LIGHT_BLUE(txt)  txt
 #define UTK_VT102_COLOR_LIGHT_PURPLE(txt)txt
 #define UTK_VT102_COLOR_LIGHT_CYAN(txt)  txt
 #define UTK_VT102_COLOR_WHITE(txt)       txt
#endif

#endif
