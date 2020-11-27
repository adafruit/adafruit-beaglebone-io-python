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
#include "event_gpio.h"
#include "common.h"

void define_constants(PyObject *module)
{
   PyObject *object;

   object = Py_BuildValue("i", HIGH);
   PyModule_AddObject(module, "HIGH", object);

   object = Py_BuildValue("i", LOW);
   PyModule_AddObject(module, "LOW", object);

   object = Py_BuildValue("i", OUTPUT);
   PyModule_AddObject(module, "OUT", object);

   object = Py_BuildValue("i", INPUT);
   PyModule_AddObject(module, "IN", object);

   object = Py_BuildValue("i", ALT0);
   PyModule_AddObject(module, "ALT0", object);

   object = Py_BuildValue("i", PUD_OFF);
   PyModule_AddObject(module, "PUD_OFF", object);

   object = Py_BuildValue("i", PUD_UP);
   PyModule_AddObject(module, "PUD_UP", object);

   object = Py_BuildValue("i", PUD_DOWN);
   PyModule_AddObject(module, "PUD_DOWN", object);
   
   object = Py_BuildValue("i", RISING_EDGE);
   PyModule_AddObject(module, "RISING", object);
   
   object = Py_BuildValue("i", FALLING_EDGE);
   PyModule_AddObject(module, "FALLING", object);

   object = Py_BuildValue("i", BOTH_EDGE);
   PyModule_AddObject(module, "BOTH", object);

   object = Py_BuildValue("s", "0.0.20");
   PyModule_AddObject(module, "VERSION", object);
}
