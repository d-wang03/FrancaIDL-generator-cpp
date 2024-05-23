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
#ifndef F_METHOD_H
#define F_METHOD_H

#include "FAnnotationBlock.h"
#include "FArgument.h"
#include "FEnumerationType.h"
#include "FTypedElement.h"
#include <list>
#include <memory>

namespace BstIdl
{
class FMethod : public FModelElement
{
public:
    FMethod() = default;
    virtual ~FMethod() = default;
    FMethod(const FMethod &) = default;
    FMethod(FMethod &&) noexcept = default;
    FMethod &operator=(const FMethod &) = default;
    FMethod &operator=(FMethod &&) = default;

    std::string getFQN() const override
    {
        if (getSelector().empty())
            return getContainer() ? (getContainer()->getFQN() + "." + getName()) : getName();
        else
            return getContainer() ? (getContainer()->getFQN() + "." + getName() + ":" + m_selector)
                                  : getName() + ":" + m_selector;
    }
    bool isFireAndForget() const
    {
        return m_isFireAndForget;
    }
    void setFireAndForget(bool value)
    {
        m_isFireAndForget = value;
    }
    std::list<std::shared_ptr<FArgument>> &getInArgs()
    {
        return m_inArgs;
    }
    std::list<std::shared_ptr<FArgument>> &getOutArgs()
    {
        return m_outArgs;
    }
    std::shared_ptr<FEnumerationType> getErrorEnum() const
    {
        return m_errorEnum;
    }
    void setErrorEnum(const std::shared_ptr<FEnumerationType> &value)
    {
        m_errorEnum = value;
    }
    std::shared_ptr<FEnumerationType> getErrors() const
    {
        return m_errors;
    }
    void setErrors(const std::shared_ptr<FEnumerationType> &value)
    {
        m_errors = value;
    }
    std::shared_ptr<FEnumerationType> getErrorType() const
    {
        return m_errors ? m_errors : m_errorEnum;
    }
    std::shared_ptr<FArgument> getErrorArg()
    {
        if (!m_errArg)
        {
            m_errArg = std::make_shared<FArgument>();
            m_errArg->setName("err");
            auto typeref = std::make_shared<FTypeRef>();
            typeref->setDerived(getErrorType());
            m_errArg->setType(typeref);
        }
        return m_errArg;
    }
    std::string getSelector() const
    {
        return m_selector;
    }
    void setSelector(const std::string &value)
    {
        m_selector = value;
    }
    std::shared_ptr<FAnnotationBlock> getErrorComment() const
    {
        return m_errorComment;
    }
    void setErrorComment(const std::shared_ptr<FAnnotationBlock> &value)
    {
        m_errorComment = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFMethod)
            ret = trans->forFMethod(this);
        return ret;
    }

protected:
    bool m_isFireAndForget;
    std::list<std::shared_ptr<FArgument>> m_inArgs;
    std::list<std::shared_ptr<FArgument>> m_outArgs;
    std::shared_ptr<FEnumerationType> m_errorEnum;
    std::shared_ptr<FEnumerationType> m_errors;
    std::shared_ptr<FArgument> m_errArg;
    std::string m_selector;
    std::shared_ptr<FAnnotationBlock> m_errorComment;
};

} // namespace BstIdl

#endif