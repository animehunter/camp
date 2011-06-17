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


#ifndef CAMP_CLASS_HPP
#define CAMP_CLASS_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <camp/classget.hpp>
#include <camp/classcast.hpp>
#include <camp/property.hpp>
#include <camp/function.hpp>
#include <camp/tagholder.hpp>
#include <camp/errors.hpp>
#include <camp/userobject.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/typeid.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <string>


namespace bm = boost::multi_index;

namespace camp
{
template <typename T> class ClassBuilder;
class Constructor;
class Value;
class ClassVisitor;

/**
 * \brief camp::Class represents a metaclass composed of properties and functions
 *
 * camp::Class is the main class of the CAMP API. It defines a metaclass, which
 * is an abstract representation of a C++ class with its own properties,
 * functions, constructors, base classes, etc.
 *
 * Classes are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * class MyClass
 * {
 * public:
 *
 *     MyClass();
 *     int getProp() const;
 *     void setProp(int);
 *     std::string func();
 * };
 *
 * camp::Class::declare<MyClass>("MyClass")
 *     .tag("help", "this is my class")
 *     .constructor0()
 *     .property("prop", &MyClass::getProp, &MyClass::setProp)
 *     .function("func", &MyClass::func);
 * \endcode
 *
 * It then provides a set of accessors to retrieve its member functions and properties.
 *
 * \code
 * const camp::Class& metaclass = camp::classByType<MyClass>();
 *
 * const camp::Property& prop = metaclass.property("prop");
 * const camp::Function& func = metaclass.function("func");
 * \endcode
 *
 * Another way to inspect a class, which is more type-safe, is to use a ClassVisitor.
 *
 * \code
 * MyVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 *
 * It also allows to create and destroy instances of the bound C++ class.
 *
 * \code
 * MyClass* obj = metaclass.construct<MyClass>();
 * metaclass.destroy(obj);
 * \endcode
 *
 * \remark All function and property names are unique within the metaclass.
 *
 * \sa Enum, TagHolder, ClassBuilder, Function, Property
 */
class CAMP_API Class : public TagHolder, boost::noncopyable
{
public:

    /**
     * \brief Declare a new metaclass
     *
     * This is the function to call to create a new metaclass. The template
     * parameter T is the C++ class that will be bound to the metaclass.
     *
     * \param name Name of the metaclass in CAMP. This name identifies
     *             the metaclass and thus has to be unique
     *
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     */
    template <typename T>
    static ClassBuilder<T> declare(const std::string& name);

public:

    /**
     * \brief Return the name of the metaclass
     *
     * \return String containing the name of the metaclass
     */
    const std::string& name() const;

    /**
     * \brief Return the total number of base metaclasses of this metaclass
     *
     * \return Number of base metaclasses
     */
    std::size_t baseCount() const;

    /**
     * \brief Return a base metaclass from its index
     *
     * \param index Index of the base to get
     *
     * \return Reference to the index-th base metaclass of this metaclass
     *
     * \throw OutOfRange index is out of range
     */
    const Class& base(std::size_t index) const;

    /**
     * \brief Return the total number of functions of this metaclass
     *
     * \param ownOnly If functions from base classes should be excluded. Defaults to false.
     *
     * \return Number of functions
     */
    std::size_t functionCount(bool ownOnly = false) const;

    /**
     * \brief Check if this metaclass contains the given function
     *
     * \param name Name of the function to check
     * \param ownOnly If functions from base classes should be excluded. Defaults to false.
     *
     * \return True if the function is in the metaclass, false otherwise
     */
    bool hasFunction(const std::string& name, bool ownOnly = false) const;

    /**
     * \brief Get a function from its index in this metaclass
     *
     * \param index Index of the function to get
     * \param ownOnly If functions from base classes should be excluded. Defaults to false.
     *
     * \return Reference to the function
     *
     * \throw OutOfRange index is out of range
     */
    const Function& function(std::size_t index, bool ownOnly = false) const;

    /**
     * \brief Get a function from its name
     *
     * \param name Name of the function to get (case sensitive)
     * \param ownOnly If functions from base classes should be excluded. Defaults to false.
     *
     * \return Reference to the function
     *
     * \throw FunctionNotFound \a name is not a function of the metaclass
     */
    const Function& function(const std::string& name, bool ownOnly = false) const;

    /**
     * \brief Return the total number of properties of this metaclass
     *
     * \param ownOnly If properties from base classes should be excluded. Defaults to false.
     *
     * \return Number of properties
     */
    std::size_t propertyCount(bool ownOnly = false) const;

    /**
     * \brief Check if this metaclass contains the given property
     *
     * \param name Name of the property to check
     * \param ownOnly If properties from base classes should be excluded. Defaults to false.
     *
     * \return True if the property is in the metaclass, false otherwise
     */
    bool hasProperty(const std::string& name, bool ownOnly = false) const;

    /**
     * \brief Get a property from its index in this metaclass
     *
     * \param index Index of the property to get
     * \param ownOnly If properties from base classes should be excluded. Defaults to false.
     *
     * \return Reference to the property
     *
     * \throw OutOfRange index is out of range
     */
    const Property& property(std::size_t index, bool ownOnly = false) const;

    /**
     * \brief Get a property from its name
     *
     * \param name Name of the property to get (case sensitive)
     * \param ownOnly If properties from base classes should be excluded. Defaults to false.
     *
     * \return Reference to the property
     *
     * \throw PropertyNotFound \a name is not a property of the metaclass
     */
    const Property& property(const std::string& name, bool ownOnly = false) const;

    /**
     * \brief Return the total number of constructors of this metaclass
     *
     * \return Number of constructors
     */
    std::size_t constructorCount() const;

    /**
     * \brief Get a constructor from its index in this metaclass
     *
     * \param index Index of the constructor to get
     *
     * \return Reference to the constructor
     *
     * \throw OutOfRange index is out of range
     */
    const Constructor& constructor(std::size_t index) const;

    /**
     * \brief Construct a new instance of the C++ class bound to the metaclass
     *
     * If no constructor can match the provided arguments, UserObject::nothing
     * is returned.
     * The new instance is wrapped into a UserObject. It must be destroyed
     * with the Class::destroy function.
     *
     * \param args Arguments to pass to the constructor (empty by default)
     *
     * \return New instance wrapped into a UserObject, or UserObject::nothing if it failed
     */
    UserObject construct(const Args& args = Args::empty) const;

    /**
     * \brief Destroy an instance of the C++ class bound to the metaclass
     *
     * This function must be called to destroy every instance created with
     * Class::construct.
     *
     * \param object Object to be destroyed
     */
    void destroy(const UserObject& object) const;

    /**
     * \brief Start visitation of a class
     *
     * \param visitor Visitor to use for visitation
     */
    void visit(ClassVisitor& visitor) const;

    /**
     * \brief Convert a pointer to an object to be compatible with a base or derived metaclass
     *
     * The target metaclass may be a base or a derived of this, both cases are properly handled.
     *
     * \param pointer Pointer to convert
     * \param target Target metaclass to convert to
     *
     * \return Converted pointer
     *
     * \throw ClassUnrelated \a target is not a base nor a derived class of this
     */
    void* applyOffset(void* pointer, const Class& target) const;

    /**
     * \brief Operator == to check equality between two metaclasses
     *
     * Two metaclasses are equal if their name is the same.
     *
     * \param other Metaclass to compare with this
     *
     * \return True if both metaclasses are the same, false otherwise
     */
    bool operator==(const Class& other) const;

    /**
     * \brief Operator != to check inequality between two metaclasses
     *
     * \param other Metaclass to compare with this
     *
     * \return True if metaclasses are different, false if they are equal
     */
     bool operator!=(const Class& other) const;

private:

    template <typename T> friend class ClassBuilder;
    friend class detail::ClassManager;

    /**
     * \brief Construct the metaclass from its name
     *
     * \param name Name of the metaclass
     */
    Class(const std::string& name);

    /**
     * \brief Get the offset of a base metaclass
     *
     * \param base Base metaclass to check
     *
     * \return offset between this and base, or -1 if both classes are unrelated
     */
    int baseOffset(const Class& base) const;

private:

    /**
     * \brief Structure holding informations about a base metaclass
     */
    struct BaseInfo
    {
        const Class* base;
        int offset;
    };

    typedef boost::shared_ptr<Property> PropertyPtr;
    typedef boost::shared_ptr<Function> FunctionPtr;
    typedef boost::shared_ptr<Constructor> ConstructorPtr;
    typedef std::vector<ConstructorPtr> ConstructorList;
    typedef std::vector<BaseInfo> BaseList;

    struct Id;
    struct Name;

    typedef boost::multi_index_container<PropertyPtr,
        bm::indexed_by<bm::random_access<bm::tag<Id> >,
                       bm::ordered_unique<bm::tag<Name>, bm::const_mem_fun<Property, const std::string&, &Property::name> >
        >
    > PropertyTable;

    typedef boost::multi_index_container<FunctionPtr,
        bm::indexed_by<bm::random_access<bm::tag<Id> >,
                       bm::ordered_unique<bm::tag<Name>, bm::const_mem_fun<Function, const std::string&, &Function::name> >
        >
    > FunctionTable;

    typedef PropertyTable::index<Name>::type PropertyNameIndex;
    typedef FunctionTable::index<Name>::type FunctionNameIndex;
    typedef void (*Destructor)(const UserObject&);

    std::string m_name; ///< Name of the metaclass
    FunctionTable m_functions; ///< Table of metafunctions indexed by name
    FunctionTable m_own_functions; ///< Table of metafunctions (not from bases) indexed by name
    PropertyTable m_properties; ///< Table of metaproperties indexed by name
    PropertyTable m_own_properties; ///< Table of metaproperties (not from bases) indexed by name
    BaseList m_bases; ///< List of base metaclasses
    ConstructorList m_constructors; ///< List of metaconstructors
    Destructor m_destructor; ///< Destructor (function that is able to delete an abstract object)
};

} // namespace camp

// Must be included here because of mutual dependance between Class and ClassBuilder
#include <camp/classbuilder.hpp>

#include <camp/class.inl>


#endif // CAMP_CLASS_HPP
