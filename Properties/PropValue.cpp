#include "PropValue.h"
#include "../MyClass.h"

namespace Properties
{

    std::string PropValue<int>::convertToJson(const int& value)
    {
        return "int to str: " + std::to_string(value);
    }

    std::string PropValue<double>::convertToJson(const double& value)
    {
        return "double to str: " + std::to_string(value);
    }

    std::string PropValue<bool>::convertToJson(const bool& value)
    {
        return "bool to str: " + std::to_string(value);
    }

    std::string PropValue<MyClassNamespace::MyClass>::convertToJson(const MyClassNamespace::MyClass & value)
    {
        return value.toJson();
    }




}
