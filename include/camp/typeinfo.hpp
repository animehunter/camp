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


#ifndef CAMP_TYPEINFO_HPP
#define CAMP_TYPEINFO_HPP


#include <camp/type.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>


namespace camp
{
struct ArrayType;
struct DictionaryType;
class Class;
class Enum;

/**
 * \brief Variant type for type info.
 */
typedef boost::variant<Type, const Enum&, const Class&, boost::recursive_wrapper<ArrayType>, boost::recursive_wrapper<DictionaryType> > CampType;

/**
 * \brief Type info for \c arrayType.
 */
struct ArrayType
{
    CampType m_elementtype;
};

/**
 * \brief Type info for \c dictionaryType.
 */
struct DictionaryType
{
    CampType m_keytype;
    CampType m_elementtype;
};

template <typename T = void>
class TypeVisitor : public boost::static_visitor<T>
{

};

} // namespace camp


#endif // CAMP_TYPE_HPP
