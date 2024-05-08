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
#include "capicxx-core-tools/FTypeGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"
namespace BstCommonAPI
{

std::string FTypeGenerator::breaktext(std::string text, const BstIdl::FAnnotationType &annotation)
{
    std::string commentText = " * " + annotation.getName() + ":\n" + text;
    commentText = " " + simplify(replace_all(commentText, "\r\n", "\n"));
    return replace_all(commentText, "\n", "\n * ");
}

std::string FTypeGenerator::generateComments(const std::shared_ptr<BstIdl::FModelElement> &model, bool inline_)
{
    std::string intro = "";
    std::string tail = "";
    std::string annoCommentText = "";
    if (model != nullptr && model->getComment() != nullptr)
    {
        if (!inline_)
        {
            intro = "/*\n";
            tail = " */\n";
        }
        for (auto annoComment : model->getComment()->getElements())
        {
            if (annoComment != nullptr)
            {
                auto type = annoComment->getType();
                annoCommentText += breaktext(annoComment->getComment(), type) + "\n";
            }
        }
        return intro + annoCommentText + tail;
    }
    return "";
}

void FTypeGenerator::getRequiredHeaderPath(const std::shared_ptr<BstIdl::FTypeRef> &fTypeRef,
                                           std::list<std::string> &generatedHeaders,
                                           std::list<std::string> &libraryHeaders)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fTypeRef->getDerived() != nullptr)
    {
        addListItem(generatedHeaders, gen.getHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(
                                          fTypeRef->getDerived()->getContainer())));
    }
    if (fTypeRef->getInterval())
    {
        addListItem(libraryHeaders, "CommonAPI/RangedInteger.hpp");
    }
    getRequiredHeaderPath(fTypeRef->getPredefined(), generatedHeaders, libraryHeaders);
}
void FTypeGenerator::getRequiredHeaderPath(const std::shared_ptr<BstIdl::FBasicTypeId> &basic_id,
                                           std::list<std::string> &generatedHeaders,
                                           std::list<std::string> &libraryHeaders)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (basic_id != nullptr)
    {
        switch (basic_id->getValue())
        {
        case BstIdl::FBasicTypeId::STRING:
            addListItem(libraryHeaders, "string");
            break;
        case BstIdl::FBasicTypeId::BYTE_BUFFER:
            addListItem(libraryHeaders, "CommonAPI/ByteBuffer.hpp");
            break;
        default:
            addListItem(libraryHeaders, "cstdint");
            break;
        }
    }
}

void FTypeGenerator::addFTypeRequiredHeaders(const std::shared_ptr<BstIdl::FType> &type,
                                             std::list<std::string> &generatedHeaders,
                                             std::list<std::string> &libraryHeaders)
{
    if (!type)
        return;
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto type_def = std::dynamic_pointer_cast<BstIdl::FTypeDef>(type);
    if (type_def != nullptr)
    {
        getRequiredHeaderPath(type_def->getActualType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fArrayType = std::dynamic_pointer_cast<BstIdl::FArrayType>(type);
    if (fArrayType != nullptr)
    {
        addListItem(libraryHeaders, "vector");
        getRequiredHeaderPath(fArrayType->getElementType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fMapType = std::dynamic_pointer_cast<BstIdl::FMapType>(type);
    if (fMapType != nullptr)
    {
        addListItem(libraryHeaders, "unordered_map");
        getRequiredHeaderPath(fMapType->getKeyType(), generatedHeaders, libraryHeaders);
        getRequiredHeaderPath(fMapType->getValueType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fStructType = std::dynamic_pointer_cast<BstIdl::FStructType>(type);
    if (fStructType != nullptr)
    {
        auto base = fStructType->getBase();
        if (base != nullptr)
        {
            addListItem(generatedHeaders,
                        gen.getHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(base->getContainer())));
        }
        else
        {
            addListItem(libraryHeaders, "CommonAPI/Deployment.hpp");
            addListItem(libraryHeaders, "CommonAPI/InputStream.hpp");
            addListItem(libraryHeaders, "CommonAPI/OutputStream.hpp");
            addListItem(libraryHeaders, "CommonAPI/Struct.hpp");
        }
        if (fStructType->isPolymorphic() || (gen.hasPolymorphicBase(fStructType) && gen.hasDerivedTypes(fStructType)))
        {
            addListItem(libraryHeaders, "CommonAPI/Export.hpp");
        }
        for (auto element : fStructType->getElements())
        {
            getRequiredHeaderPath(element->getType(), generatedHeaders, libraryHeaders);
        }
        return;
    }
    auto enumerationType = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(type);
    if (enumerationType != nullptr)
    {
        if (enumerationType->getBase() != nullptr)
        {
            auto tc = std::dynamic_pointer_cast<BstIdl::FTypeCollection>(enumerationType->getBase()->getContainer());
            addListItem(generatedHeaders, gen.getHeaderPath(tc));
        }
        addListItem(libraryHeaders, "cstdint");
        addListItem(libraryHeaders, "CommonAPI/InputStream.hpp");
        addListItem(libraryHeaders, "CommonAPI/OutputStream.hpp");
        return;
    }
    auto unionType = std::dynamic_pointer_cast<BstIdl::FUnionType>(type);
    if (unionType != nullptr)
    {
        auto base = unionType->getBase();
        if (base != nullptr)
        {
            addListItem(generatedHeaders,
                        gen.getHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(base->getContainer())));
        }
        else
        {
            addListItem(libraryHeaders, "CommonAPI/Variant.hpp");
        }
        auto elements = unionType->getElements();
        for (auto element : elements)
        {
            getRequiredHeaderPath(element->getType(), generatedHeaders, libraryHeaders);
        }
        addListItem(libraryHeaders, "cstdint");
        addListItem(libraryHeaders, "memory");
        return;
    }
}

void FTypeGenerator::addRequiredHeaders(const std::shared_ptr<BstIdl::FType> &fType,
                                        std::list<std::string> &generatedHeaders,
                                        std::list<std::string> &libraryHeaders)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto str = gen.getHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(fType->getContainer()));
    addListItem(generatedHeaders, str);
    addFTypeRequiredHeaders(fType, generatedHeaders, libraryHeaders);
}

std::string FTypeGenerator::generateHasher(const std::shared_ptr<BstIdl::FMapType> &fMap)
{
    if (std::dynamic_pointer_cast<BstIdl::FEnumerationType>(fMap->getKeyType()->getDerived()) != nullptr)
    {
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        return ", CommonAPI::EnumHasher< " + gen.getFullName(fMap->getKeyType()->getDerived()) + ">";
    }
    return "";
}

std::string FTypeGenerator::getConstReferenceVariable(const std::shared_ptr<BstIdl::FField> &destination,
                                                      const std::shared_ptr<BstIdl::FModelElement> &source)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return "const " + gen.getTypeName(destination, source, false) + "&_" + destination->getName();
}

std::string FTypeGenerator::generateLiterals(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                             std::string _backingType)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> literals;
    if (_enumeration->getBase() != nullptr)
        literals.emplace_back(generateLiterals(_enumeration->getBase(), _backingType));
    for (auto enumerator : _enumeration->getEnumerators())
    {
        auto item = generateComments(enumerator, false);
        if (!item.empty())
        {
            item.push_back('\n');
        }
        item += gen.getEnumPrefix() + gen.getElementName(enumerator) + " = " +
                gen.doCast(gen.getEnumeratorValue(enumerator->getValue()), _backingType);
        literals.emplace_back(item);
    }
    return join(literals, ",\n");
}

std::string FTypeGenerator::getInitialValue(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (_enumeration->getBase() != nullptr)
        return getInitialValue(_enumeration->getBase());
    if (!_enumeration->getEnumerators().empty())
        return gen.getEnumPrefix() + _enumeration->getEnumerators().front()->getName();
    return gen.getEnumPrefix();
}

std::string FTypeGenerator::generateBaseTypeAssignmentOperator(
    const std::shared_ptr<BstIdl::FEnumerationType> _enumeration,
    const std::shared_ptr<BstIdl::FEnumerationType> _other, const std::shared_ptr<CommonapiPropertyAccessor> &_accessor)
{
    if (_other->getBase() != nullptr)
    {
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        auto backingType = gen.getPrimitiveTypeName(gen.getBackingType(_enumeration, _accessor));
        auto baseTypeName = gen.getBaseType(_other, _enumeration, backingType);
        std::string str = _enumeration->getName() + " &operator=(const " + baseTypeName + "::Literal &_value) {\n";
        str += "\tvalue_ = static_cast< " + backingType + ">(_value);\n\treturn (*this);\n}\n";
        str += generateBaseTypeAssignmentOperator(_enumeration, _other->getBase(), _accessor);
        return str;
    }
    return "";
}

std::string FTypeGenerator::generateLiteralValidation(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                                      const std::string backingType, std::list<std::string> &_values)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> literals;
    if (_enumeration->getBase() != nullptr)
    {
        literals.emplace_back(generateLiteralValidation(_enumeration->getBase(), backingType, _values));
    }
    for (auto enumerator : _enumeration->getEnumerators())
    {
        std::string item;
        auto it = std::find(_values.begin(), _values.end(), gen.getEnumeratorValue(enumerator->getValue()));
        if (it != _values.end())
            item = "//";
        item += "case static_cast< " + backingType + ">(Literal::" + gen.getEnumPrefix() +
                gen.getElementName(enumerator) + "):";
        _values.emplace_back(gen.getEnumeratorValue(enumerator->getValue()));
        literals.emplace_back(std::move(item));
    }
    return join(literals, "\n");
}

std::string FTypeGenerator::generateLiteralString(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                                  const std::string &backingType, std::list<std::string> &_values)
{
    if (!_enumeration)
        return "";

    auto &gen = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> literals;
    if (_enumeration->getBase())
        literals.emplace_back(generateLiteralString(_enumeration->getBase(), backingType, _values));

    for (const auto &it : _enumeration->getEnumerators())
    {
        std::string item;
        if (contains(_values, gen.getEnumeratorValue(it->getValue())))
            item = "//";
        item += "case static_cast< " + backingType + ">(Literal::" + gen.getEnumPrefix() + gen.getElementName(it) +
                "): return \"" + gen.getElementName(it) + "\";";
        _values.emplace_back(gen.getEnumeratorValue(it->getValue()));
        literals.emplace_back(std::move(item));
    }
    return join(literals, "\n");
}

std::string FTypeGenerator::generateDeclaration(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                                const std::shared_ptr<BstIdl::FModelElement> &_parent,
                                                const std::shared_ptr<CommonapiPropertyAccessor> &_accessor)
{
    std::string str;
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    gen.setEnumerationValues(_enumeration);
    if (_enumeration->getName().empty())
    {
        if (!_parent->getName().empty())
        {
            _enumeration->setName(_parent->getName() + "Enum");
        }
        else
        {
            _enumeration->setName(_parent->getContainer()->getName() + "Error");
        }
    }
    auto backingType = gen.getPrimitiveTypeName(gen.getBackingType(_enumeration, _accessor));
    str += "struct " + _enumeration->getName() + " : CommonAPI::Enumeration< " + backingType + "> {\n";
    str += "\tenum Literal : " + backingType + " {\n\t\t" +
           replace_all(generateLiterals(_enumeration, backingType), "\n", "\n\t\t") + "\n\t};\n\n";
    str += "\t" + _enumeration->getName() + "()\n";
    str += "\t\t: CommonAPI::Enumeration< " + backingType + ">(static_cast< " + backingType +
           ">(Literal::" + getInitialValue(_enumeration) + ")) {}\n";
    str += "\t" + _enumeration->getName() + "(Literal _literal)\n";
    str += "\t\t: CommonAPI::Enumeration< " + backingType + ">(static_cast< " + backingType + ">(_literal)) {}\n";
    str += "\t" + replace_all(generateBaseTypeAssignmentOperator(_enumeration, _enumeration, _accessor), "\n", "\n\t") +
           "\n";
    str += "\tinline bool validate() const {\n\t\tswitch (value_) {\n";
    std::list<std::string> values;
    str += "\t\t" + replace_all(generateLiteralValidation(_enumeration, backingType, values), "\n", "\n\t\t") +
           "\n\t\t\treturn true;\n\t\tdefault:\n\t\t\treturn false;\n\t\t}\n\t}\n\n";

    str += "\tinline bool operator==(const " + _enumeration->getName() +
           " &_other) const { return (value_ == _other.value_); }\n";
    str += "\tinline bool operator!=(const " + _enumeration->getName() +
           " &_other) const { return (value_ != _other.value_); }\n";
    str += "\tinline bool operator<=(const " + _enumeration->getName() +
           " &_other) const { return (value_ <= _other.value_); }\n";
    str += "\tinline bool operator>=(const " + _enumeration->getName() +
           " &_other) const { return (value_ >= _other.value_); }\n";
    str += "\tinline bool operator<(const " + _enumeration->getName() +
           " &_other) const { return (value_ < _other.value_); }\n";
    str += "\tinline bool operator>(const " + _enumeration->getName() +
           " &_other) const { return (value_ > _other.value_); }\n\n";
    str += "\tinline bool operator==(const Literal &_value) const { return (value_ == static_cast< " + backingType +
           ">(_value)); }\n";
    str += "\tinline bool operator!=(const Literal &_value) const { return (value_ != static_cast< " + backingType +
           ">(_value)); }\n";
    str += "\tinline bool operator<=(const Literal &_value) const { return (value_ <= static_cast< " + backingType +
           ">(_value)); }\n";
    str += "\tinline bool operator>=(const Literal &_value) const { return (value_ >= static_cast< " + backingType +
           ">(_value)); }\n";
    str += "\tinline bool operator<(const Literal &_value) const { return (value_ < static_cast< " + backingType +
           ">(_value)); }\n";
    str += "\tinline bool operator>(const Literal &_value) const { return (value_ > static_cast< " + backingType +
           ">(_value)); }\n";

    std::string toStringliteral = R"(
    const char* toString() const noexcept
    {
        switch(value_)
        {
        ENUM_LITERAL_STRING
        default: return "UNDEFINED";    
        }
    }
)";
    values.clear();
    replace_one(toStringliteral, "ENUM_LITERAL_STRING",
                replace_all(generateLiteralString(_enumeration, backingType, values), "\n", "\n\t\t"));
    str += toStringliteral + "};";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FTypeDef> &fTypeDef,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fTypeDef, false);
    str += "\ntypedef " + gen.getElementType(fTypeDef->getActualType(), fTypeDef, false) + " " +
           gen.getElementName(fTypeDef) + ";\n";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FArrayType> &fArrayType,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fArrayType, false);
    if (fArrayType->getElementType()->getDerived() != nullptr &&
        std::dynamic_pointer_cast<BstIdl::FStructType>(fArrayType->getElementType()->getDerived()) != nullptr &&
        std::dynamic_pointer_cast<BstIdl::FStructType>(fArrayType->getElementType()->getDerived())->isPolymorphic())
        str += "\ntypedef std::vector<std::shared_ptr<" +
               gen.getElementType(fArrayType->getElementType(), fArrayType, true) + ">>" +
               gen.getElementName(fArrayType) + ";";
    else
    {
        str += "\ntypedef std::vector<" + gen.getElementType(fArrayType->getElementType(), fArrayType, true) + ">" +
               gen.getElementName(fArrayType) + ";";
    }
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FMapType> &fMap, const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fMap, false);
    str += "\ntypedef std::unordered_map<" + generateKeyType(fMap) + "," + generateValueType(fMap) +
           generateHasher(fMap) + "> " + gen.getElementName(fMap) + ";";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FStructType> &fStructType,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fStructType, false);
    if (fStructType->isPolymorphic())
        str += gen.createSerials(fStructType);
    if (gen.hasPolymorphicBase(fStructType))
        if (fStructType->getBase() == nullptr)
        {
            str += "\nstruct " + gen.getElementName(fStructType) + " : CommonAPI::PolymorphicStruct {\n";
        }
        else
        {
            str += "\nstruct " + gen.getElementName(fStructType) + ":" +
                   gen.getElementName(fStructType->getBase(), fStructType, false) + " {\n";
        }
    else
    {
        str += "\nstruct " + gen.getElementName(fStructType) + ": CommonAPI::Struct< ";
        std::list<std::string> args;
        for (auto it : gen.getAllElements(fStructType))
        {
            args.emplace_back(gen.getTypeName(it, fStructType, false));
        }
        str += join(args, ", ") + "> {\n";
    }

    if (gen.hasPolymorphicBase(fStructType))
    {
        if (fStructType->isPolymorphic() || gen.hasDerivedTypes(fStructType))
        {
            str += "\tstatic COMMONAPI_EXPORT std::shared_ptr< " + gen.getElementName(fStructType) +
                   "> create(CommonAPI::Serial _serial);\n";
        }
        auto name = gen.getElementName(fStructType);
        transform(name.begin(), name.end(), name.begin(), ::toupper);
        str += "\tCommonAPI::Serial getSerial() const { return " + name + "_SERIAL; }\n";
    }
    str += "\t" + gen.getElementName(fStructType) + "()\n";
    if (gen.hasPolymorphicBase(fStructType) && fStructType->getBase() != nullptr)
    {
        str += "\t: " + gen.getElementName(fStructType->getBase()) + "()\n";
    }
    str += "\t{\n";
    if (gen.getAllElements(fStructType).size() > 0)
    {
        auto n = 0;
        for (auto element : fStructType->getElements())
        {
            auto nindex = n;
            if (gen.hasPolymorphicBase(fStructType))
                nindex = n - (gen.getAllElements(fStructType).size() - fStructType->getElements().size());
            if (nindex >= 0)
            {
                if (std::dynamic_pointer_cast<BstIdl::FStructType>(element->getType()->getDerived()) != nullptr &&
                    gen.hasPolymorphicBase(
                        std::dynamic_pointer_cast<BstIdl::FStructType>(element->getType()->getDerived())) &&
                    !element->isArray())
                    str += "\t\tstd::get<" + std::to_string(nindex) + ">(values_) = std::make_shared<" +
                           gen.getElementType(element->getType(), fStructType, false) + ">();\n";
                else if (element->getType()->getDerived() != nullptr && !element->isArray())
                    str += "\t\tstd::get<" + std::to_string(nindex) +
                           ">(values_) = " + gen.getTypeName(element, fStructType, false) + "();\n";
                else if (element->getType()->getPredefined() != nullptr && !element->isArray())
                    str += "\t\tstd::get<" + std::to_string(nindex) +
                           ">(values_) = " + gen.generateDummyValue(element->getType()) + ";\n";
                else
                {
                    str += "\t\tstd::get< " + std::to_string(nindex) +
                           ">(values_) = " + gen.getTypeName(element, fStructType, false) + "();\n";
                }
            }
            n = n + 1;
        }
    }
    str += "\t}\n";
    if (gen.getAllElements(fStructType).size() > 0)
    {
        str += "\t" + gen.getElementName(fStructType) + "(";
        std::list<std::string> args;
        for (auto element : gen.getAllElements(fStructType))
        {
            args.emplace_back(getConstReferenceVariable(element, fStructType));
        }
        str += join(args, ", ") + ")\n";
        if (gen.hasPolymorphicBase(fStructType) && fStructType->getBase() != nullptr)
        {
            auto base = fStructType->getBase();
            args.clear();
            for (auto element : gen.getAllElements(base))
            {
                args.emplace_back("_" + element->getName());
            }
            str += "\t:" + gen.getElementName(base) + "(" + join(args, ", ") + ")\n";
        }
        str += "\t{\n";
        if (gen.hasPolymorphicBase(fStructType))
        {
            auto i = -1;
            for (auto element : fStructType->getElements())
            {
                str += "\t\tstd::get< " + std::to_string(++i) + ">(values_) = _" + gen.getElementName(element) + ";\n";
            }
        }
        else
        {
            auto i = -1;
            for (auto element : gen.getAllElements(fStructType))
            {
                str += "\t\tstd::get< " + std::to_string(++i) + ">(values_) = _" + gen.getElementName(element) + ";\n";
            }
        }
        str += "\t}\n";
    }
    if (gen.hasPolymorphicBase(fStructType))
    {
        str += "\ttemplate<class _Input>\n";
        str += "\tvoid readValue(CommonAPI::InputStream<_Input> &_input, const CommonAPI::EmptyDeployment *_depl) {\n";
        std::list<std::shared_ptr<BstIdl::FStructType>> types;
        gen.getDerivedFStructTypes(fStructType, types);
        if (!types.empty())
        {
            str += "\t\t(void) _depl;\n";
        }
        auto i = -1;
        for (auto element : fStructType->getElements())
        {
            i += 1;
            str += "\t\t_input.template readValue<CommonAPI::EmptyDeployment>(std::get<" + std::to_string(i) +
                   ">(values_));\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += "\t\t" + gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast< " + gen.getElementName(derived) +
                       " *>(this)->template readValue<_Input>(_input, _depl);\n\tbreak;\n";
            }
            str += "\t\tdefault:\n\t\t\tbreak;\n\t\t}\n";
        }

        str += "\t}\n\n\ttemplate<class _Input, class _Deployment>\n";
        str += "\tvoid readValue(CommonAPI::InputStream<_Input> &_input, const _Deployment *_depl) {\n";
        auto j = -1;
        auto k = gen.getAllElements(fStructType).size() - fStructType->getElements().size() - 1;
        for (auto element : fStructType->getElements())
        {
            j = j + 1;
            k = k + 1;
            str += "\t\t_input.template readValue<>(std::get< " + std::to_string(j) + ">(values_), std::get< " +
                   std::to_string(k) + ">(_depl->values_));\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += "\t\t" + gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast<" + gen.getElementName(derived) +
                       "*>(this)->template readValue<>(_input, _depl);\n\t\t\tbreak;\n";
            }
            str += "\t\tdefault\n\t\t\tbreak;\n\t\t}\n";
        }

        str += "\t}\n\ttemplate<class _Output>\n\tvoid writeType(CommonAPI::TypeOutputStream<_Output> &_output, const "
               "CommonAPI::EmptyDeployment *_depl) {\n";
        auto l = -1;
        for (auto element : fStructType->getElements())
        {
            l = l + 1;
            str += "\t\t_output.writeType(std::get< " + std::to_string(l) + ">(values_), _depl);\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast<" + gen.getElementName(derived) +
                       "*>(this)->template writeType<_Output>(_output, _depl);\n\t\t\tbreak;\n";
            }
            str += "\t\t\tdefault\n\t\t\tbreak;\n\t\t}\n";
        }

        str += "\t}\n\ttemplate<class _Output, class _Deployment>\n\tvoid "
               "writeType(CommonAPI::TypeOutputStream<_Output> &_output, const _Deployment *_depl) {\n";
        auto l1 = -1;
        auto l2 = gen.getAllElements(fStructType).size() - fStructType->getElements().size() - 1;
        for (auto element : fStructType->getElements())
        {
            l1 = l1 + 1;
            l2 = l2 + 1;
            str += "\t\t_output.writeType(std::get< " + std::to_string(l1) + "(values_), std::get<" +
                   std::to_string(l2) + ">(_depl->values_));\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast<" + gen.getElementName(derived) +
                       "*>(this)->template writeType<_Output, _Deployment>(_output, _depl);\n\t\t\tbreak;\n";
            }
            str += "\t\tdefault\n\t\t\tbreak;\n\t\t}\n";
        }

        str += "\t}\n\ttemplate<class _Output>\n";
        str +=
            "\tvoid writeValue(CommonAPI::OutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {\n";
        if (!types.empty())
        {
            str += "\t\t(void) _depl;\n";
        }
        auto m = -1;
        for (auto element : fStructType->getElements())
        {
            m = m + 1;
            str += "\t\t_output.template writeValue<CommonAPI::EmptyDeployment>(std::get<" + std::to_string(m) +
                   ">(values_));\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast<" + gen.getElementName(derived) +
                       "*>(this)->template writeValue<_Output>(_output, _depl);\n\t\t\tbreak;\n";
            }
            str += "\t\tdefault\n\t\t\tbreak;\n\t\t}\n";
        }

        str += "\t}\n\ttemplate<class _Output, class _Deployment>\n";
        str += "\tvoid writeValue(CommonAPI::OutputStream<_Output> &_output, const _Deployment *_depl) {\n";
        auto n = -1;
        auto o = gen.getAllElements(fStructType).size() - fStructType->getElements().size() - 1;
        for (auto element : fStructType->getElements())
        {
            n = n + 1;
            o = o + 1;
            str += "\t\t_output.template writeValue<>(std::get< " + std::to_string(n) + ">(values_), std::get< " +
                   std::to_string(o) + ">(_depl->values_));\n";
        }
        if (gen.hasDerivedTypes(fStructType))
        {
            str += "\t\tswitch (getSerial()) {\n";
            for (auto derived : types)
            {
                str += gen.generateCases(derived, nullptr, false) + "\n";
                str += "\t\t\tstatic_cast<" + gen.getElementName(derived) +
                       "*>(this)->template writeValue<>(_output, _depl);\n\t\t\tbreak;\n";
            }
            str += "\t\tdefault\n\t\t\tbreak;\n\t\t}\n";
        }
        str += "\t}\n";

        auto p = -1;
        for (auto element : fStructType->getElements())
        {
            str += "\t" + replace_all(generateComments(element, false), "\n", "\n\t");
            auto typeName = gen.getTypeName(element, fStructType, false);
            auto name = toFirstUpper(gen.getElementName(element));
            str += "inline const " + typeName + " &get" + name + "() const { return std::get<" + std::to_string(++p) +
                   ">(values_); }\n";
            str += "\tinline void set" + name + "(const " + typeName + " ";
            if (gen.isComplex(typeName))
            {
                str += "&";
            }
            str += " _value) { std::get< " + std::to_string(p) + ">(values_) = _value; }\n";
        }
        if (gen.hasPolymorphicBase(fStructType) && fStructType->getElements().size() > 0)
        {
            std::list<std::string> args;
            for (auto element : fStructType->getElements())
            {
                args.emplace_back(gen.getTypeName(element, fStructType, false));
            }
            str += "\tstd::tuple< " + join(args, ", ") + "> values_;\n";
        }
    }
    else
    {
        auto k = -1;
        for (auto element : gen.getAllElements(fStructType))
        {
            str += "\t" + replace_all(generateComments(element, false), "\n", "\n\t");
            auto typeName = gen.getTypeName(element, fStructType, false);
            str += "inline const " + typeName + " &get" + toFirstUpper(gen.getElementName(element)) +
                   "() const { return std::get< " + std::to_string(++k) + ">(values_); }\n";
            str += "\tinline void set" + toFirstUpper(gen.getElementName(element)) + "(const " + typeName +
                   (gen.isComplex(typeName) ? "&" : "") + " _value) { std::get<" + std::to_string(k) +
                   ">(values_) = _value; }\n";
        }
    }
    str += "\tinline bool operator==(const " + fStructType->getName() + "& _other) const {\n";
    if (gen.getAllElements(fStructType).size() > 0)
    {
        std::list<std::string> args;
        for (auto element : gen.getAllElements(fStructType))
        {
            auto name = toFirstUpper(gen.getElementName(element));
            args.emplace_back("get" + name + "() == _other.get" + name + "()");
        }
        str += "\t\treturn ( " + join(args, " && ") + ");\n";
    }
    else
    {
        str += "\t\t(void) _other;\n\t\treturn true;\n";
    }
    str += "\t}\n\tinline bool operator!=(const " + fStructType->getName() + " &_other) const {\n";
    str += "\t\treturn !((*this) == _other);\n\t}\n};";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FEnumerationType> &fEnumerationType,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    return generateDeclaration(fEnumerationType, fEnumerationType, deploymentAccessor);
}

std::string FTypeGenerator::getElementNames(const std::shared_ptr<BstIdl::FUnionType> &fUnion)
{
    std::list<std::string> names;
    if (fUnion->getBase())
        names.emplace_back(getElementNames(fUnion->getBase()));

    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    for (auto item : fUnion->getElements())
    {
        names.emplace_back(gen.getTypeName(item, fUnion, false));
    }

    return join(names, ", ");
}

std::string FTypeGenerator::generateFTypeDeclaration(
    const std::shared_ptr<BstIdl::FUnionType> &fUnionType,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fUnionType, false);
    str += "typedef CommonAPI::Variant<" + getElementNames(fUnionType) + "> " + gen.getElementName(fUnionType) + ";\n";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclarations(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string str;
    for (auto type : sortTypes(fTypeCollection->getTypes()))
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FMapType>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(type))
            str += "\n" + generateFTypeDeclaration(ptr, deploymentAccessor);
        str += "\n";
    }
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FInterface>(fTypeCollection))
    {
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        for (auto method : gen.getMethodsWithError(ptr))
        {
            str += generateComments(method, false);
            auto errors = method->getErrors();
            str += generateDeclaration(errors, errors, deploymentAccessor);
        }
    }
    return str;
}

std::vector<std::shared_ptr<BstIdl::FType>> FTypeGenerator::sortTypes(
    const std::list<std::shared_ptr<BstIdl::FType>> &typeList)
{
    // check circular dependencies.
    std::vector<std::shared_ptr<BstIdl::FType>> ret;
    auto &ins = FrancaGeneratorExtensions::getInstance();
    for (const auto &type : typeList)
    {
        auto it = ret.begin();
        while (it != ret.end())
        {
            auto refs = ins.getDirectlyReferencedTypes(*it);
            auto found = std::find(refs.begin(), refs.end(), type);
            if (found != refs.end())
                break;
            else
                ++it;
        }
        ret.emplace(it, type);
    }
    return ret;
}

std::string FTypeGenerator::generateFConstDeclarations(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::list<std::string> literals;
    for (const auto &fconst : fTypeCollection->getConstants())
    {
        std::string item;
        if (fconst->getType() && fconst->getType()->getPredefined() &&
            fconst->getType()->getPredefined()->getValue() <= 9)
        {
            item = "static const " + FrancaGeneratorExtensions::getInstance().getTypeName(fconst, nullptr, false) +
                   " " + fconst->getName() + " = " + printInitializerExpression(fconst->getRhs()) + ";";
        }
        else
        {
            item = "static const " + FrancaGeneratorExtensions::getInstance().getTypeName(fconst, nullptr, false) +
                   " " + fconst->getName() + ";";
        }
        literals.emplace_back(std::move(item));
    }
    return "\n" + join(literals, "\n");
}

std::string FTypeGenerator::generateFConstDefinitions(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::list<std::string> literals;
    for (const auto &fconst : fTypeCollection->getConstants())
    {
        if (fconst->getType() && fconst->getType()->getPredefined() &&
            fconst->getType()->getPredefined()->getValue() <= 9)
            continue;

        auto item = "const " + FrancaGeneratorExtensions::getInstance().getTypeName(fconst, nullptr, false) + " " +
                    FrancaGeneratorExtensions::getInstance().getElementName(fTypeCollection) +
                    "::" + fconst->getName() + " = " + printInitializerExpression(fconst->getRhs()) + ";";
        literals.emplace_back(std::move(item));
    }
    return "\n" + join(literals, "\n");
}

bool FTypeGenerator::hasImplementation(const std::shared_ptr<BstIdl::FType> &fType)
{
    if (std::dynamic_pointer_cast<BstIdl::FStructType>(fType))
        return std::dynamic_pointer_cast<BstIdl::FStructType>(fType)->isPolymorphic();
    return false;
}

bool FTypeGenerator::hasImplementation(const std::shared_ptr<BstIdl::FConstantDef> &fConst)
{
    if (fConst->getType() && fConst->getType()->getPredefined() && fConst->getType()->getPredefined()->getValue() <= 9)
        return false;
    return true;
}

std::string FTypeGenerator::generateKeyType(const std::shared_ptr<BstIdl::FMapType> &fMap)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fMap->getKeyType()->isPolymorphic())
        return "std::shared_ptr<" + gen.getElementType(fMap->getKeyType(), nullptr, true) + ">";
    if (fMap->getKeyType()->getDerived() != nullptr)
        return gen.getFullName(fMap->getKeyType()->getDerived());
    else
        return gen.getElementType(fMap->getKeyType(), nullptr, true);
}

std::string FTypeGenerator::generateValueType(const std::shared_ptr<BstIdl::FMapType> &fMap)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fMap->getValueType()->isPolymorphic())
        return "std::shared_ptr<" + gen.getElementType(fMap->getKeyType(), nullptr, true) + ">";
    if (fMap->getValueType()->getDerived() != nullptr)
        return gen.getFullName(fMap->getValueType()->getDerived());
    else
        return gen.getElementType(fMap->getValueType(), nullptr, true);
}

void FTypeGenerator::generateRequiredTypeIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                  std::list<std::string> &generatedHeaders,
                                                  std::list<std::string> &libraryHeaders, bool isStub)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    for (auto attribute : fInterface->getAttributes())
    {
        if (attribute->isArray())
        {
            addListItem(libraryHeaders, "vector");
            break;
        }
    }
    for (auto method : fInterface->getMethods())
    {
        for (auto inArg : method->getInArgs())
        {
            if (inArg->isArray())
            {
                addListItem(libraryHeaders, "vector");
                break;
            }
        }
        for (auto outArg : method->getOutArgs())
        {
            if (outArg->isArray())
            {
                addListItem(libraryHeaders, "vector");
                break;
            }
        }
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        for (auto outArg : broadcast->getOutArgs())
        {
            if (outArg->isArray())
            {
                addListItem(libraryHeaders, "vector");
                break;
            }
        }
    }
    for (auto attribute : fInterface->getAttributes())
    {
        auto d_type = attribute->getType()->getDerived();
        if (d_type != nullptr)
        {
            addRequiredHeaders(d_type, generatedHeaders, libraryHeaders);
        }
    }
    for (auto method : fInterface->getMethods())
    {
        for (auto inArg : method->getInArgs())
        {
            auto d_type = inArg->getType()->getDerived();
            if (d_type != nullptr)
            {
                addRequiredHeaders(d_type, generatedHeaders, libraryHeaders);
            }
        }
        for (auto outArg : method->getOutArgs())
        {
            auto d_type = outArg->getType()->getDerived();
            if (d_type != nullptr)
            {
                addRequiredHeaders(d_type, generatedHeaders, libraryHeaders);
            }
        }
        if (method->getErrorEnum() != nullptr)
            addRequiredHeaders(method->getErrorEnum(), generatedHeaders, libraryHeaders);
    }
    if (isStub)
    {
        for (auto manage : fInterface->getManagedInterfaces())
        {
            addListItem(generatedHeaders, ins.getStubHeaderPath(manage));
        }
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        for (auto outArg : broadcast->getOutArgs())
        {
            auto d_type = outArg->getType()->getDerived();
            if (d_type != nullptr)
            {
                addRequiredHeaders(d_type, generatedHeaders, libraryHeaders);
            }
        }
    }

    auto interfaceHeaderPath = ins.getHeaderPath(fInterface);
    for (auto it = generatedHeaders.begin(); it != generatedHeaders.end();)
    {
        if (*it == interfaceHeaderPath)
            it = generatedHeaders.erase(it);
        else
            ++it;
    }
}

bool FTypeGenerator::isdeprecated(const std::shared_ptr<BstIdl::FAnnotationBlock> &annotations)
{
    if (annotations == nullptr)
        return false;
    for (auto annotation : annotations->getElements())
    {
        if (annotation->getType().getValue() == BstIdl::FAnnotationType::DEPRECATED)
        {
            return true;
        }
    }
    return false;
}

void FTypeGenerator::generateInheritanceIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                 std::list<std::string> &generatedHeaders,
                                                 std::list<std::string> &libraryHeaders)
{
    if (fInterface->getBase() != nullptr)
    {
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        auto stubHeaderPath = gen.getStubHeaderPath(fInterface->getBase());
        generatedHeaders.push_back(stubHeaderPath);
    }
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FInitializerExpression> &rhs)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FCurrentError>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FBracketInitializer>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FCompoundInitializer>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FBooleanConstant>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDoubleConstant>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FFloatConstant>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FIntegerConstant>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStringConstant>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FMethodErrorEnumRef>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FBinaryOperation>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnaryOperation>(rhs))
        return printInitializerExpression(ptr);
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FQualifiedElementRef>(rhs))
        return printInitializerExpression(ptr);

    return "";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FCurrentError> &rhs)
{
    return "error";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FBracketInitializer> &rhs)
{
    std::string ret = "{ ";
    std::list<std::string> literals;
    for (const auto &element : rhs->getElements())
    {
        std::string item;
        if (element->getSecond())
            item = "{ ";
        item += printInitializerExpression(element->getFirst());
        if (element->getSecond())
            item += ", " + printInitializerExpression(element->getSecond()) + "}";
        literals.emplace_back(std::move(item));
    }
    ret += join(literals, " , ") + "}";
    return ret;
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FCompoundInitializer> &rhs)
{
    std::string ret = "{ ";
    std::list<std::string> literals;
    for (const auto &element : rhs->getElements())
    {
        std::string item =
            "." + element->getElement()->getName() + " = " + printInitializerExpression(element->getValue());
        literals.emplace_back(std::move(item));
    }
    ret += join(literals, " , ") + "}";
    return ret;
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FBooleanConstant> &rhs)
{
    return rhs->isVal() ? "true" : "false";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FDoubleConstant> &rhs)
{
    return rhs->toString();
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FFloatConstant> &rhs)
{
    return rhs->toString();
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FIntegerConstant> &rhs)
{
    return rhs->toString();
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FStringConstant> &rhs)
{
    return "\"" + rhs->toString() + "\"";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FMethodErrorEnumRef> &rhs)
{
    return "method error enum ref";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FBinaryOperation> &rhs)
{
    std::string ret = "(" + printInitializerExpression(rhs->getLeft()) + ")" + rhs->getOp()->getLiteral() + "(" +
                      printInitializerExpression(rhs->getRight()) + ")";
    return ret;
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FUnaryOperation> &rhs)
{
    return rhs->getOp()->getLiteral() + "(" + printInitializerExpression(rhs->getOperand()) + ")";
}

std::string FTypeGenerator::printInitializerExpression(const std::shared_ptr<BstIdl::FQualifiedElementRef> &rhs)
{
    if (!rhs)
        return "";

    std::string ret;
    auto &ins = FrancaGeneratorExtensions::getInstance();
    if (rhs->getElement())
    {
        ret = ins.getFullyQualifiedCppName(rhs->getElement());
    }
    else
    {
        std::string getter = "get()";
        if (rhs->getField() && rhs->getField()->getContainer())
        {
            auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(rhs->getField()->getContainer());
            if (ptr)
                getter = "get" + toFirstUpper(rhs->getField()->getName()) + "()";
        }
        ret = ins.getFullyQualifiedCppName(rhs->getQualifier()->getElement()) + "." + getter;
    }

    return ret;
}
} // namespace BstCommonAPI