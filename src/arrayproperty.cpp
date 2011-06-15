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


#include <camp/arrayproperty.hpp>
#include <camp/classvisitor.hpp>
#include <camp/errors.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
ArrayProperty::ArrayProperty(const std::string& name, Type elementType, TypeInfo elementTypeInfo, bool dynamic)
    : Property(name, arrayType, ArrayType(elementTypeInfo))
    , m_elementType(elementType)
    , m_elementTypeInfo(elementTypeInfo)
    , m_dynamic(dynamic)
{
}

//-------------------------------------------------------------------------------------------------
ArrayProperty::~ArrayProperty()
{
}

//-------------------------------------------------------------------------------------------------
Type ArrayProperty::elementType() const
{
    return m_elementType;
}

//-------------------------------------------------------------------------------------------------
bool ArrayProperty::dynamic() const
{
    return m_dynamic;
}

//-------------------------------------------------------------------------------------------------
std::size_t ArrayProperty::size(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    return getSize(object);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::resize(const UserObject& object, std::size_t newSize) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(ForbiddenWrite(name()));

    setSize(object, newSize);
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::get(const UserObject& object, std::size_t index) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(ForbiddenRead(name()));

    // Make sure that the index is not out of range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(OutOfRange(index, range));

    return getElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::set(const UserObject& object, std::size_t index, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
    {
        m_setted_nonwritable_signal(object, *this, index, value);
        CAMP_ERROR(ForbiddenWrite(name()));
    }

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(OutOfRange(index, range));

    // Signal before setting the value so slots can throw an exception to prevent setting.
    m_setted_signal(object, *this, index, value);

    return setElement(object, index, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::insert(const UserObject& object, std::size_t before, const Value& value) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!writable(object))
    {
        m_inserted_nonwritable_signal(object, *this, value);
        CAMP_ERROR(ForbiddenWrite(name()));
    }

    // Check if the index is in range
    std::size_t range = size(object) + 1;
    if (before >= range)
        CAMP_ERROR(OutOfRange(before, range));

    // Signal before inserting the value so slots can throw an exception to prevent insertion.
    m_inserted_signal(object, *this, value);

    return insertElement(object, before, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::remove(const UserObject& object, std::size_t index) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(ForbiddenWrite(name()));

    // Check if the property is writable
    if (!writable(object))
    {
        m_removed_nonwritable_signal(object, *this, index);
        CAMP_ERROR(ForbiddenWrite(name()));
    }

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(OutOfRange(index, range));

    // Signal before removing the value so slots can throw an exception to prevent removal.
    m_removed_signal(object, *this, index);

    return removeElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectInserted(const OnInsert::slot_type& slot) const
{
    return m_inserted_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectInsertedNonwritable(const OnInsert::slot_type& slot) const
{
    return m_inserted_nonwritable_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectSetted( const OnSet::slot_type& slot ) const
{
    return m_setted_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectSettedNonwritable( const OnSet::slot_type& slot ) const
{
    return m_setted_nonwritable_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectRemoved( const OnRemove::slot_type& slot ) const
{
    return m_removed_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
boost::signals2::connection ArrayProperty::connectRemovedNonwritable( const OnRemove::slot_type& slot ) const
{
    return m_removed_nonwritable_signal.connect(slot);
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::getValue(const UserObject& object) const
{
    // Return first element
    return get(object, 0);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::setValue(const UserObject& object, const Value& value) const
{
    // Set first element
    set(object, 0, value);
}

} // namespace camp
