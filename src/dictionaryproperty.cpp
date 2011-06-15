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


#include <camp/dictionaryproperty.hpp>
#include <camp/classvisitor.hpp>
#include <camp/errors.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
DictionaryProperty::DictionaryProperty(const std::string& name, Type keyType, TypeInfo keyTypeInfo, 
    Type elementType, TypeInfo elementTypeInfo)
    : Property(name, dictionaryType, DictionaryType(keyTypeInfo, elementTypeInfo))
    , m_keyType(keyType)
    , m_keyTypeInfo(keyTypeInfo)
    , m_elementType(elementType)
    , m_elementTypeInfo(elementTypeInfo)
{
}

//-------------------------------------------------------------------------------------------------
DictionaryProperty::~DictionaryProperty()
{
}

//-------------------------------------------------------------------------------------------------
Type DictionaryProperty::keyType() const
{
    return m_keyType;
}

//-------------------------------------------------------------------------------------------------
TypeInfo DictionaryProperty::keyTypeInfo() const
{
    return m_keyTypeInfo;
}

//-------------------------------------------------------------------------------------------------
Type DictionaryProperty::elementType() const
{
    return m_elementType;
}

//-------------------------------------------------------------------------------------------------
TypeInfo DictionaryProperty::elementTypeInfo() const
{
    return m_elementTypeInfo;
}

//-------------------------------------------------------------------------------------------------
std::size_t DictionaryProperty::size(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    return getSize(object);
}

//-------------------------------------------------------------------------------------------------
bool DictionaryProperty::exists(const UserObject& object, const camp::Value& key) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    return queryExists(object,key);
}

//-------------------------------------------------------------------------------------------------
Value DictionaryProperty::get(const UserObject& object, const camp::Value& key) const
{
    // Check if element exists
    if(!exists(object, key))
        CAMP_ERROR(ElementNotFound());

    return getElement(object, key);
}

//-------------------------------------------------------------------------------------------------
DictionaryIteratorPtr DictionaryProperty::iterator(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    return getIterator(object);
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::set(const UserObject& object, const camp::Value& key, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
    {
        m_setted_nonwritable_signal(object, *this, key, value);
        CAMP_ERROR(ForbiddenWrite(name()));
    }

    // Signal before setting the value so slots can throw an exception to prevent setting.
    m_setted_signal(object, *this, key, value);

    return setElement(object, key, value);
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::remove(const UserObject& object, const camp::Value& key) const
{
    // Check if element exists
    if(!exists(object, key))
    {
        m_removed_nonwritable_signal(object, *this, key);
        CAMP_ERROR(ElementNotFound());
    }

    // Signal before removing the value so slots can throw an exception to prevent removal.
    m_removed_signal(object, *this, key);

    return removeElement(object, key);
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection DictionaryProperty::connectSetted( const OnSet::slot_type& slot ) const
{
    return m_setted_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection DictionaryProperty::connectSettedNonwritable( const OnSet::slot_type& slot ) const
{
    return m_setted_nonwritable_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection DictionaryProperty::connectRemoved( const OnRemove::slot_type& slot ) const
{
    return m_removed_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection DictionaryProperty::connectRemovedNonwritable( const OnRemove::slot_type& slot ) const
{
    return m_removed_nonwritable_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
Value DictionaryProperty::getValue(const UserObject& object) const
{
    // TODO: Return first element
    //return get(object, 0);
    return Value();
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::setValue(const UserObject& object, const Value& value) const
{
    // TODO: Set first element
    //set(object, 0, value);
}

} // namespace camp
