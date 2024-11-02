#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "simplex_wrapper.h"

void init_python(void) {
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
}

void finalize_python(void) {
    Py_Finalize();
}

PyObject* call_simplex_method(PyObject* c, PyObject* A, PyObject* b, double epsilon, int maximize) {
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    pModule = PyImport_ImportModule("simplex");
    if (pModule == NULL) {
        PyErr_Print();
        return NULL;
    }

    pFunc = PyObject_GetAttrString(pModule, "simplex_method");
    if (pFunc == NULL) {
        PyErr_Print();
        Py_DECREF(pModule);
        return NULL;
    }

    pArgs = PyTuple_Pack(5, c, A, b, PyFloat_FromDouble(epsilon), PyBool_FromLong(maximize));
    if (pArgs == NULL) {
        PyErr_Print();
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        return NULL;
    }

    pValue = PyObject_CallObject(pFunc, pArgs);
    
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    
    return pValue;
}