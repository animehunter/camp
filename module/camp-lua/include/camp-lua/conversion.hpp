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


#ifndef CAMP_LUA_CONVERSION_HPP
#define CAMP_LUA_CONVERSION_HPP

#include <camp/value.hpp>
#include <string>

struct lua_State;

namespace camp
{
namespace lua
{

/**
 * \brief Return the value at \a index in the Lua stack \a L as a camp::Value
 *
 * No value is pushed or poped to/from the Lua stack.
 *
 * \param L The Lua stack holding the desired value
 * \param index Lua index (or pseudo-index) of the desired value in the stack
 * \return The value as camp::Value
 */
camp::Value valueFromLua(lua_State* L, int index);

/**
 * \brief Push onto the Lua stack \a L the given \a value
 *
 * \param L The Lua stack receiving the new value
 * \param value Value to be pushed on the stack
 */
void valueToLua(lua_State* L, camp::Value value);

} // namespace lua

} // namespace camp

#endif // CAMP_LUA_CONVERSION_HPP

