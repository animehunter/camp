#include <string>
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


#ifndef CAMP_LUA_CALLBACK_HPP
#define CAMP_LUA_CALLBACK_HPP

#include <string>

struct lua_State;

namespace camp
{

namespace lua
{

/**
 * \brief Callback called when a Lua userdata holding a camp::UserObject triggers the '__index' event.
 *
 * \param L Lua stack holding the userdata
 * \return Number of return value pushed on the stack
 */
int indexCallback(lua_State* L);

/**
 * \brief Callback called when a Lua userdata holding a camp::UserObject triggers the '__newindex'
 * event.
 *
 * \param L Lua stack holding the userdata
 * \return Number of return value pushed on the stack
 */
int newIndexCallback(lua_State* L);

/**
 * \brief Callback called when a Lua C closure used to wrap a CAMP function call is called.
 *
 * \param L Lua stack holding the userdata
 * \return Number of return value pushed on the stack
 */
int callCallback(lua_State* L);

/**
 * \brief Callback called when a factory "new" function is called.
 *
 * \param L Lua stack holding the metaclass
 * \return Number of return value pushed on the stack
 */
int constructCallback(lua_State* L);

/**
 * \brief Callback called before a camp::UserObject is garbage collected.
 *
 * \param L Lua stack holding the userdata
 * \return Number of return value pushed on the stack
 */
int destructCallback(lua_State* L);

/**
 * \brief Callback called when an operator is called in Lua.
 *
 * \param L Lua stack holding the userdata
 * \param rType The type of operator
 * \return Number of return value pushed on the stack
 */
int operatorCallback(lua_State* L, const std::string& rType);
inline int addCallback(lua_State* L) {return operatorCallback(L, "Add");}
inline int subtractCallback(lua_State* L) {return operatorCallback(L, "Subtract");}
inline int multiplyCallback(lua_State* L) {return operatorCallback(L, "Multiply");}
inline int divideCallback(lua_State* L) {return operatorCallback(L, "Divide");}
inline int modulusCallback(lua_State* L) {return operatorCallback(L, "Modulus");}
inline int equalsCallback(lua_State* L) {return operatorCallback(L, "Equals");}
inline int lessCallback(lua_State* L) {return operatorCallback(L, "Less");}
inline int lessEqualsCallback(lua_State* L) {return operatorCallback(L, "LessEquals");}
int unaryMinusCallback(lua_State* L);

} // namespace lua

} // namespace camp

#endif // CAMP_LUA_CALLBACK_HPP

