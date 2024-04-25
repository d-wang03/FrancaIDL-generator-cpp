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
#ifndef F_D_GENERIC_PROPERTY_ACCESSOR_H
#define F_D_GENERIC_PROPERTY_ACCESSOR_H
#include "FDBoolean.h"
#include "FDComplexValue.h"
#include "FDElement.h"
#include "FDEnumerator.h"
#include "FDGeneric.h"
#include "FDInteger.h"
#include "FDInterfaceRef.h"
#include "FDProperty.h"
#include "FDPropertyDecl.h"
#include "FDPropertyFlag.h"
#include "FDSpecification.h"
#include "FDString.h"
#include "FDTypeOverwrites.h"
#include "FDValue.h"
#include "FDValueArray.h"
#include "model/FInterface.h"
namespace BstIdl
{
class GenericPropertyAccessor
{
private:
    std::shared_ptr<FDSpecification> m_spec;

    std::shared_ptr<FDValue> getSingleValue(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        std::shared_ptr<FDComplexValue> val = getValue(elem, property);
        if (val != nullptr && val->isSingle())
        {
            return val->getSingle();
        }

        return nullptr;
    }

    std::shared_ptr<FDValueArray> getValueArray(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        std::shared_ptr<FDComplexValue> val = getValue(elem, property);
        if (val != nullptr && val->isArray())
        {
            return val->getArray();
        }

        return nullptr;
    }

    std::shared_ptr<FDComplexValue> getValue(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        return elem ? elem->getValue(property) : nullptr;
    }
    bool isOverwrite(std::shared_ptr<FDElement> elem)
    {
        if (std::dynamic_pointer_cast<FDTypeOverwrites>(elem) != nullptr)
        {
            return true;
        }
        return false;
    }

public:
    GenericPropertyAccessor() = default;
    GenericPropertyAccessor(const std::shared_ptr<FDSpecification> &spec) : m_spec(spec)
    {
    }
    virtual ~GenericPropertyAccessor() = default;
    GenericPropertyAccessor(const GenericPropertyAccessor &) = default;
    GenericPropertyAccessor(GenericPropertyAccessor &&) noexcept = default;
    GenericPropertyAccessor &operator=(const GenericPropertyAccessor &) = default;
    GenericPropertyAccessor &operator=(GenericPropertyAccessor &&) = default;

    bool getBoolean(const std::shared_ptr<FDElement> &elem, std::string property, bool &isOK)
    {
        auto val = getSingleValue(elem, property);
        if (val != nullptr && std::dynamic_pointer_cast<FDBoolean>(val))
        {
            isOK = true;
            return std::dynamic_pointer_cast<FDBoolean>(val)->getValue() == "true";
        }
        isOK = false;
        return false;
    }

    std::list<bool> getBooleanArray(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto valarray = getValueArray(elem, property);
        if (valarray == nullptr)
            return std::list<bool>();

        std::list<bool> vals;
        for (std::shared_ptr<FDValue> v : valarray->getValues())
        {
            if (std::dynamic_pointer_cast<FDBoolean>(v))
            {
                vals.push_back(std::dynamic_pointer_cast<FDBoolean>(v)->getValue() == "true");
            }
            else
            {
                return std::list<bool>();
            }
        }
        return vals;
    }

    int getInteger(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto val = getSingleValue(elem, property);
        if (val != nullptr && std::dynamic_pointer_cast<FDInteger>(val) != nullptr)
        {
            return std::dynamic_pointer_cast<FDInteger>(val)->getValue();
        }
        return -1;
    }

    std::list<int> getIntegerArray(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto valarray = getValueArray(elem, property);
        if (valarray == nullptr)
            return std::list<int>();

        std::list<int> vals;
        for (auto v : valarray->getValues())
        {
            if (std::dynamic_pointer_cast<FDInteger>(v) != nullptr)
            {
                vals.push_back(std::dynamic_pointer_cast<FDInteger>(v)->getValue());
            }
            else
            {
                return std::list<int>();
            }
        }
        return vals;
    }

    std::string getString(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto val = getSingleValue(elem, property);
        if (val != nullptr && std::dynamic_pointer_cast<FDString>(val) != nullptr)
        {
            return std::dynamic_pointer_cast<FDString>(val)->getValue();
        }
        return std::string("");
    }

    std::list<std::string> getStringArray(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto valarray = getValueArray(elem, property);
        if (valarray == nullptr)
            return std::list<std::string>();

        std::list<std::string> vals;
        for (auto v : valarray->getValues())
        {
            if (std::dynamic_pointer_cast<FDString>(v) != nullptr)
            {
                vals.push_back(std::dynamic_pointer_cast<FDString>(v)->getValue());
            }
            else
            {
                return std::list<std::string>();
            }
        }
        return vals;
    }

    std::shared_ptr<BstIdl::FInterface> getInterface(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto val = getSingleValue(elem, property);
        if (val != nullptr && std::dynamic_pointer_cast<FDInterfaceRef>(val) != nullptr)
        {
            return std::dynamic_pointer_cast<FDInterfaceRef>(val)->getTarget();
        }
        return nullptr;
    }

    std::list<std::shared_ptr<BstIdl::FInterface>> getInterfaceArray(const std::shared_ptr<FDElement> &elem,
                                                                     std::string property)
    {
        auto valarray = getValueArray(elem, property);
        if (valarray == nullptr)
            return std::list<std::shared_ptr<BstIdl::FInterface>>();

        std::list<std::shared_ptr<BstIdl::FInterface>> vals;
        for (auto v : valarray->getValues())
        {
            if (std::dynamic_pointer_cast<FDInterfaceRef>(v) != nullptr)
            {
                vals.push_back(std::dynamic_pointer_cast<FDInterfaceRef>(v)->getTarget());
            }
            else
            {
                return std::list<std::shared_ptr<BstIdl::FInterface>>();
            }
        }
        return vals;
    }

    std::string getEnum(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto val = getSingleValue(elem, property);
        if (val != nullptr && isEnumerator(val))
        {
            return getEnumerator(val)->getName();
        }
        return std::string();
    }

    bool isEnumerator(const std::shared_ptr<FDValue> &val)
    {
        if (auto ptr = std::dynamic_pointer_cast<FDGeneric>(val))
        {
            auto vgen = ptr->getValue();
            return std::dynamic_pointer_cast<FDEnumerator>(vgen) != nullptr;
        }
        return false;
    }

    std::shared_ptr<FDEnumerator> getEnumerator(const std::shared_ptr<FDValue> &val)
    {
        if (auto ptr = std::dynamic_pointer_cast<FDGeneric>(val))
        {
            auto vgen = ptr->getValue();
            return std::dynamic_pointer_cast<FDEnumerator>(vgen);
        }
        return nullptr;
    }

    std::list<std::string> getEnumArray(const std::shared_ptr<FDElement> &elem, std::string property)
    {
        auto valarray = getValueArray(elem, property);
        if (valarray == nullptr)
            return std::list<std::string>();

        std::list<std::string> vals;
        for (auto v : valarray->getValues())
        {
            if (isEnumerator(v))
            {
                auto e = getEnumerator(v);
                vals.push_back(e->getName());
            }
            else
            {
                return std::list<std::string>();
            }
        }
        return vals;
    }

    std::shared_ptr<FDComplexValue> getDefault(const std::shared_ptr<FDPropertyDecl> &decl)
    {
        for (auto flag : decl->getFlags())
        {
            if (flag->getDefault() != nullptr)
            {
                return flag->getDefault();
            }
        }
        return nullptr;
    }

    /**
     * Returns the belonging property declaration of a deployment element.
     *
     * @param property
     * @return
     */
    std::shared_ptr<FDPropertyDecl> getPropertyDecl(const std::shared_ptr<FDObject> &property)
    {
        auto current = property;

        while (current != nullptr && std::dynamic_pointer_cast<FDPropertyDecl>(current) != nullptr)
            current = current->getContainer();
        return std::dynamic_pointer_cast<FDPropertyDecl>(current);
    }

    /**
     * Checks if the EObject is part of a deployment specification.
     *
     * @param property
     * @return
     */
    bool isSpecification(const std::shared_ptr<BstIdl::FDObject> &property)
    {
        auto current = property;

        while (current != nullptr)
        {
            if (std::dynamic_pointer_cast<FDPropertyDecl>(current) != nullptr)
                return true;
            if (std::dynamic_pointer_cast<FDProperty>(current) != nullptr)
                return false;
            current = current->getContainer();
        }
        return false;
    }

    /**
     * Checks if the EObject is the default value for its belonging property.
     *
     * @param element
     * @return true if the EObject is the default value
     */
    bool isDefault(const std::shared_ptr<FDObject> &element)
    {
        auto decl = getPropertyDecl(element);
        auto value = getDefault(decl);

        if (value != nullptr && value->getSingle() != nullptr)
        {
            auto single = value->getSingle();
            if (isEnumerator(single))
                return getEnumerator(single) == element;
            else
                return single == element;
        }
        return false;
    }
};
} // namespace BstIdl
#endif