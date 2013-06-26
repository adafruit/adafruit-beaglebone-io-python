#include "Python.h"
#include "constants.h"
#include "event_gpio.h"
#include "common.h"

void define_constants(PyObject *module)
{
   high = Py_BuildValue("i", HIGH);
   PyModule_AddObject(module, "HIGH", high);

   low = Py_BuildValue("i", LOW);
   PyModule_AddObject(module, "LOW", low);

   output = Py_BuildValue("i", OUTPUT);
   PyModule_AddObject(module, "OUT", output);

   input = Py_BuildValue("i", INPUT);
   PyModule_AddObject(module, "IN", input);   

   alt0 = Py_BuildValue("i", ALT0);
   PyModule_AddObject(module, "ALT0", alt0);

   pud_off = Py_BuildValue("i", PUD_OFF);
   PyModule_AddObject(module, "PUD_OFF", pud_off);

   pud_up = Py_BuildValue("i", PUD_UP);
   PyModule_AddObject(module, "PUD_UP", pud_up);

   pud_down = Py_BuildValue("i", PUD_DOWN);
   PyModule_AddObject(module, "PUD_DOWN", pud_down);
   
   rising_edge = Py_BuildValue("i", RISING_EDGE);
   PyModule_AddObject(module, "RISING", rising_edge);
   
   falling_edge = Py_BuildValue("i", FALLING_EDGE);
   PyModule_AddObject(module, "FALLING", falling_edge);

   both_edge = Py_BuildValue("i", BOTH_EDGE);
   PyModule_AddObject(module, "BOTH", both_edge);

   version = Py_BuildValue("s", "0.0.4");
   PyModule_AddObject(module, "VERSION", version);
}
