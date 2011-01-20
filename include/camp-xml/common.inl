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
                    // Add a new XML node for each array element
                    typename Proxy::NodeType item = Proxy::addChild(child, "item");
                    if (Proxy::isValid(item))
                    {
                        if (arrayProperty.elementType() == userType)
                        {
                            // The array elements are composed objects: serialize them recursively
                            serialize<Proxy>(arrayProperty.get(object, j).to<UserObject>(), item, tag, 
                                include, throwExceptions);
                        }
                        else
                        {
                            // The array elements are simple properties: write them as the text of their XML node
                            Proxy::setText(item, arrayProperty.get(object, j));
                        }
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
                    // Add a new XML node for each dictionary element
                    typename Proxy::NodeType item = Proxy::addChild(child, "item");
                    if (Proxy::isValid(item))
                    {
                        // Serialize element key
                        typename Proxy::NodeType key = Proxy::addChild(item, "key");
                        if (Proxy::isValid(key))
                        {
                            if (dictionaryProperty.keyType() == userType)
                            {
                                // The dictionary keys are composed objects: serialize them recursively
                                serialize<Proxy>(j->key().to<UserObject>(), key, tag, include, 
                                    throwExceptions);
                            }
                            else
                            {
                                // The dictionary keys are simple properties: write them as the text of their XML node
                                Proxy::setText(key, j->key());
                            }
                        }

                        // Serialize element value
                        typename Proxy::NodeType value = Proxy::addChild(item, "value");
                        if (Proxy::isValid(value))
                        {
                            if (dictionaryProperty.elementType() == userType)
                            {
                                // The dictionary elements are composed objects: serialize them recursively
                                serialize<Proxy>(j->value().to<UserObject>(), value, tag, include, 
                                    throwExceptions);
                            }
                            else
                            {
                                // The dictionary elements are simple properties: write them as the text of their XML node
                                Proxy::setText(value, j->value());
                            }
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
	            // The current property is a composed type: deserialize it recursively
	            deserialize<Proxy>(property.get(object).to<UserObject>(), child, tag, include, 
                    throwExceptions);
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
	                    // The array elements are composed objects: deserialize them recursively
	                    deserialize<Proxy>(arrayProperty.get(object, index).to<UserObject>(), item, 
                            tag, include, throwExceptions);
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
	                        UserObject keyObject = dictionaryProperty.keyClass().construct();
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
	
	                        // The array elements are composed objects: deserialize them recursively
	                        deserialize<Proxy>(dictionaryProperty.get(object, keyValue).to<UserObject>(), value, tag, 
	                            include, throwExceptions);
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

} // namespace detail

} // namespace xml

} // namespace camp
