#include <Python.h>
#include <iostream>
#include <memory>
#include <map>

class SimpleMath_CPP_WRAPPER
{
    public:
    
        enum Operations {
            MULTIPLY,
            DIVIDE,
            ADD
        };

    public:
        SimpleMath_CPP_WRAPPER()
        {
            // 2. Set the path to the directory where the .so file is located 
            // because of python indentation. DO NOT change indentation
            PyRun_SimpleString(R"(
import sys
path = './bundle/lib/'
if path not in sys.path:
    sys.path.append(path)
)"
            );

            // initilaize with .so file name 
            PyObject* pName = PyUnicode_FromString("simple_class");
            pModule = PyImport_Import(pName);
            Py_DECREF(pName);

            if (pModule == nullptr) {        
                PyErr_Print();
            }

            // Get the class from the module
            pClass = PyObject_GetAttrString(pModule, "SimpleMath");
            if (!pClass || ! PyCallable_Check(pClass)) {        
                PyErr_Print();
            }

            // Create an instance of the class with arguments
            //PyObject* pArgs = PyTuple_Pack(2, PyLong_FromLong(6), PyLong_FromLong(3));
            pInstance = PyObject_CallObject(pClass, nullptr);
            //Py_DECREF(pArgs);

            if (pInstance == nullptr) {        
                PyErr_Print();
            }
            
        }

        double performOperation(Operations operation, int a, int b)
        {
            double result = -1;

            // Create Python arguments
            PyObject* arg1 = PyLong_FromLong(a);
            PyObject* arg2 = PyLong_FromLong(b);

            PyObject* pResult = PyObject_CallMethodObjArgs(pInstance, PyUnicode_FromString(enumToString[operation].c_str()), arg1, arg2, NULL);  // PyObject_CallMethodObjArgs  always ends with NULL
            if (pResult != nullptr) {
                result = PyFloat_AsDouble(pResult);
            } else {
                std::cout << "PResult error :";
                PyErr_Print();
            }

            // Clean up
            Py_XDECREF(arg1);
            Py_XDECREF(arg2);
            Py_XDECREF(pResult);

            return result;
        }

        ~SimpleMath_CPP_WRAPPER()
        {
            std::cout << "\n In destructor \n";
            
            Py_XDECREF(pInstance);

            Py_XDECREF(pClass);

            Py_XDECREF(pModule);

        }
    private:
        PyObject* pModule = nullptr;
        PyObject* pClass = nullptr;
        PyObject* pInstance = nullptr;


        // Create a map to associate enum with strings
        std::map<Operations, std::string> enumToString = {
            {MULTIPLY, "multiply"},
            {DIVIDE, "divide"},
            {ADD, "add"}
        };
};
int main() {

    // this wil overwrite the PYTHONPATH 
    // Don't use this in every class. Instead, keep all necessary packages in same environment
    setenv("PYTHONPATH",
       "./bundle/venv/lib/python3.9:"
       "./bundle/venv/lib/python3.9/site-packages:"
       "./bundle/lib", 1);

    // The program entry point should initialize Python interpreter
    // Never initialize it in class
    Py_Initialize();

    

    std::unique_ptr<SimpleMath_CPP_WRAPPER> cpp_warapper = std::make_unique<SimpleMath_CPP_WRAPPER>();

    std::cout << "Multiply :" << cpp_warapper->performOperation(SimpleMath_CPP_WRAPPER::Operations::MULTIPLY, 2,3) << "\n";
    std::cout << "Divide :" << cpp_warapper->performOperation(SimpleMath_CPP_WRAPPER::Operations::DIVIDE, 2,3) << "\n";

    cpp_warapper.reset();

    Py_Finalize();
    return 0;
}
