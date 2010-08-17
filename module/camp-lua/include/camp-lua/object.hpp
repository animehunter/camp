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


#ifndef CAMP_LUA_OBJECT_HPP
#define CAMP_LUA_OBJECT_HPP

#include <camp/config.hpp>
#include <camp/value.hpp>
#include <string>

struct lua_State;

namespace camp
{
namespace lua
{

/**
 * \brief camp::lua::Object is a proxy-like class providing read/write access to a global Lua
 * variable.
 *
 * Read access is provided through the conversion of the Lua value to a camp::Value using the
 * conversion operator.
 * Write access is provided through the affectation operator with a camp::Value parameter.
 */
class CAMP_API Object 
{
public:
    /**
     * \brief Create an object providing read/write access to the global variable named \a name in
     * the Lua state \a L.
     *
     * \param L Lua stack holding the desired global variable
     * \param name Name of the desired global variable
     */
    Object(lua_State* L, const std::string& name);

    /**
     * \brief Convert the global variable value represented by this object to a camp::Value.
     *
     * \return A camp::Value holding the value associated to the global variable represented by this
     * object.
     */
    operator camp::Value();

    /**
     * \brief Affect a new value to the global variable represented by this object.
     *
     * \param value The new global variable value.
     * \return A reference to this.
     */
    Object& operator=(camp::Value value);

private:
    lua_State* m_L; ///< Lua state holding the global variable
    std::string m_name; ///< Name of the global variable
};

} // namespace lua

} // namespace camp

#endif // CAMP_LUA_OBJECT_HPP

