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
#include "FDParser/FDParser.h"
#include "FDModel/FDFactory.h"
#include "FDModel/FDModel.h"
#include "utilities/string_utility.h"
#include <gtest/gtest.h>
#include <memory>

TEST(FDParserTest, ConstantTest_IntegerValueSingle)
{
    BstIdl::FDParser parser;
    std::string fileName("../newidlFile/ValueSingle.fdepl");
    auto ret = parser.parse(fileName);
    EXPECT_EQ(ret.size(), 3);
}

TEST(FDParserTest, EnumerationOverwrite_Property)
{
    BstIdl::FDParser parser;
    std::string fileName("../newidlFile/OverWrites.fdepl");
    auto ret = parser.parse(fileName);
    EXPECT_EQ(ret.size(), 3);
}

TEST(FDParserTest, Map_MapKey_empty)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for interface interface2{
        map Map1 {
            key {

            }
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "interface2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
}

TEST(FDParserTest, Map_MapValue_empty)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for interface interface2 as I2{
        map Map1 {
            value {

            }
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
}

TEST(FDParserTest, Map_Properties_MapValue)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for interface interface2 as FDModel2.I2{
        map Map1 {
            MapProp = 105
            value {

            }
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
    EXPECT_EQ(Map1->getProperties().size(), 1U);
    EXPECT_STREQ(Map1->getProperties().front()->getName().c_str(), "MapProp");
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->getInteger(), 105U);
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->isInteger(), true);
}

TEST(FDParserTest, Map_Properties_MapKey)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for interface fdmodel2.interface2 as FDModel2.I2{
        map Map1 {
            MapProp = 110
            key {

            }
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
    EXPECT_EQ(Map1->getProperties().size(), 1U);
    EXPECT_STREQ(Map1->getProperties().front()->getName().c_str(), "MapProp");
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->getInteger(), 110U);
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->isInteger(), true);
}

TEST(FDParserTest, Map_Properties)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface fdmodel2.interface2 as FDModel2.I2{
        map Map1 {
            MapProp = 115
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
    EXPECT_EQ(Map1->getProperties().size(), 1U);
    EXPECT_STREQ(Map1->getProperties().front()->getName().c_str(), "MapProp");
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->getInteger(), 115U);
    EXPECT_EQ(Map1->getProperties().front()->getValue()->getSingle()->isInteger(), true);
}

TEST(FDParserTest, Map_Properties_MapKeyAndValue)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface fdmodel2.interface2 as I2{
        InterfaceProp = 300
        map Map1 {
            MapProp = 120
            key {

            }
            value {

            }
        }
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getTypes().size(), 1U);
    auto Map1 = std::dynamic_pointer_cast<BstIdl::FDMap>(infDepl->getTypes().front());
    EXPECT_STREQ(Map1->getName().c_str(), "Map1");
    EXPECT_EQ(Map1->getProperties().size(), 1U);
}

TEST(FDParserTest, Broadcast_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface fdmodel2.interface2 as I2{
        InterfaceProp = 300
     	broadcast fdmodel2.interface2.broadcast1 {
            BroascastProp = 500
    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getBroadcasts().size(), 1U);
    auto broadcast = infDepl->getBroadcasts().front();
    EXPECT_STREQ(broadcast->getName().c_str(), "fdmodel2.interface2.broadcast1");
}

TEST(FDParserTest, BroadcastHead)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface fdmodel2.interface2 as I2{
        InterfaceProp = 300
     	broadcast fdmodel2.interface2.broadcast1 {
    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getBroadcasts().size(), 1U);
    auto broadcast = infDepl->getBroadcasts().front();
    EXPECT_STREQ(broadcast->getName().c_str(), "fdmodel2.interface2.broadcast1");
}

TEST(FDParserTest, Method_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface fdmodel2.interface2 as I2{
        InterfaceProp = 300
     	method fdmodel2.interface2.Method2 {

    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getMethods().size(), 1U);
    auto method = infDepl->getMethods().front();
    EXPECT_STREQ(method->getName().c_str(), "fdmodel2.interface2.Method2");
}

TEST(FDParserTest, Method_Properties_Out_Arg)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface interface2 as I2{
        InterfaceProp = 300
     	method fdmodel2.interface2.Method2 {
            CallSemantics = synchronous
            IsOptional = true
            PerformanceImpact = none
            out {
                argStr {

                }
			    argg{

                }
            }
    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getMethods().size(), 1U);
    auto method = infDepl->getMethods().front();
    EXPECT_STREQ(method->getName().c_str(), "fdmodel2.interface2.Method2");
    auto props = method->getProperties();
    EXPECT_EQ(props.size(), 3U);
    EXPECT_STREQ(props.front()->getName().c_str(), "CallSemantics");
    props.pop_front();
    EXPECT_STREQ(props.front()->getName().c_str(), "IsOptional");
    EXPECT_STREQ(props.back()->getName().c_str(), "PerformanceImpact");
    auto outArgs = method->getOutArguments()->getArguments();
    EXPECT_EQ(outArgs.size(), 2U);
    EXPECT_STREQ(outArgs.front()->getName().c_str(), "argStr");
    EXPECT_STREQ(outArgs.back()->getName().c_str(), "argg");
}

TEST(FDParserTest, Method_Properties_In_Arg)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface interface2 as I2{
        InterfaceProp = 300
     	method fdmodel2.interface2.Method2 {
            CallSemantics = synchronous
            IsOptional = true
            PerformanceImpact = none
            in {
                argStr {

                }
			    argg{

                }
            }
    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getMethods().size(), 1U);
    auto method = infDepl->getMethods().front();
    EXPECT_STREQ(method->getName().c_str(), "fdmodel2.interface2.Method2");
    auto props = method->getProperties();
    EXPECT_EQ(props.size(), 3U);
    EXPECT_STREQ(props.front()->getName().c_str(), "CallSemantics");
    props.pop_front();
    EXPECT_STREQ(props.front()->getName().c_str(), "IsOptional");
    EXPECT_STREQ(props.back()->getName().c_str(), "PerformanceImpact");
    auto outArgs = method->getInArguments()->getArguments();
    EXPECT_EQ(outArgs.size(), 2U);
    EXPECT_STREQ(outArgs.front()->getName().c_str(), "argStr");
    EXPECT_STREQ(outArgs.back()->getName().c_str(), "argg");
}

TEST(FDParserTest, Method_Properties)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface interface2 as FDModel2.I2{
        InterfaceProp = 300
     	method fdmodel2.interface2.Method2 {
            CallSemantics = synchronous
            IsOptional = true
            PerformanceImpact = none
    	}
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    auto infDepl = std::dynamic_pointer_cast<BstIdl::FDInterface>(deploys.front());
    EXPECT_EQ(infDepl->getMethods().size(), 1U);
    auto method = infDepl->getMethods().front();
    auto props = method->getProperties();
    EXPECT_STREQ(method->getName().c_str(), "fdmodel2.interface2.Method2");
    EXPECT_EQ(props.size(), 3U);
    EXPECT_STREQ(props.front()->getName().c_str(), "CallSemantics");
    props.pop_front();
    EXPECT_STREQ(props.front()->getName().c_str(), "IsOptional");
    EXPECT_STREQ(props.back()->getName().c_str(), "PerformanceImpact");
}

TEST(FDParserTest, InterfaceHead_FI_useFQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for typeCollection fdmodel2.tc1 as FDModel2.TC1{

    }
    define repeat.MySpec3 for interface interface2 {
        use FDModel2.TC1

    }

    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 2U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
    EXPECT_STREQ(deploys.back()->getName().c_str(), "interface2");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "interfaces");
    EXPECT_EQ(deploys.back()->getUse().size(), 1U);
    EXPECT_EQ(deploys.back()->getUse().front(), deploys.front());
}

TEST(FDParserTest, Interface_useID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"

    define repeat.MySpec3 for typeCollection fdmodel2.tc1 as TC1{

    }
    define repeat.MySpec3 for interface interface2 {
        use TC1

    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 2U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
    EXPECT_STREQ(deploys.back()->getName().c_str(), "interface2");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "interfaces");
    EXPECT_EQ(deploys.back()->getUse().size(), 1U);
    EXPECT_EQ(deploys.back()->getUse().front(), deploys.front());
}

TEST(FDParserTest, TypeCollectionHead_FIF_useFQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for interface interface2 as FDModel2.I2{

    }
    define repeat.MySpec3 for typeCollection tc1 as FDModel2.TC1{
        use FDModel2.I2
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 2U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.I2");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "interfaces");
    EXPECT_STREQ(deploys.back()->getName().c_str(), "FDModel2.TC1");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "type_collections");
    EXPECT_EQ(deploys.back()->getUse().size(), 1U);
    EXPECT_EQ(deploys.back()->getUse().front(), deploys.front());
}

TEST(FDParserTest, TypeCollectionHead_FII)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for typeCollection tc1 as TC1{
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_IFI)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for typeCollection fdmodel2.tc1 as TC1{
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_IFF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for typeCollection fdmodel2.tc1 as FDModel2.TC1{
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_IIF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for typeCollection tc1 as FDModel2.TC1{
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_III)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for typeCollection tc1 as TC1{
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_FF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for typeCollection fdmodel2.tc1 {
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "fdmodel2.tc1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_FI)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for typeCollection tc1 {
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "tc1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, TypeCollectionHead_II)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define MySpec3 for typeCollection tc1 {
    }
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "tc1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
}

TEST(FDParserTest, ExtensionHead_FQN_as_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/MySpec3.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.MySpec3 for typeCollection tc1 as FDModel2.TC1 {
    }
    define repeat.ExtensionSpec for provider as ProviderA {
        use FDModel2.TC1
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 2U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel2.TC1");
    EXPECT_STREQ(deploys.front()->getHost()->getName().c_str(), "type_collections");
    EXPECT_STREQ(deploys.back()->getName().c_str(), "ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.back())->getElements();
    EXPECT_EQ(elements.size(), 0U);
    auto useRefs = deploys.back()->getUse();
    EXPECT_EQ(useRefs.size(), 1U);
    EXPECT_EQ(useRefs.front(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FQN_as_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider as fdmodel.ProviderA {
        instance {

        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "fdmodel.ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "instance");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_ID_as_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider as fdmodel.ProviderA {
        instance as client{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "fdmodel.ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "client");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FQN_for_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider {
        instance as fdmodel.client{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "provider");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "fdmodel.client");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_ID_for_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/FDModel2.fidl"
    specification ExtensionSpec{
        for providers {
            ProcessName: String; 
        }
        for instances {
            IPAddress: String;
            Port: Integer (optional);
            AccessControl: { local, subnet, global } (default: global);
        } 
    }
    define ExtensionSpec for provider {
        instance fdmodel2.interface2 {
            IPAddress = "192.168.2.4"
            Port = 1412
            AccessControl = global
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto spec = std::string("ExtensionSpec");
    EXPECT_TRUE(ret->getProviderDeployment(spec) != nullptr);
    auto extensionRoot = ret->getProviderDeployment(spec);
    EXPECT_EQ(extensionRoot->getElements().size(), 1U);
    EXPECT_STREQ(extensionRoot->getName().c_str(), "provider");
    EXPECT_STREQ(extensionRoot->getHost()->getName().c_str(), "providers");
    auto element = extensionRoot->getElements().front();
    EXPECT_EQ(element->getHost()->getName(), std::string("instances"));
    EXPECT_EQ(element->getName(), std::string("fdmodel2.interface2"));
}

TEST(FDParserTest, ExtensionHead_ID_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider ProviderA {
        instance interface2{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "interface2");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_ID_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider fdmodel.ProviderA {
        instance interface2 as CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "fdmodel.ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FQN_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider ProviderA {
        instance interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FQN_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider fdmodel.ProviderA {
        instance fdmodel2.interface2 as CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "fdmodel.ProviderA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_III)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider ProviderA as PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_IIF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider ProviderA as FDModel.PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel.PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_IFI)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider fdmodel.ProviderA as PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_IFF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define ExtensionSpec for provider fdmodel.ProviderA as FDModel.PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel.PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FII)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider ProviderA as PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FIF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider ProviderA as FDModel.PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel.PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FFI)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider fdmodel.ProviderA as PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, ExtensionHead_FFF)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    import "../newidlFile/ExtensionSpec.fdepl"
    import "../newidlFile/FDModel2.fidl"
    define repeat.ExtensionSpec for provider fdmodel.ProviderA as FDModel.PA{
        instance fdmodel2.interface2 as FDModel.CA{
            
        }
    }    
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto deploys = ret->getDeployments();
    EXPECT_EQ(deploys.size(), 1U);
    EXPECT_STREQ(deploys.front()->getName().c_str(), "FDModel.PA");
    EXPECT_STREQ(deploys.back()->getHost()->getName().c_str(), "providers");
    auto elements = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(deploys.front())->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "FDModel.CA");
    EXPECT_STREQ(element->getHost()->getName().c_str(), "instances");
    EXPECT_EQ(element->getContainer(), deploys.front());
}

TEST(FDParserTest, Specification_FQN_EXTENDS_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    specification fdmodel.MySpec1{}
    specification fdmodel.MySpec2 extends fdmodel.MySpec1{}
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto specs = ret->getSpecifications();
    EXPECT_EQ(specs.size(), 2U);
    EXPECT_STREQ(specs.front()->getName().c_str(), "fdmodel.MySpec1");
    EXPECT_STREQ(specs.back()->getName().c_str(), "fdmodel.MySpec2");
    EXPECT_EQ(specs.back()->getSpecNames().size(), 2U);
    EXPECT_STREQ(specs.back()->getSpecNames().front().c_str(), "fdmodel.MySpec1");
}

TEST(FDParserTest, Specification_FQN_EXTENDS_ID)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    specification MySpec1{}
    specification fdmodel.MySpec2 extends MySpec1{}
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto specs = ret->getSpecifications();
    EXPECT_EQ(specs.size(), 2U);
    EXPECT_STREQ(specs.front()->getName().c_str(), "MySpec1");
    EXPECT_STREQ(specs.back()->getName().c_str(), "fdmodel.MySpec2");
    EXPECT_EQ(specs.back()->getSpecNames().size(), 2U);
    EXPECT_STREQ(specs.back()->getSpecNames().front().c_str(), "MySpec1");
}

TEST(FDParserTest, Specification_ID_EXTENDS_FQN)
{
    BstIdl::FDParser parser;
    auto content = R"(package com.bst.idl.testFDParser.fdmodel
    import "../newidlFile/Extension.fidl"
    specification fdmodel.MySpec1{}
    specification MySpec2 extends fdmodel.MySpec1{}
    )";
    auto ret = parser.parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto specs = ret->getSpecifications();
    EXPECT_EQ(specs.size(), 2U);
    EXPECT_STREQ(specs.front()->getName().c_str(), "fdmodel.MySpec1");
    EXPECT_STREQ(specs.back()->getName().c_str(), "MySpec2");
    EXPECT_EQ(specs.back()->getSpecNames().size(), 2U);
    EXPECT_STREQ(specs.back()->getSpecNames().front().c_str(), "fdmodel.MySpec1");
}
