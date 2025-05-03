#include <Python.h>
#include <iostream>

int main() {
    // 1. Initialize the Python interpreter
    Py_Initialize();

    // 2. Set the path to the directory where the .so file is located (current directory)
    PyRun_SimpleString("import sys; sys.path.append('./compiled_python')");

    // 3. Import the compiled .so module (no need to add .so extension)
    PyObject* pName = PyUnicode_FromString("multiply_script");  // 'script' matches the Python module name
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // 4. Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "multiply");

        if (pFunc && PyCallable_Check(pFunc)) {

            // -----------------------------------------------------
            // 5. Build arguments: multiply(6, 7)
            PyObject* pArgs = PyTuple_Pack(2, PyLong_FromLong(6), PyLong_FromLong(7));
            //------------------------------------------------

            // 6. Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                std::cout << "Result: " << PyLong_AsLong(pValue) << std::endl;
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cerr << "Function call failed.\n";
            }

            Py_DECREF(pFunc);
        } else {
            if (PyErr_Occurred()) PyErr_Print();
            std::cerr << "Cannot find function 'multiply'.\n";
        }

        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load module 'script'.\n";
    }

    // 7. Finalize the interpreter
    Py_Finalize();
    return 0;
}
