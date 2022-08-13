#include "PropValue.h"
#include "MyClass.h"

namespace Properties
{

    std::string PropValue<int>::convertToJson(const int& value) const
    {
        return "int to str: " + std::to_string(value);
    }

    std::string PropValue<double>::convertToJson(const double& value) const
    {
        return "double to str: " + std::to_string(value);
    }

    std::string PropValue<bool>::convertToJson(const bool& value) const
    {
        return "bool to str: " + std::to_string(value);
    }

    std::string PropValue<MyClassNamespace::MyClass>::convertToJson(const MyClassNamespace::MyClass & value) const
    {
        return value.toJson();
    }




}
