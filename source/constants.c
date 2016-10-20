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

   version = Py_BuildValue("s", "0.0.20");
   PyModule_AddObject(module, "VERSION", version);
}
