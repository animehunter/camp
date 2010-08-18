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
#include <camp-lua/valuetoluavisitor.hpp>
#include <lua.hpp>

namespace camp
{
namespace lua
{

camp::Value valueFromLua(lua_State* L, int index)
{
    camp::Value v;
    if (lua_isboolean(L, index))
        v = lua_toboolean(L, index);
    else if (lua_isnumber(L, index))
        v = lua_tonumber(L, index);
    else if (lua_isstring(L, index))
        v = lua_tostring(L, index);
    else if (lua_isuserdata(L, index))
    {
        camp::UserObject* userdata = static_cast<camp::UserObject*>(lua_touserdata(L, index));
        v = *userdata;
    }
    // TODO handle other types
    return v;
}

void valueToLua(lua_State* L, camp::Value value)
{
    ValueToLuaVisitor visitor(L);
    value.visit(visitor);
}

} // namespace lua

} // namespace camp

