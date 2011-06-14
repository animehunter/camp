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


#ifndef CAMP_DICTIONARYMAPPER_HPP
#define CAMP_DICTIONARYMAPPER_HPP


#include <camp/config.hpp>
#include <camp/detail/yesnotype.hpp>
#include <camp/dictionaryiterator.hpp>
#include <camp/valuemapper.hpp>
#include <map>
#include <set>

namespace camp_ext
{
/**
 * \class DictionaryMapper
 *
 * \brief Template providing a mapping between C++ dictionaries and CAMP DictionaryProperty
 *
 * DictionaryMapper<T> must define the following members in order to make T fully compliant with the system:
 *
 * \li \c KeyType: type of the key in the dictionary
 * \li \c ElementType: type of the elements stored in the dictionary
 * \li \c size(): retrieve the size of the dictionary
 * \li \c exists(): query if an element exists
 * \li \c get(): get the value of an element
 * \li \c iterator(): get an iterator to iterate over all elements of the dictionary
 * \li \c set(): sets an element
 * \li \c remove(): remove an element
 *
 * ValueMapper is specialized for every supported type, and can be specialized
 * for any of your own array types in order to extend the system.
 *
 * By default, ValueMapper supports the following types of dictionaries:
 *
 * \li std::map
 * \li std::set
 *
 * Here is an example of mapping for the std::map class:
 *
 * \code
 * namespace camp_ext
 * {
 *     template <typename K, typename E>
 *     struct DictionaryMapper<std::map<K, E> >
 *     {
 *         typedef K KeyType;
 *         typedef E ElementType;
 *
 *         static std::size_t size(const std::map<K, E>& dict)
 *         {
 *             return dict.size();
 *         }
 *
 *         static bool exists(const std::map<K, E>& dict, const K& key)
 *         {
 *             return dict.count(key) == 1;
 *         }
 *
 *         static const E& get(const std::map<K, E>& dict, const K& key)
 *         {
 *             return dict.find(key)->second;
 *         }
 *
 *         static camp::DictionaryIteratorPtr iterator(std::map<K, E>& dict)
 *         {
 *             return camp::DictionaryIteratorPtr(
 *                 new camp::MapDictionaryIterator<std::map<K, E> >(dict.begin(), dict));
 *         }
 *
 *         static void set(std::map<K, E>& dict, const K& key, const E& value)
 *         {
 *             dict[key] = value;
 *         }
 *
 *         static void remove(std::map<K, E>& dict, const K& key)
 *         {
 *             dict.erase(key);
 *         }
 *     } 
 * }
 * \endcode
 */

/*
 * Generic version -- doesn't define anything
 */
template <typename T>
struct DictionaryMapper
{
};

/*
 * Specialization of DictionaryMapper for std::map
 */
template <typename K, typename E>
struct DictionaryMapper<std::map<K, E> >
{
    typedef K KeyType;
    typedef E ElementType;

    static std::size_t size(const std::map<K, E>& dict)
    {
        return dict.size();
    }

    static bool exists(const std::map<K, E>& dict, const K& key)
    {
        return dict.count(key) == 1;
    }

    static const E& get(const std::map<K, E>& dict, const K& key)
    {
        return dict.find(key)->second;
    }

    static camp::DictionaryIteratorPtr iterator(std::map<K, E>& dict)
    {
        return camp::DictionaryIteratorPtr(
            new camp::MapDictionaryIterator<std::map<K, E> >(dict.begin(), dict));
    }

    static void set(std::map<K, E>& dict, const K& key, const E& value)
    {
        dict[key] = value;
    }

    static void remove(std::map<K, E>& dict, const K& key)
    {
        dict.erase(key);
    }
};

/*
 * Specialization of DictionaryMapper for std::set
 */
template <typename T>
struct DictionaryMapper<std::set<T> >
{
    typedef T KeyType;
    typedef T ElementType;

    static std::size_t size(const std::set<T>& dict)
    {
        return dict.size();
    }

    static bool exists(const std::set<T>& dict, const T& key)
    {
        return dict.count(key) == 1;
    }

    static const T& get(const std::set<T>& dict, const T& key)
    {
        return *dict.find(key);
    }

    static camp::DictionaryIteratorPtr iterator(std::set<T>& dict)
    {
        return camp::DictionaryIteratorPtr(
            new camp::SetDictionaryIterator<std::set<T> >(dict.begin(), dict));
    }

    static void set(std::set<T>& dict, const T& key, const T& value)
    {
        dict.insert(key);
    }

    static void remove(std::set<T>& dict, const T& key)
    {
        dict.erase(key);
    }
};


} // namespace camp_ext


namespace camp
{
namespace detail
{
/**
 * \brief Helper structure to check at compile time if a type is a dictionary
 *
 * This structure check if the specialization of DictionaryMapper for the
 * template parameter T exists (i.e. the ElementType member is properly defined).
 */
template <typename T>
struct IsDictionary
{
    template <typename U> static TypeYes check(typename U::ElementType*);
    template <typename U> static TypeNo  check(...);

    enum {value = sizeof(check<camp_ext::DictionaryMapper<T> >(0)) == sizeof(TypeYes)};
};

} // namespace detail

} // namespace camp


namespace camp_ext
{
    /*
     * Specialization of ValueMapper for dictionaries.
     * No conversion allowed, only type mapping is provided.
     */
    template <typename T>
    struct ValueMapper<T, typename boost::enable_if_c<camp::detail::IsDictionary<T>::value>::type>
    {
        static const int type = camp::dictionaryType;

        static camp::CampType typeInfo() 
        { 
            camp::DictionaryType dictType;
            dictType.m_keytype = ValueMapper<typename DictionaryMapper<T>::KeyType>::typeInfo();
            dictType.m_elementtype = ValueMapper<typename DictionaryMapper<T>::ElementType>::typeInfo();
            return dictType; 
        }
    };
}


#endif // CAMP_DICTIONARYMAPPER_HPP
