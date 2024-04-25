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
#include <gtest/gtest.h>

using namespace BstIdl;

TEST(ModelTest, Annotation_null)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("ding.wang");
    auto type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
}

TEST(ModelTest, Annotation_null2)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText(": ding.wang");
    auto type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
}

TEST(ModelTest, Annotation_null3)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@: ding.wang");
    auto type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
}

TEST(ModelTest, Annotation_invalid)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@param1: ding.wang");
    auto type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "ding.wang");
}

TEST(ModelTest, Annotation_author1)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@author: ding.wang");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "author");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "ding.wang");
}

TEST(ModelTest, Annotation_author2)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText(" @author  : ding.wang");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "author");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "ding.wang");
}

TEST(ModelTest, Annotation_description)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText(" @description : this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "description");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}

TEST(ModelTest, Annotation_experimental)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@experimental: this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "experimental");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}

TEST(ModelTest, Annotation_deprecated)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText(" @deprecated : this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "deprecated");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}

TEST(ModelTest, Annotation_see)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@see: this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "see");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}

TEST(ModelTest, Annotation_source_uri)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText(" @source-uri : this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "source_uri");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}

TEST(ModelTest, Annotation_source_alias)
{
    FFactory &factory = FFactory::getInstance();
    auto anno = factory.createFAnnotation();
    anno->setRawText("@source-alias: this is something.");
    auto type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "source_alias");
    auto comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "this is something.");
}
