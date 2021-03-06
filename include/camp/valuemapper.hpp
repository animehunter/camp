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


#ifndef CAMP_VALUEMAPPER_HPP
#define CAMP_VALUEMAPPER_HPP


#include <camp/enum.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>
#include <camp/arraymapper.hpp>
#include <camp/errors.hpp>
#include <camp/value.hpp>
#include <camp/typeinfo.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>


namespace camp_ext
{
    template <typename T, typename C = void> struct ValueMapper;
}


namespace camp
{
/**
 * \brief Map a C++ type to a CAMP type
 *
 * This function simply returns the mapping defined by ValueMapper (i.e. \c ValueMapper<T>::type).
 *
 * \return CAMP type which T maps to
 */
template <typename T>
Type mapType()
{
    return static_cast<Type>(camp_ext::ValueMapper<T>::type);
}

} // namespace camp


namespace camp_ext
{
/**
 * \class ValueMapper
 *
 * \brief Template providing a mapping between C++ types/values and CAMP types/values
 *
 * ValueMapper<T> defines three things in order to make T fully compliant with the system:
 *
 * \li The abstract CAMP type that T is mapped to
 * \li A function to convert from T to the mapped CAMP type
 * \li A function to convert from all supported CAMP types to T
 *
 * ValueMapper is specialized for every supported type, and can be specialized
 * for any of your own types in order to extend the system.
 *
 * Here is an example of mapping for a custom string class:
 *
 * \code
 * namespace camp_ext
 * {
 *     template <>
 *     struct ValueMapper<MyStringClass>
 *     {
 *         // The corresponding CAMP type is "string"
 *         static const int type = camp::string;
 *  
 *         // Convert from MyStringClass to std::string
 *         static std::string to(const MyStringClass& source)
 *         {
 *             return source.to_std_string();
 *         }
 * 
 *         // Convert from any type to MyStringClass
 *         // Be smart, juste reuse ValueMapper<std::string> :)
 *         template <typename T>
 *         static MyStringClass from(const T& source)
 *         {
 *             return MyStringClass(ValueMapper<std::string>::from(source));
 *         }
 *     };
 * }
 * \endcode
 */

/*
 * Generic version of ValueMapper -- T doesn't match with any specialization
 * and is thus treated as a user object
 */
template <typename T, typename C>
struct ValueMapper
{
    static const int type = camp::userType;
    static camp::UserObject to(const T& source) {return camp::UserObject(source);}

    static T from(bool)                           {CAMP_ERROR(camp::BadType(camp::boolType,   camp::mapType<T>()));}
    static T from(long)                           {CAMP_ERROR(camp::BadType(camp::intType,    camp::mapType<T>()));}
    static T from(double)                         {CAMP_ERROR(camp::BadType(camp::realType,   camp::mapType<T>()));}
    static T from(const std::string&)             {CAMP_ERROR(camp::BadType(camp::stringType, camp::mapType<T>()));}
    static T from(const camp::EnumObject&)        {CAMP_ERROR(camp::BadType(camp::enumType,   camp::mapType<T>()));}
    static T from(const camp::UserObject& source) {return source.get<T>();}

    static camp::TypeInfo typeInfo() { return &camp::classByType<T>(); }
};

/*
 * Specialization of ValueMapper for abstract types
 */
template <typename T>
struct ValueMapper<T, typename boost::enable_if<boost::is_abstract<T> >::type>
{
    static const int type = camp::userType;
    static camp::UserObject to(const T& source) {return camp::UserObject(source);}

    static camp::TypeInfo typeInfo() { return &camp::classByType<T>(); }
};

/*
 * Specialization of ValueMapper for booleans
 */
template <>
struct ValueMapper<bool>
{
    static const int type = camp::boolType;
    static bool to(bool source) {return source;}

    static bool from(bool source)                    {return source;}
    static bool from(long source)                    {return source != 0;}
    static bool from(double source)                  {return source != 0.;}
    static bool from(const std::string& source)      {return boost::lexical_cast<bool>(source);}
    static bool from(const camp::EnumObject& source) {return source.value() != 0;}
    static bool from(const camp::UserObject& source) {return source.pointer() != 0;}

    static camp::TypeInfo typeInfo() { return camp::boolType; }
};

/*
 * Specialization of ValueMapper for unsigned char
 */
template <>
struct ValueMapper<unsigned char>
{
    static const int type = camp::intType;
    static long to(unsigned char source) {return static_cast<unsigned char>(source);}

    static unsigned char from(bool source)                    {return static_cast<unsigned char>(source);}
    static unsigned char from(long source)                    {return static_cast<unsigned char>(source);}
    static unsigned char from(double source)                  {return static_cast<unsigned char>(source);}
    // Lexical cast to unsigned short to make sure it doesn't cast to a character, but the actual number.
    static unsigned char from(const std::string& source)      {return static_cast<unsigned char>(boost::lexical_cast<unsigned short>(source));}
    static unsigned char from(const camp::EnumObject& source) {return static_cast<unsigned char>(source.value());}
    static unsigned char from(const camp::UserObject&)        {CAMP_ERROR(camp::BadType(camp::userType, camp::intType));}

    static camp::TypeInfo typeInfo() { return camp::intType; }
};

/*
 * Specialization of ValueMapper for integers
 */
template <typename T>
struct ValueMapper<T, typename boost::enable_if_c<boost::is_integral<T>::value
                                                  && !boost::is_const<T>::value // to avoid conflict with ValueMapper<const T>
                                                  && !boost::is_reference<T>::value // to avoid conflict with ValueMapper<T&>
                                                 >::type>
{
    static const int type = camp::intType;
    static long to(T source) {return static_cast<long>(source);}

    static T from(bool source)                    {return static_cast<T>(source);}
    static T from(long source)                    {return static_cast<T>(source);}
    static T from(double source)                  {return static_cast<T>(source);}
    static T from(const std::string& source)      {return boost::lexical_cast<T>(source);}
    static T from(const camp::EnumObject& source) {return static_cast<T>(source.value());}
    static T from(const camp::UserObject&)        {CAMP_ERROR(camp::BadType(camp::userType, camp::intType));}

    static camp::TypeInfo typeInfo() { return camp::intType; }
};

/*
 * Specialization of ValueMapper for reals
 */
template <typename T>
struct ValueMapper<T, typename boost::enable_if_c<boost::is_float<T>::value
                                                  && !boost::is_const<T>::value // to avoid conflict with ValueMapper<const T>
                                                  && !boost::is_reference<T>::value // to avoid conflict with ValueMapper<T&>
                                                 >::type>
{
    static const int type = camp::realType;
    static double to(T source) {return static_cast<double>(source);}

    static T from(bool source)                    {return static_cast<T>(source);}
    static T from(long source)                    {return static_cast<T>(source);}
    static T from(double source)                  {return static_cast<T>(source);}
    static T from(const std::string& source)      {return boost::lexical_cast<T>(source);}
    static T from(const camp::EnumObject& source) {return static_cast<T>(source.value());}
    static T from(const camp::UserObject&)        {CAMP_ERROR(camp::BadType(camp::userType, camp::realType));}

    static camp::TypeInfo typeInfo() { return camp::realType; }
};

/*
 * Specialization of ValueMapper for std::string
 */
template <>
struct ValueMapper<std::string>
{
    static const int type = camp::stringType;
#ifdef CAMP_COPY_STRING
    static std::string to(const std::string& source) {return source;}
#else
    static const std::string& to(const std::string& source) {return source;}
#endif

    static std::string from(bool source)                    {return boost::lexical_cast<std::string>(source);}
    static std::string from(long source)                    {return boost::lexical_cast<std::string>(source);}
    static std::string from(double source)                  {return boost::lexical_cast<std::string>(source);}
    static std::string from(const std::string& source)      {return source;}
    static std::string from(const camp::EnumObject& source) {return source.name();}
    static std::string from(const camp::UserObject&)        {CAMP_ERROR(camp::BadType(camp::userType, camp::stringType));}

    static camp::TypeInfo typeInfo() { return camp::stringType; }
};

/*
 * Specialization of ValueMapper for arrays.
 * No conversion allowed, only type mapping is provided.
 *
 * Warning: special case for char[] and const char[], they are strings not arrays
 */
template <typename T>
struct ValueMapper<T, typename boost::enable_if_c<camp::detail::IsArray<T>::value
                                                  && !boost::is_same<typename camp_ext::ArrayMapper<T>::ElementType, char>::value
                                                  && !boost::is_same<typename camp_ext::ArrayMapper<T>::ElementType, const char>::value
                                                 >::type>
{
    static const int type = camp::arrayType;

    static camp::TypeInfo typeInfo() 
    { 
        camp::ArrayType arrayType;
        arrayType.m_elementtype = ValueMapper<typename ArrayMapper<T>::ElementType>::typeInfo();
        return arrayType; 
    }
};

/*
 * Specializations of ValueMapper for char arrays.
 * Conversion to char[N] is disabled (can't return an array).
 */
template <int N>
struct ValueMapper<char[N]>
{
    static const int type = camp::stringType;
    static std::string to(const char source[N]) {return std::string(source);}

    static camp::TypeInfo typeInfo() { return camp::stringType; }
};
template <int N>
struct ValueMapper<const char[N]>
{
    static const int type = camp::stringType;
    static std::string to(const char source[N]) {return std::string(source);}

    static camp::TypeInfo typeInfo() { return camp::stringType; }
};

/*
 * Specialization of ValueMapper for enum types
 */
template <typename T>
struct ValueMapper<T, typename boost::enable_if_c<boost::is_enum<T>::value
                                                  && !boost::is_const<T>::value // to avoid conflict with ValueMapper<const T>
                                                  && !boost::is_reference<T>::value // to avoid conflict with ValueMapper<T&>
                                                 >::type>
{
    static const int type = camp::enumType;
    static camp::EnumObject to(T source) {return camp::EnumObject(source);}

    static T from(bool source)                    {return static_cast<T>(static_cast<long>(source));}
    static T from(long source)                    {return static_cast<T>(source);}
    static T from(double source)                  {return static_cast<T>(static_cast<long>(source));}
    static T from(const camp::EnumObject& source) {return static_cast<T>(source.value());}
    static T from(const camp::UserObject&)        {CAMP_ERROR(camp::BadType(camp::userType, camp::enumType));}

    // The string -> enum conversion involves a little more work:
    // we try two different conversions (as a name and as a value)
    static T from(const std::string& source)
    {
        // Get the metaenum of T, if any
        const camp::Enum* metaenum = camp::enumByTypeSafe<T>();

        // First try as a name
        if (metaenum && metaenum->hasName(source))
            return static_cast<T>(metaenum->value(source));

        // Then try as a number
        long value = boost::lexical_cast<long>(source);
        if (!metaenum || metaenum->hasValue(value))
            return static_cast<T>(value);

        // Not a valid enum name or number: throw an error
        CAMP_ERROR(camp::BadType(camp::stringType, camp::enumType));
    }

    static camp::TypeInfo typeInfo() { return &camp::enumByType<T>(); }
};

/*
 * Specialization of ValueMapper for EnumObject
 */
template <>
struct ValueMapper<camp::EnumObject>
{
    static const int type = camp::enumType;
    static const camp::EnumObject& to(const camp::EnumObject& source) {return source;}
    static const camp::EnumObject& from(const camp::EnumObject& source) {return source;}

    static camp::EnumObject from(bool)                    {CAMP_ERROR(camp::BadType(camp::boolType,   camp::enumType));}
    static camp::EnumObject from(long)                    {CAMP_ERROR(camp::BadType(camp::intType,    camp::enumType));}
    static camp::EnumObject from(double)                  {CAMP_ERROR(camp::BadType(camp::realType,   camp::enumType));}
    static camp::EnumObject from(const std::string&)      {CAMP_ERROR(camp::BadType(camp::stringType, camp::enumType));}
    static camp::EnumObject from(const camp::UserObject&) {CAMP_ERROR(camp::BadType(camp::userType,   camp::enumType));}
};

/*
 * Specialization of ValueMapper for UserObject
 */
template <>
struct ValueMapper<camp::UserObject>
{
    static const int type = camp::userType;
    static const camp::UserObject& to(const camp::UserObject& source) {return source;}
    static const camp::UserObject& from(const camp::UserObject& source) {return source;}

    static camp::UserObject from(bool)                    {CAMP_ERROR(camp::BadType(camp::boolType,   camp::userType));}
    static camp::UserObject from(long)                    {CAMP_ERROR(camp::BadType(camp::intType,    camp::userType));}
    static camp::UserObject from(double)                  {CAMP_ERROR(camp::BadType(camp::realType,   camp::userType));}
    static camp::UserObject from(const std::string&)      {CAMP_ERROR(camp::BadType(camp::stringType, camp::userType));}
    static camp::UserObject from(const camp::EnumObject&) {CAMP_ERROR(camp::BadType(camp::enumType,   camp::userType));}
};

/*
 * Specialization of ValueMapper for const T& -- just forward to ValueMapper<T>
 */
template <typename T>
struct ValueMapper<const T&> : public ValueMapper<T>
{
};

/*
 * Specialization of ValueMapper for const T -- just forward to ValueMapper<T>
 */
template <typename T>
struct ValueMapper<const T> : public ValueMapper<T>
{
};

/*
 * Specialization of ValueMapper for void.
 * Conversion to void should never happen, the only aim of this
 * specialization is to define the proper type mapping.
 */
template <>
struct ValueMapper<void>
{
    static const int type = camp::noType;

    static camp::TypeInfo typeInfo() { return camp::noType; }
};

/*
 * Specialization of ValueMapper for NoType.
 * Conversion to NoType should never happen, the only aim of this
 * specialization is to define the proper mapped type.
 */
template <>
struct ValueMapper<camp::NoType>
{
    static const int type = camp::noType;

    static camp::TypeInfo typeInfo() { return camp::noType; }
};

/*
 * Specialization of ValueMapper for non-const references.
 * Conversions to non-const references are disabled (can't return a temporary by reference)
 */
template <typename T>
struct ValueMapper<T&> : public ValueMapper<T>
{
    template <typename U>
    static T& from(const U&)
    {
        // If you get this error, it means you're trying to cast
        // a camp::Value to a non-const reference type, which is not allowed
        return U::CONVERSION_TO_NON_CONST_REFERENCE_IS_NOT_ALLOWED();
    }
};

/*
 * Specialization of ValueMapper for const char*.
 * Conversions to const char* are disabled (can't return a pointer to a temporary)
 */
template <>
struct ValueMapper<const char*>
{
    static const int type = camp::stringType;
    static std::string to(const char* source) {return std::string(source);}

    template <typename T>
    static const char* from(const T&)
    {
        // If you get this error, it means you're trying to cast
        // a camp::Value to a const char*, which is not allowed
        return T::CONVERSION_TO_CONST_CHAR_PTR_IS_NOT_ALLOWED();
    }
};

} // namespace camp_ext

#endif // CAMP_VALUEMAPPER_HPP
