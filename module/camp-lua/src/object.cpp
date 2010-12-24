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

#include <camp-lua/object.hpp>
#include <camp-lua/conversion.hpp>
#include <lua.hpp>

namespace camp
{
namespace lua
{

Object::Object(lua_State* L, const std::string& name)
    : m_L(L)
    , m_name(name)
{

}

Object::operator camp::Value()
{
    lua_getglobal(m_L, m_name.c_str());
    camp::Value v = valueFromLua(m_L, -1);
    lua_pop(m_L, 1);
    return v;
}

Object& Object::operator=(camp::Value value)
{
    valueToLua(m_L, value);
    lua_setglobal(m_L, m_name.c_str());
    return *this;
}

Object& Object::operator=(const camp::Class& metaclass)
{
    classToLua(m_L, metaclass);
    lua_setglobal(m_L, m_name.c_str());
    return *this;
}

Object& Object::operator=(const camp::Enum& metaenum)
{
    enumToLua(m_L, metaenum);
    lua_setglobal(m_L, m_name.c_str());
    return *this;
}

} // namespace lua

} // namespace camp

