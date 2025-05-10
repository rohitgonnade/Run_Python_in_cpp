#include <Python.h>
#include <iostream>

long performOperation( PyObject* pInstance , std::string operation)
{
    PyObject* pResult = PyObject_CallMethod(pInstance, operation.c_str(), nullptr);
    if (pResult != nullptr) {
        long result = PyLong_AsLong(pResult);
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

    if (pModule != nullptr) {
        // Get the class from the module
        PyObject* pClass = PyObject_GetAttrString(pModule, "SimpleMath");
        if (pClass && PyCallable_Check(pClass)) {
            // Create an instance of the class with arguments
            PyObject* pArgs = PyTuple_Pack(2, PyLong_FromLong(6), PyLong_FromLong(3));
            PyObject* pInstance = PyObject_CallObject(pClass, pArgs);
            Py_DECREF(pArgs);

            if (pInstance != nullptr) {
                // Call multiply method
                long result = performOperation(pInstance, "multiply");
                std::cout << "Result of multiply: " << result << std::endl;

                result = performOperation(pInstance, "divide");
                std::cout << "Result of divide: " << result << std::endl;

                Py_DECREF(pInstance);
            } else {
                PyErr_Print();
            }

            Py_DECREF(pClass);
        } else {
            PyErr_Print();
        }

        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
    return 0;
}
