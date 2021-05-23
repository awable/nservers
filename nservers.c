/*
 * Author: Akhil Wable <awable@gmail.com>
 *
 * Implementing Google Jump Hash
 *
 *    A Fast, Minimal Memory, Consistent Hash Algorithm
 *    John Lamping, Eric Veach
 *    https://arxiv.org/pdf/1406.2294.pdf
 *
 */

#include <Python.h>
#include <stdint.h>
#include "nservers.h"

#define NSERVERS_RANDNEXT(key) key*2862933555777941757ULL + 1

static PyObject*
NSERVERS_jump(PyObject *null, PyObject *args)
{
  uint64_t key;
  int32_t nservers;
  if (!PyArg_ParseTuple(args, "Ki", &key, &nservers)) { return NULL; }


  // Randomly but consistently assign when a key would jump to next
  // upon nservers growth. When a key jumps, it always jumps to the
  // newest server. Find the greatest jump less than  nservers is
  // where the key will currently be found.

  int64_t current_idx = -1;
  int64_t next_idx = 0;
  while (next_idx < nservers) {
    current_idx = next_idx;
    key = NSERVERS_RANDNEXT(key);
    next_idx = (current_idx+1) * ((double)(1LL << 31) / (double)((key >> 33) + 1));
  }

  return PyLong_FromLongLong(current_idx);
}

/* List of functions defined in the module */
static PyMethodDef nservers_methods[] = {
    {"jump", (PyCFunction)NSERVERS_jump,  METH_VARARGS,
     PyDoc_STR("jump(int:key, int:nservers) -> server idx in range [0 and nservers)")},

    {NULL, NULL}  // sentinel
};

PyDoc_STRVAR(module_doc,
"nservers: utilities for dealing with distributed servers"
);

/* Initialization function for the module (*must* be called initnservers) */

PyMODINIT_FUNC
initnservers(void)
{
    PyObject *m = Py_InitModule3("nservers", nservers_methods, module_doc);
    if (m == NULL) { return; }

    NSERVERS_Error = PyErr_NewException("on.Error", NULL, NULL);
    if (NSERVERS_Error == NULL) { return; }
    Py_INCREF(NSERVERS_Error);
    PyModule_AddObject(m, "Error", NSERVERS_Error);

    // Module version (the MODULE_VERSION macro is defined by setup.py)
    PyModule_AddStringConstant(m, "__version__", MODULE_VERSION);

}
