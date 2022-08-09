#include "Properties/PropValue.h"
#include "MyClass.h"

using namespace std;
using namespace Properties;
using namespace MyClassNamespace;



int main(int argc, char *argv[])
{

    printf("Start...\n");

    UndoRedoManager undoRedoMan;

    PropValue<double> MyProb("myPropname", std::make_shared<double>(100.0), undoRedoMan);

    PropValue<double> MyProb2("myPropname", 200.0, undoRedoMan);  // automatic creation of shared ptr for 200.0


    shared_ptr<double> myP = make_shared<double>(2);

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


    PropValue<int> myInt = PropValue("myInt", 200, undoRedoMan);
    PropValue<double> myDouble = PropValue("myDouble", 300.4, undoRedoMan);
    PropValue<bool> myBool = PropValue("myBool", true, undoRedoMan);
    PropValue<MyClass> myMyClass = PropValue("myMyClass", make_shared<MyClass>(), undoRedoMan);

    auto jsonStr = myInt.toJson();
    printf("JSON: myInt: %s\n", jsonStr.c_str());
    jsonStr = myDouble.toJson();
    printf("JSON: myDouble: %s\n", jsonStr.c_str());
    jsonStr = myBool.toJson();
    printf("JSON: myBool: %s\n", jsonStr.c_str());

    jsonStr = myMyClass.toJson();
    printf("JSON: myMyClass: %s\n", jsonStr.c_str());




    printf("test: %f\n", *myP);

    return 0;
}