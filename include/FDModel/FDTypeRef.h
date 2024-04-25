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
#ifndef F_D_TYPE_REF_H
#define F_D_TYPE_REF_H
#include "FDPredefinedTypeId.h"
#include "FDType.h"
#include <list>
#include <memory>

namespace BstIdl
{
class FDTypeRef
{
protected:
    std::shared_ptr<FDPredefinedTypeId> m_predefined;
    std::shared_ptr<FDType> m_complex;
    std::string m_array;

public:
    FDTypeRef() = default;
    virtual ~FDTypeRef() = default;
    FDTypeRef(const FDTypeRef &) = default;
    FDTypeRef(FDTypeRef &&) noexcept = default;
    FDTypeRef &operator=(const FDTypeRef &) = default;
    FDTypeRef &operator=(FDTypeRef &&) = default;

    std::shared_ptr<FDPredefinedTypeId> const getPredefined()
    {
        return m_predefined;
    }
    void setPredefined(const std::shared_ptr<FDPredefinedTypeId> &new_predefined)
    {
        m_predefined = new_predefined;
    }
    std::shared_ptr<FDType> const getComplex()
    {
        return m_complex;
    }
    void setComplex(const std::shared_ptr<FDType> &new_complex)
    {
        m_complex = new_complex;
    }
    std::string const getArray()
    {
        return m_array;
    }
    void setArray(const std::string &new_array)
    {
        m_array = new_array;
    }
    std::string getName() const
    {
        if (m_predefined)
            return m_predefined->getName();
        if (!m_array.empty())
            return m_array;
        if (m_complex)
        {
            return m_complex->getName();
        }
        return "UnknownType";
    }
};
} // namespace BstIdl

#endif