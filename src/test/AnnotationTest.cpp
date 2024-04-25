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
#include "model/FAnnotation.h"
#include "model/FAnnotationBlock.h"
#include "model/FAnnotationType.h"
#include "model/FFactory.h"
#include <gtest/gtest.h>
#include <string>

class AnnotationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        anno = factory.createFAnnotation();
    }

    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    std::shared_ptr<BstIdl::FAnnotation> anno;
    BstIdl::FAnnotationType type;
    std::string comment;
};

TEST_F(AnnotationTest, Annotation_null)
{
    anno->setRawText("");
    type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    comment = anno->getComment();
    EXPECT_TRUE(comment.empty());
}

TEST_F(AnnotationTest, Annotation_null1)
{
    anno->setRawText("tong.liu");
    type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    comment = anno->getComment();
    EXPECT_TRUE(comment.empty());
}

TEST_F(AnnotationTest, Annotation_null2)
{
    anno->setRawText(": to ng.li u ");
    type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "to ng.li u");
}

TEST_F(AnnotationTest, Annotation_null3)
{
    anno->setRawText(" @  : ");
    type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), " ");
}

TEST_F(AnnotationTest, Annotation_invalid)
{
    anno->setRawText(" @seee  : ");
    type = anno->getType();
    EXPECT_TRUE(type.getName().empty());
    comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), " ");
}

TEST_F(AnnotationTest, Annotation_description)
{
    anno->setRawText(" @description : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "description");
    comment = anno->getComment();
    EXPECT_STREQ(comment.c_str(), "tong.liu");
}

TEST_F(AnnotationTest, Annotation_author)
{
    anno->setRawText("@author : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "author");
}

TEST_F(AnnotationTest, Annotation_experimental)
{
    anno->setRawText("@experimental : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "experimental");
}

TEST_F(AnnotationTest, Annotation_deprecated)
{
    anno->setRawText("@deprecated : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "deprecated");
}

TEST_F(AnnotationTest, Annotation_see)
{
    anno->setRawText("@see : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "see");
}

TEST_F(AnnotationTest, Annotation_param)
{
    anno->setRawText(" @param : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "param");
}

TEST_F(AnnotationTest, Annotation_high_volume)
{
    anno->setRawText(" @high-volume : tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "high_volume");
}

TEST_F(AnnotationTest, Annotation_high_frequency)
{
    anno->setRawText("@high-frequency: tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "high_frequency");
}

TEST_F(AnnotationTest, Annotation_source_uri)
{
    anno->setRawText("@source-uri: tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "source_uri");
}

TEST_F(AnnotationTest, Annotation_source_alias)
{
    anno->setRawText(" @source-alias: tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "source_alias");
}

TEST_F(AnnotationTest, Annotation_details)
{
    anno->setRawText("@details: tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "details");
}

TEST_F(AnnotationTest, Annotation_return)
{
    anno->setRawText("@return: tong.liu");
    type = anno->getType();
    EXPECT_STREQ(type.getName().c_str(), "return");
}

TEST_F(AnnotationTest, Annotation_setComment)
{
    anno->setComment("tong.liu");
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@undefined : tong.liu");
}

TEST_F(AnnotationTest, Annotation_setComment1)
{
    anno->setRawText("@return: liu");
    anno->setComment("tong.liu");
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@return : tong.liu");
}

TEST_F(AnnotationTest, Annotation_setType)
{
    anno->setType(type.getByName("sha@@@"));
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@undefined : ");
}

TEST_F(AnnotationTest, Annotation_setType1)
{
    anno->setType(type.getByName("description"));
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@description : ");
}

TEST_F(AnnotationTest, Annotation_setType2)
{
    anno->setType(type.get(10000));
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@undefined : ");
}

TEST_F(AnnotationTest, Annotation_setType3)
{
    anno->setType(type.get(1));
    auto text = anno->getRawText();
    EXPECT_STREQ(text.c_str(), "@description : ");
}
