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

#include <camp-lua/conversion.hpp>
#include <camp/valuevisitor.hpp>
#include <camp/class.hpp>
#include <camp/property.hpp>
#include <camp/function.hpp>
#include <camp/invalidobject.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/invalidproperty.hpp>
#include <camp/invalidfunction.hpp>
#include <lua.hpp>

namespace camp
{
namespace lua
{

class ValueToLuaVisitor : public ValueVisitor<void>
{
public:
    ValueToLuaVisitor(lua_State* L);
    void operator()(camp::NoType value);
    void operator()(bool value);
    void operator()(long value);
    void operator()(double value);
    void operator()(const std::string& value);
    void operator()(const camp::EnumObject& value);
    void operator()(const camp::UserObject& value);

private:
    lua_State* m_L;
};

int indexCallback(lua_State* L);
int newIndexCallback(lua_State* L);
int callCallback(lua_State* L);

camp::Value valueFromLua(lua_State* L, int index)
{
    camp::Value v;
    if (lua_isboolean(L, index))
        v = lua_toboolean(L, index);
    else if (lua_isnumber(L, index))
        v = lua_tonumber(L, index);
    else if (lua_isstring(L, index))
        v = lua_tostring(L, index);
    // TODO handle other types
    return v;
}

void valueToLua(lua_State* L, camp::Value value)
{
    ValueToLuaVisitor visitor(L);
    value.visit(visitor);
}

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
        valueToLua(L, value);

        return 1; // One value returned
    }
    catch (const camp::InvalidObject&)
    {
        // The userdata has an invalid value
        // This should not happen
        return luaL_error(L, "Invalid userdata value");
    }
    catch (const camp::InvalidAccess&)
    {
        // The property is not readable
        return luaL_error(L, "Property not readable");
    }
    catch (const camp::InvalidProperty&)
    {
        // No property with the desired name
        try
        {
            // Try to retrieve a function
            const camp::Function& function = metaclass.function(key);

            // Push the callback function onto the stack
            // The function to be called is added to the C closure
            lua_pushlightuserdata(L, const_cast<camp::Function*>(&function));
            lua_pushcclosure(L, &callCallback, 1);

            return 1; // One value returned (the C closure)
        } catch (const camp::InvalidFunction&)
        {
            // No property nor function with the requested name
            return luaL_error(L, "Invalid key '%s'", key.c_str());
        }
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
    camp::Value newValue = valueFromLua(L, 3);

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
    catch (const camp::InvalidObject&)
    {
        // The userdata has an invalid value
        // This should not happen
        return luaL_error(L, "Invalid userdata value");
     }
    catch (const camp::InvalidAccess&)
    {
        // The property is not writable
        return luaL_error(L, "Property not writable");
    }
    catch (const camp::InvalidValue&)
    {
        // The new value can't be converted to the property's type
        return luaL_error(L, "New value cannot be converted to the property's type");
    }
    catch (const camp::InvalidProperty&)
    {
        // No property with the desired name
        return luaL_error(L, "Invalid key '%s'", key.c_str());
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
        args += valueFromLua(L, i);

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
            valueToLua(L, result);
            return 1;
        }
    }
    catch (const camp::InvalidObject&)
    {
        // The userdata has an invalid value
        // This should not happen
        return luaL_error(L, "Invalid userdata value");
     }
    catch (const camp::InvalidAccess&)
    {
        // The function is not writable
        return luaL_error(L, "Function not callable");
    }
    catch (const camp::InvalidArgument&)
    {
        // An argument can't be converted to the requested type
        return luaL_error(L, "An argument cannot be converted to the needed type");
    }

    return 0;
}

} // namespace lua

} // namespace camp

