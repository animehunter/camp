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


#ifndef CAMP_DETAIL_DICTIONARYPROPERTYIMPL_HPP
#define CAMP_DETAIL_DICTIONARYPROPERTYIMPL_HPP


#include <camp/dictionaryproperty.hpp>
#include <camp/dictionarymapper.hpp>
#include <camp/detail/valueprovider.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Typed implementation of DictionaryProperty
 *
 * DictionaryPropertyImpl is a template implementation of DictionaryProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * This class uses the camp_ext::DictionaryMapper template to implement its operations according
 * to the type of array.
 *
 * \sa DictionaryProperty, camp_ext::DictionaryMapper
 */
template <typename A>
class DictionaryPropertyImpl : public DictionaryProperty
{
public:

    /**
     * \brief Construct the property
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    DictionaryPropertyImpl(const std::string& name, const A& accessor);

protected:

    /**
     * \see DictionaryProperty::getSize
     */
    virtual std::size_t getSize(const UserObject& object) const;

    /**
     * \see DictionaryProperty::queryExists
     */
    virtual bool queryExists(const UserObject& object, const Value& key) const;

    /**
     * \see DictionaryProperty::getElement
     */
    virtual Value getElement(const UserObject& object, const Value& key) const;

    /**
     * \see DictionaryProperty::getIterator
     */
    virtual DictionaryIteratorPtr getIterator(const UserObject& object) const;

    /**
     * \see DictionaryProperty::setElement
     */
    virtual void setElement(const UserObject& object, const Value& key, const Value& value) const;

    /**
     * \see DictionaryProperty::removeElement
     */
    virtual void removeElement(const UserObject& object, const Value& key) const;

private:

    typedef typename boost::remove_reference<typename A::DataType>::type DictionaryType;
    typedef camp_ext::DictionaryMapper<DictionaryType> Mapper;
    typedef typename Mapper::KeyType KeyType;
    typedef typename Mapper::ElementType ElementType;

    /**
     * \brief Retrieve a reference to the dictionary
     *
     * \param object Owner object
     *
     * \return Reference to the underlying dictionary
     */
    DictionaryType& dictionary(const UserObject& object) const;

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace camp

#include <camp/detail/dictionarypropertyimpl.inl>


#endif // CAMP_DETAIL_DICTIONARYPROPERTYIMPL_HPP
