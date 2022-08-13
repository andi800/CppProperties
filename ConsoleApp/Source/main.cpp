#include "PropValue.h"
#include "MyClass.h"

using namespace std;
using namespace Properties;
using namespace MyClassNamespace;


struct StyleType
{
    UndoRedoManager *_undoRedoMan;
    PropertyManager _localPropMan;
    PropValue<double> Margin = PropValue<double>("Margin", std::make_shared<double>(1000.0), _undoRedoMan, &_localPropMan);
    PropValue<double> Size = PropValue<double>("Size", 2000.0, _undoRedoMan, &_localPropMan);  // automatic creation of shared ptr for 200.0
    PropValue<int> Padding = PropValue<int>("Padding", 3000, _undoRedoMan, &_localPropMan);
    PropValue<bool> IsDrawn = PropValue<bool>("IsDrawn", true, _undoRedoMan, &_localPropMan);

    StyleType(const std::string& prefix, UndoRedoManager *undoRedoMan, PropertyManager *propMan) : _undoRedoMan(undoRedoMan), _localPropMan(PropertyManager(prefix))
    {
        if (propMan != nullptr) {
            propMan->addChild(&_localPropMan);
        }
    }
};


class MyModel
{
public:

    // Note: The members will be initialized in the same order they are declared (even if you write the initializers in constructor in a different order).
    UndoRedoManager *_undoRedoMan;
    PropertyManager *_propMan;

    PropValue<double> Speed = PropValue("Speed", std::make_shared<double>(100.0), _undoRedoMan, _propMan);
    PropValue<double> SpeedNoSave = PropValue("SpeedNoSave", std::make_shared<double>(110.0), _undoRedoMan, nullptr);
    PropValue<double> Size = PropValue("Size", 200.0, _undoRedoMan, _propMan);  // automatic creation of shared ptr for 200.0
    PropValue<int> Count = PropValue("Count", 300, _undoRedoMan, _propMan);
    PropValue<bool> IsEmpty = PropValue("IsEmpty", true, _undoRedoMan, _propMan);
    PropValue<MyClass> Custom = PropValue("Custom", make_shared<MyClass>(), _undoRedoMan, _propMan);

    StyleType StyleA = StyleType("StyleA/", _undoRedoMan, _propMan);
    StyleType StyleB = StyleType("StyleB/", _undoRedoMan, _propMan);

    // TODO: list, set, map, container(class or struct)

    MyModel(UndoRedoManager *undoRedoMan, PropertyManager *propMan) : _undoRedoMan(undoRedoMan), _propMan(propMan)
    {
    }
};



int main(int argc, char *argv[])
{
    UndoRedoManager undoRedoMan;
    PropertyManager propMan;
    MyModel model(&undoRedoMan, &propMan);

    printf("Save all properties to JSON:\n");
    printf("---------------------------------------\n");
    propMan.toJson();
    printf("---------------------------------------\n");

    printf("Start...\n");


    PropValue MyProb("myPropname", std::make_shared<double>(100.0), &undoRedoMan, &propMan);

    PropValue MyProb2("myPropname2", 200.0, &undoRedoMan, &propMan);  // automatic creation of shared ptr for 200.0


    shared_ptr myP = make_shared<double>(2);

    *myP = 0;
    printf("myP: %f\n", *myP);

    MyProb.setUpateFunction([=](double newValue) { // update function using lambda function
        printf("old: %f new: %f\n", *myP, newValue);
        *myP = newValue;
    });

    
    undoRedoMan.beginCommand("NewCommand");
    MyProb.set(std::make_shared<double>(5));

    MyProb2.set(6); // automatic creation of shared ptr for 6
    undoRedoMan.endCommand();


    PropValue<int> myInt = PropValue("myInt", 200, &undoRedoMan, &propMan);
    PropValue<double> myDouble = PropValue("myDouble", 300.4, &undoRedoMan, &propMan);
    PropValue<bool> myBool = PropValue("myBool", true, &undoRedoMan, &propMan);
    PropValue<MyClass> myMyClass = PropValue("myMyClass", make_shared<MyClass>(), &undoRedoMan, &propMan);

    // Get tests
    printf("GET: myInt: %d\n", myInt.get());
    printf("GET: myDouble: %f\n", myDouble.get());
    printf("GET: myBool: %d\n", myBool.get());
    printf("GET: myMyClass: %s\n", myMyClass.get().getString().c_str());


    // Json tests
    auto jsonStr = myInt.toJson();
    printf("JSON: myInt: %s\n", jsonStr.c_str());
    jsonStr = myDouble.toJson();
    printf("JSON: myDouble: %s\n", jsonStr.c_str());
    jsonStr = myBool.toJson();
    printf("JSON: myBool: %s\n", jsonStr.c_str());

    jsonStr = myMyClass.toJson();
    printf("JSON: myMyClass: %s\n", jsonStr.c_str());


    printf("Save all properties to JSON:\n");
    printf("---------------------------------------\n");
    propMan.toJson();
    printf("---------------------------------------\n");


    printf("test: %f\n", *myP);

    return 0;
}