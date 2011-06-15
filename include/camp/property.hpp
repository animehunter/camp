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


#ifndef CAMP_PROPERTY_HPP
#define CAMP_PROPERTY_HPP


#include <camp/config.hpp>
#include <camp/detail/getter.hpp>
#include <camp/tagholder.hpp>
#include <camp/type.hpp>
#include <camp/typeinfo.hpp>
#include <boost/signals2.hpp>
#include <string>


namespace camp
{
template <typename T> class ClassBuilder;
class Value;
class UserObject;
class ClassVisitor;

/**
 * \brief Abstract representation of a property
 *
 * Properties are members of metaclasses. Their main purpose is to be get and/or set;
 * They also provide detailed informations about their type.
 *
 * \sa PSimpleroperty, ArrayProperty, EnumProperty, ObjectProperty
 */
class CAMP_API Property : public TagHolder
{
public:

    typedef boost::signals2::signal<void (const UserObject&, const Property&, const Value&, const int)> OnSet;

    /**
     * \brief Destructor
     */
    virtual ~Property();

    /**
     * \brief Get the name of the property
     *
     * \return Name of the property
     */
    const std::string& name() const;

    /**
     * \brief Get the type of the property
     *
     * \return Type of the property
     */
    Type type() const;

    /**
     * \brief Get the type info of the property
     *
     * \return Type of the property
     */
    TypeInfo typeInfo() const;

    /**
     * \brief Check if the property is currently readable for a given object
     *
     * \param object Object
     *
     * \return True if the property can be read, false otherwise
     *
     * \throw NullObject object is invalid
     */
    bool readable(const UserObject& object) const;

    /**
     * \brief Check if the property is writable, ie if a setter exists for this property
     *
     * \return True if the property can be written, false otherwise
     */
    bool writable() const;

    /**
     * \brief Check if the property is currently writable for a given object
     *
     * \param object Object
     *
     * \return True if the property can be written, false otherwise
     *
     * \throw NullObject object is invalid
     */
    bool writable(const UserObject& object) const;

    /**
     * \brief Get the current value of the property for a given object
     *
     * \param object Object
     *
     * \return Value of the property
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    Value get(const UserObject& object) const;

    /**
     * \brief Set the current value of the property for a given object
     *
     * \param object Object
     * \param value New value to assign to the property
     * \param reason Optional reason number that gets sent to the change signal
     *
     * \throw NullObject \a object is invalid
     * \throw ForbiddenWrite property is not writable
     * \throw BadType \a value can't be converted to the property's type
     */
    void set(const UserObject& object, const Value& value, const int reason = 0) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

    /**
     * \brief Connects a slot to the property setted signal.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectSetted(const OnSet::slot_type& slot) const;

    /**
     * \brief Connects a slot to the non-writable property setted signal. This is only called when
     * a value is set while the object is not writable.
     * 
     * \param slot The slot to connect to the signal.
     *
     * \return Connection object to block or disconnect the signal-slot connection.
     */
    boost::signals2::connection connectSettedNonwritable(const OnSet::slot_type& slot) const;

protected:

    template <typename T> friend class ClassBuilder;
    friend class UserObject;

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param type Type of the property
     * \param typeInfo Type info of the property
     */
    Property(const std::string& name, Type type, TypeInfo typeInfo);

    /**
     * \brief Do the actual reading of the value
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     *
     * \return Value of the property
     */
    virtual Value getValue(const UserObject& object) const = 0;

    /**
     * \brief Do the actual writing of the value
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     * \param value New value to assign to the property
     */
    virtual void setValue(const UserObject& object, const Value& value) const = 0;

    /**
     * \brief Check if the property can be read
     *
     * \return True if the property can be read, false otherwise
     */
    virtual bool isReadable() const;

    /**
     * \brief Check if the property can be written
     *
     * \return True if the property can be written, false otherwise
     */
    virtual bool isWritable() const;

private:

    std::string m_name; ///< Name of the property
    Type m_type; ///< Type of the property
    TypeInfo m_typeInfo; ///< Type info of the property
    detail::Getter<bool> m_readable; ///< Accessor to get the readable state of the property
    detail::Getter<bool> m_writable; ///< Accessor to get the writable state of the property
    mutable OnSet m_setted_signal; ///< Setter signal
    mutable OnSet m_setted_nonwritable_signal; ///< Non-writable setter signal
};

} // namespace camp


#endif // CAMP_PROPERTY_HPP
