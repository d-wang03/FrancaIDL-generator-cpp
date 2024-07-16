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
%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.7"
%defines

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <stdio.h>
  #include <string>
  #include <memory>
  #include <climits>
  #include <experimental/filesystem>
  #include<stack>
  #include "FDModel/FDFactory.h"
  #include "utilities/list_utility.h"
  namespace BstIdl
  {
      class FDParserPrivate;
  }
  using namespace BstIdl;
  namespace fs = std::experimental::filesystem;
}

// The parsing context.
%param { FDParserPrivate& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "FDParserPrivate.h"
#include "FDModel/FDModelManager.h"
#include "model/FModelManager.h"
#include "FDModel/FDModelManager.h"
#include "utilities/path_utility.h"
std::shared_ptr<FDModel> fdModel;
std::string curOperation;
std::string curtypeDefination;
std::string curDeployments;
std::string curSpecificationRef;;
std::string curEnumValue;
std::stack<std::string> fieldPathStack;
FDParserPrivate * pParser = nullptr;

static inline void addProperty(const std::shared_ptr<FDElement> &element, const std::shared_ptr<FDProperty> &property)
{
    if (element->addProperty(property))
        property->setContainer(element);
    else
        throw yy::parser::syntax_error(pParser->location, "Duplicate property "+ property->getName());    
}
}

%token
    LPAREN  "("
    RPAREN  ")"
    LBRAN   "{"
    RBRAN   "}"
    LSQUR   "["
    RSQUR   "]"
    REF     "&"

    COMMA   ","
    COLON   ":"
    EXCL    "!"
    SEMICOLON ";"
    WAVE    "~"
    PERCENT "%"
    POW     "^"
    OR      "|"
    QUE     "?"
    ARRAW   "=>"
    LCOMMENT  "<**"
    RCOMMENT  "**>"
    POINT     "->"

    ASSIGNMENT   "="
    MINUS   "-"
    PLUS    "+"
    HASH_SIGN "#"
    
    PACKAGE         "package"
    IMPORT          "import"
    SPEC            "specification"
    EXTENDS         "extends"	
    FOR             "for"
    DEFINE          "define"
    AS              "as"
    USE             "use"
    OPTIONAL        "optional"
    DEFAULT         "default"
    TYPECOLLECTION  "typeCollection"
    ARRAY           "array"
    STRUCTOVERWRITES"#struct"
    UNIONOVERWRITES "#union"
    ENUMOVERWRITES  "#enumeration"
    STRUCT          "struct"
    UNION           "union"
    ENUMERATION     "enumeration"
    MAP             "map"
    KEY             "key"
    VALUE           "value"
    TYPEDEF         "typedef"
    ATTRIBUTE       "attribute"
    INTERFACE       "interface"
    METHOD          "method"
    BROADCAST       "broadcast"

    IN              "in"
    OUT             "out"

    INSTANCE        "instance"
;

%token <std::shared_ptr<FDBuiltInPropertyHost>>              PROPERTYHOST
%token <std::shared_ptr<FDPredefinedTypeId>>              PREDEFTYPE

%token <std::string>                                IMPORTED_FQN
%token <std::string>                                IMPORT_URI
%token <std::string>                                STRING
%token <std::string>                                ANNOTATION
%token <std::string>                                IDENTIFIER
%token <int>                                        NUMBER
%token <std::string>                                ID
%token <std::string>                                FQN
%token <double>                                     DOUBLE_CONSTANT
%token <float>                                      FLOAT_CONSTANT
%token <bool>                                       BOOL_CONSTANT
%token <int>                                        INTEGER_CONSTANT
%token <std::string>                                FQN_WITH_SELECTOR
; 
  

%nterm <std::shared_ptr<FDModel>>                   model
%nterm <std::shared_ptr<FDModel>>                   modelHead
%nterm <std::shared_ptr<FDImport>>                  imports
%nterm <std::shared_ptr<FDSpecification>>           specificationHead
%nterm <std::shared_ptr<FDSpecification>>           specificationBody
%nterm <std::shared_ptr<FDSpecification>>           specification
%nterm <std::shared_ptr<FDRootElement>>             deployment
%nterm <std::shared_ptr<FDAttribute>>               attribute
%nterm <std::shared_ptr<FDAttribute>>               attributeHead
%nterm <std::shared_ptr<FDArgument>>                argument
%nterm <std::shared_ptr<FDArgument>>                argumentHead
%nterm <std::shared_ptr<FDTypeDefinition>>          typeDefination
%nterm <std::shared_ptr<FDField>>                   field
%nterm <std::shared_ptr<FDField>>                   fieldHead
%nterm <std::shared_ptr<FDEnumeration>>             enumeration
%nterm <std::shared_ptr<FDEnumeration>>             enumerationHead
%nterm <std::shared_ptr<FDEnumValue>>               enumValue
%nterm <std::shared_ptr<FDEnumValue>>               enumValueHead
%nterm <std::shared_ptr<FDMapKey>>                  mapKey
%nterm <std::shared_ptr<FDMapKey>>                  mapKeyHead
%nterm <std::shared_ptr<FDMapValue>>                mapValue
%nterm <std::shared_ptr<FDMapValue>>                mapValueHead
%nterm <std::shared_ptr<FDDeclaration>>             declaration
%nterm <std::shared_ptr<FDDeclaration>>             declarationHead
%nterm <std::shared_ptr<FDDeclaration>>             declarationBody
%nterm <std::shared_ptr<FDPropertyDecl>>            propertyDecl
%nterm <std::shared_ptr<FDPropertyFlag>>            flags
%nterm <std::list<std::shared_ptr<FDPropertyFlag>>> flagsListHead
%nterm <std::shared_ptr<FDTypeRef>>                 typeRef
%nterm <std::shared_ptr<FDType>>                    complex
%nterm <std::shared_ptr<FDComplexValue>>            complexValue
%nterm <std::shared_ptr<FDEnumType>>                enumType
%nterm <std::shared_ptr<FDEnumType>>                enumTypeBody
%nterm <std::shared_ptr<FDExtensionType>>           extensionType
%nterm <std::shared_ptr<FDValue>>                   valueSingle
%nterm <std::shared_ptr<FDValueArray>>              valueArray
%nterm <std::shared_ptr<FDValueArray>>              valueArrayHead
%nterm <std::shared_ptr<FDValueArray>>              valueArrayBody
%nterm <std::shared_ptr<FDExtensionRoot>>           extensionRoot
%nterm <std::shared_ptr<FDExtensionRoot>>           extensionRootHead
%nterm <std::shared_ptr<FDExtensionRoot>>           extensionRootBody
%nterm <std::shared_ptr<FDExtensionElement>>        extensionElementHead
%nterm <std::shared_ptr<FDExtensionElement>>        extensionElementBody
%nterm <std::shared_ptr<FDExtensionElement>>        extensionElement
%nterm <std::shared_ptr<FDTypes>>                   types
%nterm <std::shared_ptr<FDTypes>>                   typesHead
%nterm <std::shared_ptr<FDTypes>>                   typesBody
%nterm <std::shared_ptr<FDInterface>>               interface
%nterm <std::shared_ptr<FDInterface>>               interfaceHead
%nterm <std::shared_ptr<FDInterface>>               interfaceBody
%nterm <std::shared_ptr<FDMethod>>                  method
%nterm <std::shared_ptr<FDMethod>>                  methodHead
%nterm <std::shared_ptr<FDBroadcast>>               broadcast
%nterm <std::shared_ptr<FDBroadcast>>               broadcastHead
%nterm <std::shared_ptr<FDTypeOverwrites>>          overwrites
%nterm <std::shared_ptr<FDTypeOverwrites>>          plainOverwrites
%nterm <std::shared_ptr<FDTypeOverwrites>>          plainOverwritesHead
%nterm <std::shared_ptr<FDStructOverwrites>>        structOverwrites
%nterm <std::shared_ptr<FDStructOverwrites>>        structOverwritesHead
%nterm <std::shared_ptr<FDUnionOverwrites>>         unionOverwrites
%nterm <std::shared_ptr<FDUnionOverwrites>>         unionOverwritesHead
%nterm <std::shared_ptr<FDEnumerationOverwrites>>   enumerationOverwrites
%nterm <std::shared_ptr<FDEnumerationOverwrites>>   enumerationOverwritesHead
%nterm <std::shared_ptr<FDArgumentList>>            arguments
%nterm <std::shared_ptr<FDArgumentList>>            argumentsHead
%nterm <std::shared_ptr<FDArgumentList>>            argumentsBody
%nterm <std::shared_ptr<FDArray>>                   arrayHead
%nterm <std::shared_ptr<FDArray>>                   array
%nterm <std::shared_ptr<FDCompound>>                compound
%nterm <std::shared_ptr<FDTypedef>>                 typeDefHead
%nterm <std::shared_ptr<FDTypedef>>                 typeDef
%nterm <std::shared_ptr<FDMap>>                     map
%nterm <std::shared_ptr<FDMap>>                     mapHead
%nterm <std::shared_ptr<FDStruct>>                  struct_
%nterm <std::shared_ptr<FDStruct>>                  structHead
%nterm <std::shared_ptr<FDUnion>>                   union_
%nterm <std::shared_ptr<FDUnion>>                   unionHead
%nterm <std::shared_ptr<FDProperty>>                property
%nterm <std::shared_ptr<FDInterfaceRef>>            interfaceRef
%%
%start model;


modelHead: "package" FQN  
{ 
    auto &ins = FDModelManager::getInstance();
    drv.modelName = $2;
    std::string modelName=ins.makeModelName(drv.modelName);
    drv.currentModel = ins.addModel(modelName);
    pParser = &drv;
}
| "package" ID 
{ 
    auto &ins = FDModelManager::getInstance();
    drv.modelName = $2;
    std::string modelName=ins.makeModelName(drv.modelName);
    drv.currentModel = ins.addModel(modelName);
    pParser = &drv;
}
;

model:
modelHead
{
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
}
|imports 
{
    drv.currentModel = FDModelManager::getInstance().addModel(FDModelManager::getInstance().makeModelName(drv.modelName));
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    addListItem(drv.currentModel->getImports(), std::move($1));
}
|specification 
{
    drv.currentModel = FDModelManager::getInstance().addModel(FDModelManager::getInstance().makeModelName(drv.modelName));
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    addListItem(drv.currentModel->getSpecifications(), std::move($1));
}
|deployment 
{
    drv.currentModel = FDModelManager::getInstance().addModel(FDModelManager::getInstance().makeModelName(drv.modelName));
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    $1->setContainer(drv.currentModel);
    addListItem(drv.currentModel->getDeployments(), std::move($1));
}
|model imports 
{
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    addListItem(drv.currentModel->getImports(), std::move($2));
}
|model specification 
{
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    $2->setContainer(drv.currentModel);
    addListItem(drv.currentModel->getSpecifications(), std::move($2));
}
|model deployment 
{
    if (drv.currentModel == nullptr)
        throw yy::parser::syntax_error(drv.location, "Invalid Package.");
    $2->setContainer(drv.currentModel);
    addListItem(drv.currentModel->getDeployments(), std::move($2));
}
;

imports: "import" STRING 
{
    $$=FDFactory::getInstance().createImport();
    auto filename = $2;
    if (filename.substr(0,9) == "platform:")
    {
        filename = CurrentAppPath::getInstance().parent_path().parent_path().string() + filename.substr(9);
    }

    auto currentFile = fs::system_complete(drv.file);
    auto path = fs::path(filename);
    if (fs::exists(currentFile) && !fs::exists(path))
    {
        path = fs::path(currentFile.parent_path().string() + "/" + path.string());
    }

    if (!fs::exists(path))
        throw yy::parser::syntax_error(drv.location, "Imported file does not exist:"+filename);
    auto abs_path = fs::system_complete(path).string();
    $$->setImportURI(abs_path);
    if(path.extension().string()==".fidl"){
        BstIdl::FModelManager &mgr=BstIdl::FModelManager::getInstance();
        if(!mgr.addPendingFidl(abs_path))
            throw yy::parser::syntax_error(drv.location, "Add FIDL pending file fail.");
    }
    else if(path.extension().string()==".fdepl")
    {
        FDModelManager &mgr=FDModelManager::getInstance();
        if(!mgr.addPendingModel(abs_path))
            throw yy::parser::syntax_error(drv.location, "Add FDEPL pending file fail.");
    }
    else
    {
        throw yy::parser::syntax_error(drv.location, "Unsupported import file type.");
    }
}
| "import" FQN 
{
    $$=FDFactory::getInstance().createImport();
    $$->setImportedSpec($2);
}
;

specificationHead:
"specification" FQN "extends" FQN
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addSpecificationExtendsRef($$,$4);
}
|"specification" FQN "extends" ID
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addSpecificationExtendsRef($$,$4);
}
|"specification" ID "extends" FQN
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addSpecificationExtendsRef($$,$4);
}
|"specification" ID "extends" ID
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addSpecificationExtendsRef($$,$4);
}
|"specification" FQN
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
}
|"specification" ID
{
    $$=FDFactory::getInstance().createSpecification();
    $$->setName($2);
}
;

specificationBody:specificationHead "{"
{
    $$=$1;
}
|specificationBody declaration
{
    addListItem($1->getDeclarations(), $2);
    $$=$1;
    $2->setContainer($1);
}
;

specification: specificationBody "}"
{
    $$=$1;
}
;

declarationHead:"for" PROPERTYHOST
{
    $$=FDFactory::getInstance().createFDDeclaration();
    $$->setHost(std::make_shared<FDPropertyHost>($2));
    $$->setName($2->getName());
}
|"for" ID
{
    $$=FDFactory::getInstance().createFDDeclaration();
    $$->setHost(std::make_shared<FDPropertyHost>($2));
    $$->setName($2);
}
;

declarationBody:declarationHead "{"
{
    $$=$1;
}
|declarationBody propertyDecl
{
    $$=$1;
    addListItem($$->getProperties(), $2);
    $2->setContainer($1);
}
;

declaration:declarationBody "}"
{
    $$=$1;
}
;

propertyDecl:
ID ":" typeRef flagsListHead ")" ";"
{
    $$=std::make_shared<FDPropertyDecl>();
    $$->setName($1);
    $$->setType($3);
    while(!$4.empty())
    {
        $$->setFlags($4.front());
        $4.pop_front();
    }
}
|ID ":" typeRef ";"
{
    $$=std::make_shared<FDPropertyDecl>();
    $$->setName($1);
    $$->setType($3);
}
;

typeRef:PREDEFTYPE "[" "]"
{
    $$=std::make_shared<FDTypeRef>();
    $$->setPredefined($1);
    $$->setArray("[]");
} 
|complex "[" "]"
{
    $$=std::make_shared<FDTypeRef>();
    $$->setComplex($1);
    $$->setArray("[]");
}
|PREDEFTYPE
{
    $$=std::make_shared<FDTypeRef>();
    $$->setPredefined($1);
}
|complex
{
    $$=std::make_shared<FDTypeRef>();
    $$->setComplex($1);
}
;

flagsListHead: "(" flags
{
    $$.emplace_back($2);
}
|flagsListHead "," flags
{
    $$.emplace_back($3);
}
;

flags:"optional"
{
    $$=std::make_shared<FDPropertyFlag>();
    $$->setOptional(true);
}
|"default" ":" complexValue
{
    $$=std::make_shared<FDPropertyFlag>();
    $$->setDefault($3);
}
;

complex:enumType
{
    $$=$1;
}
|extensionType
{
    $$=$1;
}
;

deployment:extensionRoot
{
    $$=$1;
}
|types
{
    $$=$1;
    curDeployments="";
    curSpecificationRef="";
}
|interface
{
    $$=$1;
    curDeployments="";
    curSpecificationRef="";
}
;

extensionRootHead:
"define" FQN "for" ID FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" ID FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" ID ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" ID ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" ID FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" ID FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" ID ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" ID ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" ID FQN 
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($5);
}
|"define" FQN "for" ID ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" ID FQN 
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" ID ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    refMgr.addExtensionRootTargetRef($$,$5);
    $$->setName($5);
}
|"define" FQN "for" ID "as" FQN 
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($6);
}
|"define" FQN "for" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($6);
}
|"define" ID "for" ID "as" FQN 
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($6);
}
|"define" ID "for" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($6);
}
|"define" FQN "for" ID 
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($4);
}
|"define" ID "for" ID
{
    curSpecificationRef=$2;
    curDeployments="";
    $$=FDFactory::getInstance().createFDExtensionRoot();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootSpecificationRef($$,$2);
    $$->setTag($4);
    $$->setName($4);
}
;

extensionRootBody:extensionRootHead "{"
{
    $$=$1;
}
|extensionRootBody "use" FQN
{
    $$=$1;
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootUseRef($$,$3);
}
|extensionRootBody "use" ID
{
    $$=$1;
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addExtensionRootUseRef($$,$3);
}
|extensionRootBody property
{
    $$=$1;
    addProperty($1,$2);
}
|extensionRootBody extensionElement
{
    $$=$1;
    $2->setContainer($1);
    addListItem($$->getElements(),std::move($2));
}
;

extensionRoot:extensionRootBody "}"
{
    $$=$1;
}

extensionElementHead:"instance" FQN "as" FQN
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    refMgr.addExtensionElementTargetRef($$,$2);
    $$->setName($4);
}
|"instance" FQN "as" ID
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    refMgr.addExtensionElementTargetRef($$,$2);
    $$->setName($4);
}
|"instance" ID "as" FQN
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    refMgr.addExtensionElementTargetRef($$,$2);
    $$->setName($4);
}
|"instance" ID "as" ID
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    refMgr.addExtensionElementTargetRef($$,$2);
    $$->setName($4);
}
|"instance" FQN
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    $$->setName($2);
    refMgr.addExtensionElementTargetRef($$,$2);
}
|"instance" ID
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    auto &refMgr = FDReferenceManager::getInstance();
    $$->setTag("instance");
    $$->setName($2);
    refMgr.addExtensionElementTargetRef($$,$2);
}
|"instance" "as" FQN
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    $$->setTag("instance");
    $$->setName($3);
}
|"instance" "as" ID
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    $$->setTag("instance");
    $$->setName($3);
}
|"instance"
{
    $$=FDFactory::getInstance().createFDExtensionElement();
    $$->setTag("instance");
    $$->setName("instance");
}
;

extensionElementBody:extensionElementHead "{" 
{
    $$=$1;
}
|extensionElementBody property
{
    $$=$1;
    addProperty($1,$2);
}
|extensionElementBody extensionElement
{
    $$=$1;
    $2->setContainer($1);
    addListItem($1->getElements(), $2);
}
;

extensionElement:extensionElementBody "}"
{
    $$=$1;
}
;

typesHead:
"define" FQN "for" "typeCollection" FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "typeCollection" FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "typeCollection" ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "typeCollection" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "typeCollection" FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "typeCollection" FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "typeCollection" ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "typeCollection" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "typeCollection" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($5);
}
|"define" FQN "for" "typeCollection" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" "typeCollection" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" "typeCollection" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDTypes();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesSpecificationRef($$,$2);
    refMgr.addTypesTypeCollectionRef($$,$5);
    $$->setName($5);
}
;
typesBody:typesHead "{"
{
    $$=$1;
}
|typesBody "use" FQN
{
    $$=$1;
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addTypesUseRef($$,$3);
}
|typesBody property
{
    $$=$1;
    addProperty($1,$2);
}
|typesBody typeDefination
{
    $$=$1;
    $$->setTypes($2);
    curtypeDefination="";
    $2->setContainer($1);
}
;

types:typesBody "}"
{
    $$=$1;
    curSpecificationRef="";
    curDeployments="";
}

interfaceHead:"define" FQN "for" "interface" FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "interface" FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "interface" ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "interface" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "interface" FQN "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "interface" FQN "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "interface" ID "as" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" ID "for" "interface" ID "as" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($7);
}
|"define" FQN "for" "interface" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($5);
}
|"define" FQN "for" "interface" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" "interface" FQN
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($5);
}
|"define" ID "for" "interface" ID
{
    curSpecificationRef=$2;
    curDeployments=$5;
    $$=FDFactory::getInstance().createFDInterface();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceSpecificationRef($$,$2);
    refMgr.addInterfaceInterfaceRef($$,$5);
    $$->setName($5);
}
;

interfaceBody:interfaceHead "{"
{
    if($1==nullptr)
        throw yy::parser::syntax_error(drv.location, "Interface head is null.");
    $$=$1;
}
|interfaceBody typeDefination
{
    $$=$1;
    $$->setTypes($2);
    curtypeDefination="";
    $2->setContainer($1);
}
|interfaceBody "use" FQN
{
    $$=$1;
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceUseRef($$,$3);
}
|interfaceBody "use" ID
{
    $$=$1;
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addInterfaceUseRef($$,$3);
}
|interfaceBody property
{
    $$=$1;
    addProperty($1,$2);
}
|interfaceBody attribute
{
    $$=$1;
    $$->setAttributes($2);
    $2->setContainer($1);
}
|interfaceBody method
{
    $$=$1;
    $$->setMethods($2);
    curOperation="";
    $2->setContainer($1);
}
|interfaceBody broadcast
{
    $$=$1;
    $$->setBroadcasts($2);
    curOperation="";
    $2->setContainer($1);
}
;

interface:interfaceBody "}"
{
    if($1==nullptr)
        throw yy::parser::syntax_error(drv.location, "Interface body is null.");
    $$=$1;
    curDeployments="";
    curSpecificationRef="";
}
;

attributeHead:"attribute" FQN "{"
{
    $$=std::make_shared<FDAttribute>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addAttributeAttributeRef($$,$2);
    fieldPathStack.push($2);
    $$->setName($2);
    curtypeDefination = fieldPathStack.top();
}
|"attribute" ID "{"
{
    $$=std::make_shared<FDAttribute>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addAttributeAttributeRef($$,curDeployments+"."+$2);
    fieldPathStack.push(curDeployments+"."+$2);
    $$->setName($2);
    curtypeDefination = fieldPathStack.top();
}
| attributeHead property
{
    $$=$1;
    addProperty($1,$2);  
}
| attributeHead overwrites
{
    $$=$1;
    $$->setOverwrites($2);
    $2->setContainer($1);    
}
;

attribute: attributeHead "}"
{
    $$=$1;
    fieldPathStack.pop();
}
;

methodHead:
"method" FQN_WITH_SELECTOR "{"
{
    curOperation=$2;
    $$=std::make_shared<FDMethod>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addMethodTargetRef($$,curDeployments + "." + curOperation);
    fieldPathStack.push(curDeployments + "." + curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|"method" FQN "{"
{
    curOperation=$2;
    $$=std::make_shared<FDMethod>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addMethodTargetRef($$,$2);
    fieldPathStack.push(curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|"method" ID "{"
{
    curOperation=curDeployments+"."+$2;
    $$=std::make_shared<FDMethod>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addMethodTargetRef($$,curOperation);
    fieldPathStack.push(curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|methodHead property
{
    $$=$1;
    addProperty($1,$2);   
}
;

method:
methodHead "in" arguments "out" arguments "}"
{
    $$=$1;
    $$->setInArguments($3);
    $$->setOutArguments($5);
    curOperation="";
    $3->setContainer($1);
    $5->setContainer($1);
}
|methodHead "in" arguments"}"
{
    $$=$1;
    $$->setInArguments($3);
    curOperation="";
    $3->setContainer($1);
}
|methodHead "out" arguments "}"
{
    $$=$1;
    $$->setOutArguments($3);
    curOperation="";
    $3->setContainer($1);
}
|methodHead "}"
{
    $$=$1;
    curOperation="";
}
;

broadcastHead:
"broadcast" FQN_WITH_SELECTOR "{"
{
    curOperation=$2;
    $$=std::make_shared<FDBroadcast>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addBroadcastTargetRef($$,curDeployments + "." + $2);
    fieldPathStack.push(curDeployments + "." + curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|"broadcast" FQN "{"
{
    curOperation=$2;
    $$=std::make_shared<FDBroadcast>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addBroadcastTargetRef($$,$2);
    fieldPathStack.push(curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|"broadcast" ID "{"
{
    curOperation=curDeployments+"."+$2;
    $$=std::make_shared<FDBroadcast>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addBroadcastTargetRef($$,curOperation);
    fieldPathStack.push(curOperation);
    $$->setName(curOperation);
    curtypeDefination = fieldPathStack.top();
}
|broadcastHead property
{
    $$ = $1;
    addProperty($1,$2);
}
;

broadcast:
broadcastHead "out" arguments "}"
{
    $$=$1;
    if ($1)
    {
        if ($3)
        {
            $$->setOutArguments($3);
            $3->setContainer($1);
        }
    }
    curOperation="";
}
|broadcastHead "}"
{
    $$=$1;
    curOperation="";
}
;

argumentsHead:"{"
{
    $$=std::make_shared<FDArgumentList>();
    $$->setName("FDArgumentList");
}
;

argumentsBody:argumentsHead argument
{
    $$=$1;
    $$->setArguments($2);
    $2->setContainer($$);
}
|argumentsBody argument
{
    $$=$1;
    $$->setArguments($2);
    $2->setContainer($$);
}
;

arguments:argumentsBody "}"
{
    $$=$1;
}
| argumentsHead "}"   
{
    $$=$1;
}
;
argumentHead:FQN "{"
{ 
    $$=std::make_shared<FDArgument>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    auto first_pos=($1).find_first_of(".");
    auto last_pos=($1).find_last_of(".");
    if(first_pos!=last_pos)
    {
        fieldPathStack.push($1);
        refMgr.addArgumentTargetRef($$,$1);
    }
    else
    {
        fieldPathStack.push(curDeployments+"."+$1);
        refMgr.addArgumentTargetRef($$,curDeployments+"."+$1);
    }
    curtypeDefination = fieldPathStack.top();
}
|ID "{"
{
    $$=std::make_shared<FDArgument>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    fieldPathStack.push(curtypeDefination+"."+$1);
    refMgr.addArgumentTargetRef($$,curtypeDefination+"."+$1);
    curtypeDefination = fieldPathStack.top();
}
|argumentHead property
{
    $$ = $1;
    addProperty($1,$2);
}
;
argument:argumentHead overwrites "}"
{
    $$=$1;
    $$->setOverwrites($2);
    fieldPathStack.pop();
    curtypeDefination = fieldPathStack.top();
    $2->setContainer($1);
}
|argumentHead "}"
{
    $$=$1;
    fieldPathStack.pop();
    curtypeDefination = fieldPathStack.top();
}
;

typeDefination:compound
{
    $$=$1;
    curtypeDefination="";
}
|array
{
    $$=$1;
    curtypeDefination="";
}
|enumeration
{
    $$=$1;
    curtypeDefination="";
}
|typeDef
{
    $$=$1;
    curtypeDefination="";
}
|map
{
    $$=$1;
    curtypeDefination="";
}
;

compound:struct_
{
    $$=$1;
}
|union_
{
    $$=$1;
}
;
arrayHead: "array"  FQN "{"
{
    fieldPathStack.push($2);
    curtypeDefination=$2;
    $$=std::make_shared<FDArray>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addArrayTargetRef($$,$2);
    $$->setName(curtypeDefination);
}
|"array"  ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDArray>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addArrayTargetRef($$,curtypeDefination);
    fieldPathStack.push(curtypeDefination);
    $$->setName($2);
}
|arrayHead property
{
    $$ = $1;
    addProperty($1,$2);
}
;
array: arrayHead overwrites "}"
{
    $$=$1;
    $$->setOverwrites($2);
    fieldPathStack.pop();
    $2->setContainer($1);
}
|arrayHead "}"
{
    $$=$1;
    fieldPathStack.pop();
}
;

structHead:"struct" FQN "{"
{
    curtypeDefination=$2;
    $$=std::make_shared<FDStruct>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDStructTargetRef($$,$2);
    fieldPathStack.push($2);
    $$->setName(curtypeDefination);
}
|"struct" ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDStruct>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDStructTargetRef($$,curtypeDefination);
    fieldPathStack.push(curtypeDefination);
    $$->setName($2);
}
| structHead property
{
    $$ = $1;
    addProperty($1,$2);
}
|structHead field
{
    $$=$1;
    $$->setFields($2);
    $2->setContainer($1);
}
;

struct_: structHead "}"
{
    $$=$1;
    fieldPathStack.pop();
}
;

unionHead:"union" FQN "{"
{
    curtypeDefination=$2;
    $$=std::make_shared<FDUnion>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDUnionTargetRef($$,$2);
    fieldPathStack.push($2);
    $$->setName(curtypeDefination);
}
|"union" ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDUnion>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDUnionTargetRef($$,curtypeDefination);
    fieldPathStack.push(curtypeDefination);
    $$->setName($2);
}
|unionHead property
{
    $$ = $1;
    addProperty($1,$2);
}
|unionHead field
{
    $$=$1;
    $$->setFields($2);
    $2->setContainer($$);
}
;
union_: unionHead "}"
{
    $$=$1;
    fieldPathStack.pop();
}
;

typeDefHead: "typedef" FQN "{"
{
    curtypeDefination=$2;
    $$=std::make_shared<FDTypedef>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDTypedefTargetRef($$,$2);
    $$->setName(curtypeDefination);
}
| "typedef" ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDTypedef>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDTypedefTargetRef($$,curtypeDefination);
    $$->setName($2);
}
| typeDefHead property
{
    $$ = $1;
    addProperty($1,$2);
}
;
typeDef: typeDefHead "}"
{
    $$ = $1;
}
;

fieldHead:FQN "{"
{
    $$=std::make_shared<FDField>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    int point_cnt=count($1.begin(),$1.end(),'.');
    if(point_cnt>=2)
    {
        fieldPathStack.push($1);
        refMgr.addFDFieldTargetRef($$,$1);
    }
    else
    {
        fieldPathStack.push(curDeployments+"."+$1);
        refMgr.addFDFieldTargetRef($$,curDeployments+"."+$1);
    }
    curtypeDefination = fieldPathStack.top();
}
|ID "{"
{
    $$=std::make_shared<FDField>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    if (fieldPathStack.empty())
        throw yy::parser::syntax_error(drv.location, "Invalid fieldPathStack.");
    fieldPathStack.push(fieldPathStack.top()+"."+$1);
    refMgr.addFDFieldTargetRef($$,fieldPathStack.top());
    curtypeDefination = fieldPathStack.top();
}
| fieldHead property
{
    $$ = $1;
    addProperty($1,$2);
}
| fieldHead overwrites
{
    $$=$1;
    $$->setOverwrites($2);
    $2->setContainer($1);    
}
;

field: fieldHead "}"
{
    $$=$1;
    fieldPathStack.pop();
}
;

enumerationHead: "enumeration" FQN "{"
{
    curtypeDefination=$2;
    $$=std::make_shared<FDEnumeration>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDEnumerationTargetRef($$,$2);
    $$->setName(curtypeDefination);
}
|"enumeration" ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDEnumeration>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDEnumerationTargetRef($$,curtypeDefination);
    $$->setName($2);
}
| enumerationHead property
{
    $$=$1;
    addProperty($1,$2);
}
| enumerationHead enumValue
{
    $$=$1;
    $$->setEnumerators($2);
    $2->setContainer($1);
}
;

enumeration: enumerationHead "}"
{
    $$=$1;
    curtypeDefination="";
}
;

enumValueHead:FQN "{"
{
    $$=std::make_shared<FDEnumValue>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDEnumValueTargetRef($$,$1);
    int point_cnt=count($1.begin(),$1.end(),'.');
    if(point_cnt>=2)
    {
        refMgr.addFDEnumValueTargetRef($$,$1);
    }  
    else
    {
        refMgr.addFDEnumValueTargetRef($$,curDeployments+"."+$1);
    }
}
|ID "{"
{
    $$=std::make_shared<FDEnumValue>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDEnumValueTargetRef($$,curtypeDefination+"."+$1);
}
| enumValueHead property
{
    $$=$1;
    addProperty($1,$2);    
}
;

enumValue: enumValueHead "}"
{
    $$=$1;
}
;

mapHead:"map" FQN "{"
{
    curtypeDefination=$2;
    $$=std::make_shared<FDMap>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDMapTargetRef($$,$2);
    $$->setName(curtypeDefination);
}
|"map" ID "{"
{
    curtypeDefination=curDeployments+"."+$2;
    $$=std::make_shared<FDMap>();
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDMapTargetRef($$,curtypeDefination);
    $$->setName($2);
}
| mapHead property
{
    $$=$1;
    addProperty($1,$2);    
}
;

map: mapHead mapKey mapValue "}"
{
    $$=$1;
    $$->setKey($2);
    $$->setValue($3);
    $2->setContainer($1);
    $3->setContainer($1);
}
|mapHead mapKey "}"
{
    $$=$1;
    $$->setKey($2);
}
|mapHead mapValue "}"
{
    $$=$1;
    $$->setValue($2);
}
|mapHead "}"
{
    $$=$1;
}
;

mapKeyHead: "key" "{"
{
    $$=std::make_shared<FDMapKey>();
}
| mapKeyHead property
{
    $$=$1;
    addProperty($1,$2);  
}
;

mapKey: mapKeyHead "}"
{
    $$ = $1;
}
;

mapValueHead:"value" "{" 
{
    $$=std::make_shared<FDMapValue>();
}
| mapValueHead property
{
    $$ = $1;
    addProperty($1,$2);  
}
;

mapValue: mapValueHead "}"
{
    $$ = $1;
}
;

plainOverwritesHead: "#" "{" 
{
    $$=std::make_shared<FDPlainTypeOverwrites>();
}
|plainOverwritesHead property
{
    $$ = $1;
    addProperty($1,$2);  
}
;

plainOverwrites: plainOverwritesHead "}"
{
    $$ = $1;
}
;

overwrites: plainOverwrites
{
    $$=$1;
}
|structOverwrites
{
    $$=$1;
}
|unionOverwrites
{
    $$=$1;
}
|enumerationOverwrites
{
    $$=$1;
}
;

structOverwritesHead: "#struct" "{" 
{
    $$=std::make_shared<FDStructOverwrites>();
}
| structOverwritesHead property
{
    $$ = $1;
    addProperty($1,$2);  
}
|structOverwritesHead field
{
    $$=$1;
    $$->setFields($2);
    $2->setContainer($1);
}
;

structOverwrites:structOverwritesHead "}"
{
    $$=$1;
}
;

unionOverwritesHead: "#union" "{"
{
    $$=std::make_shared<FDUnionOverwrites>();
}
| unionOverwritesHead property
{
    $$ = $1;
    addProperty($1,$2);  
}
|unionOverwritesHead field
{
    $$=$1;
    $$->setFields($2);
    $2->setContainer($1);
}
;

unionOverwrites:unionOverwritesHead "}"
{
    $$=$1;
}
;

enumerationOverwritesHead: "#enumeration"  "{"
{
    $$=std::make_shared<FDEnumerationOverwrites>();
}
| enumerationOverwritesHead property
{
    $$ = $1;
    addProperty($1,$2);  
}
| enumerationOverwritesHead enumValue
{
    $$=$1;
    $$->setEnumerators($2);
    $2->setContainer($1);
}
;

enumerationOverwrites:enumerationOverwritesHead "}"
{
    $$=$1;
}
;

property:ID "=" complexValue
{
    $$=std::make_shared<FDProperty>();
    $$->setName($1);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDPropertyDeclRef($$,curSpecificationRef+"."+$1);
    $$->setValue($3);
    $3->setContainer($$);
}
;

complexValue:valueSingle
{
    $$=std::make_shared<FDComplexValue>();
    $$->setSingle($1);
    $1->setContainer($$);
}
|valueArray
{
    $$=std::make_shared<FDComplexValue>();
    $$->setArray($1);
    $1->setContainer($$);
}
;

valueSingle:"-" INTEGER_CONSTANT
{
    auto integer=std::make_shared<FDInteger>();
    integer->setValue(-$2);
    integer->setName(std::to_string(-$2));
    $$=integer;
}
|"+" INTEGER_CONSTANT
{
    auto integer=std::make_shared<FDInteger>();
    integer->setValue($2);
    integer->setName(std::to_string($2));
    $$=integer;
}
|INTEGER_CONSTANT
{
    auto integer=std::make_shared<FDInteger>();
    integer->setValue($1);
    integer->setName(std::to_string($1));
    $$=std::static_pointer_cast<FDValue>(integer);
}
|STRING
{
    auto newString=std::make_shared<FDString>();
    newString->setValue($1);
    newString->setName($1);
    $$=newString;
}
|BOOL_CONSTANT
{
    auto tmp=std::make_shared<FDBoolean>();
    std::string value = $1 ? "true" : "false";
    tmp->setName(value);
    tmp->setValue(value);
    $$=tmp;
}
|interfaceRef
{
    $$=$1;
}
|ID
{
    auto generic=std::make_shared<FDGeneric>();
    auto enumrator=std::make_shared<FDEnumerator>();
    enumrator->setName($1);
    generic->setValue(enumrator);
    generic->setName($1);
    $$=generic;
}
;//FDGeneric

interfaceRef:"&" FQN
{
    $$=std::make_shared<FDInterfaceRef>();
    $$->setName($2);
    auto &refMgr = FDReferenceManager::getInstance();
    refMgr.addFDInterfaceValueRef($$,$2);
}
;

valueArrayHead: "{"
{
    $$=std::make_shared<FDValueArray>();
}
;

valueArrayBody: valueArrayHead valueSingle
{
    $$=$1;
    $$->setValues($2);
}
|valueArrayBody "," valueSingle
{
    $$=$1;
    $$->setValues($3);
}
;

valueArray:valueArrayBody "}"
{
    $$=$1;
}

enumTypeBody:"{" ID
{
    $$=std::make_shared<FDEnumType>();
    auto enumrator=std::make_shared<FDEnumerator>();
    enumrator->setName($2);
    $$->setEnumerators(enumrator);
}
|enumTypeBody "," ID
{
    $$=$1;
    auto enumrator=std::make_shared<FDEnumerator>();
    enumrator->setName($3);
    $$->setEnumerators(enumrator);
}
;
enumType:enumTypeBody "}"
{
    $$=$1;
}
;

extensionType:ID
{
    $$=std::make_shared<FDExtensionType>();
    $$->setName($1);
}
;


%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << "Syntax error! " << l << ": " << m << '\n';
}
