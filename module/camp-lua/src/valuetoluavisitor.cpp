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
#include <camp-lua/callback.hpp>
#include <camp/class.hpp>
#include <lua.hpp>

namespace camp
{
namespace lua
{

ValueToLuaVisitor::ValueToLuaVisitor(lua_State* L, bool constructed /*= false*/)
    : m_L(L)
    , m_Constructed(constructed)
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
    lua_pushnumber(m_L, value.value());
}

void ValueToLuaVisitor::operator()(const camp::UserObject& value)
{
    // Create a userdata to hold the user object
    camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_newuserdata(m_L, sizeof(camp::UserObject)));
    new (userdata) camp::UserObject(value); // Call the placement 'new' operator

    // Try to get the metatable corresponding to the metaclass from the Lua registry
    const camp::Class& metaclass = userdata->getClass();
    std::string key = "camp-lua/";
    key += metaclass.name();
    if (luaL_newmetatable(m_L, key.c_str()))
    {
        // A new metatable has been created as the metaclass has not been published previously

        // Set the __index event to call the indexCallback function
        lua_pushstring(m_L, "__index");
        lua_pushcfunction(m_L, &indexCallback);
        lua_rawset(m_L, -3);

        // Set the __newindex event to call the newIndexCallback function
        lua_pushstring(m_L, "__newindex");
        lua_pushcfunction(m_L, &newIndexCallback);
        lua_rawset(m_L, -3);

        if(metaclass.hasTag("EnableAddOperator"))
        {
            // Set the __add event to call the addCallback function
            lua_pushstring(m_L, "__add");
            lua_pushcfunction(m_L, &addCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableSubtractOperator"))
        {
            // Set the __sub event to call the subtractCallback function
            lua_pushstring(m_L, "__sub");
            lua_pushcfunction(m_L, &subtractCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableMultiplyOperator"))
        {
            // Set the __mul event to call the multiplyCallback function
            lua_pushstring(m_L, "__mul");
            lua_pushcfunction(m_L, &multiplyCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableDivideOperator"))
        {
            // Set the __div event to call the divideCallback function
            lua_pushstring(m_L, "__div");
            lua_pushcfunction(m_L, &divideCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableModulusOperator"))
        {
            // Set the __mod event to call the modulusCallback function
            lua_pushstring(m_L, "__mod");
            lua_pushcfunction(m_L, &modulusCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableEqualsOperator"))
        {
            // Set the __eq event to call the equalsCallback function
            lua_pushstring(m_L, "__eq");
            lua_pushcfunction(m_L, &equalsCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableLessOperator"))
        {
            // Set the __lt event to call the lessCallback function
            lua_pushstring(m_L, "__lt");
            lua_pushcfunction(m_L, &lessCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableLessEqualsOperator"))
        {
            // Set the __le event to call the lessEqualsCallback function
            lua_pushstring(m_L, "__le");
            lua_pushcfunction(m_L, &lessEqualsCallback);
            lua_rawset(m_L, -3);
        }
        if(metaclass.hasTag("EnableUnaryMinusOperator"))
        {
            // Set the __unm event to call the unaryMinusCallback function
            lua_pushstring(m_L, "__unm");
            lua_pushcfunction(m_L, &unaryMinusCallback);
            lua_rawset(m_L, -3);
        }
    }

    if(m_Constructed)
    {
        // Set the __gc event to call the destructCallback function
        lua_pushstring(m_L, "__gc");
        lua_pushcfunction(m_L, &destructCallback);
        lua_rawset(m_L, -3);
    }

    // Set the table as the metatable of the userdata
    lua_setmetatable(m_L, -2);
}

} // namespace lua

} // namespace camp

