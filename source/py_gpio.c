/*
Copyright (c) 2013 Adafruit

Original RPi.GPIO Author Ben Croston
Modified for BBIO Author Justin Cooper

This file incorporates work covered by the following copyright and 
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Ben Croston

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
#include "event_gpio.h"

static PyObject *bb_revision;
static int gpio_warnings = 1;

struct py_callback
{
   char channel[32];
   unsigned int gpio;
   PyObject *py_cb;
   unsigned long long lastcall;
   unsigned int bouncetime;
   struct py_callback *next;
};
static struct py_callback *py_callbacks = NULL;

static int init_module(void)
{
    int i;

    for (i=0; i<120; i++)
        gpio_direction[i] = -1;

    module_setup = 1;

    return 0;
}

// python function cleanup()
static PyObject *py_cleanup(PyObject *self, PyObject *args)
{
    // clean up any exports
    event_cleanup();

    Py_RETURN_NONE;
}

// python function setup(channel, direction, pull_up_down=PUD_OFF, initial=None)
static PyObject *py_setup_channel(PyObject *self, PyObject *args, PyObject *kwargs)
{
   unsigned int gpio;
   char *channel;
   int direction;
   int pud = PUD_OFF;
   int initial = 0;
   static char *kwlist[] = {"channel", "direction", "pull_up_down", "initial", NULL};

   if (!PyArg_ParseTupleAndKeywords(args, kwargs, "si|ii", kwlist, &channel, &direction, &pud, &initial))
      return NULL;

   if (!module_setup) {
      init_module();
   }
   

   if (direction != INPUT && direction != OUTPUT)
   {
      PyErr_SetString(PyExc_ValueError, "An invalid direction was passed to setup()");
      return NULL;
   }

   if (direction == OUTPUT)
      pud = PUD_OFF;

   if (pud != PUD_OFF && pud != PUD_DOWN && pud != PUD_UP)
   {
      PyErr_SetString(PyExc_ValueError, "Invalid value for pull_up_down - should be either PUD_OFF, PUD_UP or PUD_DOWN");
      return NULL;
   }

    

   if (get_gpio_number(channel, &gpio))
       return NULL;

   gpio_export(gpio);
   gpio_set_direction(gpio, direction);
   if (direction == OUTPUT) {
       gpio_set_value(gpio, initial);
   } else {
       gpio_set_value(gpio, pud);
   }

   gpio_direction[gpio] = direction;

   Py_RETURN_NONE;
}

// python function output(channel, value)
static PyObject *py_output_gpio(PyObject *self, PyObject *args)
{
    unsigned int gpio;
    int value;
    char *channel;

    if (!PyArg_ParseTuple(args, "si", &channel, &value))
        return NULL;

    if (get_gpio_number(channel, &gpio))
        return NULL;      

    if (!module_setup || gpio_direction[gpio] != OUTPUT)
    {
        PyErr_SetString(PyExc_RuntimeError, "The GPIO channel has not been setup() as an OUTPUT");
        return NULL;
    }

    gpio_set_value(gpio, value);

    Py_RETURN_NONE;
}

// python function value = input(channel)
static PyObject *py_input_gpio(PyObject *self, PyObject *args)
{
    unsigned int gpio;
    char *channel;
    unsigned int value;
    PyObject *py_value;

    if (!PyArg_ParseTuple(args, "s", &channel))
        return NULL;

    if (get_gpio_number(channel, &gpio))
        return NULL;

   // check channel is set up as an input or output
    if (!module_setup || (gpio_direction[gpio] != INPUT && gpio_direction[gpio] != OUTPUT))
    {
        PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel first");
        return NULL;
    }

    gpio_get_value(gpio, &value);

    py_value = Py_BuildValue("i", value);

    return py_value;
}

static void run_py_callbacks(unsigned int gpio)
{
   PyObject *result;
   PyGILState_STATE gstate;
   struct py_callback *cb = py_callbacks;
   struct timeval tv_timenow;
   unsigned long long timenow;

   while (cb != NULL)
   {
      if (cb->gpio == gpio)
      {
         gettimeofday(&tv_timenow, NULL);
         timenow = tv_timenow.tv_sec*1E6 + tv_timenow.tv_usec;
         if (cb->bouncetime == 0 || timenow - cb->lastcall > cb->bouncetime*1000 || cb->lastcall == 0 || cb->lastcall > timenow) {
            // run callback
            gstate = PyGILState_Ensure();
            result = PyObject_CallFunction(cb->py_cb, "s", cb->channel);

            if (result == NULL && PyErr_Occurred())
            {
               PyErr_Print();
               PyErr_Clear();
            }
            Py_XDECREF(result);
            PyGILState_Release(gstate);
         }
         cb->lastcall = timenow;
      }
      cb = cb->next;
   }
}

static int add_py_callback(char *channel, unsigned int gpio, unsigned int bouncetime, PyObject *cb_func)
{
   struct py_callback *new_py_cb;
   struct py_callback *cb = py_callbacks;

   // add callback to py_callbacks list
   new_py_cb = malloc(sizeof(struct py_callback));
   if (new_py_cb == 0)
   {
      PyErr_NoMemory();
      return -1;
   }
   new_py_cb->py_cb = cb_func;
   Py_XINCREF(cb_func);         // Add a reference to new callback
   memset(new_py_cb->channel, 0, sizeof(new_py_cb->channel));
   strncpy(new_py_cb->channel, channel, sizeof(new_py_cb->channel) - 1);
   new_py_cb->gpio = gpio;
   new_py_cb->lastcall = 0;
   new_py_cb->bouncetime = bouncetime;
   new_py_cb->next = NULL;
   if (py_callbacks == NULL) {
      py_callbacks = new_py_cb;
   } else {
      // add to end of list
      while (cb->next != NULL)
         cb = cb->next;
      cb->next = new_py_cb;
   }
   add_edge_callback(gpio, run_py_callbacks);
   return 0;
}

// python function add_event_callback(gpio, callback, bouncetime=0)
static PyObject *py_add_event_callback(PyObject *self, PyObject *args, PyObject *kwargs)
{
   unsigned int gpio;
   char *channel;
   unsigned int bouncetime = 0;
   PyObject *cb_func;
   char *kwlist[] = {"gpio", "callback", "bouncetime", NULL};

   if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sO|i", kwlist, &channel, &cb_func, &bouncetime))
      return NULL;

   if (!PyCallable_Check(cb_func))
   {
      PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
      return NULL;
   }

   if (get_gpio_number(channel, &gpio))
       return NULL;

   // check channel is set up as an input
   if (!module_setup || gpio_direction[gpio] != INPUT)
   {
      PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
      return NULL;
   }

   if (!gpio_is_evented(gpio))
   {
      PyErr_SetString(PyExc_RuntimeError, "Add event detection using add_event_detect first before adding a callback");
      return NULL;
   }

   if (add_py_callback(channel, gpio, bouncetime, cb_func) != 0)
      return NULL;

   Py_RETURN_NONE;
}

// python function add_event_detect(gpio, edge, callback=None, bouncetime=0
static PyObject *py_add_event_detect(PyObject *self, PyObject *args, PyObject *kwargs)
{
   unsigned int gpio;
   char *channel;
   int edge, result;
   unsigned int bouncetime = 0;
   PyObject *cb_func = NULL;
   char *kwlist[] = {"gpio", "edge", "callback", "bouncetime", NULL};

   if (!PyArg_ParseTupleAndKeywords(args, kwargs, "si|Oi", kwlist, &channel, &edge, &cb_func, &bouncetime))
      return NULL;

   if (cb_func != NULL && !PyCallable_Check(cb_func))
   {
      PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
      return NULL;
   }

   if (get_gpio_number(channel, &gpio))
       return NULL;

   // check channel is set up as an input
   if (!module_setup || gpio_direction[gpio] != INPUT)
   {
      PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
      return NULL;
   }

   // is edge valid value
   if (edge != RISING_EDGE && edge != FALLING_EDGE && edge != BOTH_EDGE)
   {
      PyErr_SetString(PyExc_ValueError, "The edge must be set to RISING, FALLING or BOTH");
      return NULL;
   }

   if ((result = add_edge_detect(gpio, edge)) != 0)   // starts a thread
   {
      if (result == 1)
      {
         PyErr_SetString(PyExc_RuntimeError, "Edge detection already enabled for this GPIO channel");
         return NULL;
      } else {
         PyErr_SetString(PyExc_RuntimeError, "Failed to add edge detection");
         return NULL;
      }
   }

   if (cb_func != NULL)
      if (add_py_callback(channel, gpio, bouncetime, cb_func) != 0)
         return NULL;

   Py_RETURN_NONE;
}

// python function remove_event_detect(gpio)
static PyObject *py_remove_event_detect(PyObject *self, PyObject *args)
{
   unsigned int gpio;
   char *channel;
   struct py_callback *cb = py_callbacks;
   struct py_callback *temp;
   struct py_callback *prev = NULL;

   if (!PyArg_ParseTuple(args, "s", &channel))
      return NULL;

   if (get_gpio_number(channel, &gpio))
        return NULL;

   // remove all python callbacks for gpio
   while (cb != NULL)
   {
      if (cb->gpio == gpio)
      {
         Py_XDECREF(cb->py_cb);
         if (prev == NULL)
            py_callbacks = cb->next;
         else
            prev->next = cb->next;
         temp = cb;
         cb = cb->next;
         free(temp);
      } else {
         prev = cb;
         cb = cb->next;
      }
   }

   remove_edge_detect(gpio);

   Py_RETURN_NONE;
}

// python function value = event_detected(channel)
static PyObject *py_event_detected(PyObject *self, PyObject *args)
{
   unsigned int gpio;
   char *channel;

   if (!PyArg_ParseTuple(args, "s", &channel))
      return NULL;

   if (get_gpio_number(channel, &gpio))
       return NULL;

   if (event_detected(gpio))
      Py_RETURN_TRUE;
   else
      Py_RETURN_FALSE;
}

// python function py_wait_for_edge(gpio, edge)
static PyObject *py_wait_for_edge(PyObject *self, PyObject *args)
{
   unsigned int gpio;
   int edge, result;
   char *channel;
   char error[30];

   if (!PyArg_ParseTuple(args, "si", &channel, &edge))
      return NULL;

   if (get_gpio_number(channel, &gpio))
      return NULL;

   // check channel is setup as an input
   if (!module_setup || gpio_direction[gpio] != INPUT)
   {
      PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
      return NULL;
   }

   // is edge a valid value?
   if (edge != RISING_EDGE && edge != FALLING_EDGE && edge != BOTH_EDGE)
   {
      PyErr_SetString(PyExc_ValueError, "The edge must be set to RISING, FALLING or BOTH");
      return NULL;
   }

   Py_BEGIN_ALLOW_THREADS // disable GIL
   result = blocking_wait_for_edge(gpio, edge);
   Py_END_ALLOW_THREADS   // enable GIL

   if (result == 0) {
      Py_INCREF(Py_None);
      return Py_None;
   } else if (result == 2) {
      PyErr_SetString(PyExc_RuntimeError, "Edge detection events already enabled for this GPIO channel");
      return NULL;
   } else {
      sprintf(error, "Error #%d waiting for edge", result);
      PyErr_SetString(PyExc_RuntimeError, error);
      return NULL;
   }
   
   Py_RETURN_NONE;
}

// python function value = gpio_function(gpio)
static PyObject *py_gpio_function(PyObject *self, PyObject *args)
{
    unsigned int gpio;
    unsigned int value;
    PyObject *func;
    char *channel;


    if (!PyArg_ParseTuple(args, "s", &channel))
       return NULL;

    if (get_gpio_number(channel, &gpio))
        return NULL;

    if (setup_error)
    {
        PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
        return NULL;
    }

    gpio_get_direction(gpio, &value);
    func = Py_BuildValue("i", value);
    return func;
}

// python function setwarnings(state)
static PyObject *py_setwarnings(PyObject *self, PyObject *args)
{
   if (!PyArg_ParseTuple(args, "i", &gpio_warnings))
      return NULL;

   if (setup_error)
   {
      PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
      return NULL;
   }

   Py_RETURN_NONE;
}

static const char moduledocstring[] = "GPIO functionality of a BeagleBone using Python";

PyMethodDef gpio_methods[] = {
   {"setup", (PyCFunction)py_setup_channel, METH_VARARGS | METH_KEYWORDS, "Set up the GPIO channel, direction and (optional) pull/up down control\nchannel        - Either: RPi board pin number (not BCM GPIO 00..nn number).  Pins start from 1\n                 or    : BCM GPIO number\ndirection      - INPUT or OUTPUT\n[pull_up_down] - PUD_OFF (default), PUD_UP or PUD_DOWN\n[initial]      - Initial value for an output channel"},
   {"cleanup", py_cleanup, METH_VARARGS, "Clean up by resetting all GPIO channels that have been used by this program to INPUT with no pullup/pulldown and no event detection"},
   {"output", py_output_gpio, METH_VARARGS, "Output to a GPIO channel\ngpio  - gpio channel\nvalue - 0/1 or False/True or LOW/HIGH"},
   {"input", py_input_gpio, METH_VARARGS, "Input from a GPIO channel.  Returns HIGH=1=True or LOW=0=False\ngpio - gpio channel"},
   {"add_event_detect", (PyCFunction)py_add_event_detect, METH_VARARGS | METH_KEYWORDS, "Enable edge detection events for a particular GPIO channel.\nchannel      - either board pin number or BCM number depending on which mode is set.\nedge         - RISING, FALLING or BOTH\n[callback]   - A callback function for the event (optional)\n[bouncetime] - Switch bounce timeout in ms for callback"},
   {"remove_event_detect", py_remove_event_detect, METH_VARARGS, "Remove edge detection for a particular GPIO channel\ngpio - gpio channel"},
   {"event_detected", py_event_detected, METH_VARARGS, "Returns True if an edge has occured on a given GPIO.  You need to enable edge detection using add_event_detect() first.\ngpio - gpio channel"},
   {"add_event_callback", (PyCFunction)py_add_event_callback, METH_VARARGS | METH_KEYWORDS, "Add a callback for an event already defined using add_event_detect()\ngpio         - gpio channel\ncallback     - a callback function\n[bouncetime] - Switch bounce timeout in ms"},
   {"wait_for_edge", py_wait_for_edge, METH_VARARGS, "Wait for an edge.\ngpio - gpio channel\nedge - RISING, FALLING or BOTH"},
   {"gpio_function", py_gpio_function, METH_VARARGS, "Return the current GPIO function (IN, OUT, ALT0)\ngpio - gpio channel"},
   {"setwarnings", py_setwarnings, METH_VARARGS, "Enable or disable warning messages"},
   {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef rpigpiomodule = {
   PyModuleDef_HEAD_INIT,
   "GPIO",       // name of module
   moduledocstring,  // module documentation, may be NULL
   -1,               // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
   gpio_methods
};
#endif

#if PY_MAJOR_VERSION > 2
PyMODINIT_FUNC PyInit_GPIO(void)
#else
PyMODINIT_FUNC initGPIO(void)
#endif
{
   PyObject *module = NULL;

#if PY_MAJOR_VERSION > 2
   if ((module = PyModule_Create(&rpigpiomodule)) == NULL)
      return NULL;
#else
   if ((module = Py_InitModule3("GPIO", gpio_methods, moduledocstring)) == NULL)
      return;
#endif

   define_constants(module);

   if (!PyEval_ThreadsInitialized())
      PyEval_InitThreads();

   if (Py_AtExit(event_cleanup) != 0)
   {
      setup_error = 1;
      event_cleanup();
#if PY_MAJOR_VERSION > 2
      return NULL;
#else
      return;
#endif
   }

#if PY_MAJOR_VERSION > 2
   return module;
#else
   return;
#endif
}
