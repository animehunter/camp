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

#include <camp-lua/valuetoluavisitor.hpp>
#include <camp-lua/conversion.hpp>
#include <camp/class.hpp>
#include <camp/property.hpp>
#include <camp/function.hpp>
#include <camp/error.hpp>
#include <lua.hpp>

namespace
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

} // anonymous namespace

namespace camp
{
namespace lua
{

ValueToLuaVisitor::ValueToLuaVisitor(lua_State* L)
    : m_L(L)
{

}

void ValueToLuaVisitor::operator()(camp::NoType value)
{
    // TODO
}

void ValueToLuaVisitor::operator()(bool value)
{
    lua_pushboolean(m_L, value);
}

void ValueToLuaVisitor::operator()(long value)
{
    lua_pushinteger(m_L, value);
}

void ValueToLuaVisitor::operator()(double value)
{
    lua_pushnumber(m_L, value);
}

void ValueToLuaVisitor::operator()(const std::string& value)
{
    lua_pushstring(m_L, value.c_str());
}

void ValueToLuaVisitor::operator()(const camp::EnumObject& value)
{
    // TODO
}

void ValueToLuaVisitor::operator()(const camp::UserObject& value)
{
    // Create a userdata to hold the user object
    camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_newuserdata(m_L,
                sizeof(camp::UserObject)));
    new (userdata) camp::UserObject(value); // Call the placement 'new' operator

    // Create the table to be used as metatable
    // Pre-allocated space for 2 non-array elements (__index and __newindex)
    lua_createtable(m_L, 0, 2);

    // Set the __index event to call the indexCallback function
    lua_pushstring(m_L, "__index");
    lua_pushcfunction(m_L, &indexCallback);
    lua_settable(m_L, -3);

    // Set the __newindex event to call the newIndexCallback function
    lua_pushstring(m_L, "__newindex");
    lua_pushcfunction(m_L, &newIndexCallback);
    lua_settable(m_L, -3);

    // Set the table as the metatable of the userdata
    lua_setmetatable(m_L, -2);
}

} // namespace lua

} // namespace camp

namespace
{

int indexCallback(lua_State* L)
{
    // Check args
    if (lua_gettop(L) != 2)
        return luaL_error(L, "Invalid args count (got %d, expected 2)", lua_gettop(L));
    if (!lua_isuserdata(L, 1))
        return luaL_error(L, "First arg must be a userdata (got %s)", lua_typename(L, lua_type(L, 1)));
    if (!lua_isstring(L, 2))
        return luaL_error(L, "Second arg must be a string (got %s)", lua_typename(L, lua_type(L, 2)));

    // Get args
    camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_touserdata(L, 1));
    std::string key(lua_tostring(L, 2));

    // Clear the stack
    lua_settop(L, 0);

    // Access to the property/function from the metaclass
    const camp::Class& metaclass = userdata->getClass();
    try
    {
        // Try to retrieve a property with the requested name
        const camp::Property& property = metaclass.property(key);

        // Call the property
        camp::Value value = property.get(*userdata);

        // Publish to Lua
        camp::lua::valueToLua(L, value);

        return 1; // One value returned
    }
    catch (const camp::Error& err)
    {
        return luaL_error(L, err.what());
    }

    return 0;
}

int newIndexCallback(lua_State* L)
{
    // Check args
    if (lua_gettop(L) != 3)
        return luaL_error(L, "Invalid args count (got %d, expected 3)", lua_gettop(L));
    if (!lua_isuserdata(L, 1))
        return luaL_error(L, "First arg must be a userdata (got %s)", lua_typename(L, lua_type(L, 1)));
    if (!lua_isstring(L, 2))
        return luaL_error(L, "Second arg must be a string (got %s)", lua_typename(L, lua_type(L, 2)));

    // Get args
    camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_touserdata(L, 1));
    std::string key(lua_tostring(L, 2));
    camp::Value newValue = camp::lua::valueFromLua(L, 3);

    // Clear the stack
    lua_settop(L, 0);

    // Access to the property from the metaclass
    const camp::Class& metaclass = userdata->getClass();
    try
    {
        // Try to retrieve a property with the requested name
        const camp::Property& property = metaclass.property(key);

        // Call the property
        property.set(*userdata, newValue);

        return 0; // No value returned
    }
    catch (const camp::Error& err)
    {
        return luaL_error(L, err.what());
    }

    return 0;
}

int callCallback(lua_State* L)
{
    // Check args
    int argc = lua_gettop(L);
    if (argc < 1)
        return luaL_error(L, "Invalid args count (got %d, expected at least the object instance)", argc);
    if (!lua_isuserdata(L, 1))
        return luaL_error(L, "First arg must be a userdata (got %s)", lua_typename(L, lua_type(L, 1)));

    // Get args
    camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_touserdata(L, 1));
    camp::Args args;
    for (int i = 2; i <= argc; ++i)
        args += camp::lua::valueFromLua(L, i);

    // Retrieve the function to be called
    int fctIndex = lua_upvalueindex(1);
    if (!lua_isuserdata(L, fctIndex))
        return luaL_error(L, "The function to be called could not be retrieved");
    camp::Function* function = static_cast<camp::Function*>(lua_touserdata(L, fctIndex));

    // Clear the stack
    lua_settop(L, 0);

    try
    {
        // Call the function.
        camp::Value result = function->call(*userdata, args);

        // Push the result if needed
        if (result.type() == camp::noType)
        {
            return 0;
        }
        else
        {
            camp::lua::valueToLua(L, result);
            return 1;
        }
    }
    catch (const camp::Error& err)
    {
        return luaL_error(L, err.what());
    }

    return 0;
}

} // anonymous namespace
