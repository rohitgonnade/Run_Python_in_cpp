#include <Python.h>
#include <iostream>

int main() {
    setenv("PYTHONPATH",
       "./bundle/venv/lib/python3.9:"
       "./bundle/venv/lib/python3.9/site-packages:"
       "./bundle/lib", 1);
    // 1. Initialize the Python interpreter
    Py_Initialize();

    // 2. Set the path to the directory where the .so file is located (current directory)
    PyRun_SimpleString("import sys; sys.path.append('./bundle/lib/')");

    // 3. Import the compiled .so module (no need to add .so extension)
    PyObject* pName = PyUnicode_FromString("multiply");  // 'script' matches the Python module name
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // 4. Get the function from the module
        //PyObject* pFunc = PyObject_GetAttrString(pModule, "multiply_num");

        PyObject* pFunc = PyObject_GetAttrString(pModule, "add_arrays");

        if (pFunc && PyCallable_Check(pFunc)) {

            // -----------------------------------------------------
            // 5. Build arguments: multiply(6, 7)
           // PyObject* pArgs = PyTuple_Pack(2, PyLong_FromLong(6), PyLong_FromLong(7));
           PyObject* pArgs = nullptr;
            //------------------------------------------------

            // 6. Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            
            if (pArgs) Py_DECREF(pArgs);

            if (pValue != nullptr) {
                // std::cout << "Result: " << PyLong_AsLong(pValue) << std::endl;
                // Py_DECREF(pValue);

                std::cout << "Result from NumPy: ";
                PyObject* repr = PyObject_Repr(pValue);
                std::cout << PyUnicode_AsUTF8(repr) << std::endl;
                
                if(repr) Py_DECREF(repr);
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