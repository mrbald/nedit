#ifndef NEDIT_UTIL_LIBRARY_H
#define NEDIT_UTIL_LIBRARY_H
#pragma once

/* 
 * Nirvana Editor C++ Dynamic Library Abstraction
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
#include <functional>
#include <stdexcept>
#include <type_traits>

#include <dlfcn.h>

namespace nedit {

/* 
 * Dynamic Library Abstraction
 */
struct library: lifecycle_tracker<library>, std::enable_shared_from_this<library> {
    static std::shared_ptr<library> load(char const* path);

    ~library() noexcept
    {
        if (handle_) dlclose(handle_);
    }

    template <class F>
    std::function<F> function(char const* name) const
    {
        if (!handle_) throw std::runtime_error("library not open");
        dlerror();
        auto fp = reinterpret_cast<typename std::add_pointer<F>::type>(dlsym(handle_, name));
        const char *dlsym_error = dlerror();
        if (dlsym_error) throw std::runtime_error(dlsym_error);

        // aliased shared_ptr to hold the library alive until all references
        // to functions loaded from it are disposed
        return func_helper<F>::create(std::shared_ptr<F>(shared_from_this(), fp));
    }

    // ctors are not to be called directly, static method create() should be
    // used instead
    library(char const* path):
        handle_(dlopen(path, RTLD_LAZY))
    {
        if (!handle_) throw std::runtime_error(dlerror());
    }

private:
    template <class F> struct func_helper;
    template <class R, class... Args> struct func_helper <R(Args...)>
    {
        static std::function<R(Args...)> create(std::shared_ptr<R(Args...)> ptr)
        {
            return [=](Args&&... args) { return (**ptr)(std::forward<Args>(args)...); };
        }
    };

    void* handle_ = nullptr;
};
using library_ptr = std::shared_ptr<library>;

library_ptr library::load(char const* path)
{
    return std::make_shared<library>(path);
}

} // namespace nedit

#endif // NEDIT_UTIL_LIBRARY_H
