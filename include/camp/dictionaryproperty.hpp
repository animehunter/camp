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


#ifndef CAMP_DICTIONARYPROPERTY_HPP
#define CAMP_DICTIONARYPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
/**
 * \brief Specialized type of property for dictionaries
 *
 */
class CAMP_API DictionaryProperty : public Property
{
public:

    typedef boost::signals2::signal<void (const UserObject&, const DictionaryProperty&, const Value&)> OnInsert;

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param keyType Type of the dictionary key.
     * \param elementType Type of the dictionary element.
     */
    DictionaryProperty(const std::string& name, Type keyType, Type elementType);

    /**
     * \brief Destructor
     */
    virtual ~DictionaryProperty();

    /**
     * \brief Get the type of the dictionary keys
     *
     * \return Type of keys
     */
    Type keyType() const;

    /**
     * \brief Get the type of the dictionary elements
     *
     * \return Type of elements
     */
    Type elementType() const;

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
     * \brief Check if an element exists for given key.
     *
     * \param object Object
     * \param key Key of the element to check
     *
     * \return True if element is found, false if not
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    bool exists(const UserObject& object, const camp::Value& key) const;

    /**
     * \brief Get an element of the dictionary for a given object and key
     *
     * \param object Object
     * \param key Key of the element to get
     *
     * \return Value of the index-th element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    Value get(const UserObject& object, const camp::Value& key) const;

    /**
     * \brief Set an element of the dictionary for a given object and key
     *
     * \param object Object
     * \param key Key of the element to set
     * \param value New value to assign to the element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite property is not writable
     * \throw BadType \a key or \a value can't be converted to the property's type
     */
    void set(const UserObject& object, const camp::Value& key, const Value& value) const;

    /**
     * \brief Remove an element from the dictionary
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param key Key of the element to remove
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite array is not writable or not dynamic
     */
    void remove(const UserObject& object, const camp::Value& key) const;

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
    boost::signals2::connection connectInsertion(const OnInsert::slot_type& slot) const;

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
     * \brief Do the actual existance checking.
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param key Key of the element to check
     *
     * \return True if element is found, false if not
     */
    virtual bool queryExists(const UserObject& object, const camp::Value& key) const = 0;

    /**
     * \brief Do the actual reading of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param key Key of the element to get
     *
     * \return Value of the index-th element
     */
    virtual Value getElement(const UserObject& object, const camp::Value& key) const = 0;

    /**
     * \brief Do the actual writing of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param key Key of the element to set
     * \param value New value to assign to the element
     */
    virtual void setElement(const UserObject& object, const camp::Value& key, const Value& value) const = 0;

    /**
     * \brief Do the actual removal of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param key Key of the element to remove
     */
    virtual void removeElement(const UserObject& object, const camp::Value& key) const = 0;

private:

    Type m_keyType; ///< Type of the keys of the dictionary
    Type m_elementType; ///< Type of the elements of the dictionary
    mutable OnInsert m_signal; ///< Insertion signal
};

} // namespace camp


#endif // CAMP_DICTIONARYPROPERTY_HPP
