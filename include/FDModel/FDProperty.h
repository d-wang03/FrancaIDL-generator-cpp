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
#ifndef F_D_PROPERTY_H
#define F_D_PROPERTY_H
#include "FDComplexValue.h"
#include "FDPropertyDecl.h"
namespace BstIdl
{
class FDProperty : public FDObject
{
public:
    FDProperty() = default;
    virtual ~FDProperty() = default;
    FDProperty(const FDProperty &) = default;
    FDProperty(FDProperty &&) noexcept = default;
    FDProperty &operator=(const FDProperty &) = default;
    FDProperty &operator=(FDProperty &&) = default;
    std::shared_ptr<FDPropertyDecl> const getDecl()
    {
        return m_decl;
    }
    void setDecl(const std::shared_ptr<FDPropertyDecl> &newDecl)
    {
        m_decl = newDecl;
    }
    std::shared_ptr<FDComplexValue> const getValue()
    {
        return m_value;
    }
    void setValue(const std::shared_ptr<FDComplexValue> &value)
    {
        m_value = value;
    }
    std::string toGraph() override
    {
        auto ret = FDObject::toGraph();
        if (m_decl && m_value)
        {
            ret += getInstanceName() + " : " + m_decl->getName() + " = " + m_value->toString() + "\n";
        }
        return ret;
    }

protected:
    std::shared_ptr<FDPropertyDecl> m_decl;
    std::shared_ptr<FDComplexValue> m_value;
};
} // namespace BstIdl
#endif