#include <Python.h>
#include <iostream>

double performOperation( PyObject* pInstance , std::string operation, int a , int b)
{
    PyObject* pResult = PyObject_CallMethod(pInstance, operation.c_str(),  "(ll)", a, b);
    if (pResult != nullptr) {
        double result = PyFloat_AsDouble(pResult);
        Py_DECREF(pResult);
        return result;
    } else {
        PyErr_Print();
    }

    return -1;
}

int main() {
     setenv("PYTHONPATH",
       "./bundle/venv/lib/python3.9:"
       "./bundle/venv/lib/python3.9/site-packages:"
       "./bundle/lib", 1);

    Py_Initialize();

    
    // 2. Set the path to the directory where the .so file is located (current directory)
    PyRun_SimpleString("import sys; sys.path.append('./bundle/lib/')");


    // Import the .so module (just use name without .so)
    PyObject* pName = PyUnicode_FromString("simple_class");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);


    if (pModule == nullptr) {
        
        PyErr_Print();
        return -1;
    }

    // Get the class from the module
    PyObject* pClass = PyObject_GetAttrString(pModule, "SimpleMath");
    if (!pClass || ! PyCallable_Check(pClass)) {        
        PyErr_Print();
        return -1;
    }

    // Create an instance of the class with arguments
    //PyObject* pArgs = PyTuple_Pack(2, PyLong_FromLong(6), PyLong_FromLong(3));
    PyObject* pInstance = PyObject_CallObject(pClass, nullptr);
    //Py_DECREF(pArgs);

    if (pInstance == nullptr) {        
        PyErr_Print();
        return -1;
    }
    
    // Call multiply method
    double result = performOperation(pInstance, "multiply", 4, 2);
    std::cout << "Result of multiply: " << result << std::endl;

    result = performOperation(pInstance, "divide", 8, 10);
    std::cout << "Result of divide: " << result << std::endl;

    Py_DECREF(pInstance);

    Py_DECREF(pClass);

    Py_DECREF(pModule);


    Py_Finalize();
    return 0;
}
