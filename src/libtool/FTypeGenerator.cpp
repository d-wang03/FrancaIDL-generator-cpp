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
#include "libtool/FTypeGenerator.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"
#include <algorithm>
namespace BstIdl
{
void FTypeGenerator::addFTypeRequiredHeaders(const std::shared_ptr<FType> &type,
                                             std::list<std::string> &generatedHeaders,
                                             std::list<std::string> &libraryHeaders)
{
    auto type_def = std::dynamic_pointer_cast<FTypeDef>(type);
    if (type_def != nullptr)
    {
        getRequiredHeaderPath(type_def->getActualType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fArrayType = std::dynamic_pointer_cast<FArrayType>(type);
    if (fArrayType != nullptr)
    {
        addListItem(libraryHeaders, "vector");
        getRequiredHeaderPath(fArrayType->getElementType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fMapType = std::dynamic_pointer_cast<FMapType>(type);
    if (fMapType != nullptr)
    {
        addListItem(libraryHeaders, "unordered_map");
        getRequiredHeaderPath(fMapType->getKeyType(), generatedHeaders, libraryHeaders);
        getRequiredHeaderPath(fMapType->getValueType(), generatedHeaders, libraryHeaders);
        return;
    }
    auto fStructType = std::dynamic_pointer_cast<FStructType>(type);
    if (fStructType != nullptr)
    {
        auto base = fStructType->getBase();
        if (base != nullptr)
        {
            FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
            auto header = gen.getHeaderPath(std::dynamic_pointer_cast<FTypeCollection>(base->getContainer()));
            addListItem(generatedHeaders, header);
        }
        auto elements = fStructType->getElements();
        for (auto element : elements)
        {
            getRequiredHeaderPath(element->getType(), generatedHeaders, libraryHeaders);
        }
        return;
    }
    auto enumerationType = std::dynamic_pointer_cast<FEnumerationType>(type);
    if (enumerationType != nullptr)
    {
        if (enumerationType->getBase() != nullptr)
        {
            FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
            auto tc = enumerationType->getBase()->getContainer();
            auto header = gen.getHeaderPath(std::dynamic_pointer_cast<FTypeCollection>(tc));
            addListItem(generatedHeaders, header);
        }
        addListItem(libraryHeaders, "cstdint");
    }
    auto unionType = std::dynamic_pointer_cast<FUnionType>(type);
    if (unionType != nullptr)
    {
        auto base = unionType->getBase();
        if (base != nullptr)
        {
            FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
            auto header = gen.getHeaderPath(std::dynamic_pointer_cast<FTypeCollection>(base->getContainer()));
            addListItem(generatedHeaders, header);
        }

        auto elements = unionType->getElements();
        for (auto element : elements)
        {
            getRequiredHeaderPath(element->getType(), generatedHeaders, libraryHeaders);
        }
        addListItem(libraryHeaders, "cstdint");
        addListItem(libraryHeaders, "memory");
    }
}

std::string FTypeGenerator::getConstReferenceVariable(const std::shared_ptr<FField> &destination,
                                                      const std::shared_ptr<FModelElement> &source)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return "const " + gen.getTypeName(destination, source, false) + "&_" + destination->getName();
}

void FTypeGenerator::getRequiredHeaderPath(const std::shared_ptr<FTypeRef> &fTypeRef,
                                           std::list<std::string> &generatedHeaders,
                                           std::list<std::string> &libraryHeaders)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fTypeRef->getDerived() != nullptr)
    {
        addListItem(generatedHeaders, gen.getHeaderPath(std::dynamic_pointer_cast<FTypeCollection>(
                                          fTypeRef->getDerived()->getContainer())));
    }
    getRequiredHeaderPath(fTypeRef->getPredefined(), generatedHeaders, libraryHeaders);
}

void FTypeGenerator::getRequiredHeaderPath(const std::shared_ptr<FBasicTypeId> &basic_id,
                                           std::list<std::string> &generatedHeaders,
                                           std::list<std::string> &libraryHeaders)
{
    addListItem(libraryHeaders, "cstdint");
    if (basic_id && basic_id->getValue() == FBasicTypeId::STRING)
    {
        addListItem(libraryHeaders, "string");
    }
}

std::list<std::shared_ptr<FType>> FTypeGenerator::sortTypes(std::list<std::shared_ptr<FType>> &typeList,
                                                            std::shared_ptr<FTypeCollection> containingTypeCollection)
{
    if (!hasNoCircularDependencies(typeList))
    {
        std::cout << "FTypeCollection or FInterface has circular dependencies\n";
    }
    // TODO: incomplete function.
    return typeList;
}

bool FTypeGenerator::hasNoCircularDependencies(std::list<std::shared_ptr<FType>> types)
{
    auto cycleDetector = new FTypeCycleDetector(francaGeneratorExtensions);
    return !cycleDetector->hasCycle(types);
}

std::string FTypeGenerator::generateKeyType(const std::shared_ptr<FMapType> &fMap)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fMap->getKeyType()->isPolymorphic())
        return "std::shared_ptr<" + gen.getElementType(fMap->getKeyType(), nullptr, true) + ">";
    if (fMap->getKeyType()->getDerived() != nullptr)
        return gen.getFullName(fMap->getKeyType()->getDerived());
    else
        return gen.getElementType(fMap->getKeyType(), nullptr, true);
}
std::string FTypeGenerator::generateValueType(const std::shared_ptr<FMapType> &fMap)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (fMap->getValueType()->isPolymorphic())
        return "std::shared_ptr<" + gen.getElementType(fMap->getKeyType(), nullptr, true) + ">";
    if (fMap->getValueType()->getDerived() != nullptr)
        return gen.getFullName(fMap->getValueType()->getDerived());
    else
        return gen.getElementType(fMap->getValueType(), nullptr, true);
}

bool FTypeGenerator::hasImplementation(const std::shared_ptr<FType> &fType)
{
    if (std::dynamic_pointer_cast<FStructType>(fType))
        return std::dynamic_pointer_cast<FStructType>(fType)->isPolymorphic();
    return false;
}

bool FTypeGenerator::isdeprecated(const std::shared_ptr<FAnnotationBlock> &annotations)
{
    if (annotations == nullptr)
        return false;
    for (auto annotation : annotations->getElements())
    {
        if (annotation->getType().getValue() == FAnnotationType::DEPRECATED)
        {
            return true;
        }
    }
    return false;
}

std::string FTypeGenerator::breaktext(std::string text, const FAnnotationType &annotation)
{
    std::string commentBody = "";
    uint32_t startIndex = 0;
    uint32_t endIndex = 0;
    std::string commentText = text;
    while (commentText.find("\r\n") != commentText.npos)
    {
        commentText = commentText.replace(commentText.find("\r\n"), 2, "\n");
    }
    if (annotation.getValue() != FAnnotationType::DESCRIPTION)
        commentBody += " * " + annotation.getLiteral() + ": \n";
    auto size = commentText.size();
    while (startIndex < size)
    {
        endIndex = commentText.find("\n", startIndex);
        if (endIndex > commentText.length())
        {
            endIndex = size;
        }
        auto str = trim(commentText.substr(startIndex, (endIndex - startIndex)));
        commentBody += " * " + str + "\n";
        startIndex = endIndex + 1;
    }
    return commentBody;
}
std::string FTypeGenerator::generateComments(const std::shared_ptr<FModelElement> &model, bool inline_)
{
    std::string intro = "";
    std::string tail = "";
    std::string annoCommentText = "";
    if (model != nullptr && model->getComment() != nullptr)
    {
        if (!inline_)
        {
            intro = "/*\n";
            tail = " */";
        }
        for (auto annoComment : model->getComment()->getElements())
        {
            if (annoComment != nullptr)
            {
                auto type = annoComment->getType();
                annoCommentText += breaktext(annoComment->getComment(), type);
            }
        }
        return intro + annoCommentText + tail;
    }
    return "";
}

void FTypeGenerator::addRequiredHeaders(const std::shared_ptr<FType> &fType, std::list<std::string> &generatedHeaders,
                                        std::list<std::string> &libraryHeaders)
{
    FModelManager &mgr = FModelManager::getInstance();
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto headerPath = gen.getHeaderPath(std::dynamic_pointer_cast<FTypeCollection>(fType->getContainer()));
    addListItem(generatedHeaders, headerPath);
    addFTypeRequiredHeaders(fType, generatedHeaders, libraryHeaders);
}

std::string FTypeGenerator::getInitialValue(const std::shared_ptr<FEnumerationType> &_enumeration)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (_enumeration->getBase() != nullptr)
        return getInitialValue(_enumeration->getBase());
    return gen.getEnumPrefix() + _enumeration->getEnumerators().front()->getName();
}
std::string FTypeGenerator::generateLiterals(const std::shared_ptr<FEnumerationType> &_enumeration,
                                             std::string _backingType)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string literals;
    std::string comment;
    if (_enumeration->getBase() != nullptr)
        literals += generateLiterals(_enumeration->getBase(), _backingType) + ",\n";
    for (auto enumerator : _enumeration->getEnumerators())
    {
        comment = generateComments(enumerator, false);
        if (!comment.empty())
        {
            literals += comment + "\n";
        }
        literals +=
            gen.getEnumPrefix() + enumerator->getName() + " = " + gen.doCast(enumerator->valueToString(), _backingType);
        if (enumerator != _enumeration->getEnumerators().back())
        {
            literals += ",\n";
        }
    }
    return literals;
}

std::string FTypeGenerator::generateFConstDeclarations(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> literals;
    for (const auto &fconst : fTypeCollection->getConstants())
    {
        auto item = "static const " + gen.getTypeName(fconst, nullptr, false) + " " + fconst->getName() + " = " +
                    printInitializerExpression(fconst->getRhs()) + ";";
        literals.emplace_back(std::move(item));
    }
    return "\n" + join(literals, "\n");
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FTypeDef> &fTypeDef)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fTypeDef, false);
    str += "\ntypedef " + gen.getElementType(fTypeDef->getActualType(), fTypeDef, false) + " " + fTypeDef->getName() +
           ";\n";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FArrayType> &fArrayType)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fArrayType, false);
    if (fArrayType->getElementType()->getDerived() != nullptr &&
        std::dynamic_pointer_cast<FStructType>(fArrayType->getElementType()->getDerived()) != nullptr &&
        std::dynamic_pointer_cast<FStructType>(fArrayType->getElementType()->getDerived())->isPolymorphic())
        str += "\ntypedef std::vector<std::shared_ptr<" +
               gen.getElementType(fArrayType->getElementType(), fArrayType, true) + ">>" + fArrayType->getName() + ";";
    else
    {
        str += "\ntypedef std::vector<" + gen.getElementType(fArrayType->getElementType(), fArrayType, true) + "> " +
               fArrayType->getName() + ";";
    }
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FMapType> &fMap)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fMap, false);
    str +=
        "\ntypedef std::unordered_map<" + generateKeyType(fMap) + "," + generateValueType(fMap) + ">" + fMap->getName();
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FStructType> &fStructType)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::string str = generateComments(fStructType, false);
    if (fStructType->getBase() == nullptr)
    {
        str += "\nstruct " + fStructType->getName() + " \n{\n\t";
    }
    else
    {
        str += "\nstruct " + fStructType->getName() + " : public " +
               gen.getElementName(fStructType->getBase(), fStructType, false) + " \n{\n\t";
    }
    for (auto element : fStructType->getElements())
    {
        if (std::dynamic_pointer_cast<FStructType>(element->getType()->getDerived()) != nullptr &&
            gen.hasPolymorphicBase(std::dynamic_pointer_cast<FStructType>(element->getType()->getDerived())) &&
            !element->isArray())
            str += "std::shared_ptr<" + gen.getElementType(element->getType(), fStructType, false) + ">" +
                   element->getName() + ";\n\t";
        else
            str += gen.getTypeName(element, fStructType, false) + " " + element->getName() + ";\n\t";
    }
    str += fStructType->getName() + "()";
    if (fStructType->getBase())
    {
        str += ":" + fStructType->getBase()->getName() + "()";
    }
    str += "\n\t{\n\t\t";
    int n = 0;
    for (auto element : fStructType->getElements())
    {
        if (std::dynamic_pointer_cast<FEnumerationType>(element->getType()->getDerived()) != nullptr &&
            !element->isArray())
            str += element->getName() + " = " + gen.getTypeName(element, fStructType, false) + "::" +
                   getInitialValue(std::dynamic_pointer_cast<FEnumerationType>(element->getType()->getDerived())) +
                   ";\n\t\t";
        else if (element->getType()->getDerived() != nullptr && !element->isArray())
        {
            auto derived = element->getType()->getDerived();
            if (auto typeDef = std::dynamic_pointer_cast<FTypeDef>(derived))
            {
                if (typeDef->getActualType()->getPredefined())
                {
                    str += element->getName() + " = " + gen.generateDummyValue(typeDef->getActualType()) + ";\n\t\t";
                }
                else
                    str += element->getName() + " = " + gen.getTypeName(element, fStructType, false) + "();\n\t\t";
            }
            else
                str += element->getName() + " = " + gen.getTypeName(element, fStructType, false) + "();\n\t\t";
        }
        else if (element->getType()->getPredefined() != nullptr && !element->isArray())
            str += element->getName() + " = " + gen.generateDummyValue(element->getType()) + ";\n\t\t";
        else if (element->isArray())
            str += element->getName() + " = nullptr;\n\t\t";
        else
        {
            str += element->getName() + " = " + gen.getTypeName(element, fStructType, false) + "();\n\t\t";
        }
    }
    str.pop_back(); // remove last '\t'.
    str += "}\n\t";

    std::list<std::string> elements;
    std::list<std::string> params_initial_list;
    bool is_byte_buffer = false;
    for (auto element : gen.getAllElements(fStructType))
    {
        auto basic_type = element->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer = true;
        }

        if (element->isArray() || is_byte_buffer)
            elements.emplace_back(gen.getTypeName(element, fStructType, false) + " _" + element->getName());
        else
            elements.emplace_back(getConstReferenceVariable(element, fStructType));

        is_byte_buffer = false;
        params_initial_list.emplace_back(element->getName() + "(_" + element->getName() + ")");
    }
    str += fStructType->getName() + "(" + join(elements, ", ");
    if (elements.empty())
        str += ")\n\t";
    else
        str += ") : " + join(params_initial_list, ", ") + "\n\t";
    if (fStructType->getBase())
    {
        auto base = fStructType->getBase();
        str += "\t:" + base->getName() + "(";
        elements.clear();
        for (auto element : gen.getAllElements(base))
        {
            elements.emplace_back("_" + element->getName());
        }
        str += join(elements, ", ") + ")\n\t";
    }
    str += "{\n\t\t";
    if (fStructType->getBase())
    {
        for (auto element : fStructType->getElements())
        {
            str += element->getName() + "= _" + element->getName() + ";\n\t\t";
        }
    }

    str.pop_back(); // pop the last '\t'.
    str += "}\n\t";
    str += "~" + fStructType->getName() + "() = default;\n\t";
    str += fStructType->getName() + "(const " + fStructType->getName() + "&) = default;\n\t";
    str += fStructType->getName() + "(" + fStructType->getName() + "&&)noexcept = default;\n\t";
    str += fStructType->getName() + "& operator=(const " + fStructType->getName() + "&) = default;\n\t";
    str += fStructType->getName() + "& operator=(" + fStructType->getName() + "&&) = default;\n\t";

    str += "inline bool operator==(const " + fStructType->getName() + "& _other) const \n\t{\n\t";
    if (fStructType->getElements().size() > 0)
    {
        if (fStructType->getBase())
            str += "\treturn " + fStructType->getBase()->getName() + "::operator==(_other) && ";
        else
            str += "\treturn ";
        elements.clear();
        for (auto element : fStructType->getElements())
        {
            auto elementName = element->getName();
            elements.emplace_back(elementName + " == _other." + elementName);
        }
        str += join(elements, " && ") + ";\n\t}\n\t";
    }
    else
        str += "\t(void) _other;\n\t\treturn true;\n\t}\n\t";
    str += "inline bool operator!=(const " + fStructType->getName() +
           "&_other) const \n\t{\n\t\treturn !((*this) == _other);\n\t}\n};\n";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FEnumerationType> &fEnumerationType)
{
    return generateDeclaration(fEnumerationType, fEnumerationType);
}

std::string FTypeGenerator::generateDeclaration(const std::shared_ptr<FEnumerationType> &_enumeration,
                                                const std::shared_ptr<FModelElement> &_parent)
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
    auto backingType = gen.getPrimitiveTypeName(gen.getBackingType(_enumeration));
    auto content = generateLiterals(_enumeration, backingType);
    replace_all(content, "\n", "\n\t");
    str += "\nenum class " + _enumeration->getName() + " : " + backingType + " \n{\n\t" + content + "\n};\n";
    return str;
}

std::string FTypeGenerator::generateFTypeDeclaration(const std::shared_ptr<FUnionType> &fUnionType)
{
    std::string str = generateComments(fUnionType, false);
    str += "union " + fUnionType->getName() + "\n{\n";
    str += "\t" + replace_all(getElementNames(fUnionType), "\n", "\n\t");
    // add operator==
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    str += "inline bool operator==(const " + fUnionType->getName() + "& _other) const \n\t{\n\t";
    if (fUnionType->getElements().size() > 0)
    {
        std::list<std::string> elementEquals;
        for (auto element : gen.getAllElements(fUnionType))
        {
            elementEquals.emplace_back(element->getName() + " == _other." + element->getName());
        }
        str += "\treturn " + join(elementEquals, " && ") + ";\n\t}\n\t";
    }
    else
        str += "\t(void) _other;\n\t\treturn true;\n\t}\n\t";

    str.pop_back();
    str += "};\n";
    return str;
}

std::string FTypeGenerator::getElementNames(const std::shared_ptr<FUnionType> &fUnionType)
{

    std::string names;
    if (fUnionType->getBase())
    {
        names += getElementNames(fUnionType->getBase());
    }
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    for (const auto &iter : fUnionType->getElements())
    {
        names += gen.getTypeName(iter, fUnionType, false) + " " + iter->getName() + ";\n";
    }
    return names;
}

// focus
std::string FTypeGenerator::generateFTypeDeclarations(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    std::string str;
    for (auto type : fTypeCollection->getTypes())
    {
        if (std::dynamic_pointer_cast<FTypeDef>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FTypeDef>(type)));
        else if (std::dynamic_pointer_cast<FArrayType>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FArrayType>(type)));
        else if (std::dynamic_pointer_cast<FMapType>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FMapType>(type)));
        else if (std::dynamic_pointer_cast<FStructType>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FStructType>(type)));
        else if (std::dynamic_pointer_cast<FEnumerationType>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FEnumerationType>(type)));
        else if (std::dynamic_pointer_cast<FUnionType>(type) != nullptr)
            str.append("\n" + generateFTypeDeclaration(std::dynamic_pointer_cast<FUnionType>(type)));
    }
    if (std::dynamic_pointer_cast<FInterface>(fTypeCollection) != nullptr)
    {
        auto interface = std::dynamic_pointer_cast<FInterface>(fTypeCollection);
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        for (auto method : gen.getMethodsWithError(interface))
        {
            str += generateComments(method, false);
            auto errors = method->getErrors();
            str += generateDeclaration(errors, errors);
        }
        for (auto broadcast : interface->getBroadcasts())
        {
            str += generateComments(broadcast, false);
            str += gen.generateBroadcastCallback(broadcast);
        }
    }
    return str;
}

void FTypeGenerator::generateInheritanceIncludes(const std::shared_ptr<FInterface> &fInterface,
                                                 std::list<std::string> &generatedHeaders,
                                                 std::list<std::string> &libraryHeaders)
{
    if (fInterface->getBase() != nullptr)
    {
        FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
        auto stubHeaderPath = gen.getStubHeaderPath(fInterface->getBase());
        addListItem(generatedHeaders, stubHeaderPath);
    }
}

void FTypeGenerator::generateRequiredTypeIncludes(const std::shared_ptr<FInterface> &fInterface,
                                                  std::list<std::string> &generatedHeaders,
                                                  std::list<std::string> &libraryHeaders, bool isStub)
{
    auto ins = FrancaGeneratorExtensions::getInstance();

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
    while (std::find(generatedHeaders.begin(), generatedHeaders.end(), ins.getHeaderPath(fInterface)) !=
           generatedHeaders.end())
    {
        auto it = std::find(generatedHeaders.begin(), generatedHeaders.end(), ins.getHeaderPath(fInterface));
        generatedHeaders.erase(it);
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
} // namespace BstIdl