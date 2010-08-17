/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_LUA_CONTEXT_HPP
#define CAMP_LUA_CONTEXT_HPP

#include <camp-lua/object.hpp>
#include <camp/config.hpp>
#include <boost/noncopyable.hpp>
#include <string>

struct lua_State;

namespace camp
{
namespace lua
{

/**
 * \brief camp::lua::Context represents a self contained Lua virtual machine (a Lua state).
 *
 * camp::lua::Context is the entry point of the CAMP Lua module. It allows to execute arbitrary Lua
 * codes and scripts.
 * A camp::lua::Context can have a parent one (Lua thread is used internally). A hierarchy of
 * camp::lua::Context will share the same global variables but each context has its one execution
 * stack.
 *
 * Global variables are accessible through the [] operator:
 * \code
 * camp::lua::Context c;
 * c["a"] = 1;
 * c["b"] = c["a"];
 * \endcode
 *
 * Boolean, integer, real, string and userobject are handled. All properties and functions of a
 * userobject will be accessible from Lua.
 *
 * A top-level camp::lua::Context (i.e. with no parent context) being self contained, it allows to
 * run different ones in different system threads.
 */
class CAMP_API Context : boost::noncopyable
{
public:
    /**
     * \brief Create a new Lua context.
     *
     * If a \parent Context is given, a sub-context will be created using a Lua thread. The new
     * Context will share its global variables with its hierarchy.
     * If \parent is null, the new Context will be totally independent of already created contexts.
     * It can be ran in a different system thread.
     *
     * \param parent The new Context's parent.
     */
    Context(Context* parent = 0);

    /**
     * \brief Get the parent context.
     *
     * \return The parent context, or 0 if it is a top-level context.
     */
    Context* parent() const;

    /**
     * \brief Execute the given Lua \a code as a Lua chunk.
     *
     * Note that the chunk local variables will be deleted when the function returns. For example,
     * the following will not work as the local 'a' will be deleted before the print statement
     * occurs.
     * \code
     * camp::lua::Context c;
     * c.executeString("local a = 1");
     * c.executeString("print(a)");
     * \endcode
     *
     * \param code The Lua code to be executed in the current context
     */
    void executeString(const std::string& code);

    /**
     * \brief Access to the global variable \a name.
     *
     * Return a proxy object allowing to read/write the global variable named \a name. If the
     * variable does not exists, it will be created by the affectation:
     * \code
     * camp::lua::Context c;
     * c["a"] = 1; // Create the global variable "a" with the value 1
     * c["b"] = c["a"]; // Create the global variable "b" with the value of the global variable "a"
     * \endcode
     *
     * Note that the proxy object returned should not be copied or referenced.
     *
     * \param name The global variable name to be accessed.
     * \return A proxy object providing read/write access on the global variable.
     */
    Object operator[](const std::string& name);

private:
    Context* m_parent; ///< Parent context
    lua_State* m_L; ///< The Lua state (i.e. The Lua VM)
};

} // namespace lua

} // namespace camp

#endif // CAMP_LUA_CONTEXT_HPP

