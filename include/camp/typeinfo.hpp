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


#ifndef CAMP_TYPEINFO_HPP
#define CAMP_TYPEINFO_HPP


#include <camp/type.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>


namespace camp
{
struct ArrayType;
struct DictionaryType;
class Class;
class Enum;

/**
 * \brief Variant type for type info.
 */
typedef boost::variant< Type, const camp::Enum*, const camp::Class*, boost::recursive_wrapper<ArrayType>, 
    boost::recursive_wrapper<DictionaryType> > TypeInfo;

/**
 * \brief Type info for \c arrayType.
 */
struct ArrayType
{
    TypeInfo m_elementType;
};

/**
 * \brief Type info for \c dictionaryType.
 */
struct DictionaryType
{
    TypeInfo m_keyType;
    TypeInfo m_elementType;
};

/**
 * \brief Base class for writing custom type info visitors
 *
 * A type info visitor acts like compile-time dispatchers which automatically
 * calls the function which matches the type of the given type info.
 * This is a more direct and straight-forward approach than using a runtime switch,
 * based on type() and then converting to the proper type.
 *
 * The template parameter T is the type returned by the visitor.
 *
 * To handle one of the possible types, just write the corresponding \c operator() function.
 * Here is the list of the mapping between CAMP types and their corresponding C++ types and type info:
 *
 * \li camp::noType --> camp::NoType --> Type
 * \li camp::boolType --> bool --> Type
 * \li camp::intType --> long --> Type
 * \li camp::realType --> double --> Type
 * \li camp::stringType --> std::string --> Type
 * \li camp::valueType --> camp::Value --> Type
 * \li camp::enumType --> camp::EnumObject --> const camp::Enum&
 * \li camp::userType --> camp::UserObject --> const camp::Class&
 * \li camp::arrayType --> *none yet* --> camp::ArrayType
 * \li camp::dictionaryType --> *none yet* --> camp::DictionaryType
 *
 * Here an example of a unary visitor which returns a string representing the type name.
 * \code
 * struct TypeVisitor : public camp::TypeVisitor<std::string>
 * {
 *     std::string operator()(camp::Type type)
 *     {
 *         switch(type)
 *         {
 *             case camp::noType: return "VoidType()";
 *             case camp::boolType: return "BoolType()";
 *             case camp::intType: return "IntType()";
 *             case camp::realType: return "RealType()";
 *             case camp::stringType: return "StringType()";
 *             case camp::valueType: return "ValueType()";
 *         }
 *         return "VoidType()";
 *     }
 *     std::string operator()(const camp::Enum* metaenum)
 *     {
 *         return "EnumType(\"" + metaenum->name() + "\")";
 *     }
 *     std::string operator()(const camp::Class* metaclass)
 *     {
 *         return "UserType(\"" + metaclas->name() + "\")";
 *     }
 *     std::string operator()(camp::ArrayType type)
 *     {
 *         return "ArrayType(\"" + type.m_elementType.visit(TypeVisitor()) + "\")";
 *     }
 *     std::string operator()(camp::DictionaryType type)
 *     {
 *         return "DictionaryType(\"" + type.m_keyType.visit(TypeVisitor()) + "\", \"" +
 *             + type.m_elementType.visit(TypeVisitor()) "\")";
 *     }
 * };
 * 
 * camp::TypeInfo typeInfo = function.returnTypeInfo();
 * std::cout << typeInfo.visit(TypeVisitor()) << std::endl;
 * \endcode
 */
template <typename T = void>
class TypeVisitor : public boost::static_visitor<T>
{

};

} // namespace camp


#endif // CAMP_TYPEINFO_HPP
