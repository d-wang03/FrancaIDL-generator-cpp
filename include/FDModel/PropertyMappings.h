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
#ifndef PROPERTY_MAPPINGS
#define PROPERTY_MAPPINGS
#include "FDFactory.h"
#include "FDModelManager.h"
#include "model/FArrayType.h"
#include "model/FEnumerationType.h"
#include "model/FMapType.h"
#include "model/FStructType.h"
#include "model/FType.h"
#include "model/FTypeRef.h"
#include "model/FTypedElement.h"
#include "model/FUnionType.h"
namespace BstIdl
{
class PropertyMappings
{
public:
    PropertyMappings() = default;
    virtual ~PropertyMappings() = default;
    PropertyMappings(const PropertyMappings &) = default;
    PropertyMappings(PropertyMappings &&) noexcept = default;
    PropertyMappings &operator=(const PropertyMappings &) = default;
    PropertyMappings &operator=(PropertyMappings &&) = default;

    /**
     * Check if the given list of property declarations contains at least one mandatory property.
     *
     * @param decls a list of property declarations
     * @return true if at least one of the properties is mandatory
     */
    static bool hasMandatoryProperties(const std::list<std::shared_ptr<FDPropertyDecl>> &decls)
    {
        for (auto decl : decls)
        {
            if (isMandatory(decl))
                return true;
        }
        return false;
    }
    /**
     * Check if the given property declaration is mandatory.
     *
     * Mandatory means: Not optional and no default value.
     *
     * @param decl a property declaration
     * @return true if the property is mandatory
     */
    static bool isMandatory(const std::shared_ptr<FDPropertyDecl> &decl)
    {
        auto flags = decl->getFlags();
        for (auto flag : flags)
        {
            if ((!flag->getOptional().empty()) || flag->getDefault() != nullptr)
                return false;
        }
        return true;
    }

private:
    static FDBuiltInPropertyHost::BuiltInPropertyHost getBuiltInMainHost(const std::shared_ptr<FDElement> &elem)
    {
        if (std::dynamic_pointer_cast<FDInterface>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::INTERFACES;
        }
        else if (std::dynamic_pointer_cast<FDTypes>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::TYPE_COLLECTIONS;
        }
        else if (std::dynamic_pointer_cast<FDAttribute>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::ATTRIBUTES;
        }
        else if (std::dynamic_pointer_cast<FDMethod>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::METHODS;
        }
        else if (std::dynamic_pointer_cast<FDBroadcast>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::BROADCASTS;
        }
        else if (std::dynamic_pointer_cast<FDArgument>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::ARGUMENTS;
        }
        else if (std::dynamic_pointer_cast<FDArray>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::ARRAYS;
        }
        else if (std::dynamic_pointer_cast<FDStruct>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDStructOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::STRUCTS;
        }
        else if (std::dynamic_pointer_cast<FDUnion>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDUnionOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::UNIONS;
        }
        else if (std::dynamic_pointer_cast<FDField>(elem) != nullptr)
        {
            auto field = std::dynamic_pointer_cast<FDField>(elem);
            auto &mgr = FDModelManager::getInstance();
            auto compound = mgr.getContainer(field);
            if (std::dynamic_pointer_cast<FDUnion>(compound) != nullptr)
            {
                return FDBuiltInPropertyHost::UNION_FIELDS;
            }
            else if (std::dynamic_pointer_cast<FDStruct>(compound) != nullptr)
            {
                return FDBuiltInPropertyHost::STRUCT_FIELDS;
            }
            return FDBuiltInPropertyHost::UNDEFINED;
        }
        else if (std::dynamic_pointer_cast<FDEnumeration>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::ENUMERATIONS;
        }
        else if (std::dynamic_pointer_cast<FDEnumValue>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::ENUMERATORS;
        }
        else if (std::dynamic_pointer_cast<FDMap>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDStructOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::MAPS;
        }
        else if (std::dynamic_pointer_cast<FDMapKey>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDUnionOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::MAP_KEYS;
        }
        else if (std::dynamic_pointer_cast<FDMapValue>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDStructOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::MAP_VALUES;
        }
        else if (std::dynamic_pointer_cast<FDTypedef>(elem) != nullptr ||
                 std::dynamic_pointer_cast<FDUnionOverwrites>(elem) != nullptr)
        {
            return FDBuiltInPropertyHost::TYPEDEFS;
        }
        return FDBuiltInPropertyHost::UNDEFINED;
    }
    static FDBuiltInPropertyHost::BuiltInPropertyHost getMainHost(const std::shared_ptr<BstIdl::FType> &type)
    {
        if (std::dynamic_pointer_cast<BstIdl::FArrayType>(type) != nullptr)
        {
            return FDBuiltInPropertyHost::ARRAYS;
        }
        else if (std::dynamic_pointer_cast<BstIdl::FStructType>(type) != nullptr)
        {
            return FDBuiltInPropertyHost::STRUCTS;
        }
        else if (std::dynamic_pointer_cast<BstIdl::FUnionType>(type) != nullptr)
        {
            return FDBuiltInPropertyHost::UNIONS;
        }
        else if (std::dynamic_pointer_cast<BstIdl::FMapType>(type) != nullptr)
        {
            return FDBuiltInPropertyHost::MAPS;
        }
        else if (std::dynamic_pointer_cast<BstIdl::FEnumerationType>(type) != nullptr)
        {
            return FDBuiltInPropertyHost::ENUMERATIONS;
        }
        else
            return FDBuiltInPropertyHost::UNDEFINED;
    }
};
} // namespace BstIdl
#endif