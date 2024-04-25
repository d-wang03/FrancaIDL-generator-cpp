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
%define api.prefix {bst}
%define api.namespace {bst}
//%file-prefix "bst"

%code requires {
  #include <string>
  #include <memory>
  #include <climits>
  #include <experimental/filesystem>
  #include "model/FFactory.h"
  namespace BstIdl
  {
      class FidlParserPrivate;
  }
  using namespace BstIdl;
}

// The parsing context.
%param { FidlParserPrivate& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "FidlParserPrivate.h"
#include "model/FModelManager.h"
std::shared_ptr<FModel> s_model;
}


%token
    ASSIGN  "="
    MINUS   "-"
    PLUS    "+"
    STAR    "*"
    SLASH   "/"
    LPAREN  "("
    RPAREN  ")"
    LBRAN   "{"
    RBRAN   "}"
    LSQUR   "["
    RSQUR   "]"
    LE      "<="
    GE      ">="
    LESS    "<"
    GREATER ">"
    EQ      "=="
    NE      "!="
    COMMA   ","
    COLON   ":"
    EXCL    "!"
    WAVE    "~"
    PERCENT "%"
    POW     "^"
    OR      "||"
    QUE     "?"
    ARRAW   "=>"
    AND       "&&"
    LCOMMENT  "<**"
    RCOMMENT  "**>"
    POINT     "->"
    
    PACKAGE     "package"
    INTERFACE   "interface"
    EXTENDS     "extends"
    MANAGES     "manages"
    VERSION     "version"
    MAJOR       "major"
    MINOR       "minor"
    ATTRIBUTE   "attribute"
    READONLY    "readonly"
    NOREAD      "noRead"
    NOSUBS      "noSubscriptions"
    METHOD      "method"
    FORGET      "fireAndForget"
    IN          "in"
    OUT         "out"
    ERROR       "error"
    BROADCAST   "broadcast"
    SELECTIVE   "selective"
    TYPECOLLECTION     "typeCollection"
    STRUCT      "struct"
    POLY        "polymorphic"
    ENUM        "enumeration"
    MAP         "map"
    TO          "to"
    ARRAY       "array"
    OF          "of"
    TYPEDEF     "typedef"
    IS          "is"
    UNION       "union"
    MININT      "minInt"
    MAXINT      "maxInt"
    CONST       "const"
    INTEGER     "Integer"
    IMPORT      "import"
    FROM        "from"
    MODEL       "model"
    PUBLIC      "public"
    ERRORDEF    "errordef"
    ERRORVAL    "errorval"
;

%token <std::shared_ptr<FBasicTypeId>>              BASIC_TYPE
%token <std::string>                                SL_COMMENT
%token <std::string>                                IMPORTED_FQN
%token <std::string>                                IMPORT_URI
%token <std::string>                                STRING
%token <std::string>                                ANNOTATION
%token <std::string>                                IDENTIFIER "identifier"
%token <int> NUMBER                                 "number"
%token <std::string>                                ID
%token <std::string>                                FQN
%token <std::shared_ptr<FDoubleConstant>>           DOUBLE_CONSTANT
%token <std::shared_ptr<FFloatConstant>>            FLOAT_CONSTANT
%token <std::shared_ptr<FBooleanConstant>>          BOOL_CONSTANT
%token <std::shared_ptr<FIntegerConstant>>          INTEGER_CONSTANT    

%nterm <std::shared_ptr<FImport>>                    imports
%nterm <std::shared_ptr<FTypeCollection>>           typeCollection
%nterm <std::shared_ptr<FTypeCollection>>           typeCollectionHead
%nterm <std::shared_ptr<FInterface>>                interface
%nterm <std::shared_ptr<FInterface>>                interfaceHead
%nterm <std::shared_ptr<FInterface>>                interfaceBody

%nterm <std::shared_ptr<FAnnotationBlock>>          annotationBlock
%nterm <std::shared_ptr<FAnnotationBlock>>          annotationBlockHead
%nterm <std::shared_ptr<FVersion>>                  version
%nterm <std::shared_ptr<FConstantDef>>              constantDef
%nterm <std::shared_ptr<FAttribute>>                attribute
%nterm <std::shared_ptr<FMethod>>                   method
%nterm <std::shared_ptr<FMethod>>                   methodHead
%nterm <std::shared_ptr<FBroadcast>>                broadcast
%nterm <std::shared_ptr<FBroadcast>>                broadcastHead

%nterm <std::list<std::shared_ptr<FArgument>>>      in_args
%nterm <std::list<std::shared_ptr<FArgument>>>      in_args_head
%nterm <std::list<std::shared_ptr<FArgument>>>      out_args
%nterm <std::list<std::shared_ptr<FArgument>>>      out_args_head
%nterm <std::shared_ptr<FArgument>>                 argument
%nterm <std::shared_ptr<FEnumerationType>>          err_enum
%nterm <std::shared_ptr<FEnumerationType>>          err_enum_head                   

%nterm <std::shared_ptr<FTypeRef>>                  typeRef
%nterm <std::shared_ptr<FInitializerExpression>>    initializerExpression
%nterm <std::shared_ptr<FInitializer>>              initializer
%nterm <std::shared_ptr<FCompoundInitializer>>      compoundInitializer
%nterm <std::shared_ptr<FCompoundInitializer>>      compoundInitializerHead
%nterm <std::shared_ptr<FBracketInitializer>>       bracketInitializer
%nterm <std::shared_ptr<FBracketInitializer>>       bracketInitializerHead
%nterm <std::shared_ptr<FFieldInitializer>>         fieldInitializer
%nterm <std::shared_ptr<FElementInitializer>>       elementInitializer

%nterm <std::shared_ptr<FType>>                     usertype
%nterm <std::shared_ptr<FArrayType>>                arrayType
%nterm <std::shared_ptr<FEnumerationType>>          enumType
%nterm <std::shared_ptr<FEnumerationType>>          enumTypeHead
%nterm <std::shared_ptr<FEnumerationType>>          enumTypeBody
%nterm <std::shared_ptr<FEnumerator>>               enumerator
%nterm <std::shared_ptr<FStructType>>               structType
%nterm <std::shared_ptr<FStructType>>               structTypeHead
%nterm <std::shared_ptr<FStructType>>               structTypeBody
%nterm <std::shared_ptr<FUnionType>>                unionType
%nterm <std::shared_ptr<FUnionType>>                unionTypeHead
%nterm <std::shared_ptr<FUnionType>>                unionTypeBody
%nterm <std::shared_ptr<FMapType>>                  mapType
%nterm <std::shared_ptr<FTypeDef>>                  typeDef
%nterm <std::shared_ptr<FBasicTypeId>>              predefined
%nterm <std::shared_ptr<FIntegerInterval>>          interval
%nterm <std::shared_ptr<FField>>                    field

%nterm <std::shared_ptr<FOperator>>                 operatorUnary
%nterm <std::shared_ptr<FOperator>>                 operatorMultiplicative
%nterm <std::shared_ptr<FOperator>>                 operatorAdditive
%nterm <std::shared_ptr<FOperator>>                 operatorRelational
%nterm <std::shared_ptr<FOperator>>                 operatorEquality
%nterm <std::shared_ptr<FOperator>>                 operatorAnd
%nterm <std::shared_ptr<FOperator>>                 operatorOr

%nterm <std::shared_ptr<FConstant>>                 const_expression
%nterm <std::shared_ptr<FQualifiedElementRef>>      qualifiedElementRef
%nterm <std::shared_ptr<FExpression>>               simplePrimaryExpression
%nterm <std::shared_ptr<FUnaryOperation>>           unaryOperation
%nterm <std::shared_ptr<FExpression>>               primaryExpression
%nterm <std::shared_ptr<FExpression>>               multiplicativeExpression
%nterm <std::shared_ptr<FExpression>>               additiveExpression
%nterm <std::shared_ptr<FExpression>>               relationalExpression
%nterm <std::shared_ptr<FExpression>>               equalityExpression
%nterm <std::shared_ptr<FExpression>>               logicalAndExpression
%nterm <std::shared_ptr<FExpression>>               logicalOrExpression

%%
%start model;


model: "package" FQN  
{ 
    drv.currentModel = FModelManager::getInstance().addModel($2);
    drv.currentModel->setFilenames(drv.filename);
    drv.currentInterface = nullptr;
    drv.currentTypeCollection = nullptr;
}
| "package" ID 
{ 
    drv.currentModel = FModelManager::getInstance().addModel($2);
    drv.currentModel->setFilenames(drv.filename);
    drv.currentInterface = nullptr;
    drv.currentTypeCollection = nullptr;
}
| model imports 
{
    if (drv.currentModel == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid Package.");
    addListItem(drv.currentModel->getImports(), std::move($2));
}
| model typeCollection 
{
    if (drv.currentModel == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid Package.");
    $2->setContainer(drv.currentModel);
    addListItem(drv.currentModel->getTypeCollections(), std::move($2));
}
| model interface 
{
    if (drv.currentModel == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid Package.");
    $2->setContainer(drv.currentModel);
    addListItem(drv.currentModel->getInterfaces(), std::move($2));
}
;

imports: "import" IMPORTED_FQN "from" STRING 
{
    $$=FFactory::getInstance().createImport();
    $$->setImportedNamespace($2);

    auto path = std::experimental::filesystem::path($4);
    if (!std::experimental::filesystem::exists(path))
        throw bst::parser::syntax_error(drv.location, "Imported file does not exist.");
    auto abs_path = std::experimental::filesystem::system_complete(path).string();
    $$->setImportURI(abs_path);
    BstIdl::FModelManager &mgr=BstIdl::FModelManager::getInstance();
    if(!mgr.addPendingFidl(abs_path))
        throw bst::parser::syntax_error(drv.location, "add pending file fail.");
}
| "import" FQN "from" STRING 
{
    $$=FFactory::getInstance().createImport();
    $$->setImportedNamespace($2);
    auto path = std::experimental::filesystem::path($4);
    if (!std::experimental::filesystem::exists(path))
        throw bst::parser::syntax_error(drv.location, "Imported file does not exist.");
    auto abs_path = std::experimental::filesystem::system_complete(path).string();
    $$->setImportURI(abs_path);
    BstIdl::FModelManager &mgr=BstIdl::FModelManager::getInstance();
    if(!mgr.addPendingFidl(abs_path))
        throw bst::parser::syntax_error(drv.location, "add pending file fail.");
}
| "import" ID "from" STRING 
{
    $$=FFactory::getInstance().createImport();
    $$->setImportedNamespace($2);
    auto path = std::experimental::filesystem::path($4);
    if (!std::experimental::filesystem::exists(path))
        throw bst::parser::syntax_error(drv.location, "Imported file does not exist.");
    auto abs_path = std::experimental::filesystem::system_complete(path).string();
    $$->setImportURI(abs_path);
    BstIdl::FModelManager &mgr=BstIdl::FModelManager::getInstance();
    if(!mgr.addPendingFidl(abs_path))
        throw bst::parser::syntax_error(drv.location, "add pending file fail.");
}
| "import" "model" STRING 
{
    $$=FFactory::getInstance().createImport();
    auto path = std::experimental::filesystem::path($3);
    if (!std::experimental::filesystem::exists(path))
        throw bst::parser::syntax_error(drv.location, "Imported file does not exist.");
    auto abs_path = std::experimental::filesystem::system_complete(path).string();
    $$->setImportURI(abs_path);
    BstIdl::FModelManager &mgr=BstIdl::FModelManager::getInstance();
    if(!mgr.addPendingFidl(abs_path))
        throw bst::parser::syntax_error(drv.location, "add pending file fail.");
}
;

typeCollectionHead: annotationBlock "typeCollection" ID "{" 
{
    auto typeCollection=FFactory::getInstance().createFTypeCollection();
    typeCollection->setName($3);
    typeCollection->setComment($1);
    $$ = typeCollection;
    drv.currentTypeCollection = typeCollection;
}
| "typeCollection" ID "{" 
{
    auto typeCollection=FFactory::getInstance().createFTypeCollection();
    typeCollection->setName($2);
    $$ = typeCollection;
    drv.currentTypeCollection = typeCollection;
}
| typeCollectionHead version 
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid typeCollection.");
    if ($1->getVersion() != nullptr)
        throw bst::parser::syntax_error(drv.location, "Duplicate version definition in typeCollection.");
    $1->setVersion($2);
    $$ = $1;
}
| typeCollectionHead constantDef 
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid typeCollection.");
    $2->setContainer($1);
    addListItem($1->getConstants(), std::move($2));
    $$ = $1;
}
| typeCollectionHead usertype 
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid typeCollection.");
    $2->setContainer($1);
    addListItem($1->getTypes(), std::move($2));
    $$ = $1;
}
;

typeCollection
: typeCollectionHead "}" 
{
    $$ = $1;
    drv.currentTypeCollection = nullptr;
};

interface
: interfaceBody "}"
{
    if($1==nullptr)
        throw bst::parser::syntax_error(drv.location, "Interface is a null ptr.");
    $$ = $1;
    drv.currentInterface = nullptr;
}

interfaceHead
: annotationBlock "interface" ID
{
    $$ = FFactory::getInstance().createFInterface();
    $$->setComment($1);
    $$->setName($3);
    drv.currentInterface = $$;
    drv.hasExtends = false;
    drv.hasManages = false;
}
| "interface" ID
{
    $$ = FFactory::getInstance().createFInterface();
    $$->setName($2);
    drv.currentInterface = $$;
    drv.hasExtends = false;
    drv.hasManages = false;
}
| interfaceHead "extends" ID
{
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate Interface extends.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceExtendsRef($1,fqn);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| interfaceHead "extends" FQN
{
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate Interface extends.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceExtendsRef($1,fqn);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| interfaceHead "manages" ID
{
    if (drv.hasManages)
        throw bst::parser::syntax_error(drv.location, "Duplicate Interface manages.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceManagesRef($1,fqn);
    }
    $$ = $1;
    drv.hasManages = true;
}
| interfaceHead "manages" FQN
{
    if (drv.hasManages)
        throw bst::parser::syntax_error(drv.location, "Duplicate Interface manages.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceManagesRef($1,fqn);
    }
    $$ = $1;
    drv.hasManages = true;
}
| interfaceHead "," ID
{
    if (!drv.hasManages)
        throw bst::parser::syntax_error(drv.location, "Unexpected Interface manages.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceManagesRef($1,fqn);
    }
    $$ = $1;
}
| interfaceHead "," FQN
{
    if (!drv.hasManages)
        throw bst::parser::syntax_error(drv.location, "Unexpected Interface manages.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addInterfaceManagesRef($1,fqn);
    }
    $$ = $1;
}
;

interfaceBody
: interfaceHead "{"
{
    if($1==nullptr)
        throw bst::parser::syntax_error(drv.location, "****Interface is a null ptr.");
    $$ = $1;
    drv.hasExtends = false;
    drv.hasManages = false;
}
| interfaceBody version
{
    if ($1->getVersion() != nullptr)
        throw bst::parser::syntax_error(drv.location, "Duplicate version definition in typeCollection.");
    $1->setVersion($2);
    $$ = $1;
}
| interfaceBody constantDef
{
    $2->setContainer($1);
    addListItem($1->getConstants(), std::move($2));
    $$ = $1;
}
| interfaceBody usertype
{
    $2->setContainer($1);
    addListItem($1->getTypes(), std::move($2));
    $$ = $1;
}
| interfaceBody attribute
{
    $2->setContainer($1);
    addListItem($1->getAttributes(), std::move($2));
    $$ = $1;
}
| interfaceBody method
{
    $2->setContainer($1);
    addListItem($1->getMethods(), std::move($2));
    $$ = $1;
}
| interfaceBody broadcast
{
    $2->setContainer($1);
    addListItem($1->getBroadcasts(), std::move($2));
    $$ = $1;
}
;

annotationBlockHead: "<**" ANNOTATION 
{
    auto block = FFactory::getInstance().createFAnnotationBlock();
    auto anno = std::make_shared<FAnnotation>($2);
    if (anno->getType().getValue() == FAnnotationType::UNDEFINED)
        throw bst::parser::syntax_error(drv.location, "Invalid annotation.");
    addListItem(block->getElements(), std::move(anno));
    $$ = block;
}
| annotationBlockHead ANNOTATION 
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid annotationBlock.");
    auto anno = std::make_shared<FAnnotation>($2);
    if (anno->getType().getValue() == FAnnotationType::UNDEFINED)
        throw bst::parser::syntax_error(drv.location, "Invalid annotation.");
    addListItem($1->getElements(), std::move(anno));
    $$ = $1;
}
;

version: "version" "{" "major" INTEGER_CONSTANT "minor" INTEGER_CONSTANT "}" 
{
    auto ver = FFactory::getInstance().createFVersion();
    ver->setMajor($4->getUnsignedVal());
    ver->setMinor($6->getUnsignedVal());
    $$ = ver;
};

annotationBlock: annotationBlockHead "**>" ;

constantDef: annotationBlock "const" typeRef "[" "]" ID "=" initializerExpression 
{
    auto constant = FFactory::getInstance().createFConstantDef();
    constant->setComment($1);
    constant->setType($3);
    constant->setArray(true);
    constant->setName($6);
    constant->setRhs($8);
    $$ = constant;
}
| annotationBlock "const" typeRef ID "=" initializerExpression 
{
    auto constant = FFactory::getInstance().createFConstantDef();
    constant->setComment($1);
    constant->setType($3);
    constant->setName($4);
    constant->setRhs($6);
    $$ = constant;
}
| "const" typeRef "[" "]" ID "=" initializerExpression 
{
    auto constant = FFactory::getInstance().createFConstantDef();
    constant->setArray(true);
    constant->setType($2);
    constant->setName($5);
    constant->setRhs($7);
    $$ = constant;
}
| "const" typeRef ID "=" initializerExpression 
{
    auto constant = FFactory::getInstance().createFConstantDef();
    constant->setType($2);
    constant->setName($3);
    constant->setRhs($5);
    $$ = constant;
}
;

usertype: arrayType 
{
    $$ = std::static_pointer_cast<FType>($1);
}
| enumType 
{
    $$ = std::static_pointer_cast<FType>($1);
}
| structType 
{
    $$ = std::static_pointer_cast<FType>($1);
}
| unionType 
{
    $$ = std::static_pointer_cast<FType>($1);
}
| mapType 
{ 
    $$ = std::static_pointer_cast<FType>($1);
}
| typeDef 
{
    $$ = std::static_pointer_cast<FType>($1);
}
;

attribute
: annotationBlock "attribute" typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFAttribute();
    $$->setComment($1);
    $$->setType($3);
    $$->setArray(true);
    $$->setName($6);
}
| annotationBlock "attribute" typeRef ID
{
    $$ = FFactory::getInstance().createFAttribute();
    $$->setComment($1);
    $$->setType($3);
    $$->setName($4);
}
| "attribute" typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFAttribute();
    $$->setType($2);
    $$->setArray(true);
    $$->setName($5);
}
| "attribute" typeRef ID
{
    $$ = FFactory::getInstance().createFAttribute();
    $$->setType($2);
    $$->setName($3);
}
| attribute "readonly"
{
    if ($1->isReadonly() || $1->isNoRead())
        throw bst::parser::syntax_error(drv.location, "1111Only one of readonly/noRead/noSubscriptions can be set.");
    $1->setReadonly(true);
    $$ = $1;
}
| attribute "noRead"
{
    if ($1->isReadonly() || $1->isNoRead())
        throw bst::parser::syntax_error(drv.location, "22222Only one of readonly/noRead/noSubscriptions can be set.");
    $1->setNoRead(true);
    $$ = $1;
}
| attribute "noSubscriptions"
{
    if ($1->isNoSubscriptions())
        throw bst::parser::syntax_error(drv.location, "33333Only one of readonly/noRead/noSubscriptions can be set.");
    $1->setNoSubscriptions(true);
    $$ = $1;
}
;

methodHead
: annotationBlock "method" ID ":" ID "fireAndForget" "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setComment($1);
    $$->setName($3);
    $$->setSelector($5);
    $$->setFireAndForget(true);
    drv.hasErrorSection = false;
}
| annotationBlock "method" ID "fireAndForget" "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setComment($1);
    $$->setName($3);
    $$->setFireAndForget(true);
    drv.hasErrorSection = false;
}
| annotationBlock "method" ID ":" ID "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setComment($1);
    $$->setName($3);
    $$->setSelector($5);
    drv.hasErrorSection = false;
}
| annotationBlock "method" ID "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setComment($1);
    $$->setName($3);
    drv.hasErrorSection = false;
}
| "method" ID ":" ID "fireAndForget" "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setName($2);
    $$->setSelector($4);
    $$->setFireAndForget(true);
    drv.hasErrorSection = false;
}
| "method" ID "fireAndForget" "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setName($2);
    $$->setFireAndForget(true);
    drv.hasErrorSection = false;
}
| "method" ID ":" ID "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setName($2);
    $$->setSelector($4);
    drv.hasErrorSection = false;
}
| "method" ID "{"
{
    $$ = FFactory::getInstance().createFMethod();
    $$->setName($2);
    drv.hasErrorSection = false;
}
| methodHead in_args
{
    if (!$1->getInArgs().empty())
        throw bst::parser::syntax_error(drv.location, "Duplicate Method in arg section.");
    for(auto arg:$2)
    {
        arg->setContainer($1);
    }
    $1->getInArgs() = std::move($2);
    $$= $1;
}
| methodHead out_args
{
    if (!$1->getOutArgs().empty())
        throw bst::parser::syntax_error(drv.location, "Duplicate Method out arg section.");
    for(auto arg:$2)
    {
        arg->setContainer($1);
    }
    $1->getOutArgs() = std::move($2);
    $$ = $1;
}
| methodHead "error" ID
{
    //FEnumerationType
    if (drv.hasErrorSection)
        throw bst::parser::syntax_error(drv.location, "Duplicate Method error section.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addMethodErrorRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
}
| methodHead "error" FQN
{
    //FEnumerationType
    if (drv.hasErrorSection)
        throw bst::parser::syntax_error(drv.location, "Duplicate Method error section.");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addMethodErrorRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
}
| methodHead "error" err_enum
{
    if (drv.hasErrorSection)
        throw bst::parser::syntax_error(drv.location, "Duplicate Method error section.");
    $1->setErrors($3);
    $$ = $1;
    $3->setContainer($1);
    $3->setName($1->getName() + "Error");
}
;

method : methodHead "}"
{
    drv.hasErrorSection = false;
    $$ = $1;
}

broadcastHead
: annotationBlock "broadcast" ID ":" ID "selective" "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setComment($1);
    $$->setName($3);
    $$->setSelector($5);
    $$->setSelective(true);
}
| annotationBlock "broadcast" ID ":" ID "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setComment($1);
    $$->setName($3);
    $$->setSelector($5);
}
| annotationBlock "broadcast" ID "selective" "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setComment($1);
    $$->setName($3);
    $$->setSelective(true);
}
| annotationBlock "broadcast" ID  "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setComment($1);
    $$->setName($3);
}
| "broadcast" ID ":" ID "selective" "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setName($2);
    $$->setSelector($4);
    $$->setSelective(true);
}
| "broadcast" ID ":" ID "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setName($2);
    $$->setSelector($4);
}
| "broadcast" ID "selective" "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setName($2);
    $$->setSelective(true);
}
| "broadcast" ID "{"
{
    $$ = FFactory::getInstance().createFBroadcast();
    $$->setName($2);
}
| broadcastHead out_args
{
    if (!$1->getOutArgs().empty())
        throw bst::parser::syntax_error(drv.location, "Duplicate broadcast out arg section.");
    for(auto arg:$2)
    {
        arg->setContainer($1);
    }
    $1->getOutArgs() = std::move($2);
    $$ = $1;
}
;

broadcast : broadcastHead "}"{
    $$=$1;
};

in_args_head
: "in" "{"
{
    $$ = std::list<std::shared_ptr<FArgument>>();
}
| in_args_head argument
{
    addListItem($1, std::move($2));
    $$ = std::move($1);
}
;

in_args : in_args_head "}"
{
    $$ = std::move($1);
}
;

out_args_head
: "out" "{"
{
    $$ = std::list<std::shared_ptr<FArgument>>();
}
| out_args_head argument
{
    addListItem($1, std::move($2));
    $$ = std::move($1);
}
;

out_args : out_args_head "}"
{
    $$ = std::move($1);
}
;

argument
: annotationBlock typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFArgument();
    $$->setComment($1);
    $$->setType($2);
    $$->setArray(true);
    $$->setName($5);
}
| annotationBlock typeRef ID
{
    $$ = FFactory::getInstance().createFArgument();
    $$->setComment($1);
    $$->setType($2);
    $$->setName($3);
}
| typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFArgument();
    $$->setType($1);
    $$->setArray(true);
    $$->setName($4);
}
| typeRef ID
{
    $$ = FFactory::getInstance().createFArgument();
    $$->setType($1);
    $$->setName($2);
}
;

err_enum_head
: "extends" ID "{"
{
    $$ = FFactory::getInstance().createFEnumerationType();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($2))
    {
        refMgr.addEnumerationTypeExtendsRef($$,fqn,drv.currentInterface);
    }
}
| "extends" FQN "{"
{
    $$ = FFactory::getInstance().createFEnumerationType();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($2))
    {
        refMgr.addEnumerationTypeExtendsRef($$,fqn,drv.currentInterface);
    }
}
| "{"
{
    $$ = FFactory::getInstance().createFEnumerationType();
}
| err_enum_head enumerator
{
    addListItem($1->getEnumerators(), std::move($2));
    $$ = $1;
}
;

err_enum : err_enum_head "}";

typeRef: predefined 
{
    auto ref = FFactory::getInstance().createFTypeRef();
    ref->setPredefined($1);
    $$ = ref;
}
| interval 
{
    auto ref = FFactory::getInstance().createFTypeRef();
    ref->setInterval($1);
    $$ = ref;
}
| ID 
{
    auto ref = FFactory::getInstance().createFTypeRef();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($1))
    {
        refMgr.addTypeRefDerivedRef(ref,fqn,drv.currentInterface);
    }
    $$ = ref;
}
| FQN
{
    auto ref = FFactory::getInstance().createFTypeRef();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($1))
    {
        refMgr.addTypeRefDerivedRef(ref,fqn,drv.currentInterface);
    }
    $$ = ref;
}
;

predefined: BASIC_TYPE;

interval: "Integer" 
{
    $$ = FFactory::getInstance().createFIntegerInterval();
    $$->setName("Integer");
}
|"Integer" "(" "minInt" "," "maxInt"  ")" 
{
    $$ = FFactory::getInstance().createFIntegerInterval();
    $$->setLowerBound(LLONG_MIN);
    $$->setUpperBound(LLONG_MAX);
    $$->setName("Integer");
}
|"Integer" "(" INTEGER_CONSTANT "," "maxInt"  ")" 
{
    $$ = FFactory::getInstance().createFIntegerInterval();
    $$->setLowerBound($3->getVal());
    $$->setUpperBound(LLONG_MAX);
    $$->setName("Integer");
}
|"Integer" "(" "minInt" "," INTEGER_CONSTANT  ")" 
{
    $$ = FFactory::getInstance().createFIntegerInterval();
    $$->setLowerBound(LLONG_MIN);
    $$->setUpperBound($5->getVal());
    $$->setName("Integer");
}
|"Integer" "(" INTEGER_CONSTANT "," INTEGER_CONSTANT  ")" 
{
    $$ = FFactory::getInstance().createFIntegerInterval();
    $$->setLowerBound($3->getUnsignedVal());
    $$->setUpperBound($5->getUnsignedVal());
    $$->setName("Integer");
}
;

arrayType: annotationBlock "public" "array" ID "of" typeRef 
{
    auto arrayType = FFactory::getInstance().createFArrayType();
    arrayType->setComment($1);
    arrayType->setPublic(true);
    arrayType->setName($4);
    arrayType->setElementType($6);
    $$ = arrayType;
}
| "public" "array" ID "of" typeRef 
{
    auto arrayType = FFactory::getInstance().createFArrayType();
    arrayType->setPublic(true);
    arrayType->setName($3);
    arrayType->setElementType($5);
    $$ = arrayType;
}
| annotationBlock "array" ID "of" typeRef 
{
    auto arrayType = FFactory::getInstance().createFArrayType();
    arrayType->setComment($1);
    arrayType->setName($3);
    arrayType->setElementType($5);
    $$ = arrayType;
}
| "array" ID "of" typeRef 
{
    auto arrayType = FFactory::getInstance().createFArrayType();
    arrayType->setName($2);
    arrayType->setElementType($4);
    $$ = arrayType;
}
;

typeDef: annotationBlock "public" "typedef" ID "is" typeRef 
{
    auto typeDef = FFactory::getInstance().createFTypeDef();
    typeDef->setComment($1);
    typeDef->setPublic(true);
    typeDef->setName($4);
    typeDef->setActualType($6);
    $$ = typeDef;
}
| "public" "typedef" ID "is" typeRef 
{
    auto typeDef = FFactory::getInstance().createFTypeDef();
    typeDef->setPublic(true);
    typeDef->setName($3);
    typeDef->setActualType($5);
    $$ = typeDef;
}
| annotationBlock "typedef" ID "is" typeRef 
{
    auto typeDef = FFactory::getInstance().createFTypeDef();
    typeDef->setComment($1);
    typeDef->setName($3);
    typeDef->setActualType($5);
    $$ = typeDef;
}
| "typedef" ID "is" typeRef 
{
    auto typeDef = FFactory::getInstance().createFTypeDef();
    typeDef->setName($2);
    typeDef->setActualType($4);
    $$ = typeDef;
}
;

structTypeHead: annotationBlock "public" "struct" ID 
{
    auto tmp = FFactory::getInstance().createFStructType();
    tmp->setComment($1);
    tmp->setPublic(true);
    tmp->setName($4);
    $$ = tmp;
    drv.hasExtends = false;
}
| annotationBlock "struct" ID
{
    auto tmp = FFactory::getInstance().createFStructType();
    tmp->setComment($1);
    tmp->setName($3);
    $$ = tmp;
    drv.hasExtends = false;
}
| "public" "struct" ID
{
    auto tmp = FFactory::getInstance().createFStructType();
    tmp->setPublic(true);
    tmp->setName($3);
    $$ = tmp;
    drv.hasExtends = false;
}
| "struct" ID
{
    auto tmp = FFactory::getInstance().createFStructType();
    tmp->setName($2);
    $$ = tmp;
    drv.hasExtends = false;
}
| structTypeHead "extends" ID 
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid struct.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addStructTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| structTypeHead "extends" FQN
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid struct.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addStructTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| structTypeHead "polymorphic"
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid struct.");
    if ($1->isPolymorphic() == true)
        throw bst::parser::syntax_error(drv.location, "Duplicate polymorphic");
    $1->setPolymorphic(true);
    $$ = $1;
}
; 

structTypeBody
: structTypeHead "{"
{
    $$ = $1;
    drv.hasExtends = false;
}
| structTypeBody field
{
    $$ = $1;
    $2->setContainer($1);
    addListItem($$->getElements(), std::move($2));
}
;

structType: structTypeBody "}";

unionTypeHead
: annotationBlock "public" "union" ID
{
    $$ = FFactory::getInstance().createFUnionType();
    $$->setComment($1);
    $$->setPublic(true);
    $$->setName($4);
    drv.hasExtends = false;
}
| annotationBlock "union" ID
{
    $$ = FFactory::getInstance().createFUnionType();
    $$->setComment($1);
    $$->setName($3);
    drv.hasExtends = false;
}
| "public" "union" ID
{
    $$ = FFactory::getInstance().createFUnionType();
    $$->setPublic(true);
    $$->setName($3);
    drv.hasExtends = false;
}
| "union" ID
{
    $$ = FFactory::getInstance().createFUnionType();
    $$->setName($2);
    drv.hasExtends = false;
}
| unionTypeHead "extends" ID
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid union.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addUnionTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| unionTypeHead "extends" FQN
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid union.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addUnionTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
;

unionTypeBody
: unionTypeHead "{"
{
    $$ = $1;
    drv.hasExtends = false;
}
| unionTypeBody field
{
    $2->setContainer($1);
    addListItem($1->getElements(), std::move($2));
    $$ = $1;
}
;

unionType : unionTypeBody "}";

enumTypeHead
: annotationBlock "public" "enumeration" ID 
{
    $$ = FFactory::getInstance().createFEnumerationType();
    $$->setComment($1);
    $$->setPublic(true);
    $$->setName($4);
    drv.hasExtends = false;
}
| annotationBlock "enumeration" ID
{
    $$ = FFactory::getInstance().createFEnumerationType();
    $$->setComment($1);
    $$->setName($3);
    drv.hasExtends = false;
}
| "public" "enumeration" ID
{
    $$ = FFactory::getInstance().createFEnumerationType();
    $$->setPublic(true);
    $$->setName($3);
    drv.hasExtends = false;
}
| "enumeration" ID
{
    $$ = FFactory::getInstance().createFEnumerationType();
    $$->setName($2);
    drv.hasExtends = false;
}
| enumTypeHead "extends" ID
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid union.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addEnumerationTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
| enumTypeHead "extends" FQN
{
    if ($1 == nullptr)
        throw bst::parser::syntax_error(drv.location, "Invalid union.");
    if (drv.hasExtends)
        throw bst::parser::syntax_error(drv.location, "Duplicate extends");
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($3))
    {
        refMgr.addEnumerationTypeExtendsRef($1,fqn,drv.currentInterface);
    }
    $$ = $1;
    drv.hasExtends = true;
}
;

enumTypeBody
: enumTypeHead "{"
| enumTypeBody enumerator
{
    $2->setContainer($1);
    addListItem($1->getEnumerators(), std::move($2));
    $$ = $1;
}
;

enumType : enumTypeBody "}";

enumerator
: annotationBlock ID  "=" additiveExpression
{
    $$ = FFactory::getInstance().createFEnumerator();
    $$->setComment($1);
    $$->setName($2);
    $$->setValue($4);
}
| annotationBlock ID
{
    $$ = FFactory::getInstance().createFEnumerator();
    $$->setComment($1);
    $$->setName($2);
}
| ID "=" additiveExpression
{
    $$ = FFactory::getInstance().createFEnumerator();
    $$->setName($1);
    $$->setValue($3);
}
| ID
{
    $$ = FFactory::getInstance().createFEnumerator();
    $$->setName($1);
}
;

mapType
: annotationBlock "public" "map" ID "{" typeRef "to" typeRef "}"
{
    $$ = FFactory::getInstance().createFMapType();
    $$->setComment($1);
    $$->setPublic(true);
    $$->setName($4);
    $$->setKeyType($6);
    $$->setValueType($8);
}
| annotationBlock "map" ID "{" typeRef "to" typeRef "}"
{
    $$ = FFactory::getInstance().createFMapType();
    $$->setComment($1);
    $$->setName($3);
    $$->setKeyType($5);
    $$->setValueType($7);
}
| "public" "map" ID "{" typeRef "to" typeRef "}"
{
    $$ = FFactory::getInstance().createFMapType();
    $$->setPublic(true);
    $$->setName($3);
    $$->setKeyType($5);
    $$->setValueType($7);
}
| "map" ID "{" typeRef "to" typeRef "}"
{
    $$ = FFactory::getInstance().createFMapType();
    $$->setName($2);
    $$->setKeyType($4);
    $$->setValueType($6);
}
;

field
: annotationBlock typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFField();
    $$->setComment($1);
    $$->setType($2);
    $$->setArray(true);
    $$->setName($5);
}
| annotationBlock typeRef ID
{
    $$ = FFactory::getInstance().createFField();
    $$->setComment($1);
    $$->setType($2);
    $$->setName($3);
}
| typeRef "[" "]" ID
{
    $$ = FFactory::getInstance().createFField();
    $$->setType($1);
    $$->setArray(true);
    $$->setName($4);
}
| typeRef ID
{
    $$ = FFactory::getInstance().createFField();
    $$->setType($1);
    $$->setName($2);
}
;

logicalOrExpression
: logicalAndExpression operatorOr logicalAndExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
| logicalAndExpression
;

logicalAndExpression
: equalityExpression operatorAnd equalityExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
|equalityExpression
;

equalityExpression
: relationalExpression operatorEquality relationalExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
|relationalExpression
;

relationalExpression
: additiveExpression operatorRelational additiveExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
|additiveExpression
;

additiveExpression
: multiplicativeExpression operatorAdditive multiplicativeExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
|multiplicativeExpression
;

multiplicativeExpression
: primaryExpression operatorMultiplicative primaryExpression
{
    auto ret = FFactory::getInstance().createFBinaryOperation();
    ret->setLeft($1);
    ret->setOp($2);
    ret->setRight($3);
    $$ = std::static_pointer_cast<FExpression>(ret);
}
|primaryExpression
;

primaryExpression
: unaryOperation { $$ = std::static_pointer_cast<FExpression>($1);}
| simplePrimaryExpression
;

unaryOperation
: operatorUnary simplePrimaryExpression
{
    $$ = FFactory::getInstance().createFUnaryOperation();
    $$->setOp($1);
    $$->setOperand($2);
}
;

simplePrimaryExpression
:qualifiedElementRef { $$ = std::static_pointer_cast<FExpression>($1);}
|const_expression { $$ = std::static_pointer_cast<FExpression>($1);}
|"(" logicalOrExpression ")" { $$ = $2; }
;

qualifiedElementRef
:ID{
    $$ = FFactory::getInstance().createFQualifiedElementRef();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($1))
    {
        refMgr.addQerElementRef($$,fqn,drv.currentInterface);
    }
}
|FQN{
    $$ = FFactory::getInstance().createFQualifiedElementRef();
    auto &refMgr = ReferenceManager::getInstance();
    for (auto fqn : drv.getPossibleFQN($1))
    {
        refMgr.addQerElementRef($$,fqn,drv.currentInterface);
    }
}
|qualifiedElementRef "->" ID
{
    $$ = FFactory::getInstance().createFQualifiedElementRef();
    $$->setQualifier($1);
    auto field = FFactory::getInstance().createFField();
    field->setName($3);
    $$->setField(std::move(field));
}
;

const_expression
: INTEGER_CONSTANT
{
    $$=std::static_pointer_cast<FConstant>($1);
}
| STRING
{
    FFactory &factory = FFactory::getInstance();
    auto stringConst=factory.createFStringConstant();
    stringConst->setVal($1);
    $$=stringConst;
}
| DOUBLE_CONSTANT
{
    $$=std::static_pointer_cast<FConstant>($1);
}
| BOOL_CONSTANT
{
    $$=std::static_pointer_cast<FConstant>($1);
}
| FLOAT_CONSTANT
{
    $$=std::static_pointer_cast<FConstant>($1);
}
;

operatorOr
: "||" { $$ = FOperator::get("||");}
;

operatorAnd
: "&&" { $$ = FOperator::get("&&");}
;

operatorEquality
: "==" { $$ = FOperator::get("==");}
| "!=" { $$ = FOperator::get("!=");}
;

operatorRelational
: "<" { $$ = FOperator::get("<");}
| "<=" { $$ = FOperator::get("<=");}
| ">" { $$ = FOperator::get(">");}
| ">=" { $$ = FOperator::get(">=");}
;

operatorAdditive
: "+" { $$ = FOperator::get("+");}
| "-" { $$ = FOperator::get("-");}
;

operatorMultiplicative
: "*" { $$ = FOperator::get("*");}
| "/" { $$ = FOperator::get("/");}
;

operatorUnary
: "!" { $$ = FOperator::get("!");}
| "-" { $$ = FOperator::get("-");}
;

initializerExpression
: logicalOrExpression 
{
    $$ = std::static_pointer_cast<FInitializerExpression>($1);
}
|initializer 
{
    $$ = std::static_pointer_cast<FInitializerExpression>($1);
}
;

initializer
: compoundInitializer
{
    $$ = std::static_pointer_cast<FInitializer>($1);
}
| bracketInitializer
{
    $$ = std::static_pointer_cast<FInitializer>($1);
}
;

compoundInitializer
: compoundInitializerHead "}"
;

compoundInitializerHead
: "{" 
{
    $$ = FFactory::getInstance().createFCompoundInitializer();
}
|"{" fieldInitializer
{
    $$ = FFactory::getInstance().createFCompoundInitializer();
    addListItem($$->getElements(), std::move($2));
}
| compoundInitializerHead "," fieldInitializer
{
    addListItem($1->getElements(), std::move($3));
    $$ = $1;
}
;

fieldInitializer
: ID ":" initializerExpression
{
    $$ = FFactory::getInstance().createFFieldInitializer();
    //build field from id;
    auto field = FFactory::getInstance().createFField();
    field->setName($1);
    $$->setElement(field);
    $$->setValue($3);
}
;

bracketInitializer
: bracketInitializerHead "]"
;

bracketInitializerHead
: "[" 
{
    $$ = FFactory::getInstance().createFBracketInitializer();
}
| "[" elementInitializer
{
    $$ = FFactory::getInstance().createFBracketInitializer();
    addListItem($$->getElements(), std::move($2));
}
| bracketInitializerHead "," elementInitializer
{
    addListItem($1->getElements(), std::move($3));
    $$ = $1;
}
;

elementInitializer
: initializerExpression
{
    $$ = FFactory::getInstance().createFElementInitializer();
    $$->setFirst($1);
}
| initializerExpression "=>" initializerExpression
{
    $$ = FFactory::getInstance().createFElementInitializer();
    $$->setFirst($1);
    $$->setSecond($3);
}
;

%%

void
bst::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
