#include "pch.h"
#include "CppUnitTest.h"
#include "PropValue.h"
#include "MyClass.h"
#include <iostream>
using namespace MyClassNamespace;
using namespace Properties;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestProperties
{


	TEST_CLASS(TestProperties)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            UndoRedoManager undoRedoMan;
            PropertyManager propMan;
            //MyModel model(&undoRedoMan, &propMan);

            PropValue<int> myInt = PropValue("myInt", 200, &undoRedoMan, &propMan);
            PropValue<double> myDouble = PropValue("myDouble", 300.4, &undoRedoMan, &propMan);
            PropValue<bool> myBool = PropValue("myBool", true, &undoRedoMan, &propMan);
            PropValue<MyClass> myMyClass = PropValue("myMyClass", make_shared<MyClass>(), &undoRedoMan, &propMan);

            Logger::WriteMessage(L"this is a test");

            Assert::AreEqual(
                // Expected value:
                0.0,
                // Actual value:
                1,
                // Tolerance:
                0.01,
                // Message:
                L"Basic test failed",
                // Line number - used if there is no PDB file:
                LINE_INFO());

            Assert::Fail(L"dummy", LINE_INFO());

            // Get tests
            printf("GET: myInt: %d\n", myInt.get());
            printf("GET: myDouble: %f\n", myDouble.get());
            printf("GET: myBool: %d\n", myBool.get());
            printf("GET: myMyClass: %s\n", myMyClass.get().getString().c_str());
            
		}
	};
}
