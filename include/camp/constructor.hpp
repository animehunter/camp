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


#ifndef CAMP_CONSTRUCTOR_HPP
#define CAMP_CONSTRUCTOR_HPP


#include <camp/config.hpp>
#include <camp/typeinfo.hpp>
#include <vector>


namespace camp
{
class UserObject;
class Args;

/**
 * \brief camp::Constructor represents a metaconstructor which is used to create objects instances from metaclasses
 *
 * This class is an interface which has to be derived to implement typed constructors.
 *
 * \sa Property, Function
 */
class CAMP_API Constructor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Constructor();

    /**
     * \brief Get the number of arguments of the constructor
     *
     * \return Total number of arguments taken by the constructor
     */
    std::size_t argCount() const;

    /**
     * \brief Get the type of an argument given by its index
     *
     * \param index Index of the argument
     *
     * \return Type of the index-th argument
     *
     * \throw OutOfRange index is out of range
     */
    Type argType(std::size_t index) const;

    /**
     * \brief Get the type info of an argument given by its index
     *
     * \param index Index of the argument
     *
     * \return Type info of the index-th argument
     *
     * \throw OutOfRange index is out of range
     */
    TypeInfo argTypeInfo(std::size_t index) const;

    /**
     * \brief Check if the constructor matches the given set of arguments
     *
     * \param args Set of arguments to check
     *
     * \return True if the constructor is compatible with the given arguments
     */
    virtual bool matches(const Args& args) const = 0;

    /**
     * \brief Use the constructor to create a new object
     *
     * \param args Set of arguments to pass to the constructor
     *
     * \return Pointer to the new object wrapped in a UserObject, or UserObject::nothing on failure
     */
    virtual UserObject create(const Args& args) const = 0;

protected:

    template <typename T> friend class ClassBuilder;

    /**
     * \brief Construct the constructor from its description
     *
     * \param argTypes Types of the constructor arguments (empty array by default)
     * \param argTypeInfo Type info of the constructor arguments (empty array by default)
     */
    Constructor(const std::vector<Type>& argTypes = std::vector<Type>(), 
        const std::vector<TypeInfo>& argTypeInfo = std::vector<TypeInfo>());

private:

    std::vector<Type> m_argTypes; ///< Type of all the function arguments
    std::vector<TypeInfo> m_argTypeInfo; ///< Type info of all the function arguments
};

} // namespace camp


#endif // CAMP_CONSTRUCTOR_HPP
