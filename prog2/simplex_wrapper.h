#ifndef SIMPLEX_WRAPPER_H
#define SIMPLEX_WRAPPER_H

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_python(void);
void finalize_python(void);
PyObject* call_simplex_method(PyObject* c, PyObject* A, PyObject* b, double epsilon, int maximize);

#ifdef __cplusplus
}
#endif

#endif // SIMPLEX_WRAPPER_H