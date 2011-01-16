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

#ifndef CAMPTEST_DICTIONARYPROPERTY_HPP
#define CAMPTEST_DICTIONARYPROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <string>
#include <map>
#include <set>

namespace DictionaryPropertyTest
{
    struct MyType
    {
        MyType(int x_ = 0) : x(x_)
        {
        }

        bool operator ==(const MyType& other) const
        {
            return x == other.x;
        }

        bool operator <(const MyType& other) const
        {
            return x < other.x;
        }

        int x;
    };

    struct MyClass
    {
        MyClass()
        {
            stdMap.insert(std::make_pair("0", MyType(0)));
            stdMap.insert(std::make_pair("1", MyType(1)));
            stdMap.insert(std::make_pair("2", MyType(2)));
            stdMap.insert(std::make_pair("3", MyType(3)));
            stdMap.insert(std::make_pair("4", MyType(4)));

            stdSet.insert(MyType(0));
            stdSet.insert(MyType(1));
            stdSet.insert(MyType(2));
            stdSet.insert(MyType(3));
            stdSet.insert(MyType(4));
        }

        std::map<std::string, MyType> stdMap;
        std::set<MyType> stdSet;
    };

    void declare()
    {
        camp::Class::declare<MyType>("DictionaryPropertyTest::MyType");

        camp::Class::declare<MyClass>("DictionaryPropertyTest::MyClass")
            .property("stdMap", &MyClass::stdMap)
            .property("stdSet", &MyClass::stdSet);
    }
}

CAMP_AUTO_TYPE(DictionaryPropertyTest::MyType, &DictionaryPropertyTest::declare)
CAMP_AUTO_TYPE(DictionaryPropertyTest::MyClass, &DictionaryPropertyTest::declare)

#endif // CAMPTEST_DICTIONARYPROPERTY_HPP
