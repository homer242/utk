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

#ifndef _UTK_MATH_H_
#define _UTK_MATH_H_

/*! \def utk_math_min(x, y)
 */
#define utk_math_min(x, y)			    \
        ({                                          \
                typeof(x) _x = (x);                 \
                typeof(y) _y = (y);                 \
                _x < _y ? _x : _y;                  \
        })

/*! \def utk_math_max(x, y)
 */
#define utk_math_max(x, y)			    \
        ({                                          \
                typeof(x) _x = (x);                 \
                typeof(y) _y = (y);                 \
                _x > _y ? _x : _y;                  \
        })

#endif
