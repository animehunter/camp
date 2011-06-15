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


#include <camp/constructor.hpp>
#include <camp/errors.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Constructor::~Constructor()
{

}

//-------------------------------------------------------------------------------------------------
std::size_t Constructor::argCount() const
{
    return m_argTypes.size();
}

//-------------------------------------------------------------------------------------------------
Type Constructor::argType(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_argTypes.size())
        CAMP_ERROR(OutOfRange(index, m_argTypes.size()));

    return m_argTypes[index];
}

//-------------------------------------------------------------------------------------------------
camp::TypeInfo Constructor::argTypeInfo(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_argTypeInfo.size())
        CAMP_ERROR(OutOfRange(index, m_argTypeInfo.size()));

    return m_argTypeInfo[index];
}

//-------------------------------------------------------------------------------------------------
Constructor::Constructor(const std::vector<Type>& argTypes, const std::vector<TypeInfo>& argTypeInfo)
    : m_argTypes(argTypes)
    , m_argTypeInfo(argTypeInfo)
{

}

} // namespace camp
