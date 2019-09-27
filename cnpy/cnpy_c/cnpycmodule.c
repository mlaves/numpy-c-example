#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>

static PyObject * process_arr_c(PyObject * self, PyObject * args)
{
	PyArrayObject *in_array;
    PyObject      *out_array;
    NpyIter       *in_iter;
    NpyIter       *out_iter;
    NpyIter_IterNextFunc *in_iternext;
    NpyIter_IterNextFunc *out_iternext;

    // parse single numpy array argument
    if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &in_array))
    {
      printf("Error parsing input\n");
      return NULL;
    }

    // construct the output array, like the input array
    out_array = PyArray_NewLikeArray(in_array, NPY_ANYORDER, NULL, 0);
    if (out_array == NULL)
    {
        printf("Error creating output array\n");
        return NULL;
    }

    // create the iterators
    in_iter = NpyIter_New(in_array, NPY_ITER_READONLY, NPY_KEEPORDER,
                          NPY_NO_CASTING, NULL);
    if (in_iter == NULL)
    {
        printf("Error at in_iter\n");
        Py_XDECREF(out_array);
        return NULL;
    }

    out_iter = NpyIter_New((PyArrayObject *)out_array, NPY_ITER_READWRITE,
                          NPY_KEEPORDER, NPY_NO_CASTING, NULL);
                          
    if (out_iter == NULL)
    {
        NpyIter_Deallocate(in_iter);
        printf("Error at out_iter\n");
        Py_XDECREF(out_array);
        return NULL;
    }

    in_iternext = NpyIter_GetIterNext(in_iter, NULL);
    out_iternext = NpyIter_GetIterNext(out_iter, NULL);
    
    if (in_iternext == NULL || out_iternext == NULL)
    {
        NpyIter_Deallocate(in_iter);
        NpyIter_Deallocate(out_iter);
        printf("Error at out_iter\n");
        Py_XDECREF(out_array);
        return NULL;
    }

    double ** in_dataptr = (double **) NpyIter_GetDataPtrArray(in_iter);
    double ** out_dataptr = (double **) NpyIter_GetDataPtrArray(out_iter);

    // iterate over the arrays
    do
    {
        **out_dataptr = cos(**in_dataptr);
    } while(in_iternext(in_iter) && out_iternext(out_iter));

    // clean up and return the result
    NpyIter_Deallocate(in_iter);
    NpyIter_Deallocate(out_iter);

    Py_INCREF(out_array);

    return out_array;
}

static PyObject * get_arr_c(PyObject * self, PyObject * args)
{
    PyArrayObject *in_array;
    int           nd;
    long int      *shape;
    unsigned char *data;
    int i;
    
    // parse single numpy array argument
    if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &in_array))
    {
      printf("Error parsing input\n");
      Py_RETURN_NONE;
    }
    
    nd = PyArray_NDIM(in_array);
    shape = PyArray_SHAPE(in_array);
    data = (unsigned char *)PyArray_DATA(in_array);
    
    printf("%u, %u, %u\n", shape[0], shape[1], shape[2]);

    // iterate over data and fill it
    for (i = 0; i < shape[0]*shape[1]*shape[2]; i++)
    {
        data[i] = (unsigned char) 1;
    }

    Py_RETURN_NONE;
}

static PyMethodDef cnpyMethods[] =
{
    { "process_arr", process_arr_c, METH_VARARGS, "docstring" },
    { "get_arr",     get_arr_c,     METH_VARARGS, "docstring" },
    { NULL, NULL, 0, NULL } // sentinel
};

// The method table must be referenced in the module definition structure:
static struct PyModuleDef cnpymodule = {
    PyModuleDef_HEAD_INIT,
    "cnpy_c", // name of module
    NULL,     // module documentation, may be NULL
    -1,       // size of per-interpreter state of the module, or -1 if the module keeps state in global variables
    cnpyMethods
};

PyMODINIT_FUNC PyInit_cnpy_c(void)
{
    import_array();
    return PyModule_Create(&cnpymodule);
}
