/*
Copyright (c) 2013 Adafruit
Author: Justin Cooper

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "constants.h"
#include "common.h"
#include "c_adc.h"

// Cleanup function commented out because it is currently disabled and this
// generates unused method warnings. See adc_methods[] definition further 
// below for reason for disabling the cleanup function.
// python function cleanup()
//static PyObject *py_cleanup(PyObject *self, PyObject *args)
//{
//    // unexport the ADC
//    adc_cleanup();
//
//    Py_RETURN_NONE;
//}

// python function setup()
static PyObject *py_setup_adc(__attribute__ ((unused)) PyObject *self, __attribute__ ((unused)) PyObject *args)
{
    BBIO_err err;

    err = adc_setup();
    if (err != BBIO_OK) {
        PyErr_SetString(PyExc_RuntimeError, "Unable to setup ADC system. Possible causes are: \n"
                                        "  - A cape with a conflicting pin mapping is loaded \n"
                                        "  - A device tree object is loaded that uses the same name for a fragment: helper");
        return NULL;
    }

    Py_RETURN_NONE;
}

// python function read(channel)
static PyObject *py_read(__attribute__ ((unused)) PyObject *self, PyObject *args)
{
    int ain;
    float value;
    char *channel;
    PyObject *py_value;
    BBIO_err err;

    if (!PyArg_ParseTuple(args, "s", &channel))
        return NULL;

    // check setup was called prior
    if (!adc_initialized)
    {
        PyErr_SetString(PyExc_RuntimeError, "You must setup() ADC prior to calling read.");
        return NULL;
    }    

    err = get_adc_ain(channel, &ain);
    if (err != BBIO_OK) {
        PyErr_SetString(PyExc_ValueError, "Invalid AIN key or name.");
        return NULL;    
    }

    err = read_value(ain, &value);
    if (err != BBIO_OK) {
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, "Error while reading AIN port. Invalid or locked AIN file.");
        return NULL;
    }

    //scale modifier
#ifdef BBBVERSION41
    value = value / 4095.0;
#else
    value = value / 1800.0;
#endif

    py_value = Py_BuildValue("f", value);

    return py_value;
}

// python function read(channel)
static PyObject *py_read_raw(__attribute__ ((unused)) PyObject *self, PyObject *args)
{
    int ain;
    float value;
    char *channel;
    PyObject *py_value;
    BBIO_err err;

    if (!PyArg_ParseTuple(args, "s", &channel))
        return NULL;

   // check setup was called prior
    if (!adc_initialized)
    {
        PyErr_SetString(PyExc_RuntimeError, "You must setup() ADC prior to calling read.");
        return NULL;
    }       

    err = get_adc_ain(channel, &ain);
    if (err != BBIO_OK) {
        PyErr_SetString(PyExc_ValueError, "Invalid AIN key or name.");
        return NULL;    
    }

    err = read_value(ain, &value);

    if (err != BBIO_OK) {
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, "Error while reading AIN port. Invalid or locked AIN file.");
        return NULL;
    }

    py_value = Py_BuildValue("f", value);

    return py_value;
}

static const char moduledocstring[] = "ADC functionality of a BeagleBone using Python";

PyMethodDef adc_methods[] = {
    {"setup", py_setup_adc, METH_VARARGS, "Set up and start the ADC channel."},
    {"read", (PyCFunction)py_read, METH_VARARGS, "Read the normalized 0-1.0 analog value for the channel" },
    {"read_raw", (PyCFunction)py_read_raw, METH_VARARGS, "Read the raw analog value for the channel" },
    //disable cleanup for now, as unloading the driver locks up the system
    //{"cleanup", py_cleanup, METH_VARARGS, "Clean up ADC."},
    //{"setwarnings", py_setwarnings, METH_VARARGS, "Enable or disable warning messages"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef bbadcmodule = {
    PyModuleDef_HEAD_INIT,
    "ADC",       // name of module
    moduledocstring,  // module documentation, may be NULL
    -1,               // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    adc_methods
};
#endif

#if PY_MAJOR_VERSION > 2
PyMODINIT_FUNC PyInit_ADC(void)
#else
PyMODINIT_FUNC initADC(void)
#endif
{
    PyObject *module = NULL;

#if PY_MAJOR_VERSION > 2
    if ((module = PyModule_Create(&bbadcmodule)) == NULL)
       return NULL;
#else
    if ((module = Py_InitModule3("ADC", adc_methods, moduledocstring)) == NULL)
       return;
#endif

   define_constants(module);


#if PY_MAJOR_VERSION > 2
    return module;
#else
    return;
#endif
}
