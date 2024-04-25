/*
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MAPPING_GENERIC_PROPERTY_ACCESSOR_H
#define MAPPING_GENERIC_PROPERTY_ACCESSOR_H
#include "FDArray.h"
#include "FDElement.h"
#include "FDEnumeration.h"
#include "FDMap.h"
#include "FDMapper.h"
#include "FDStruct.h"
#include "FDTypedef.h"
#include "FDUnion.h"
#include "GenericPropertyAccessor.h"

#include <map>
namespace BstIdl
{
class MappingGenericPropertyAccessor : public GenericPropertyAccessor
{
protected:
    std::shared_ptr<FDMapper> m_mapper;

public:
    MappingGenericPropertyAccessor(const std::shared_ptr<FDSpecification> &spec,
                                   const std::shared_ptr<FDMapper> &mapper)
        : GenericPropertyAccessor(spec), m_mapper(mapper)
    {
    }
    MappingGenericPropertyAccessor() = default;
    virtual ~MappingGenericPropertyAccessor() = default;
    MappingGenericPropertyAccessor(const MappingGenericPropertyAccessor &) = default;
    MappingGenericPropertyAccessor(MappingGenericPropertyAccessor &&) noexcept = default;
    MappingGenericPropertyAccessor &operator=(const MappingGenericPropertyAccessor &) = default;
    MappingGenericPropertyAccessor &operator=(MappingGenericPropertyAccessor &&) = default;

    bool getBoolean(const std::shared_ptr<BstIdl::FObject> &obj, std::string property, bool &isOK)
    {
        return GenericPropertyAccessor::getBoolean(getFDElement(obj), property, isOK);
    }

    std::list<bool> getBooleanArray(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getBooleanArray(getFDElement(obj), property);
    }

    int getInteger(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getInteger(getFDElement(obj), property);
    }

    std::list<int> getIntegerArray(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getIntegerArray(getFDElement(obj), property);
    }

    std::string getString(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getString(getFDElement(obj), property);
    }

    std::list<std::string> getStringArray(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getStringArray(getFDElement(obj), property);
    }

    std::shared_ptr<BstIdl::FInterface> getInterface(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getInterface(getFDElement(obj), property);
    }

    std::list<std::shared_ptr<BstIdl::FInterface>> getInterfaceArray(const std::shared_ptr<BstIdl::FObject> &obj,
                                                                     std::string property)
    {
        return GenericPropertyAccessor::getInterfaceArray(getFDElement(obj), property);
    }

    std::string getEnum(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getEnum(getFDElement(obj), property);
    }

    std::list<std::string> getEnumArray(const std::shared_ptr<BstIdl::FObject> &obj, std::string property)
    {
        return GenericPropertyAccessor::getEnumArray(getFDElement(obj), property);
    }

    std::shared_ptr<FDElement> createDummyFDEelement(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        std::shared_ptr<FDElement> el = nullptr;
        if (std::dynamic_pointer_cast<BstIdl::FAttribute>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDAttribute>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FAttribute>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FMethod>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDMethod>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FMethod>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FBroadcast>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDBroadcast>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FBroadcast>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FArgument>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDArgument>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FArgument>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FArrayType>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDArray>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FArrayType>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FStructType>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDStruct>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FStructType>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FUnionType>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDUnion>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FUnionType>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FField>(obj) != nullptr)
        {
            auto f = std::dynamic_pointer_cast<BstIdl::FField>(obj);
            el = std::make_shared<FDField>();
            std::dynamic_pointer_cast<FDField>(el)->setTarget(f);

            // for fields, we have to embed the dummy object into a dummy parent,
            // this is needed to indicate if it is a struct or union field
            std::shared_ptr<FDCompound> parent;
            parent->setFields(std::dynamic_pointer_cast<FDField>(el));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FEnumerationType>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDEnumeration>(el)->setTarget(
                std::dynamic_pointer_cast<BstIdl::FEnumerationType>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FEnumerator>(obj) != nullptr)
        {
            std::dynamic_pointer_cast<FDEnumValue>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FEnumerator>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FMapType>(obj) != nullptr)
        {
            el = std::make_shared<FDMap>();
            std::dynamic_pointer_cast<FDMap>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FMapType>(obj));
        }
        else if (std::dynamic_pointer_cast<BstIdl::FTypeDef>(obj) != nullptr)
        {
            el = std::make_shared<FDTypedef>();
            std::dynamic_pointer_cast<FDTypedef>(el)->setTarget(std::dynamic_pointer_cast<BstIdl::FTypeDef>(obj));
        }
        return el;
    }

    /**
     * Map an element of a Franca IDL model to the corresponding deployment element (if any).
     *
     * @param obj  the element of the Franca model
     * @return the actual mapping or null (if no mapping available)
     */
    std::shared_ptr<FDElement> getFDElement(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        std::shared_ptr<FDElement> elem = m_mapper->getFDElement(obj);
        if (elem == nullptr)
            // just to get a default value if any configured
            elem = std::make_shared<FDElement>();
        return elem;
    }
};
} // namespace BstIdl
#endif
