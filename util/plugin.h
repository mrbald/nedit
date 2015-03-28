#ifndef NEDIT_UTIL_PLUGIN_H
#define NEDIT_UTIL_PLUGIN_H
#pragma once

/* 
 * Nirvana Editor C++ Plugin Base Class
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

#include "logging.h"

#include <memory>

namespace nedit {

/* 
 * 
 */
struct plugin: lifecycle_tracker<plugin> {
    virtual void init() = 0;
    virtual void fini() = 0;

protected:
    plugin() = default;
    virtual ~plugin() = default;

    plugin(plugin const&) = delete;
    plugin(plugin &&) = delete;
    plugin& operator=(plugin const&) = delete;
    plugin& operator=(plugin &&) = delete;
};
using plugin_ptr = std::shared_ptr<plugin>;

} // namespace nedit

#endif // NEDIT_UTIL_PLUGIN_H
