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


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename A>
DictionaryPropertyImpl<A>::DictionaryPropertyImpl(const std::string& name, const A& accessor)
    : DictionaryProperty(name, mapType<KeyType>(), camp_ext::ValueMapper<KeyType>::typeInfo(), 
    mapType<ElementType>(), camp_ext::ValueMapper<ElementType>::typeInfo())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
std::size_t DictionaryPropertyImpl<A>::getSize(const UserObject& object) const
{
    return Mapper::size(dictionary(object));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool DictionaryPropertyImpl<A>::queryExists(const UserObject& object, const Value& key) const
{
    return Mapper::exists(dictionary(object), key.to<KeyType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value DictionaryPropertyImpl<A>::getElement(const UserObject& object, const Value& key) const
{
    return Mapper::get(dictionary(object), key.to<KeyType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
DictionaryIteratorPtr DictionaryPropertyImpl<A>::getIterator(const UserObject& object) const
{
    return Mapper::iterator(dictionary(object));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void DictionaryPropertyImpl<A>::setElement(const UserObject& object, const Value& key, const Value& value) const
{
    Mapper::set(dictionary(object), key.to<KeyType>(), value.to<ElementType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void DictionaryPropertyImpl<A>::removeElement(const UserObject& object, const Value& key) const
{
    Mapper::remove(dictionary(object), key.to<KeyType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
typename DictionaryPropertyImpl<A>::DictionaryType& DictionaryPropertyImpl<A>::dictionary(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

} // namespace detail

} // namespace camp
