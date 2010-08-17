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

#include <camp-lua/context.hpp>
#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <iostream>

class A
{
public:
    bool boolean;
    int integer;
    double real;
    std::string string;
    const A& self() const { return *this; }
    void setBoolean(bool value) { boolean = value; };
    void setInteger(int value) { integer = value; };
    void setReal(double value) { real = value; };
    void setString(std::string value) { string = value; };
};

CAMP_TYPE(A);

void initCAMP()
{
    camp::Class::declare<A>("A")
        .property("boolean", &A::boolean)
        .property("integer", &A::integer)
        .property("real", &A::real)
        .property("string", &A::string)
        .property("self", &A::self)
        .function("setBoolean", &A::setBoolean)
        .function("setInteger", &A::setInteger)
        .function("setReal", &A::setReal)
        .function("setString", &A::setString)
        ;
}

int main(int argc, char** argv)
{
    initCAMP();

    A a;
    a.boolean = false;
    a.integer = 2;
    a.real = 4.5;
    a.string = "plop";

    camp::lua::Context c1;
    c1["a"] = a;

    c1.executeString("print(a.boolean, a.integer, a.real, a.string)");
    c1.executeString("a.boolean = true; a.integer = 3; a.real = 6.7; a.string = \"pouet\"");
    c1.executeString("print(a.boolean, a.integer, a.real, a.string)");
    c1.executeString("a:setBoolean(false); a:setInteger(8); a:setReal(9.1); a:setString(\"youpi\")");
    c1.executeString("print(a.boolean, a.integer, a.real, a.string)");

    c1.executeString("print(\"----\")");
    c1.executeString("b = a.self");
    c1.executeString("print(b.boolean, b.integer, b.real, b.string)");
    c1.executeString("b.boolean = true; b.integer = 3; b.real = 6.7; b.string = \"pouet\"");
    c1.executeString("print(b.boolean, b.integer, b.real, b.string)");
    c1.executeString("a:setBoolean(false); a:setInteger(8); a:setReal(9.1); a:setString(\"youpi\")");
    c1.executeString("print(b.boolean, b.integer, b.real, b.string)");

    c1.executeString("print(\"----\")");
    c1.executeString("a.boolean = true; a.integer = 3; a.real = 6.7; a.string = \"pouet\"");
    c1.executeString("print(a.boolean, a.integer, a.real, a.string)");
    c1.executeString("b:setBoolean(false); b:setInteger(8); b:setReal(9.1); b:setString(\"youpi\")");
    c1.executeString("print(b.boolean, b.integer, b.real, b.string)");

    c1.executeString("print(\"----\")");
    c1.executeString("print(a.self.string)");

    return 0;
}

