#ifndef NEDIT_UTIL_LIBRARY_REPOSITORY_H
#define NEDIT_UTIL_LIBRARY_REPOSITORY_H
#pragma once

/* 
 * Nirvana Editor C++ Dynamic Library Repository
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
#include "library.h"

#include <memory>
#include <string>
#include <map>

namespace nedit {

struct library_repository;
using library_repository_ptr = std::shared_ptr<library_repository>;
struct library_repository: lifecycle_tracker<library_repository>, std::enable_shared_from_this<library_repository> {

    template <class... Args>
    static library_repository_ptr create(Args&&... args)
    {
        return std::make_shared<library_repository>(std::forward<Args>(args)...);
    }

    /* 
     * Loads the library (if not loaded yet) and caches the handle
     * associated with symbolic name
     */
    library_ptr load_library(std::string const& name, std::string const& path)
    {
        auto itr = libraries_.lower_bound(name);
        if (itr == libraries_.end() || libraries_.key_comp()(name, itr->first)) {
            itr = libraries_.emplace_hint(itr, name, library::load(path.c_str()));
        }
        else throw std::runtime_error("library " + name + "(" + path + ") already loaded");

        return itr->second;
    }

    library_ptr get_library(std::string const& name) const
    {
        return libraries_.at(name);
    }

    library_ptr find_library(std::string const& name) const
    {
        auto itr = libraries_.find(name);
        return itr != libraries_.end() ? itr->second : library_ptr();
    }

    // ctors are not to be called directly, static method create() should be
    // used instead
    library_repository() = default;

private:

    std::map<std::string, library_ptr> libraries_;

    // non-copyable
    library_repository(library_repository const&) = delete;
    library_repository(library_repository &&) = delete;
    library_repository& operator=(library_repository const&) = delete;
    library_repository& operator=(library_repository &&) = delete;
};

} // namespace nedit

#endif // NEDIT_UTIL_LIBRARY_REPOSITORY_H
