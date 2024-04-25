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
#include "model/FFactory.h"
#include "parser/FidlParser.h"
#include "utilities/string_utility.h"
#include <gtest/gtest.h>

TEST(ParserTestLT, ErrorTest)
{
    BstIdl::FidlParser parser;
    auto fileName = std::string("../newidlFile/MethodError.fidl");
    auto ret = parser.parse(fileName);
    EXPECT_EQ(ret.size(), 2);
}

TEST(ParserTestLT, InterfaceTest)
{
    BstIdl::FidlParser parser;
    auto fileName = std::string("../newidlFile/InterfaceExtends.fidl");
    auto ret = parser.parse(fileName);
    EXPECT_EQ(ret.size(), 2);
}

TEST(ParserTestLT, PublicTypeTest)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
    typeCollection TC {
        public array MyArrayI of Integer(1, 10)
        public typedef MyString is String
        public struct MyStruct {
            Boolean [] fields
        }
        public union MyUnion {
            String fieldS
        }
        public enumeration MyEnum {
            EnumeratorA
            EnumeratorB
        }
    }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getTypeCollections().size(), 1U);
    auto tc = ret->getTypeCollections().front();
    EXPECT_STREQ(tc->getName().c_str(), "TC");
    auto types = tc->getTypes();
    EXPECT_EQ(types.size(), 5U);
    auto MyArrayI = types.front();
    EXPECT_EQ(MyArrayI->isPublic(), true);
    EXPECT_STREQ(MyArrayI->getName().c_str(), "MyArrayI");
    types.pop_front();
    auto MyString = types.front();
    EXPECT_EQ(MyString->isPublic(), true);
    EXPECT_STREQ(MyString->getName().c_str(), "MyString");
    types.pop_front();
    auto MyStruct = types.front();
    EXPECT_EQ(MyStruct->isPublic(), true);
    EXPECT_STREQ(MyStruct->getName().c_str(), "MyStruct");
    types.pop_front();
    auto MyUnion = types.front();
    EXPECT_EQ(MyUnion->isPublic(), true);
    EXPECT_STREQ(MyUnion->getName().c_str(), "MyUnion");
    types.pop_front();
    auto MyEnum = types.front();
    EXPECT_EQ(MyEnum->isPublic(), true);
    EXPECT_STREQ(MyEnum->getName().c_str(), "MyEnum");
}

TEST(ParserTestLT, ConstantExpressionTest)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
    typeCollection TC {
        const Boolean MyBoolE = false
        const Boolean MyBoolK = 1 || 0
        const String MyStr = "10.12"
        const Integer MyInteger = -10 - 5 
        const Boolean MyBoolA = 1 == 0
        const Boolean MyBoolB = 1 != 0
        const Boolean MyBoolC = 1 / 0
        const Boolean MyBoolD = 1 && 0
        const Boolean MyBoolF = 1 < 0
        const Boolean MyBoolG = 1 <= 0
        const Boolean MyBoolH = 1 > 0
        const Boolean MyBoolI = 1 >= 0
        const Boolean MyBoolJ = !false
        const Double MyDouble = 101200.22d
        const Float MyFloat = 10.12f + 3.2f
    }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    auto tc = ret->getTypeCollections().front();
    auto consts = tc->getConstants();
    EXPECT_EQ(consts.size(), 15U);
    auto MyFloat = consts.back();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FBinaryOperation>(MyFloat->getRhs()), nullptr);
    consts.pop_back();
    auto MyDouble = consts.back();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FDoubleConstant>(MyDouble->getRhs()), nullptr);
    consts.pop_back();
    auto MyBoolE = consts.front();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FBooleanConstant>(MyBoolE->getRhs()), nullptr);
    consts.pop_front();
    consts.pop_front();
    auto MyStr = consts.front();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FStringConstant>(MyStr->getRhs()), nullptr);
    consts.pop_front();
    auto MyInt = consts.front();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FBinaryOperation>(MyInt->getRhs()), nullptr);
    consts.pop_back();
    auto MyBoolJ = consts.back();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FBinaryOperation>(MyBoolJ->getRhs()), nullptr);
}

TEST(ParserTestLT, AnnotationTest_tc)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            <** @author : Lt **>
            typeCollection TC {

            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getTypeCollections().size(), 1U);
    auto tc = ret->getTypeCollections().front();
    EXPECT_STREQ(tc->getName().c_str(), "TC");
    auto comment = tc->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@author : Lt ");
}

TEST(ParserTestLT, AnnotationTest_interface)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            <** @details : MyInterface is my Interface. **>
            interface MyInterface {
                const Boolean ifPublic = true
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto comment = inf->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@details : MyInterface is my Interface. ");
}

TEST(ParserTestLT, AnnotationTest_constantDef)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @param : ifPublic means if MyInterface is public or not. **>
                const Boolean ifPublic = true

                <** @param : nums means an array of integer num. **>
                const Integer[] nums = [1, 2, 3]

                const Integer[] ids = [2, 3, 43]
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto consts = inf->getConstants();
    auto comment = consts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(),
                 "@param : ifPublic means if MyInterface is public or not. ");
    consts.pop_front();
    comment = consts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@param : nums means an array of integer num. ");
}

TEST(ParserTestLT, AnnotationTest_attribute)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @param : ifPublic means if MyInterface is public or not. **>
                attribute Boolean ifPublic 

                <** @param : nums means an array of integer num. **>
                attribute Integer[] nums 

                attribute Integer[] ids 
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto attrs = inf->getAttributes();
    auto comment = attrs.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(),
                 "@param : ifPublic means if MyInterface is public or not. ");
    attrs.pop_front();
    comment = attrs.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@param : nums means an array of integer num. ");
}

TEST(ParserTestLT, AnnotationTest_method)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @see : MyMethodA **>
                method MyMethodA {
            
                }
                <** @see : MyMethodC fireAndForget **>
                method MyMethodC fireAndForget {
            
                }
                method MyMethodD fireAndForget {
            
                }
                <** @see : MyMethodB : a  **>
                method MyMethodB : a  {
            
                }
                <** @see : MyMethodB : b fireAndForget  **>
                method MyMethodB : b fireAndForget {
            
                }

            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto methods = inf->getMethods();
    auto comment = methods.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@see : MyMethodA ");
    EXPECT_STREQ(methods.front()->getName().c_str(), "MyMethodA");
    methods.pop_front();
    comment = methods.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@see : MyMethodC fireAndForget ");
    comment = methods.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@see : MyMethodB : b fireAndForget  ");
    methods.pop_back();
    comment = methods.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@see : MyMethodB : a  ");
}

TEST(ParserTestLT, AnnotationTest_broadcast)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : MyBroadcastA **>
                broadcast MyBroadcastA {
            
                }
                <** @experimental : MyBroadcastC selective **>
                broadcast MyBroadcastC selective {
            
                }
                <** @deprecated : MyBroadcastB : a  **>
                broadcast MyBroadcastB : a  {
            
                }
                <** @see : MyBroadcastB : b selective  **>
                broadcast MyBroadcastB : b selective {
            
                }

                broadcast MyBroadcastC : a  {
            
                }
                broadcast MyBroadcastC : b selective {
            
                }

            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto broadcasts = inf->getBroadcasts();
    auto comment = broadcasts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : MyBroadcastA ");
    EXPECT_STREQ(broadcasts.front()->getName().c_str(), "MyBroadcastA");
    broadcasts.pop_front();
    comment = broadcasts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@experimental : MyBroadcastC selective ");
    broadcasts.pop_front();
    comment = broadcasts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@deprecated : MyBroadcastB : a  ");
    broadcasts.pop_front();
    comment = broadcasts.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@see : MyBroadcastB : b selective  ");
}

TEST(ParserTestLT, AnnotationTest_arg)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                broadcast MyBroadcast {
                    out{
                        <** @description : ifcheck **>
                        Boolean ifcheck

                        <** @description : nums prepared to be checked **>
                        Integer[] nums 
                    }
                }

                method MyMethodA  {
                    in {
                        <** @description : ifcheck **>
                        Boolean ifcheck
                    }
            
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto broadcasts = inf->getBroadcasts();
    auto outArgs = broadcasts.front()->getOutArgs();
    EXPECT_EQ(outArgs.size(), 2U);
    auto comment = outArgs.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : ifcheck ");
    comment = outArgs.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : nums prepared to be checked ");

    auto methods = inf->getMethods();
    auto args = methods.front()->getInArgs();
    EXPECT_EQ(args.size(), 1U);
    comment = args.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : ifcheck ");
}

TEST(ParserTestLT, AnnotationTest_array)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {

                <** @description : param nums stores ids **>
                public array nums of Integer 
                        
                <** @description : param names stores names **>
                array names of String 

            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : param nums stores ids ");
    comment = types.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : param names stores names ");
}

TEST(ParserTestLT, AnnotationTest_typeDef)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {

                <** @description : MyString is String **>
                public typedef MyString is String
                        
                <** @description : MyBool is Boolean **>
                typedef MyBool is Boolean

            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : MyString is String ");
    comment = types.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : MyBool is Boolean ");
}

TEST(ParserTestLT, AnnotationTest_struct)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : information of a student **>
                public struct student{
                    String name
                    Double id
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : information of a student ");
}

TEST(ParserTestLT, AnnotationTest_union)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : operator && or || **>
                public union operator{
                    String opAnd
                    Boolean result
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : operator && or || ");
}

TEST(ParserTestLT, AnnotationTest_enumeration)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : enumeration of errors **>
                public enumeration errors{
                    SegFault
                    SyntaxError
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : enumeration of errors ");
}

TEST(ParserTestLT, AnnotationTest_enumerator)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                public enumeration errors{
                    <** @description : segment fault **>
                    SegFault
                    <** @description : Syntax fault **>
                    SyntaxError = 10
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto enumeration = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(types.front());
    auto enumerators = enumeration->getEnumerators();
    auto comment = enumerators.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : segment fault ");
    comment = enumerators.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : Syntax fault ");
}

TEST(ParserTestLT, AnnotationTest_map)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : String to Integer **>
                public map mapperA {String to Integer}

                <** @description : String to Integer **>
                map mapperB {String to Integer}

                public map mapperC {String to Integer}
                
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 3U);
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : String to Integer ");
    types.pop_front();
    comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : String to Integer ");
}

TEST(ParserTestLT, AnnotationTest_field)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                <** @description : information of a student **>
                public struct student{
                    <** @description : name of a student **>
                    String name
                    Double id
                    <** @description : student's grades  **>
                    Integer[] grades
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    EXPECT_STREQ(types.front()->getName().c_str(), "student");
    auto comment = types.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : information of a student ");
    auto fields = std::dynamic_pointer_cast<BstIdl::FStructType>(types.front())->getElements();
    EXPECT_EQ(fields.size(), 3U);
    comment = fields.front()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : name of a student ");
    comment = fields.back()->getComment();
    EXPECT_EQ(comment->getElements().size(), 1U);
    EXPECT_STREQ(comment->getElements().front()->getRawText().c_str(), "@description : student's grades  ");
}

TEST(ParserTestLT, PolymorphicTest)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                public struct A polymorphic{
                    String name
                }

                struct B extends A{
                    String sex
                }

                struct C extends A{
                    String sex
                }
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 3U);
    auto structA = std::dynamic_pointer_cast<BstIdl::FStructType>(types.front());
    EXPECT_EQ(structA->isPolymorphic(), true);
}

TEST(ParserTestLT, CompoundInitializerTest)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                public struct A {
                    String name
                }            
                const A name = {}
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    EXPECT_EQ(inf->getConstants().size(), 1U);
    auto name = inf->getConstants().front();
    EXPECT_NE(std::dynamic_pointer_cast<BstIdl::FCompoundInitializer>(name->getRhs()), nullptr);
    EXPECT_EQ(std::dynamic_pointer_cast<BstIdl::FCompoundInitializer>(name->getRhs())->getElements().size(), 0U);
}

TEST(ParserTestLT, ElementInitializerTest)
{
    auto content = R"(package com.bst.idl.testRefMgr.model
            interface MyInterface {
                map mapperA {String to Integer}
                const mapperA m1 = [ 1 => "LT", 2 => 69 ]
            }
    )";
    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.testRefMgr.model");
    EXPECT_EQ(ret->getInterfaces().size(), 1U);
    auto inf = ret->getInterfaces().front();
    EXPECT_STREQ(inf->getName().c_str(), "MyInterface");
    auto types = inf->getTypes();
    EXPECT_EQ(types.size(), 1U);
    EXPECT_EQ(inf->getConstants().size(), 1U);
    auto m1 = inf->getConstants().front();
    EXPECT_EQ(std::dynamic_pointer_cast<BstIdl::FBracketInitializer>(m1->getRhs())->getElements().size(), 2U);
    auto rhs = std::dynamic_pointer_cast<BstIdl::FBracketInitializer>(m1->getRhs())->getElements().front();
    EXPECT_NE(rhs, nullptr);
    auto first = std::dynamic_pointer_cast<BstIdl::FIntegerConstant>(rhs->getFirst());
    auto second = std::dynamic_pointer_cast<BstIdl::FStringConstant>(rhs->getSecond());
    EXPECT_EQ(first->getUnsignedVal(), 1U);
    EXPECT_STREQ(second->getVal().c_str(), "LT");
    auto rhs2 = std::dynamic_pointer_cast<BstIdl::FBracketInitializer>(m1->getRhs())->getElements().back();
    first = std::dynamic_pointer_cast<BstIdl::FIntegerConstant>(rhs2->getFirst());
    auto second2 = std::dynamic_pointer_cast<BstIdl::FIntegerConstant>(rhs2->getSecond());
    EXPECT_EQ(first->getUnsignedVal(), 2U);
    EXPECT_EQ(second2->getUnsignedVal(), 69U);
}