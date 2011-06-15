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

namespace camp
{
namespace xml
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename Proxy>
void serialize(const UserObject& object, typename Proxy::NodeType node, const Value& tag, 
    bool include, bool throwExceptions)
{
    // Iterate over the object's properties using its metaclass
    const Class& metaclass = object.getClass();
    for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
    {
        try
        {
            const Property& property = metaclass.property(i);

            // If the property has the exclude tag, ignore it
            if ((tag != Value::nothing) && include?!property.hasTag(tag):property.hasTag(tag))
                continue;

            // Create a child node for the new property
            typename Proxy::NodeType child = Proxy::addChild(node, property.name());
            if (!Proxy::isValid(child))
                continue;

            if (property.type() == userType)
            {
                if(!querySerialize(property.get(object), tag)) continue;

                // The current property is a composed type: serialize it recursively
                serialize<Proxy>(property.get(object).to<UserObject>(), child, tag, include, 
                    throwExceptions);
            }
            else if (property.type() == arrayType)
            {
                // The current property is an array
                const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);

                // Iterate over the array elements
                std::size_t count = arrayProperty.size(object);
                for (std::size_t j = 0; j < count; ++j)
                {
                    if(!querySerialize(arrayProperty.get(object, j), tag)) continue;

                    // Add a new XML node for each array element
                    typename Proxy::NodeType item = Proxy::addChild(child, "item");
                    if (Proxy::isValid(item))
                    {
                        serializeValue<Proxy>(arrayProperty.get(object, j), arrayProperty.elementType(), 
                            item, tag, include, throwExceptions);
                    }
                }
            }
            else if (property.type() == dictionaryType)
            {
                // The current property is a dictionary
                const DictionaryProperty& dictionaryProperty = static_cast<const DictionaryProperty&>(property);

                // Iterate over the dictionary elements
                DictionaryIteratorPtr j = dictionaryProperty.iterator(object);
                while(j->valid())
                {
                    if(!querySerialize(j->value(), tag)) 
                    {
                        j->next();
                        continue;
                    }

                    // Add a new XML node for each dictionary element
                    typename Proxy::NodeType item = Proxy::addChild(child, "item");
                    if (Proxy::isValid(item))
                    {
                        // Serialize element key
                        typename Proxy::NodeType key = Proxy::addChild(item, "key");
                        if (Proxy::isValid(key))
                        {
                            serializeValue<Proxy>(j->key(), dictionaryProperty.keyType(), key, 
                                tag, include, throwExceptions);
                        }

                        // Serialize element value
                        typename Proxy::NodeType value = Proxy::addChild(item, "value");
                        if (Proxy::isValid(value))
                        {
                            serializeValue<Proxy>(j->value(), dictionaryProperty.elementType(), value, 
                                tag, include, throwExceptions);
                        }
                    }

                    j->next();
                }
            }
            else
            {
                // The current property is a simple property: write its value as the node's text
                Proxy::setText(child, property.get(object));
            }
        }
        catch(camp::Error e)
        {
        	if (throwExceptions) throw e;
        }
    }
}

template <typename Proxy>
inline void serializeErasureValue(const Value& value, typename Proxy::NodeType node, 
    const Value& tag, bool include, bool throwExceptions)
{
    switch(value.type())
    {
        case boolType:
        {
            typename Proxy::NodeType type = Proxy::addChild(node, "bool");
            if (Proxy::isValid(type)) Proxy::setText(type, value);
            break;
        }
        case intType:
        {
            typename Proxy::NodeType type = Proxy::addChild(node, "int");
            if (Proxy::isValid(type)) Proxy::setText(type, value);
            break;
        }
        case realType:
        {
            typename Proxy::NodeType type = Proxy::addChild(node, "real");
            if (Proxy::isValid(type)) Proxy::setText(type, value);
            break;
        }
        case stringType:
        {
            typename Proxy::NodeType type = Proxy::addChild(node, "string");
            if (Proxy::isValid(type)) Proxy::setText(type, value);
            break;
        }
        case enumType:
        {
            typename Proxy::NodeType type = Proxy::addChild(node, "enum");
            if (Proxy::isValid(type)) 
            {
                const Enum& metaenum = value.to<EnumObject>().getEnum();
                typename Proxy::NodeType name = Proxy::addChild(type, metaenum.name());
                if (Proxy::isValid(name)) Proxy::setText(name, value);
            }
            break;
        }
        case userType:
        {
            const UserObject& object = value.to<UserObject>();
            typename Proxy::NodeType type = Proxy::addChild(node, "object");
            if (Proxy::isValid(type)) 
            {
                typename Proxy::NodeType name = Proxy::addChild(type, object.getClass().name());
                if (Proxy::isValid(name)) serialize<Proxy>(object, name, tag, include, 
                    throwExceptions);
            }
            break;
        }
    }
}

inline bool querySerialize(const Value& value, const Value& tag)
{
    switch(value.type())
    {
        case userType:
            UserObject object = value.to<UserObject>();
            const Class& metaclass = object.getClass();
            if(!metaclass.hasTag(tag)) return true;
            return !metaclass.tag(tag, object).to<bool>();
    }

    return true;
}

template <typename Proxy>
inline void serializeValue(const Value& value, Type type, typename Proxy::NodeType node, 
    const Value& tag, bool include, bool throwExceptions)
{
    switch(type)
    {
        // The dictionary keys are composed objects: serialize them recursively
        case userType:
            // If the property has the exclude tag, ignore it
            serialize<Proxy>(value.to<UserObject>(), node, tag, include, throwExceptions);
            break;
        // The dictionary keys are type erased values: serialize them and add type info
        case valueType:
            serializeErasureValue<Proxy>(value, node, tag, include, throwExceptions);
            break;
        // The dictionary keys are simple properties: write them as the text of their XML node
        default:
            Proxy::setText(node, value);
            break;
    }
}

//-------------------------------------------------------------------------------------------------
template <typename Proxy>
void deserialize(const UserObject& object, typename Proxy::NodeType node, const Value& tag,
    bool include, bool throwExceptions)
{
    // Iterate over the object's properties using its metaclass
    const Class& metaclass = object.getClass();
    for (std::size_t i = 0; i < metaclass.propertyCount(); ++i)
    {
        try
        {
	        const Property& property = metaclass.property(i);
	
	        // If the property has the exclude tag, ignore it
	        if ((tag != Value::nothing) && include?!property.hasTag(tag):property.hasTag(tag))
	            continue;
	
	        // Find the child node corresponding to the new property
	        typename Proxy::NodeType child = Proxy::findFirstChild(node, property.name());
	        if (!Proxy::isValid(child))
	            continue;
	
	        if (property.type() == userType)
	        {
                if(property.hasTag("DeserializeCopySet"))
                {
                    UserObject userObject = property.get(object).to<UserObject>();

                    // The current property is a composed type: deserialize it recursively
                    deserialize<Proxy>(userObject, child, tag, include, throwExceptions);

                    // Set the recursively deserialized user object
                    property.set(object, userObject);
                }
                else
                {
	                // The current property is a composed type: deserialize it recursively
	                deserialize<Proxy>(property.get(object).to<UserObject>(), child, tag, include, 
                        throwExceptions);

                    // Re set the property if it has the DeserializeReSet tag
                    if(property.hasTag("DeserializeReSet"))
                    {
                        Value val = property.get(object);
                        property.set(object, val);
                    }
                }
	        }
	        else if (property.type() == arrayType)
	        {
	            // The current property is an array
	            const ArrayProperty& arrayProperty = static_cast<const ArrayProperty&>(property);
	
	            // Iterate over the child XML node and extract all the array elements
	            std::size_t index = 0;
	            for (typename Proxy::NodeType item = Proxy::findFirstChild(child, "item")
	                ; Proxy::isValid(item)
	                ; item = Proxy::findNextSibling(item, "item"))
	            {
	                // Make sure that there are enough elements in the array
	                std::size_t count = arrayProperty.size(object);
	                if (index >= count)
	                {
	                    if (arrayProperty.dynamic())
	                        arrayProperty.resize(object, index + 1);
	                    else
	                        break;
	                }
	
	                if (arrayProperty.elementType() == userType)
	                {
                        if(property.hasTag("DeserializeCopySet"))
                        {
                            UserObject userObject = arrayProperty.get(object, index).to<UserObject>();

                            // The array elements are composed objects: deserialize them recursively
                            deserialize<Proxy>(userObject, child, tag, include, throwExceptions);

                            // Set the recursively deserialized user object
                            arrayProperty.set(object, index, userObject);
                        }
                        else
                        {
                            // The array elements are composed objects: deserialize them recursively
                            deserialize<Proxy>(arrayProperty.get(object, index).to<UserObject>(), 
                                item, tag, include, throwExceptions);

                            // Re set the property if it has the DeserializeReSet tag
                            if(property.hasTag("DeserializeReSet"))
                            {
                                Value val = arrayProperty.get(object, index);
                                arrayProperty.set(object, index, val);
                            }
                        }
	                }
	                else
	                {
	                    // The array elements are simple properties: read their value from the text of their XML node
	                    arrayProperty.set(object, index, Proxy::getText(item));
	                }
	
	                index++;
	            }
	        }
	        else if (property.type() == dictionaryType)
	        {
	            // The current property is a dictionary
	            const DictionaryProperty& dictionaryProperty = static_cast<const DictionaryProperty&>(property);
	
	            // Iterate over the child XML node and extract all the array elements
	            for (typename Proxy::NodeType item = Proxy::findFirstChild(child, "item")
	                ; Proxy::isValid(item)
	                ; item = Proxy::findNextSibling(item, "item"))
	            {
	                // Deserialize element key
	                Value keyValue;
	                typename Proxy::NodeType key = Proxy::findFirstChild(item, "key");
	                if (Proxy::isValid(key))
	                {
	                    if (dictionaryProperty.keyType() == userType)
	                    {
	                        // The dictionary keys are composed objects: deserialize them recursively
	                        UserObject keyObject = boost::get<const camp::Class*>(dictionaryProperty.keyTypeInfo())->construct();
	                        deserialize<Proxy>(keyObject, key, tag, include, throwExceptions);
	                        keyValue = keyObject;
	                    }
	                    else
	                    {
	                        // The dictionary keys are simple properties: read their value from the text of their XML node
	                        keyValue = Proxy::getText(key);
	                    }
	                }
	
	                // Deserialize element value
	                typename Proxy::NodeType value = Proxy::findFirstChild(item, "value");
	                if (Proxy::isValid(value))
	                {
	                    if (dictionaryProperty.elementType() == userType)
	                    {
	                        // Add key to dictionary before deserializing the value
	                        if(property.hasTag("AddFunction") && !dictionaryProperty.exists(object, keyValue))
	                        {
	                            metaclass.function(property.tag("AddFunction").to<std::string>()).call(object, keyValue);
	                        }

                            if(property.hasTag("DeserializeCopySet"))
                            {
                                UserObject userObject = dictionaryProperty.get(object, keyValue).to<UserObject>();

                                // The array elements are composed objects: deserialize them recursively
                                deserialize<Proxy>(userObject, child, tag, include, throwExceptions);

                                // Set the recursively deserialized user object
                                dictionaryProperty.set(object, keyValue, userObject);
                            }
                            else
                            {
                                // The array elements are composed objects: deserialize them recursively
                                deserialize<Proxy>(dictionaryProperty.get(object, keyValue).to<UserObject>(), value, tag, 
                                    include, throwExceptions);

                                // Re set the property if it has the DeserializeReSet tag
                                if(property.hasTag("DeserializeReSet"))
                                {
                                    Value val = dictionaryProperty.get(object, keyValue);
                                    dictionaryProperty.set(object, keyValue, val);
                                }
                            }
	                    }
                        else if (dictionaryProperty.elementType() == valueType)
                        {
                            Value val = deserializeErasureValue<Proxy>(value, tag, include, throwExceptions);

                            // Add key to dictionary before deserializing the value
                            if(property.hasTag("SetFunction"))
                            {
                                metaclass.function(property.tag("SetFunction").to<std::string>()).call(object, Args(keyValue, val));
                            }
                            else if(property.hasTag("AddFunction") && dictionaryProperty.exists(object, keyValue))
                            {
                                metaclass.function(property.tag("AddFunction").to<std::string>()).call(object, keyValue);
                            }
                            else
                            {
                                dictionaryProperty.set(object, keyValue, val);
                            }
                        }
	                    else
	                    {
	                        // The array elements are simple properties: read their value from the text of their XML node
	                        dictionaryProperty.set(object, keyValue, Proxy::getText(value));
	                    }
	                }
	            }
	        }
	        else
	        {
	            // The current property is a simple property: read its value from the node's text
	            property.set(object, Proxy::getText(child));
	        }
        }
        catch(camp::Error e)
        {
            if (throwExceptions) throw e;
        }
    }
}

template <typename Proxy>
inline Value deserializeErasureValue(typename Proxy::NodeType node, const Value& tag,
    bool include, bool throwExceptions)
{
    using namespace camp_ext;
    typename Proxy::NodeType type = Proxy::findFirstChild(node);
    
    if (Proxy::isValid(type))
    {
        std::string name = Proxy::getName(type);

        if (name == "bool")
        {
            return ValueMapper<bool>::from(Proxy::getText(type));
        }
        else if (name == "int")
        {
            return ValueMapper<int>::from(Proxy::getText(type));
        }
        else if (name == "real")
        {
            return ValueMapper<double>::from(Proxy::getText(type));
        }
        else if (name == "string")
        {
            return Proxy::getText(type);
        }
        else if (name == "enum")
        {
            typename Proxy::NodeType enumnode = Proxy::findFirstChild(type);
            if (Proxy::isValid(enumnode)) 
            {
                const Enum& metaenum = enumByName(Proxy::getName(enumnode));
                return EnumObject(metaenum.value(Proxy::getText(enumnode)), metaenum);
            }
        }
        else if (name == "object")
        {
            typename Proxy::NodeType classnode = Proxy::findFirstChild(type);
            if (Proxy::isValid(classnode)) 
            {
                const Class& metaclass = classByName(Proxy::getName(classnode));
                UserObject object = metaclass.construct();
                deserialize<Proxy>(object, classnode, tag, include, throwExceptions);
                return object;
            }
        }
    }

    return Value::nothing;
}

} // namespace detail

} // namespace xml

} // namespace camp
