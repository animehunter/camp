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

#ifndef CAMP_LUA_VALUETOLUAVISITOR_HPP
#define CAMP_LUA_VALUETOLUAVISITOR_HPP

#include <camp/valuevisitor.hpp>
#include <camp/type.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>

struct lua_State;

namespace camp
{
namespace lua
{
/**
 * \brief ValueToLuaVisitor visit a camp::Value and push on a Lua stack a Lua value according to the
 * camp::Value type.
 */
class CAMP_API ValueToLuaVisitor : public ValueVisitor<void>
{
public:
    /**
     * \brief Constructor
     *
     * \param L Lua stack which will receive the new value
     * \param constructed If the value was constructed in camp, set to true. Defaults to false
     */
    ValueToLuaVisitor(lua_State* L, bool constructed = false);

    /**
     * \brief No value is pushed when \a value as no type
     *
     * \param value Value to be converted
     */
    void operator()(camp::NoType value);

    /**
     * \brief Push a Lua boolean corresponding to \a value
     *
     * \param value Value to be converted
     */
    void operator()(bool value);

    /**
     * \brief Push a Lua 'lua_Integer' corresponding to \a value
     *
     * \param value Value to be converted
     */
    void operator()(long value);

    /**
     * \brief Push a Lua 'lua_Number' corresponding to \a value
     *
     * \param value Value to be converted
     */
    void operator()(double value);

    /**
     * \brief Pus a Lua string corresponding to \a value
     *
     * \param value Value to be converted
     */
    void operator()(const std::string& value);

    /**
     * \brief Push a Lua number corresponding to the enum value
     *
     * \param value Value to be converted
     */
    void operator()(const camp::EnumObject& value);

    /**
     * \brief Push a Lua userdata wrapping the \a value
     *
     * The userdata will have a custom metatable with its '__index' and '__newindex' events
     * redefined. These will call special callbacks allowing to expose CAMP properties and functions
     * within Lua.
     *
     * \param value Value to be converted
     */
    void operator()(const camp::UserObject& value);

private:
    lua_State* m_L; ///< Lua stack
    bool m_Constructed;	///< If the value was constructed in camp
};
} // namespace lua
} // namespace camp

#endif // CAMP_LUA_VALUETOLUAVISITOR_HPP