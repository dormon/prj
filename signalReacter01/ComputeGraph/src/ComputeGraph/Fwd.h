#pragma once

#include <memory>

namespace ComputeGraph {

enum class ResourceTypeKind;
class ResourceType;
class BoolResourceType;
class CustomResourceType;

template <typename CUSTOM_RESOURCE_TYPE_KIND_ENUM, typename>
CUSTOM_RESOURCE_TYPE_KIND_ENUM translateCustomResourceKind();

class Resource;
class BoolResource;

enum class Signal;
enum class StatementKind;
class Statement;
class StatementList;
class IfStatement;
class WhileStatement;

enum class InputKind;
class Input;
class FunctionInput;
class ResourceInput;
class Function;
}
