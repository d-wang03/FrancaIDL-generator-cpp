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
#ifndef F_INTEGER_INTERVAL_H
#define F_INTEGER_INTERVAL_H

#include "FTransformer.h"
#include "FType.h"

namespace BstIdl
{

class FIntegerInterval : public FType
{
public:
    FIntegerInterval() = default;
    virtual ~FIntegerInterval() = default;
    FIntegerInterval(const FIntegerInterval &) = default;
    FIntegerInterval(FIntegerInterval &&) noexcept = default;
    FIntegerInterval &operator=(const FIntegerInterval &) = default;
    FIntegerInterval &operator=(FIntegerInterval &&) = default;

    int64_t getLowerBound() const
    {
        return m_lowerBound;
    }
    void setLowerBound(int64_t value)
    {
        m_lowerBound = value;
    }
    int64_t getUpperBound() const
    {
        return m_upperBound;
    }
    void setUpperBound(int64_t value)
    {
        m_upperBound = value;
    }
    std::string toString() const
    {
        return "Integer(" + std::to_string(m_lowerBound) + "," + std::to_string(m_upperBound) + ")";
    }

    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFIntegerInterval)
            ret = trans->forFIntegerInterval(this);
        return ret;
    }

protected:
    int64_t m_lowerBound;
    int64_t m_upperBound;
};

} // namespace BstIdl

#endif