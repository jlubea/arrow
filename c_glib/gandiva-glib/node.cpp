/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <arrow-glib/data-type.hpp>
#include <arrow-glib/field.hpp>

#include <gandiva-glib/node.hpp>

template <typename Type>
Type
ggandiva_literal_node_get(GGandivaLiteralNode *node)
{
  auto gandiva_literal_node =
    std::static_pointer_cast<gandiva::LiteralNode>(ggandiva_node_get_raw(GGANDIVA_NODE(node)));
  return boost::get<Type>(gandiva_literal_node->holder());
}

G_BEGIN_DECLS

/**
 * SECTION: node
 * @section_id: node-classes
 * @title: Node classes
 * @include: gandiva-glib/gandiva-glib.h
 *
 * #GGandivaNode is a base class for a node in the expression tree.
 *
 * #GGandivaFieldNode is a class for a node in the expression tree, representing an Arrow field.
 *
 * #GGandivaFunctionNode is a class for a node in the expression tree, representing a function.
 *
 * #GGandivaLiteralNode is a base class for a node in the expression tree,
 * representing a literal.
 *
 * #GGandivaBooleanLiteralNode is a class for a node in the expression tree,
 * representing a boolean literal.
 *
 * #GGandivaInt8LiteralNode is a class for a node in the expression tree,
 * representing a 8-bit integer literal.
 *
 * #GGandivaUInt8LiteralNode is a class for a node in the expression tree,
 * representing a 8-bit unsigned integer literal.
 *
 * #GGandivaInt16LiteralNode is a class for a node in the expression tree,
 * representing a 16-bit integer literal.
 *
 * #GGandivaUInt16LiteralNode is a class for a node in the expression tree,
 * representing a 16-bit unsigned integer literal.
 *
 * #GGandivaInt32LiteralNode is a class for a node in the expression tree,
 * representing a 32-bit integer literal.
 *
 * #GGandivaUInt32LiteralNode is a class for a node in the expression tree,
 * representing a 32-bit unsigned integer literal.
 *
 * #GGandivaInt64LiteralNode is a class for a node in the expression tree,
 * representing a 64-bit integer literal.
 *
 * #GGandivaUInt64LiteralNode is a class for a node in the expression tree,
 * representing a 64-bit unsigned integer literal.
 *
 * #GGandivaFloatLiteralNode is a class for a node in the expression tree,
 * representing a 32-bit floating point literal.
 *
 * #GGandivaDoubleLiteralNode is a class for a node in the expression tree,
 * representing a 64-bit floating point literal.
 *
 * #GGandivaBinaryLiteralNode is a class for a node in the expression tree,
 * representing a binary literal.
 *
 * #GGandivaStringLiteralNode is a class for a node in the expression tree,
 * representing an UTF-8 encoded string literal.
 *
 * Since: 0.12.0
 */

typedef struct GGandivaNodePrivate_ {
  std::shared_ptr<gandiva::Node> node;
} GGandivaNodePrivate;

enum {
  PROP_NODE = 1
};

G_DEFINE_TYPE_WITH_PRIVATE(GGandivaNode,
                           ggandiva_node,
                           G_TYPE_OBJECT)

#define GGANDIVA_NODE_GET_PRIVATE(object)                       \
  static_cast<GGandivaNodePrivate *>(                           \
    ggandiva_node_get_instance_private(                         \
      GGANDIVA_NODE(object)))

static void
ggandiva_node_finalize(GObject *object)
{
  auto priv = GGANDIVA_NODE_GET_PRIVATE(object);

  priv->node = nullptr;

  G_OBJECT_CLASS(ggandiva_node_parent_class)->finalize(object);
}

static void
ggandiva_node_set_property(GObject *object,
                           guint prop_id,
                           const GValue *value,
                           GParamSpec *pspec)
{
  auto priv = GGANDIVA_NODE_GET_PRIVATE(object);

  switch (prop_id) {
  case PROP_NODE:
    priv->node =
      *static_cast<std::shared_ptr<gandiva::Node> *>(g_value_get_pointer(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
ggandiva_node_init(GGandivaNode *object)
{
}

static void
ggandiva_node_class_init(GGandivaNodeClass *klass)
{
  GParamSpec *spec;

  auto gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->finalize     = ggandiva_node_finalize;
  gobject_class->set_property = ggandiva_node_set_property;

  spec = g_param_spec_pointer("node",
                              "Node",
                              "The raw std::shared<gandiva::Node> *",
                              static_cast<GParamFlags>(G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property(gobject_class, PROP_NODE, spec);
}


typedef struct GGandivaFieldNodePrivate_ {
  GArrowField *field;
} GGandivaFieldNodePrivate;

enum {
  PROP_FIELD = 1
};

G_DEFINE_TYPE_WITH_PRIVATE(GGandivaFieldNode,
                           ggandiva_field_node,
                           GGANDIVA_TYPE_NODE)

#define GGANDIVA_FIELD_NODE_GET_PRIVATE(object)                 \
  static_cast<GGandivaFieldNodePrivate *>(                      \
    ggandiva_field_node_get_instance_private(                   \
      GGANDIVA_FIELD_NODE(object)))

static void
ggandiva_field_node_dispose(GObject *object)
{
  auto priv = GGANDIVA_FIELD_NODE_GET_PRIVATE(object);

  if (priv->field) {
    g_object_unref(priv->field);
    priv->field = nullptr;
  }

  G_OBJECT_CLASS(ggandiva_field_node_parent_class)->dispose(object);
}

static void
ggandiva_field_node_set_property(GObject *object,
                                 guint prop_id,
                                 const GValue *value,
                                 GParamSpec *pspec)
{
  auto priv = GGANDIVA_FIELD_NODE_GET_PRIVATE(object);

  switch (prop_id) {
  case PROP_FIELD:
    priv->field = GARROW_FIELD(g_value_dup_object(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
ggandiva_field_node_get_property(GObject *object,
                                 guint prop_id,
                                 GValue *value,
                                 GParamSpec *pspec)
{
  auto priv = GGANDIVA_FIELD_NODE_GET_PRIVATE(object);

  switch (prop_id) {
  case PROP_FIELD:
    g_value_set_object(value, priv->field);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
ggandiva_field_node_init(GGandivaFieldNode *field_node)
{
}

static void
ggandiva_field_node_class_init(GGandivaFieldNodeClass *klass)
{
  auto gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->dispose      = ggandiva_field_node_dispose;
  gobject_class->set_property = ggandiva_field_node_set_property;
  gobject_class->get_property = ggandiva_field_node_get_property;

  GParamSpec *spec;
  spec = g_param_spec_object("field",
                             "Field",
                             "The field",
                             GARROW_TYPE_FIELD,
                             static_cast<GParamFlags>(G_PARAM_READWRITE |
                                                      G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property(gobject_class, PROP_FIELD, spec);
}

/**
 * ggandiva_field_node_new:
 * @field: A #GArrowField.
 *
 * Returns: A newly created #GGandivaFieldNode for the given field.
 *
 * Since: 0.12.0
 */
GGandivaFieldNode *
ggandiva_field_node_new(GArrowField *field)
{
  auto arrow_field = garrow_field_get_raw(field);
  auto gandiva_node = gandiva::TreeExprBuilder::MakeField(arrow_field);
  return ggandiva_field_node_new_raw(&gandiva_node, field);
}


typedef struct GGandivaFunctionNodePrivate_ {
  gchar *name;
  GList *parameters;
  GArrowDataType *return_type;
} GGandivaFunctionNodePrivate;

enum {
  PROP_NAME = 1,
  PROP_RETURN_TYPE
};

G_DEFINE_TYPE_WITH_PRIVATE(GGandivaFunctionNode,
                           ggandiva_function_node,
                           GGANDIVA_TYPE_NODE)

#define GGANDIVA_FUNCTION_NODE_GET_PRIVATE(object)      \
  static_cast<GGandivaFunctionNodePrivate *>(           \
    ggandiva_function_node_get_instance_private(        \
      GGANDIVA_FUNCTION_NODE(object)))                  \

static void
ggandiva_function_node_dispose(GObject *object)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(object);

  if (priv->parameters) {
    for (auto node = priv->parameters; node; node = g_list_next(node)) {
      auto parameter = GGANDIVA_NODE(node->data);
      g_object_unref(parameter);
    }
    g_list_free(priv->parameters);
    priv->parameters = nullptr;
  }

  if (priv->return_type) {
    g_object_unref(priv->return_type);
    priv->return_type = nullptr;
  }

  G_OBJECT_CLASS(ggandiva_function_node_parent_class)->dispose(object);
}

static void
ggandiva_function_node_finalize(GObject *object)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(object);

  g_free(priv->name);

  G_OBJECT_CLASS(ggandiva_function_node_parent_class)->finalize(object);
}

static void
ggandiva_function_node_set_property(GObject *object,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(object);

  switch (prop_id) {
  case PROP_NAME:
    priv->name = g_value_dup_string(value);
    break;
  case PROP_RETURN_TYPE:
    priv->return_type = GARROW_DATA_TYPE(g_value_dup_object(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
ggandiva_function_node_get_property(GObject *object,
                                    guint prop_id,
                                    GValue *value,
                                    GParamSpec *pspec)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(object);

  switch (prop_id) {
  case PROP_NAME:
    g_value_set_string(value, priv->name);
    break;
  case PROP_RETURN_TYPE:
    g_value_set_object(value, priv->return_type);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
ggandiva_function_node_init(GGandivaFunctionNode *function_node)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(function_node);
  priv->parameters = nullptr;
}

static void
ggandiva_function_node_class_init(GGandivaFunctionNodeClass *klass)
{
  auto gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->dispose      = ggandiva_function_node_dispose;
  gobject_class->finalize     = ggandiva_function_node_finalize;
  gobject_class->set_property = ggandiva_function_node_set_property;
  gobject_class->get_property = ggandiva_function_node_get_property;

  GParamSpec *spec;
  spec = g_param_spec_string("name",
                             "Name",
                             "The name of the function",
                             nullptr,
                             static_cast<GParamFlags>(G_PARAM_READWRITE |
                                                      G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property(gobject_class, PROP_NAME, spec);

  spec = g_param_spec_object("return-type",
                             "Return type",
                             "The return type of the function",
                             GARROW_TYPE_DATA_TYPE,
                             static_cast<GParamFlags>(G_PARAM_READWRITE |
                                                      G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property(gobject_class, PROP_RETURN_TYPE, spec);
}

/**
 * ggandiva_function_node_new:
 * @name: The name of the function to be called.
 * @parameters: (element-type GGandivaNode): The parameters of the function call.
 * @return_type: The return type of the function call.
 *
 * Returns: A newly created #GGandivaFunctionNode for the function call.
 *
 * Since: 0.12.0
 */
GGandivaFunctionNode *
ggandiva_function_node_new(const gchar *name,
                           GList *parameters,
                           GArrowDataType *return_type)
{
  std::vector<std::shared_ptr<gandiva::Node>> gandiva_nodes;
  for (auto node = parameters; node; node = g_list_next(node)) {
    auto gandiva_node = ggandiva_node_get_raw(GGANDIVA_NODE(node->data));
    gandiva_nodes.push_back(gandiva_node);
  }
  auto arrow_return_type = garrow_data_type_get_raw(return_type);
  auto gandiva_node = gandiva::TreeExprBuilder::MakeFunction(name,
                                                             gandiva_nodes,
                                                             arrow_return_type);
  return ggandiva_function_node_new_raw(&gandiva_node,
                                        name,
                                        parameters,
                                        return_type);
}

/**
 * ggandiva_function_node_get_parameters:
 * @node: A #GGandivaFunctionNode.
 *
 * Returns: (transfer none) (element-type GGandivaNode):
 *   The parameters of the function node.
 *
 * Since: 0.12.0
 */
GList *
ggandiva_function_node_get_parameters(GGandivaFunctionNode *node)
{
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(node);
  return priv->parameters;
}


G_DEFINE_TYPE(GGandivaLiteralNode,
              ggandiva_literal_node,
              GGANDIVA_TYPE_NODE)

static void
ggandiva_literal_node_init(GGandivaLiteralNode *literal_node)
{
}

static void
ggandiva_literal_node_class_init(GGandivaLiteralNodeClass *klass)
{
}


G_DEFINE_TYPE(GGandivaBooleanLiteralNode,
              ggandiva_boolean_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_boolean_literal_node_init(GGandivaBooleanLiteralNode *boolean_literal_node)
{
}

static void
ggandiva_boolean_literal_node_class_init(GGandivaBooleanLiteralNodeClass *klass)
{
}

/**
 * ggandiva_boolean_literal_node_new:
 * @value: The value of the boolean literal.
 *
 * Returns: A newly created #GGandivaBooleanLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaBooleanLiteralNode *
ggandiva_boolean_literal_node_new(gboolean value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(static_cast<bool>(value));
  return GGANDIVA_BOOLEAN_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_boolean_literal_node_get_value:
 * @node: A #GGandivaBooleanLiteralNode.
 *
 * Returns: The value of the boolean literal.
 *
 * Since: 0.12.0
 */
gboolean
ggandiva_boolean_literal_node_get_value(GGandivaBooleanLiteralNode *node)
{
  auto value = ggandiva_literal_node_get<bool>(GGANDIVA_LITERAL_NODE(node));
  return static_cast<gboolean>(value);
}


G_DEFINE_TYPE(GGandivaInt8LiteralNode,
              ggandiva_int8_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_int8_literal_node_init(GGandivaInt8LiteralNode *int8_literal_node)
{
}

static void
ggandiva_int8_literal_node_class_init(GGandivaInt8LiteralNodeClass *klass)
{
}

/**
 * ggandiva_int8_literal_node_new:
 * @value: The value of the 8-bit integer literal.
 *
 * Returns: A newly created #GGandivaInt8LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaInt8LiteralNode *
ggandiva_int8_literal_node_new(gint8 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_INT8_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_int8_literal_node_get_value:
 * @node: A #GGandivaInt8LiteralNode.
 *
 * Returns: The value of the 8-bit integer literal.
 *
 * Since: 0.12.0
 */
gint8
ggandiva_int8_literal_node_get_value(GGandivaInt8LiteralNode *node)
{
  return ggandiva_literal_node_get<int8_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaUInt8LiteralNode,
              ggandiva_uint8_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_uint8_literal_node_init(GGandivaUInt8LiteralNode *uint8_literal_node)
{
}

static void
ggandiva_uint8_literal_node_class_init(GGandivaUInt8LiteralNodeClass *klass)
{
}

/**
 * ggandiva_uint8_literal_node_new:
 * @value: The value of the 8-bit unsigned integer literal.
 *
 * Returns: A newly created #GGandivaUInt8LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaUInt8LiteralNode *
ggandiva_uint8_literal_node_new(guint8 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_UINT8_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_uint8_literal_node_get_value:
 * @node: A #GGandivaUInt8LiteralNode.
 *
 * Returns: The value of the 8-bit unsigned integer literal.
 *
 * Since: 0.12.0
 */
guint8
ggandiva_uint8_literal_node_get_value(GGandivaUInt8LiteralNode *node)
{
  return ggandiva_literal_node_get<uint8_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaInt16LiteralNode,
              ggandiva_int16_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_int16_literal_node_init(GGandivaInt16LiteralNode *int16_literal_node)
{
}

static void
ggandiva_int16_literal_node_class_init(GGandivaInt16LiteralNodeClass *klass)
{
}

/**
 * ggandiva_int16_literal_node_new:
 * @value: The value of the 16-bit integer literal.
 *
 * Returns: A newly created #GGandivaInt16LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaInt16LiteralNode *
ggandiva_int16_literal_node_new(gint16 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_INT16_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_int16_literal_node_get_value:
 * @node: A #GGandivaInt16LiteralNode.
 *
 * Returns: The value of the 16-bit integer literal.
 *
 * Since: 0.12.0
 */
gint16
ggandiva_int16_literal_node_get_value(GGandivaInt16LiteralNode *node)
{
  return ggandiva_literal_node_get<int16_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaUInt16LiteralNode,
              ggandiva_uint16_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_uint16_literal_node_init(GGandivaUInt16LiteralNode *uint16_literal_node)
{
}

static void
ggandiva_uint16_literal_node_class_init(GGandivaUInt16LiteralNodeClass *klass)
{
}

/**
 * ggandiva_uint16_literal_node_new:
 * @value: The value of the 16-bit unsigned integer literal.
 *
 * Returns: A newly created #GGandivaUInt16LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaUInt16LiteralNode *
ggandiva_uint16_literal_node_new(guint16 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_UINT16_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_uint16_literal_node_get_value:
 * @node: A #GGandivaUInt16LiteralNode.
 *
 * Returns: The value of the 16-bit unsigned integer literal.
 *
 * Since: 0.12.0
 */
guint16
ggandiva_uint16_literal_node_get_value(GGandivaUInt16LiteralNode *node)
{
  return ggandiva_literal_node_get<uint16_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaInt32LiteralNode,
              ggandiva_int32_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_int32_literal_node_init(GGandivaInt32LiteralNode *int32_literal_node)
{
}

static void
ggandiva_int32_literal_node_class_init(GGandivaInt32LiteralNodeClass *klass)
{
}

/**
 * ggandiva_int32_literal_node_new:
 * @value: The value of the 32-bit integer literal.
 *
 * Returns: A newly created #GGandivaInt32LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaInt32LiteralNode *
ggandiva_int32_literal_node_new(gint32 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_INT32_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_int32_literal_node_get_value:
 * @node: A #GGandivaInt32LiteralNode.
 *
 * Returns: The value of the 32-bit integer literal.
 *
 * Since: 0.12.0
 */
gint32
ggandiva_int32_literal_node_get_value(GGandivaInt32LiteralNode *node)
{
  return ggandiva_literal_node_get<int32_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaUInt32LiteralNode,
              ggandiva_uint32_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_uint32_literal_node_init(GGandivaUInt32LiteralNode *uint32_literal_node)
{
}

static void
ggandiva_uint32_literal_node_class_init(GGandivaUInt32LiteralNodeClass *klass)
{
}

/**
 * ggandiva_uint32_literal_node_new:
 * @value: The value of the 32-bit unsigned integer literal.
 *
 * Returns: A newly created #GGandivaUInt32LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaUInt32LiteralNode *
ggandiva_uint32_literal_node_new(guint32 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_UINT32_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_uint32_literal_node_get_value:
 * @node: A #GGandivaUInt32LiteralNode.
 *
 * Returns: The value of the 32-bit unsigned integer literal.
 *
 * Since: 0.12.0
 */
guint32
ggandiva_uint32_literal_node_get_value(GGandivaUInt32LiteralNode *node)
{
  return ggandiva_literal_node_get<uint32_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaInt64LiteralNode,
              ggandiva_int64_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_int64_literal_node_init(GGandivaInt64LiteralNode *int64_literal_node)
{
}

static void
ggandiva_int64_literal_node_class_init(GGandivaInt64LiteralNodeClass *klass)
{
}

/**
 * ggandiva_int64_literal_node_new:
 * @value: The value of the 64-bit integer literal.
 *
 * Returns: A newly created #GGandivaInt64LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaInt64LiteralNode *
ggandiva_int64_literal_node_new(gint64 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_INT64_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_int64_literal_node_get_value:
 * @node: A #GGandivaInt64LiteralNode.
 *
 * Returns: The value of the 64-bit integer literal.
 *
 * Since: 0.12.0
 */
gint64
ggandiva_int64_literal_node_get_value(GGandivaInt64LiteralNode *node)
{
  return ggandiva_literal_node_get<int64_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaUInt64LiteralNode,
              ggandiva_uint64_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_uint64_literal_node_init(GGandivaUInt64LiteralNode *uint64_literal_node)
{
}

static void
ggandiva_uint64_literal_node_class_init(GGandivaUInt64LiteralNodeClass *klass)
{
}

/**
 * ggandiva_uint64_literal_node_new:
 * @value: The value of the 64-bit unsigned integer literal.
 *
 * Returns: A newly created #GGandivaUInt64LiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaUInt64LiteralNode *
ggandiva_uint64_literal_node_new(guint64 value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_UINT64_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_uint64_literal_node_get_value:
 * @node: A #GGandivaUInt64LiteralNode.
 *
 * Returns: The value of the 64-bit unsigned integer literal.
 *
 * Since: 0.12.0
 */
guint64
ggandiva_uint64_literal_node_get_value(GGandivaUInt64LiteralNode *node)
{
  return ggandiva_literal_node_get<uint64_t>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaFloatLiteralNode,
              ggandiva_float_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_float_literal_node_init(GGandivaFloatLiteralNode *float_literal_node)
{
}

static void
ggandiva_float_literal_node_class_init(GGandivaFloatLiteralNodeClass *klass)
{
}

/**
 * ggandiva_float_literal_node_new:
 * @value: The value of the 32-bit floating point literal.
 *
 * Returns: A newly created #GGandivaFloatLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaFloatLiteralNode *
ggandiva_float_literal_node_new(gfloat value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_FLOAT_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_float_literal_node_get_value:
 * @node: A #GGandivaFloatLiteralNode.
 *
 * Returns: The value of the 32-bit floating point literal.
 *
 * Since: 0.12.0
 */
gfloat
ggandiva_float_literal_node_get_value(GGandivaFloatLiteralNode *node)
{
  return ggandiva_literal_node_get<float>(GGANDIVA_LITERAL_NODE(node));
}


G_DEFINE_TYPE(GGandivaDoubleLiteralNode,
              ggandiva_double_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_double_literal_node_init(GGandivaDoubleLiteralNode *double_literal_node)
{
}

static void
ggandiva_double_literal_node_class_init(GGandivaDoubleLiteralNodeClass *klass)
{
}

/**
 * ggandiva_double_literal_node_new:
 * @value: The value of the 64-bit floating point literal.
 *
 * Returns: A newly created #GGandivaDoubleLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaDoubleLiteralNode *
ggandiva_double_literal_node_new(gdouble value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeLiteral(value);
  return GGANDIVA_DOUBLE_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_double_literal_node_get_value:
 * @node: A #GGandivaDoubleLiteralNode.
 *
 * Returns: The value of the 64-bit floating point literal.
 *
 * Since: 0.12.0
 */
gdouble
ggandiva_double_literal_node_get_value(GGandivaDoubleLiteralNode *node)
{
  return ggandiva_literal_node_get<double>(GGANDIVA_LITERAL_NODE(node));
}


typedef struct GGandivaBinaryLiteralNodePrivate_ {
  GBytes *value;
} GGandivaBinaryLiteralNodePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GGandivaBinaryLiteralNode,
                           ggandiva_binary_literal_node,
                           GGANDIVA_TYPE_LITERAL_NODE)

#define GGANDIVA_BINARY_LITERAL_NODE_GET_PRIVATE(object)                \
  static_cast<GGandivaBinaryLiteralNodePrivate *>(                      \
    ggandiva_binary_literal_node_get_instance_private(                  \
      GGANDIVA_BINARY_LITERAL_NODE(object)))

static void
ggandiva_binary_literal_node_dispose(GObject *object)
{
  auto priv = GGANDIVA_BINARY_LITERAL_NODE_GET_PRIVATE(object);

  if (priv->value) {
    g_bytes_unref(priv->value);
    priv->value = nullptr;
  }

  G_OBJECT_CLASS(ggandiva_binary_literal_node_parent_class)->dispose(object);
}

static void
ggandiva_binary_literal_node_init(GGandivaBinaryLiteralNode *binary_literal_node)
{
}

static void
ggandiva_binary_literal_node_class_init(GGandivaBinaryLiteralNodeClass *klass)
{
  auto gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->dispose = ggandiva_binary_literal_node_dispose;
}

/**
 * ggandiva_binary_literal_node_new:
 * @value: (array length=size): The value of the binary literal.
 * @size: The number of bytes of the value.
 *
 * Returns: A newly created #GGandivaBinaryLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaBinaryLiteralNode *
ggandiva_binary_literal_node_new(const guint8 *value,
                                 gsize size)
{
  auto gandiva_node =
    gandiva::TreeExprBuilder::MakeBinaryLiteral(std::string(reinterpret_cast<const char *>(value),
                                                            size));
  return GGANDIVA_BINARY_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_binary_literal_node_new_bytes:
 * @value: The value of the binary literal.
 *
 * Returns: A newly created #GGandivaBinaryLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaBinaryLiteralNode *
ggandiva_binary_literal_node_new_bytes(GBytes *value)
{
  size_t value_size;
  auto raw_value = g_bytes_get_data(value, &value_size);
  auto gandiva_node =
    gandiva::TreeExprBuilder::MakeBinaryLiteral(
      std::string(reinterpret_cast<const char *>(raw_value),
                  value_size));
  auto literal_node = ggandiva_literal_node_new_raw(&gandiva_node);
  auto priv = GGANDIVA_BINARY_LITERAL_NODE_GET_PRIVATE(literal_node);
  priv->value = value;
  g_bytes_ref(priv->value);
  return GGANDIVA_BINARY_LITERAL_NODE(literal_node);
}

/**
 * ggandiva_binary_literal_node_get_value:
 * @node: A #GGandivaBinaryLiteralNode.
 *
 * Returns: (transfer none): The value of the binary literal.
 *
 * Since: 0.12.0
 */
GBytes *
ggandiva_binary_literal_node_get_value(GGandivaBinaryLiteralNode *node)
{
  auto priv = GGANDIVA_BINARY_LITERAL_NODE_GET_PRIVATE(node);
  if (!priv->value) {
    auto value = ggandiva_literal_node_get<std::string>(GGANDIVA_LITERAL_NODE(node));
    priv->value = g_bytes_new(value.data(), value.size());
  }

  return priv->value;
}


G_DEFINE_TYPE(GGandivaStringLiteralNode,
              ggandiva_string_literal_node,
              GGANDIVA_TYPE_LITERAL_NODE)

static void
ggandiva_string_literal_node_init(GGandivaStringLiteralNode *string_literal_node)
{
}

static void
ggandiva_string_literal_node_class_init(GGandivaStringLiteralNodeClass *klass)
{
}

/**
 * ggandiva_string_literal_node_new:
 * @value: The value of the UTF-8 encoded string literal.
 *
 * Returns: A newly created #GGandivaStringLiteralNode.
 *
 * Since: 0.12.0
 */
GGandivaStringLiteralNode *
ggandiva_string_literal_node_new(const gchar *value)
{
  auto gandiva_node = gandiva::TreeExprBuilder::MakeStringLiteral(value);
  return GGANDIVA_STRING_LITERAL_NODE(ggandiva_literal_node_new_raw(&gandiva_node));
}

/**
 * ggandiva_string_literal_node_get_value:
 * @node: A #GGandivaStringLiteralNode.
 *
 * Returns: The value of the UTF-8 encoded string literal.
 *
 * Since: 0.12.0
 */
const gchar *
ggandiva_string_literal_node_get_value(GGandivaStringLiteralNode *node)
{
  auto value = ggandiva_literal_node_get<std::string>(GGANDIVA_LITERAL_NODE(node));
  return value.c_str();
}

G_END_DECLS

std::shared_ptr<gandiva::Node>
ggandiva_node_get_raw(GGandivaNode *node)
{
  auto priv = GGANDIVA_NODE_GET_PRIVATE(node);
  return priv->node;
}

GGandivaFieldNode *
ggandiva_field_node_new_raw(std::shared_ptr<gandiva::Node> *gandiva_node,
                            GArrowField *field)
{
  auto field_node = g_object_new(GGANDIVA_TYPE_FIELD_NODE,
                                 "node", gandiva_node,
                                 "field", field,
                                 NULL);
  return GGANDIVA_FIELD_NODE(field_node);
}

GGandivaFunctionNode *
ggandiva_function_node_new_raw(std::shared_ptr<gandiva::Node> *gandiva_node,
                               const gchar *name,
                               GList *parameters,
                               GArrowDataType *return_type)
{
  auto function_node = g_object_new(GGANDIVA_TYPE_FUNCTION_NODE,
                                    "node", gandiva_node,
                                    "name", name,
                                    "return-type", return_type,
                                    NULL);
  auto priv = GGANDIVA_FUNCTION_NODE_GET_PRIVATE(function_node);
  for (auto node = parameters; node; node = g_list_next(node)) {
    auto parameter = GGANDIVA_NODE(node->data);
    priv->parameters = g_list_prepend(priv->parameters, g_object_ref(parameter));
  }
  priv->parameters = g_list_reverse(priv->parameters);
  return GGANDIVA_FUNCTION_NODE(function_node);
}

GGandivaLiteralNode *
ggandiva_literal_node_new_raw(std::shared_ptr<gandiva::Node> *gandiva_node)
{
  GType type;

  switch ((*gandiva_node)->return_type()->id()) {
  case arrow::Type::BOOL:
    type = GGANDIVA_TYPE_BOOLEAN_LITERAL_NODE;
    break;
  case arrow::Type::type::UINT8:
    type = GGANDIVA_TYPE_UINT8_LITERAL_NODE;
    break;
  case arrow::Type::type::UINT16:
    type = GGANDIVA_TYPE_UINT16_LITERAL_NODE;
    break;
  case arrow::Type::type::UINT32:
    type = GGANDIVA_TYPE_UINT32_LITERAL_NODE;
    break;
  case arrow::Type::type::UINT64:
    type = GGANDIVA_TYPE_UINT64_LITERAL_NODE;
    break;
  case arrow::Type::type::INT8:
    type = GGANDIVA_TYPE_INT8_LITERAL_NODE;
    break;
  case arrow::Type::type::INT16:
    type = GGANDIVA_TYPE_INT16_LITERAL_NODE;
    break;
  case arrow::Type::type::INT32:
    type = GGANDIVA_TYPE_INT32_LITERAL_NODE;
    break;
  case arrow::Type::type::INT64:
    type = GGANDIVA_TYPE_INT64_LITERAL_NODE;
    break;
  case arrow::Type::type::FLOAT:
    type = GGANDIVA_TYPE_FLOAT_LITERAL_NODE;
    break;
  case arrow::Type::type::DOUBLE:
    type = GGANDIVA_TYPE_DOUBLE_LITERAL_NODE;
    break;
  case arrow::Type::type::STRING:
    type = GGANDIVA_TYPE_STRING_LITERAL_NODE;
    break;
  case arrow::Type::type::BINARY:
    type = GGANDIVA_TYPE_BINARY_LITERAL_NODE;
    break;
  default:
    type = GGANDIVA_TYPE_LITERAL_NODE;
    break;
  }
  auto literal_node = GGANDIVA_LITERAL_NODE(g_object_new(type,
                                                         "node", gandiva_node,
                                                         NULL));
  return literal_node;
}