#ifndef NEDIT_UTIL_LOGGING_H
#define NEDIT_UTIL_LOGGING_H
#pragma once

/* 
 * Nirvana Editor C++ Logging Facilities
 * 
 * Copyright 2015 The NEdit Developers
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * Nirvana Text Editor C++
 * 2015-03-28 - Vladimir Lysyy - Initial version
 */

#include <iostream>
#include <string>

#include <memory>
#include <cstdlib>
#include <typeinfo>

template <class T>
struct lifecycle_tracker
{
    lifecycle_tracker() { std::clog << "c-ted " << typeid(*this).name() << "(" << this << ")" << std::endl; }
    virtual ~lifecycle_tracker() { std::clog << "d-ted " << typeid(*this).name() << "(" << this << ")" << std::endl; }
};

#endif // NEDIT_UTIL_LOGGING_H
