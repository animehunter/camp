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

#include "dictionaryproperty.hpp"
#include <camp/classget.hpp>
#include <camp/errors.hpp>
#include <camp/dictionaryproperty.hpp>
#include <camp/dictionaryiterator.hpp>
#include <boost/test/unit_test.hpp>

using namespace DictionaryPropertyTest;

//-----------------------------------------------------------------------------
struct DictionaryPropertyFixture
{
    DictionaryPropertyFixture()
    {
        const camp::Class& metaclass = camp::classByType<MyClass>();
        mapDict = &static_cast<const camp::DictionaryProperty&>(metaclass.property("stdMap"));
        setDict = &static_cast<const camp::DictionaryProperty&>(metaclass.property("stdSet"));
    }

    const camp::DictionaryProperty* mapDict;
    const camp::DictionaryProperty* setDict;
    MyClass myObject;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::DictionaryProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(DICTIONARYPROPERTY, DictionaryPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(mapDict->type(), camp::dictionaryType);
    BOOST_CHECK(mapDict->typeInfo() == camp::TypeInfo(camp::DictionaryType(camp::stringType, &camp::classByType<MyType>())));
    BOOST_CHECK_EQUAL(setDict->type(), camp::dictionaryType);
    BOOST_CHECK(setDict->typeInfo() == camp::TypeInfo(camp::DictionaryType(&camp::classByType<MyType>(), &camp::classByType<MyType>())));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(keyType)
{
    BOOST_CHECK_EQUAL(mapDict->keyType(), camp::stringType);
    BOOST_CHECK_EQUAL(boost::get<camp::Type>(mapDict->keyTypeInfo()), camp::stringType);
    BOOST_CHECK_EQUAL(setDict->keyType(), camp::userType);
    BOOST_CHECK(*boost::get<const camp::Class*>(setDict->keyTypeInfo()) == camp::classByType<MyType>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(elementType)
{
    BOOST_CHECK_EQUAL(mapDict->elementType(), camp::userType);
    BOOST_CHECK(*boost::get<const camp::Class*>(mapDict->elementTypeInfo()) == camp::classByType<MyType>());
    BOOST_CHECK_EQUAL(setDict->elementType(), camp::userType);
    BOOST_CHECK(*boost::get<const camp::Class*>(setDict->elementTypeInfo()) == camp::classByType<MyType>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(size)
{
    BOOST_CHECK_EQUAL(mapDict->size(myObject), myObject.stdMap.size());
    BOOST_CHECK_EQUAL(setDict->size(myObject), myObject.stdSet.size());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(exists)
{
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "0"), true);
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "1"), true);
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "2"), true);
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "3"), true);
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "4"), true);
    BOOST_CHECK_EQUAL(mapDict->exists(myObject, "5"), false);

    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(0)), true);
    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(1)), true);
    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(2)), true);
    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(3)), true);
    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(4)), true);
    BOOST_CHECK_EQUAL(setDict->exists(myObject, MyType(5)), false);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(mapDict->get(myObject, "0"), camp::Value(myObject.stdMap["0"]));
    BOOST_CHECK_EQUAL(mapDict->get(myObject, "1"), camp::Value(myObject.stdMap["1"]));
    BOOST_CHECK_EQUAL(mapDict->get(myObject, "2"), camp::Value(myObject.stdMap["2"]));
    BOOST_CHECK_EQUAL(mapDict->get(myObject, "3"), camp::Value(myObject.stdMap["3"]));
    BOOST_CHECK_EQUAL(mapDict->get(myObject, "4"), camp::Value(myObject.stdMap["4"]));
    BOOST_CHECK_THROW(mapDict->get(myObject, "5"), camp::ElementNotFound);

    BOOST_CHECK_EQUAL(setDict->get(myObject, MyType(0)), camp::Value(*myObject.stdSet.find(MyType(0))));
    BOOST_CHECK_EQUAL(setDict->get(myObject, MyType(1)), camp::Value(*myObject.stdSet.find(MyType(1))));
    BOOST_CHECK_EQUAL(setDict->get(myObject, MyType(2)), camp::Value(*myObject.stdSet.find(MyType(2))));
    BOOST_CHECK_EQUAL(setDict->get(myObject, MyType(3)), camp::Value(*myObject.stdSet.find(MyType(3))));
    BOOST_CHECK_EQUAL(setDict->get(myObject, MyType(4)), camp::Value(*myObject.stdSet.find(MyType(4))));
    BOOST_CHECK_THROW(setDict->get(myObject, MyType(5)), camp::ElementNotFound);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(iterator)
{
    camp::DictionaryIteratorPtr iter = mapDict->iterator(myObject);
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(myObject.stdMap["0"]));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(myObject.stdMap["1"]));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(myObject.stdMap["2"]));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(myObject.stdMap["3"]));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(myObject.stdMap["4"]));

    iter = mapDict->iterator(myObject);
    unsigned short i = 0;
    while(iter->valid())
    {
        BOOST_CHECK_EQUAL(iter->nextKey().to<std::string>(), boost::lexical_cast<std::string>(i));
        ++i;
    }

    iter = setDict->iterator(myObject);
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(*myObject.stdSet.find(MyType(0))));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(*myObject.stdSet.find(MyType(1))));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(*myObject.stdSet.find(MyType(2))));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(*myObject.stdSet.find(MyType(3))));
    BOOST_CHECK_EQUAL(iter->nextValue(), camp::Value(*myObject.stdSet.find(MyType(4))));

    iter = setDict->iterator(myObject);
    i = 0;
    while(iter->valid())
    {
        BOOST_CHECK_EQUAL(iter->key().to<MyType>().x, i);
        BOOST_CHECK_EQUAL(iter->value().to<MyType>().x, i);
        iter->next();
        ++i;
    }
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    mapDict->set(myObject, "4", MyType(8));
    mapDict->set(myObject, "5", MyType(9));
    BOOST_CHECK(myObject.stdMap["4"] == MyType(8));
    BOOST_CHECK(myObject.stdMap["5"] == MyType(9));

    setDict->set(myObject, MyType(8), MyType(8));
    setDict->set(myObject, MyType(9), MyType(9));
    BOOST_CHECK_EQUAL(myObject.stdSet.count(MyType(8)), 1);
    BOOST_CHECK_EQUAL(myObject.stdSet.count(MyType(9)), 1);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(remove)
{
    std::size_t objectsSize = myObject.stdMap.size();
    mapDict->remove(myObject, "4");
    BOOST_CHECK_THROW(mapDict->remove(myObject, "5"), camp::ElementNotFound);
    BOOST_CHECK_EQUAL(myObject.stdMap.size(), objectsSize - 1);

    objectsSize = myObject.stdSet.size();
    setDict->remove(myObject, MyType(4));
    BOOST_CHECK_THROW(setDict->remove(myObject, MyType(5)), camp::ElementNotFound);
    BOOST_CHECK_EQUAL(myObject.stdSet.size(), objectsSize - 1);
}

BOOST_AUTO_TEST_SUITE_END()
