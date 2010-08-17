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

#include <camp-lua/context.hpp>
#include <lua.hpp>

namespace camp
{
namespace lua
{

Context::Context(Context* parent)
    : m_parent(parent)
    , m_L(0)
{
    if (parent)
    {
        // Create a sub-context (i.e. a Lua thread)
        // The new context will share its global variables with its parent
        m_L = lua_newthread(parent->m_L);
    }
    else
    {
        // Create a whole new context (i.e. a Lua state)
        // The new context will be totally independent of already created contexts
        m_L = luaL_newstate();
        luaL_openlibs(m_L);
    }
}

Context* Context::parent() const
{
    return m_parent;
}

void Context::executeString(const std::string& code)
{
    luaL_dostring(m_L, code.c_str());
}

Object Context::operator[](const std::string& name)
{
    return Object(m_L, name);
}

} // namespace lua

} // namespace camp

