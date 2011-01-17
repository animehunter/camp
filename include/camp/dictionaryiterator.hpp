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


#ifndef CAMP_DICTIONARYITERATOR_HPP
#define CAMP_DICTIONARYITERATOR_HPP


#include <camp/config.hpp>
#include <camp/value.hpp>

namespace camp
{

/**
 * \brief Interface for a dictionary iterator wrapper.
 *
 * This class is an interface which has to be derived to implement dictionary iterator wrappers.
 */
class DictionaryIterator
{
public:
    /**
     * \brief Destructor
     */
    virtual ~DictionaryIterator() {}

    /**
     * \brief Gets the key the iterator is pointing to
     */
    virtual Value key() = 0;
    /**
     * \brief Gets the key the iterator is pointing to and increments the iterator
     */
    virtual Value nextKey() = 0;
    /**
     * \brief Gets the value the iterator is pointing to
     */
    virtual Value value() = 0;
    /**
     * \brief Gets the value the iterator is pointing to and increments the iterator
     */
    virtual Value nextValue() = 0;
    /**
     * \brief Increments the iterator
     */
    virtual void next() = 0;
    /**
     * \brief Query if the iterator is still valid (not at the end)
     */
    virtual bool valid() = 0;

};

typedef boost::shared_ptr<DictionaryIterator> DictionaryIteratorPtr;

/*
 * Implementation of DictionaryIterator for std::map
 */
template <typename T>
class MapDictionaryIterator : public DictionaryIterator
{
public:
    typedef typename T::iterator Iterator;

    MapDictionaryIterator(Iterator iter, const T& map): m_iterator(iter), m_map(map) {}
    ~MapDictionaryIterator() {}

    Value key()
    {
        return m_iterator->first;
    }
    Value nextKey()
    {
        return (m_iterator++)->first;
    }
    Value value()
    {
        return m_iterator->second;
    }
    Value nextValue()
    {
        return (m_iterator++)->second;
    }
    void next()
    {
        ++m_iterator;
    }
    bool valid()
    {
        return m_iterator != m_map.end();
    }

private:
    Iterator m_iterator;
    const T& m_map;

};

/*
 * Implementation of DictionaryIterator for std::set
 */
template <typename T>
class SetDictionaryIterator : public DictionaryIterator
{
public:
    typedef typename T::iterator Iterator;

    SetDictionaryIterator(Iterator iter, const T& set): m_iterator(iter), m_set(set) {}
    ~SetDictionaryIterator() {}

    Value key()
    {
        return *m_iterator;
    }
    Value nextKey()
    {
        return *(m_iterator++);
    }
    Value value()
    {
        return *m_iterator;
    }
    Value nextValue()
    {
        return *(m_iterator++);
    }
    void next()
    {
        ++m_iterator;
    }
    bool valid()
    {
        return m_iterator != m_set.end();
    }

private:
    Iterator m_iterator;
    const T& m_set;

};

} // namespace camp


#endif // CAMP_DICTIONARYITERATOR_HPP
