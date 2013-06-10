#include "Python.h"
#include "constants.h"
#include "common.h"
#include "c_pwm.h"

static PyObject *bb_revision;
static int gpio_warnings = 1;

struct py_callback
{
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

    module_setup = 0;

    for (i=0; i<120; i++)
        pwm_pins[i] = -1;

    return 0;
}

// python function cleanup()
static PyObject *py_cleanup(PyObject *self, PyObject *args)
{
    // unexport the PWM
    fprintf(stderr, "in py_cleanup\n");
    pwm_cleanup();

    Py_RETURN_NONE;
}

// python function setup(channel, direction, pull_up_down=PUD_OFF, initial=None)
static PyObject *py_setup_channel(PyObject *self, PyObject *args, PyObject *kwargs)
{
    char key[8];
    char *channel;
    int direction;
    int initial = -1;
    static char *kwlist[] = {"channel", "direction", "initial", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ii", kwlist, &channel, &direction, &initial))
        return NULL;

    if (get_pwm_key(channel, key))
        return NULL;

    pwm_enable(key);

    Py_RETURN_NONE;
}


static const char moduledocstring[] = "PWM functionality of a BeagleBone using Python";

PyMethodDef gpio_methods[] = {
    {"setup", (PyCFunction)py_setup_channel, METH_VARARGS | METH_KEYWORDS, "Optional: Set up the PWM channel.  channel can be in the form of 'P8_10', or 'EHRPWM2A'"},
    {"cleanup", py_cleanup, METH_VARARGS, "Clean up by resetting all GPIO channels that have been used by this program to INPUT with no pullup/pulldown and no event detection"},
    //{"setwarnings", py_setwarnings, METH_VARARGS, "Enable or disable warning messages"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef rpigpiomodule = {
    PyModuleDef_HEAD_INIT,
    "PWM",       // name of module
    moduledocstring,  // module documentation, may be NULL
    -1,               // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    gpio_methods
};
#endif

#if PY_MAJOR_VERSION > 2
PyMODINIT_FUNC PyInit_PWM(void)
#else
PyMODINIT_FUNC initPWM(void)
#endif
{
    PyObject *module = NULL;

#if PY_MAJOR_VERSION > 2
    if ((module = PyModule_Create(&rpigpiomodule)) == NULL)
       return NULL;
#else
    if ((module = Py_InitModule3("PWM", gpio_methods, moduledocstring)) == NULL)
       return;
#endif

   define_constants(module);


#if PY_MAJOR_VERSION > 2
    return module;
#else
    return;
#endif
}