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
DictionaryProperty::DictionaryProperty(const std::string& name, Type keyType, Type elementType)
    : Property(name, dictionaryType)
    , m_keyType(keyType)
    , m_elementType(elementType)
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
Type DictionaryProperty::elementType() const
{
    return m_elementType;
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
void DictionaryProperty::set(const UserObject& object, const camp::Value& key, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(ForbiddenWrite(name()));

    return setElement(object, key, value);
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::remove(const UserObject& object, const camp::Value& key) const
{
    // Check if element exists
    if(!exists(object, key))
        CAMP_ERROR(ElementNotFound());

    return removeElement(object, key);
}

//-------------------------------------------------------------------------------------------------
void DictionaryProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection DictionaryProperty::connectInsertion(const OnInsert::slot_type& slot) const
{
    return m_signal.connect(slot);
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
