#ifndef NEDIT_UTIL_PLUGIN_REPOSITORY_H
#define NEDIT_UTIL_PLUGIN_REPOSITORY_H
#pragma once

/* 
 * Nirvana Editor C++ Plugin Repository
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

#include "plugin.h"
#include "library_repository.h"
#include "library.h"

#include <memory>
#include <string>
#include <map>

namespace nedit {

struct plugin_repository;
using plugin_repository_ptr = std::shared_ptr<plugin_repository>;
struct plugin_repository: lifecycle_tracker<plugin_repository>, std::enable_shared_from_this<plugin_repository>  {
    template <class... Args>
    static plugin_repository_ptr create(Args&&... args)
    {
        return std::make_shared<plugin_repository>(std::forward<Args>(args)...);
    }

    /* 
     * Loads the plugin `plugin_name` from the shared library `library_name`.
     * The library should export a plugin constructor function
     * plugin* `plugin_name`_construct(void) and plugin destructor
     * function `plugin_name`_destruct(plugin*).
     */
    plugin_ptr load_plugin(std::string const& library_name, std::string const& plugin_name)
    {
        return load_plugin_(*library_repo_->get_library(library_name), plugin_name);
    }

    /* 
     * Loads all plugins from the shared library `library_name`
     * The library should export an automatic plugin discovery function
     * char const** `library_name`_plugins(void) returning a null-terminated
     * array of C strings, and for each plugin a respective plugin
     * constructor function.
     */
    void load_all_plugins(std::string const& library_name)
    {
        auto lib = library_repo_->get_library(library_name);
        auto list_func = lib->function<char const**(void)>((library_name + "_plugins").c_str());

        for (auto ptr = list_func(); ptr && *ptr; ++ptr) {
            load_plugin_(*lib, *ptr);
            std::clog <<"plugin: " << *ptr << std::endl;
        }
    }

    plugin_ptr get_plugin(std::string const& plugin_name) const
    {
        return plugins_.at(plugin_name);
    }

    plugin_ptr find_plugin(std::string const& plugin_name) const
    {
        auto itr = plugins_.find(plugin_name);
        return itr != plugins_.end() ? itr->second : plugin_ptr();
    }

    // ctors are not to be called directly, static method create() should be
    // used instead
    plugin_repository() = delete;
    plugin_repository(library_repository_ptr library_repo):
        library_repo_(std::move(library_repo)) {}

private:
    plugin_ptr load_plugin_(library const& lib, std::string const& plugin_name)
    {
        auto itr = plugins_.lower_bound(plugin_name);
        if (itr == plugins_.end() || plugins_.key_comp()(plugin_name, itr->first)) {
            auto ctor = lib.function<plugin*(void)>((plugin_name + "_construct").c_str());
            auto dtor = lib.function<void(plugin*)>((plugin_name + "_destruct").c_str());

            itr = plugins_.emplace_hint(itr, plugin_name, plugin_ptr(ctor(), std::move(dtor)));
        }
        else throw std::runtime_error("plugin " + plugin_name + "already loaded");

        return itr->second;
    }

    library_repository_ptr library_repo_;
    std::map<std::string, plugin_ptr> plugins_;

    // non-copyable
    plugin_repository(plugin_repository const&) = delete;
    plugin_repository(plugin_repository &&) = delete;
    plugin_repository& operator=(plugin_repository const&) = delete;
    plugin_repository& operator=(plugin_repository &&) = delete;
};

} // namespace nedit

#endif // NEDIT_UTIL_PLUGIN_REPOSITORY_H
