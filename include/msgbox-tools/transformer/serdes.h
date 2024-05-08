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
#ifndef TRANSFORMER_BAREMETAL_SERDES_H
#define TRANSFORMER_BAREMETAL_SERDES_H

#include "model/FObject.h"
#include "model/FTransformer.h"
#include "msgbox-tools/transformer/typename.h"
#include "msgbox-tools/transformer/initvar.h"
#include "msgbox-tools/transformer/isfixed.h"
#include "msgbox-tools/transformer/getAlign.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace MsgBoxGen
{
static inline std::string serdesFBasicType(FBasicTypeId *type)
{
    return "";
}

static inline std::string serdesFIntegerInterval(FIntegerInterval *type)
{
    return "";
}

static inline std::string serdesFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret;
    if (!type->isFixedSize())
    {
        ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;

    if (ret >= 0)
        ret = ipc_ser_put_32(ser, &in->size);
$SER_ELE
    return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME *out)
{
    int32_t ret = 0;
    uint32_t size = 0;
	uint32_t *size_ptr = NULL;

    if (!out || !buf)
        return -1;

	size_ptr = (uint32_t *)alloc_data(buf, 4, 4);
	if (!size_ptr)
		return -1;

    out->size = *size_ptr;
    size = out->size * sizeof($ELE_TYPENAME);
    out->data = ($ELE_TYPENAME *)alloc_data(buf, size, $ALIGN);
	if (size > 0 && !out->data)
		return -1;
$DES_ELE
    return ret >= 0 ? 0 : -1;
}

)";
    }
    else
    {
        ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;
$SER_ELE
    return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME **out)
{
    int32_t ret = 0;
    uint32_t size = $SIZE * sizeof($ELE_TYPENAME);

    if (!out || !buf)
        return -1;

    *out = ($TYPENAME *)alloc_data(buf, size, $ALIGN);
	if (!*out)
		return -1;
$DES_ELE
    return ret >= 0 ? 0 : -1;
}

)";
        replace_all(ret, "$SIZE", type->getFixedSize()->toString());
    }

    std::string ele_ser;
    std::string ele_des;
    std::string ele_name;
    auto ele_type = type->getElementType();
    bool is_fixed = BstIdl::Transformer::MsgBoxGen::isFixed(ele_type) == "true";
    auto pred = ele_type->getPredefined();
    bool is_string = (pred && BstIdl::FBasicTypeId::isString(pred->getValue()));
    bool is_byte_buffer = (pred && BstIdl::FBasicTypeId::isByteBuffer(pred->getValue()));
    if (is_string)
        ele_name = "string";
    else if (is_byte_buffer)
        ele_name = "byte_buffer";
    else
        ele_name = remove_last(getTypeName(ele_type), 2);

    if (is_fixed)
    {
        ele_ser = R"(
    if (ret >= 0)
        ret = ipc_ser_put_align(ser, (uint8_t *)in->data,
                in->size * sizeof($ELE_TYPENAME), $ALIGN);
)";
        ele_des = "";
    }
    else
    {
        if (is_string)
                    ele_ser = R"(
    for (uint32_t i = 0; ret >= 0 && i < in->size; ++i)
        ret = serialize_$ELE_NAME(ser, in->data[i]);
)";
        else
            ele_ser = R"(
    for (uint32_t i = 0; ret >= 0 && i < in->size; ++i)
        ret = serialize_$ELE_NAME(ser, &in->data[i]);
)";
        ele_des = R"(
    for (uint32_t i = 0; ret >= 0 && i < out->size; ++i)
        ret = deserialize_$ELE_NAME(buf, &out->data[i]);
)";
        replace_all(ret, "alloc_data(buf, size, $ALIGN)", "alloc_obj(buf, size, $ALIGN)");
    }

    if (type->isFixedSize())
    {
        replace_all(ele_ser, "in->data", "(*in)");
        replace_all(ele_ser, "in->size", type->getFixedSize()->toString());
        replace_all(ele_des, "out->size", type->getFixedSize()->toString());
        replace_all(ele_des, "out->data", "(**out)");
    }

    replace_all(ret, "$SER_ELE", ele_ser);
    replace_all(ret, "$DES_ELE", ele_des);
    replace_all(ret, "$ALIGN", getAlign(type));
    replace_all(ret, "$ELE_NAME", ele_name);
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$ELE_TYPENAME", getTypeName(ele_type));
    replace_all(ret, "$NAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serdesFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;
	uint32_t val = (uint32_t)(*in);

	ret = ipc_ser_put_32(ser, (uint32_t *)&val);
	return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME *out)
{
	uint32_t *ptr = NULL;

    if (!out || !buf)
        return -1;

	ptr = (uint32_t *)alloc_data(buf, 4, 4);
	if (!ptr)
		return -1;
	*out = ($TYPENAME)(*ptr);
	return 0;
}

)";
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$NAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serdes(const std::shared_ptr<FObject> &obj);
static inline std::string serdesFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret;
    if (BstIdl::Transformer::MsgBoxGen::isFixed(type) == "true")
    {
        ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;

    ret = ipc_ser_put_align(ser, (const uint8_t *)in,
				sizeof($TYPENAME), $ALIGN);
    return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME **out)
{
    if (!out || !buf)
        return -1;

    *out = ($TYPENAME *)alloc_data(buf, sizeof($TYPENAME), $ALIGN);
    if (!*out)
		return -1;

    return 0;
}

)";
    }
    else
    {
        ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;

    $ELE_SER
    return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME *out)
{
    int32_t ret = 0;

    if (!out || !buf)
        return -1;

    $ELE_DES
    return ret >= 0 ? 0 : -1;
}

)";
    }

    std::string ele_sers;
    std::string ele_deses;
    for (const auto &e : type->getElements())
    {
        auto ele_ser = serialize(e->getType());
        replace_all(ele_ser, "$NAME", std::string("$NAME->").append(e->getName()));
        ele_sers.append(ele_ser);

        auto ele_des = deserialize(e->getType());
        replace_all(ele_des, "$NAME", std::string("$NAME->").append(e->getName()));
        ele_deses.append(ele_des);
    }
    replace_all(ele_sers, "$NAME", "in");
    replace_all(ele_sers, "\n", "\n\t");
    replace_all(ret, "$ELE_SER", ele_sers);

    replace_all(ele_deses, "$NAME", "out");
    replace_all(ele_deses, "\n", "\n\t");
    replace_all(ret, "$ELE_DES", ele_deses);

    replace_all(ret, "$ALIGN", getAlign(type));
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$NAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serdesFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 * @note union size greater than 64 bit is NOT supported, return -1.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    int32_t ret = 0;

    ret = ipc_ser_put_align(ser, (uint8_t *)in, sizeof($TYPENAME), $ALIGN);
    return ret >= 0 ? 0 : -1;
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 * @note union size greater than 64 bit is NOT supported, return -1.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME *out)
{
	$TYPENAME *ptr = NULL;

    if (!out || !buf)
        return -1;

    ptr = ($TYPENAME *)alloc_data(buf, sizeof($TYPENAME), $ALIGN);
    if (!ptr)
        return -1;
    *out = *ptr;
    return 0;
}

)";
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$ALIGN", getAlign(type));
    replace_all(ret, "$NAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serdesFTypeRef(FTypeRef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getPredefined())
        return ptr->transform("baremetal_serdes");
    if (auto ptr = type->getDerived())
        return ptr->transform("baremetal_serdes");
    if (auto ptr = type->getInterval())
        return ptr->transform("baremetal_serdes");

    //empty typeref
    return "";
}

static inline std::string serdesFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "";

    std::string ret = R"(/**
 * Serialize $TYPENAME.
 *
 * @param ser Pointer to serdes_t, which stores the serialized data.
 * @param in The input data to be serialized.
 * @return 0 if success, negative if fail.
 */
static inline int32_t serialize_$NAME(
                            serdes_t *ser,
                            const $TYPENAME *in)
{
    return serialize_$ELE_NAME(ser, in);
}

/**
 * Deserialize $TYPENAME.
 *
 * @param des Pointer to serdes_t, which stores the serialized data.
 * @param out The output of deserialized data.
 * @param buf The buffer used to make the deserialized objects.
 * @return 0 if success, negative if fail.
 */
static inline int32_t deserialize_$NAME(
                            des_buf_t *buf,
                            $TYPENAME *out)
{
    return deserialize_$ELE_NAME(buf, out);
}

)";
    if (isFixed(type) == "true")
        replace_one(ret, "$TYPENAME *out)", "$TYPENAME **out)");
    std::string ele_name;
    auto ele_type = type->getActualType();
    auto pred = ele_type->getPredefined();
    if (pred && pred->getValue() == BstIdl::FBasicTypeId::STRING)
        ele_name = "string";
    else if(pred && pred->getValue() == BstIdl::FBasicTypeId::BYTE_BUFFER)
        ele_name = "byte_buffer";
    else
        ele_name = remove_last(getTypeName(ele_type), 2);

    replace_all(ret, "$ELE_NAME", ele_name);
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$NAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serdesFArgument(FArgument *type)
{
    return "";
}

static inline bool registerSerdes()
{
    if (BstIdl::FObject::hasTransformer("baremetal_serdes"))
        return false;

    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = serdesFBasicType;
    t->forFIntegerInterval = serdesFIntegerInterval;
    t->forFArrayType = serdesFArrayType;
    t->forFEnumerationType = serdesFEnumerationType;
    t->forFStructType = serdesFStructType;
    t->forFUnionType = serdesFUnionType;
    t->forFTypeRef = serdesFTypeRef;
    t->forFTypeDef = serdesFTypeDef;
    t->forFArgument = serdesFArgument;
    return BstIdl::FObject::addTransformer("baremetal_serdes", t);
}

static inline std::string serdes(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_serdes"))
        (void)registerSerdes();
    auto ret = obj->transform("baremetal_serdes");
    replace_all(ret, " * *", " **");
    return ret;
}

static inline std::string serdes(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_serdes"))
        (void)registerSerdes();
    auto ret = obj->transform("baremetal_serdes");
    replace_all(ret, " * *", " **");
    return ret;
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif