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


#ifndef CAMP_ARRAYPROPERTY_HPP
#define CAMP_ARRAYPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
/**
 * \brief Specialized type of property for arrays
 *
 */
class CAMP_API ArrayProperty : public Property
{
public:

    typedef boost::signals2::signal<void (const UserObject&, const ArrayProperty&, const Value&)> OnInsert;
    typedef boost::signals2::signal<void (const UserObject&, const ArrayProperty&, std::size_t, const Value&)> OnSet;
    typedef boost::signals2::signal<void (const UserObject&, const ArrayProperty&, std::size_t)> OnRemove;

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param elementType Type of the property
     * \param dynamic Tells if the array is dynamic or not
     */
    ArrayProperty(const std::string& name, Type elementType, bool dynamic);

    /**
     * \brief Destructor
     */
    virtual ~ArrayProperty();

    /**
     * \brief Get the type of the array elements
     *
     * \return Type of elements
     */
    Type elementType() const;

    /**
     * \brief Check if the array is dynamic
     *
     * If the array is not dynamic, the insert and remove functions
     * are disabled.
     *
     * \return True if the array is dynamic, false otherwise
     */
    bool dynamic() const;

    /**
     * \brief Get the current size of the array
     *
     * \param object Object
     *
     * \return Number of elements in the array
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    std::size_t size(const UserObject& object) const;

    /**
     * \brief Resize the array
     *
     * If \a size is lesser than the current size of the array,
     * the last elements will be removed; if \a size is greater
     * than the current size of the array, default-constructed
     * elements will be added at the end.
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param newSize New size of the array
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess array is not writable or not dynamic
     * \throw InvalidIndex index is out of range
     */
    void resize(const UserObject& object, std::size_t newSize) const;

    /**
     * \brief Get an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     * \throw OutOfRange index is out of range
     */
    Value get(const UserObject& object, std::size_t index) const;

    /**
     * \brief Set an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite property is not writable
     * \throw BadType \a value can't be converted to the property's type
     * \throw OutOfRange index is out of range
     */
    void set(const UserObject& object, std::size_t index, const Value& value) const;

    /**
     * \brief Insert a new element into the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite array is not writable or not dynamic
     * \throw BadType \a value can't be converted to the property's type
     * \throw OutOfRange index is out of range
     */
    void insert(const UserObject& object, std::size_t before, const Value& value) const;

    /**
     * \brief Remove an element from the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param index Index of the element to remove
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite array is not writable or not dynamic
     * \throw OutOfRange index is out of range
     */
    void remove(const UserObject& object, std::size_t index) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

    /**
     * \brief Connects a slot to the value inserted signal.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectInserted(const OnInsert::slot_type& slot) const;

    /**
     * \brief Connects a slot to the non-writable value inserted signal. This is only called when
     * a value is inserted while the object is not writable.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectInsertedNonwritable(const OnInsert::slot_type& slot) const;

    /**
     * \brief Connects a slot to the value setted signal.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectSetted(const OnSet::slot_type& slot) const;

    /**
     * \brief Connects a slot to the non-writable value setted signal. This is only called when
     * a value is set while the object is not writable.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectSettedNonwritable(const OnSet::slot_type& slot) const;

    /**
     * \brief Connects a slot to the value removed signal.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectRemoved(const OnRemove::slot_type& slot) const;

    /**
     * \brief Connects a slot to the non-writable value removed signal. This is only called when
     * a value is set while the object is not writable.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectRemovedNonwritable(const OnRemove::slot_type& slot) const;

protected:

    /**
     * \see Property::getValue
     */
    virtual Value getValue(const UserObject& object) const;

    /**
     * \see Property::setValue
     */
    virtual void setValue(const UserObject& object, const Value& value) const;

    /**
     * \brief Do the actual retrieval of the size
     *
     * \param object Object
     *
     * \return Size of the array
     */
    virtual std::size_t getSize(const UserObject& object) const = 0;

    /**
     * \brief Do the actual resize of the array
     *
     * \param object Object
     * \param size New size of the array
     */
    virtual void setSize(const UserObject& object, std::size_t size) const = 0;

    /**
     * \brief Do the actual reading of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     */
    virtual Value getElement(const UserObject& object, std::size_t index) const = 0;

    /**
     * \brief Do the actual writing of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     */
    virtual void setElement(const UserObject& object, std::size_t index, const Value& value) const = 0;

    /**
     * \brief Do the actual insertion of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     */
    virtual void insertElement(const UserObject& object, std::size_t before, const Value& value) const = 0;

    /**
     * \brief Do the actual removal of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to remove
     */
    virtual void removeElement(const UserObject& object, std::size_t index) const = 0;

private:

    Type m_elementType; ///< Type of the individual elements of the array
    bool m_dynamic; ///< Is the array dynamic?
    mutable OnInsert m_inserted_signal; ///< Inserted signal
    mutable OnInsert m_inserted_nonwritable_signal; ///< Non-writable inserted signal
    mutable OnSet m_setted_signal; ///< Setted signal
    mutable OnSet m_setted_nonwritable_signal; ///< Non-writable setted signal
    mutable OnRemove m_removed_signal; ///< Removed signal
    mutable OnRemove m_removed_nonwritable_signal; ///< Non-writable removed signal
};

} // namespace camp


#endif // CAMP_ARRAYPROPERTY_HPP
