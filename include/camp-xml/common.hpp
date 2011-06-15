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

#ifndef CAMP_XML_COMMON_HPP
#define CAMP_XML_COMMON_HPP

#include <camp/userobject.hpp>
#include <camp/valuemapper.hpp>
#include <camp/value.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/dictionaryproperty.hpp>
#include <camp/typeinfo.hpp>
#include <string>

namespace camp
{
namespace xml
{
namespace detail
{
/**
 * \brief Serialize a CAMP object into XML elements
 *
 * This function uses a proxy class that defines a unified
 * interface for creating and populating XML nodes. To
 * provide support for a particular XML library, you just
 * have to provide a specialized Proxy which maps the
 * unified interface to the library's API, and call this
 * function.
 *
 * \param object Object to serialize
 * \param node Parent for the generated XML nodes
 * \param tag Tag to include or exclude from the serialization process.
 * \param include Set this to true to make the tag an including tag, false for excluding tag.
 * \param throwExceptions Set this to false to ignore thrown exceptions.
 */
template <typename Proxy>
void serialize(const UserObject& object, typename Proxy::NodeType node, const Value& tag, 
    bool include, bool throwExceptions);

/**
 * \brief Deserialize a CAMP object from XML elements
 *
 * This function uses a proxy class that defines a unified
 * interface for reading and iterating over XML nodes. To
 * provide support for a particular XML library, you just
 * have to provide a specialized Proxy which maps the
 * unified interface to the library's API, and call this
 * function.
 *
 * \param object Object to serialize
 * \param node XML node to parse
 * \param tag Tag to include or exclude from the serialization process.
 * \param include Set this to true to make the tag an including tag, false for excluding tag.
 * \param throwExceptions Set this to false to ignore thrown exceptions.
 */
template <typename Proxy>
void deserialize(const UserObject& object, typename Proxy::NodeType node, const Value& tag, 
    bool include, bool throwExceptions);

} // namespace detail

} // namespace xml

} // namespace camp

#include <camp-xml/common.inl>

#endif // CAMP_XML_COMMON_HPP
