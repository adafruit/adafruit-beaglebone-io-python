/*
 * Copyright (c) 2013 Adafruit

 * Modified for BBIO Author Justin Cooper

 * This file incorporates work covered by the following copyright and 
 * permission notice, all modified code adopts the original license:
 *
 * spimodule.c - Python bindings for Linux SPI access through spidev
 * Copyright (C) 2009 Volker Thoms <unconnected@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <Python.h>
#include "structmember.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include "common.h"

PyDoc_STRVAR(SPI_module_doc,
	"This module defines an object type that allows SPI transactions\n"
	"on hosts running the Linux kernel. The host kernel must have SPI\n"
	"support and SPI device interface support.\n"
	"All of these can be either built-in to the kernel, or loaded from\n"
	"modules.\n"
	"\n"
	"Because the SPI device interface is opened R/W, users of this\n"
	"module usually must have root permissions.\n");

typedef struct {
	PyObject_HEAD

	int fd;	/* open file descriptor: /dev/spi-X.Y */
	uint8_t mode;	/* current SPI mode */
	uint8_t bpw;	/* current SPI bits per word setting */
	uint32_t msh;	/* current SPI max speed setting in Hz */
} SPI;

static PyObject *
SPI_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	SPI *self;
	if ((self = (SPI *)type->tp_alloc(type, 0)) == NULL)
		return NULL;

	self->fd = -1;
	self->mode = 0;
	self->bpw = 0;
	self->msh = 0;

	return (PyObject *)self;
}

PyDoc_STRVAR(SPI_close_doc,
	"close()\n\n"
	"Disconnects the object from the interface.\n");

static PyObject *
SPI_close(SPI *self)
{
	if ((self->fd != -1) && (close(self->fd) == -1)) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	self->fd = -1;
	self->mode = 0;
	self->bpw = 0;
	self->msh = 0;
	
	Py_INCREF(Py_None);
	return Py_None;
}

static void
SPI_dealloc(SPI *self)
{
	PyObject *ref = SPI_close(self);
	Py_XDECREF(ref);

	self->ob_type->tp_free((PyObject *)self);
}

#define MAXPATH 16
// DAW - 8/12/12 - increased buffer size from 32 to 1024 bytes
#define MAXMSGLEN 1024

static char *wrmsg = "Argument must be a list of at least one, "
				"but not more than 1024 integers";

PyDoc_STRVAR(SPI_write_doc,
	"write([values]) -> None\n\n"
	"Write bytes to SPI device.\n");

static PyObject *
SPI_writebytes(SPI *self, PyObject *args)
{
	int		status;
	uint16_t	ii, len;
	uint8_t	buf[MAXMSGLEN];
	PyObject	*list;

	if (!PyArg_ParseTuple(args, "O:write", &list))
		return NULL;

	if (!PyList_Check(list)) {
		PyErr_SetString(PyExc_TypeError, wrmsg);
		return NULL;
	}

	if ((len = PyList_GET_SIZE(list)) >  MAXMSGLEN) {
		PyErr_SetString(PyExc_OverflowError, wrmsg);
		return NULL;
	}

	for (ii = 0; ii < len; ii++) {
		PyObject *val = PyList_GET_ITEM(list, ii);
		if (!PyInt_Check(val)) {
			PyErr_SetString(PyExc_TypeError, wrmsg);
			return NULL;
		}
		buf[ii] = (__u8)PyInt_AS_LONG(val);
	}

	status = write(self->fd, &buf[0], len);

	if (status < 0) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	if (status != len) {
		perror("short write");
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyDoc_STRVAR(SPI_read_doc,
	"read(len) -> [values]\n\n"
	"Read len bytes from SPI device.\n");

static PyObject *
SPI_readbytes(SPI *self, PyObject *args)
{
	uint8_t	rxbuf[MAXMSGLEN];
	int		status, len, ii;
	PyObject	*list;

	if (!PyArg_ParseTuple(args, "i:read", &len))
		return NULL;

	/* read at least 1 byte, no more than 1024 */
	if (len < 1)
		len = 1;
	else if (len > sizeof(rxbuf))
		len = sizeof(rxbuf);

	memset(rxbuf, 0, sizeof rxbuf);
	status = read(self->fd, &rxbuf[0], len);

	if (status < 0) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	if (status != len) {
		perror("short read");
		return NULL;
	}

	list = PyList_New(len);

	for (ii = 0; ii < len; ii++) {
		PyObject *val = Py_BuildValue("l", (long)rxbuf[ii]);
		PyList_SET_ITEM(list, ii, val);
	}

	return list;
}

PyDoc_STRVAR(SPI_xfer_doc,
	"xfer([values]) -> [values]\n\n"
	"Perform SPI transaction.\n"
	"CS will be released and reactivated between blocks.\n"
	"delay specifies delay in usec between blocks.\n");

static PyObject *
SPI_xfer(SPI *self, PyObject *args)
{
	uint16_t ii, len;
	int status;
	int delay = -1;
	//uint8_t ret = 0;
	PyObject *list;
	struct spi_ioc_transfer *xferptr;
	uint8_t *txbuf, *rxbuf;

	if (!PyArg_ParseTuple(args, "O|i:msg", &list, &delay))
		return NULL;

	if (!PyList_Check(list)) {
		PyErr_SetString(PyExc_TypeError, wrmsg);
		return NULL;
	}

	if ((len = PyList_GET_SIZE(list)) > MAXMSGLEN) {
		PyErr_SetString(PyExc_OverflowError, wrmsg);
		return NULL;
	}

	if (delay == -1) {
		delay = 0;
	}

	xferptr = (struct spi_ioc_transfer*) malloc(sizeof(struct spi_ioc_transfer) * len);
	txbuf = malloc(sizeof(__u8) * len);
	rxbuf = malloc(sizeof(__u8) * len);

	for (ii = 0; ii < len; ii++) {
		PyObject *val = PyList_GET_ITEM(list, ii);
		if (!PyInt_Check(val)) {
			free(txbuf);
			free(rxbuf);
			free(xferptr);
			PyErr_SetString(PyExc_TypeError, wrmsg);
			return NULL;
		}
		txbuf[ii] = (__u8)PyInt_AS_LONG(val);
		xferptr[ii].tx_buf = (unsigned long)&txbuf[ii];
		xferptr[ii].rx_buf = (unsigned long)&rxbuf[ii];
		xferptr[ii].len = 1;
		xferptr[ii].delay_usecs = delay;
		xferptr[ii].speed_hz = 0;      
		xferptr[ii].bits_per_word = 0;
	}

	status = ioctl(self->fd, SPI_IOC_MESSAGE(len), xferptr);
	if (status < 0) {
		free(txbuf);
		free(rxbuf);
		free(xferptr);
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	for (ii = 0; ii < len; ii++) {
		PyObject *val = Py_BuildValue("l", (long)rxbuf[ii]);
		PyList_SET_ITEM(list, ii, val);
	}

	// WA:
	// in CS_HIGH mode CS isn't pulled to low after transfer, but after read
	// reading 0 bytes doesnt matter but brings cs down
	status = read(self->fd, &rxbuf[0], 0);

	free(txbuf);
	free(rxbuf);
	free(xferptr);

	Py_INCREF(list);
	return list;
}


PyDoc_STRVAR(SPI_xfer2_doc,
	"xfer2([values]) -> [values]\n\n"
	"Perform SPI transaction.\n"
	"CS will be held active between blocks.\n");

static PyObject *
SPI_xfer2(SPI *self, PyObject *args)
{
	static char *msg = "Argument must be a list of at least one, "
				"but not more than 1024 integers";
	int status;	
	uint16_t ii, len;
	PyObject *list;
	struct spi_ioc_transfer	xfer;
	uint8_t *txbuf, *rxbuf;

	if (!PyArg_ParseTuple(args, "O:xfer2", &list))
		return NULL;

	if (!PyList_Check(list)) {
		PyErr_SetString(PyExc_TypeError, wrmsg);
		return NULL;
	}

	if ((len = PyList_GET_SIZE(list)) > MAXMSGLEN) {
		PyErr_SetString(PyExc_OverflowError, wrmsg);
		return NULL;
	}

	txbuf = malloc(sizeof(__u8) * len);
	rxbuf = malloc(sizeof(__u8) * len);

	for (ii = 0; ii < len; ii++) {
		PyObject *val = PyList_GET_ITEM(list, ii);
		if (!PyInt_Check(val)) {
			free(txbuf);
			free(rxbuf);
			PyErr_SetString(PyExc_TypeError, msg);
			return NULL;
		}
		txbuf[ii] = (__u8)PyInt_AS_LONG(val);
	}

	xfer.tx_buf = (unsigned long)txbuf;
	xfer.rx_buf = (unsigned long)rxbuf;
	xfer.len = len;
	xfer.delay_usecs = 0;
	xfer.speed_hz = 0;      
	xfer.bits_per_word = 0;
	
	status = ioctl(self->fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 0) {
		free(txbuf);
		free(rxbuf);
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	for (ii = 0; ii < len; ii++) {
		PyObject *val = Py_BuildValue("l", (long)rxbuf[ii]);
		PyList_SET_ITEM(list, ii, val);
	}
	// WA:
	// in CS_HIGH mode CS isnt pulled to low after transfer
	// reading 0 bytes doesn't really matter but brings CS down
	status = read(self->fd, &rxbuf[0], 0);

	free(txbuf);
	free(rxbuf);

	Py_INCREF(list);
	return list;
}

static int __SPI_set_mode( int fd, __u8 mode) {
	__u8 test;
	//printf("setmode called with mode = %x\n", mode);
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return -1;
	}
	if (ioctl(fd, SPI_IOC_RD_MODE, &test) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return -1;
	}
	if (test != mode) {
		return -1;
	}
	return 0;
}

static PyObject *
SPI_get_mode(SPI *self, void *closure)
{
	PyObject *result = Py_BuildValue("i", (self->mode & (SPI_CPHA | SPI_CPOL) ) );

	return result;
}

static PyObject *
SPI_get_cshigh(SPI *self, void *closure)
{
	PyObject *result;

	if (self->mode & SPI_CS_HIGH)
		result = Py_True;
	else
		result = Py_False;

	Py_INCREF(result);
	return result;
}

static PyObject *
SPI_get_lsbfirst(SPI *self, void *closure)
{
	PyObject *result;

	if (self->mode & SPI_LSB_FIRST)
		result = Py_True;
	else
		result = Py_False;

        Py_INCREF(result);
	return result;
}

static PyObject *
SPI_get_3wire(SPI *self, void *closure)
{
	PyObject *result;

	if (self->mode & SPI_3WIRE)
		result = Py_True;
	else
		result = Py_False;

        Py_INCREF(result);
	return result;
}

static PyObject *
SPI_get_loop(SPI *self, void *closure)
{
	PyObject *result;

	if (self->mode & SPI_LOOP)
		result = Py_True;
	else
		result = Py_False;

        Py_INCREF(result);
	return result;
}


static int
SPI_set_mode(SPI *self, PyObject *val, void *closure)
{
	uint8_t mode, tmp;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyInt_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The mode attribute must be an integer");
		return -1;
	}

	mode = PyInt_AsLong(val);

	if ( mode > 3 ) {
		PyErr_SetString(PyExc_TypeError,
			"The mode attribute must be an integer"
				 "between 0 and 3.");
		return -1;
	}

	// clean and set CPHA and CPOL bits
	tmp = ( self->mode & ~(SPI_CPHA | SPI_CPOL) ) | mode ;

	__SPI_set_mode(self->fd, tmp);

	self->mode = tmp;
	//printf("mode now: %x\n", self->mode);
	return 0;
}

static int
SPI_set_cshigh(SPI *self, PyObject *val, void *closure)
{
	uint8_t tmp;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyBool_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The cshigh attribute must be boolean");
		return -1;
	}

	if (val == Py_True)
		tmp = self->mode | SPI_CS_HIGH;
	else
		tmp = self->mode & ~SPI_CS_HIGH;

	__SPI_set_mode(self->fd, tmp);

	self->mode = tmp;
	//printf("mode now: %x\n", self->mode);
	return 0;
}

static int
SPI_set_lsbfirst(SPI *self, PyObject *val, void *closure)
{
	uint8_t tmp;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyBool_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The lsbfirst attribute must be boolean");
		return -1;
	}

	if (val == Py_True)
		tmp = self->mode | SPI_LSB_FIRST;
	else
		tmp = self->mode & ~SPI_LSB_FIRST;

	__SPI_set_mode(self->fd, tmp);

	self->mode = tmp;
	//printf("mode now: %x\n", self->mode);
	return 0;
}

static int
SPI_set_3wire(SPI *self, PyObject *val, void *closure)
{
	uint8_t tmp;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyBool_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The 3wire attribute must be boolean");
		return -1;
	}

	if (val == Py_True)
		tmp = self->mode | SPI_3WIRE;
	else
		tmp = self->mode & ~SPI_3WIRE;

	__SPI_set_mode(self->fd, tmp);

	self->mode = tmp;
	//printf("mode now: %x\n", self->mode);
	return 0;
}

static int
SPI_set_loop(SPI *self, PyObject *val, void *closure)
{
	uint8_t tmp;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyBool_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The loop attribute must be boolean");
		return -1;
	}

	if (val == Py_True)
		tmp = self->mode | SPI_LOOP;
	else
		tmp = self->mode & ~SPI_LOOP;

	__SPI_set_mode(self->fd, tmp);

	self->mode = tmp;
	//printf("mode now: %x\n", self->mode);
	return 0;
}

static PyObject *
SPI_get_bpw(SPI *self, void *closure)
{
	PyObject *result = Py_BuildValue("i", self->bpw);
	return result;
}

static int
SPI_set_bpw(SPI *self, PyObject *val, void *closure)
{
	uint8_t bits;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyInt_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The bpw attribute must be an integer");
		return -1;
	}

	bits = PyInt_AsLong(val);

        if (bits < 8 || bits > 16) {
		PyErr_SetString(PyExc_TypeError,
                                "invalid bits_per_word (8 to 16)");
		return -1;
	}

	if (self->bpw != bits) {
		if (ioctl(self->fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
			PyErr_SetFromErrno(PyExc_IOError);
			return -1;
		}
		self->bpw = bits;
	}
	return 0;
}

static PyObject *
SPI_get_msh(SPI *self, void *closure)
{
	PyObject *result = Py_BuildValue("i", self->msh);
	return result;
}

static int
SPI_set_msh(SPI *self, PyObject *val, void *closure)
{
	uint32_t msh;

	if (val == NULL) {
		PyErr_SetString(PyExc_TypeError,
			"Cannot delete attribute");
		return -1;
	}
	else if (!PyInt_Check(val)) {
		PyErr_SetString(PyExc_TypeError,
			"The msh attribute must be an integer");
		return -1;
	}

	msh = PyInt_AsLong(val);
	// DAW - 8/12/12 - removed limitation on SPI speed
	// if (8000000 < msh) {
		// PyErr_SetString(PyExc_TypeError,
			// "The mode attribute must be an integer < 8000000.");
		// return -1;
	// }

	if (self->msh != msh) {
		if (ioctl(self->fd, SPI_IOC_WR_MAX_SPEED_HZ, &msh) == -1) {
			PyErr_SetFromErrno(PyExc_IOError);
			return -1;
		}
		self->msh = msh;
	}
	return 0;
}

static PyGetSetDef SPI_getset[] = {
	{"mode", (getter)SPI_get_mode, (setter)SPI_set_mode,
			"SPI mode as two bit pattern of \n"
			"Clock Polarity  and Phase [CPOL|CPHA]\n"
			"min: 0b00 = 0 max: 0b11 = 3\n"},
	{"cshigh", (getter)SPI_get_cshigh, (setter)SPI_set_cshigh,
			"CS active high\n"},
	{"threewire", (getter)SPI_get_3wire, (setter)SPI_set_3wire,
			"SI/SO signals shared\n"},
	{"lsbfirst", (getter)SPI_get_lsbfirst, (setter)SPI_set_lsbfirst,
			"LSB first\n"},
	{"loop", (getter)SPI_get_loop, (setter)SPI_set_loop,
			"loopback configuration\n"},
	{"bpw", (getter)SPI_get_bpw, (setter)SPI_set_bpw,
			"bits per word\n"},
	{"msh", (getter)SPI_get_msh, (setter)SPI_set_msh,
			"maximum speed in Hz\n"},
	{NULL},
};

PyDoc_STRVAR(SPI_open_doc,
	"open(bus, device)\n\n"
	"Connects the object to the specified SPI device.\n"
	"open(X,Y) will open /dev/spidev-X.Y\n");

static PyObject *
SPI_open(SPI *self, PyObject *args, PyObject *kwds)
{
	int bus, device;
	int bus_path;
	int max_dt_length = 15;
	char device_tree_name[max_dt_length];
	char path[MAXPATH];
	uint8_t tmp8;
	uint32_t tmp32;	
	static char *kwlist[] = {"bus", "device", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii:open", kwlist, &bus, &device))
		return NULL;
	if (snprintf(device_tree_name, max_dt_length, "BB-SPIDEV%d", bus) >= max_dt_length) {
		PyErr_SetString(PyExc_OverflowError,
			"Bus and/or device number is invalid.");
		return NULL;
	}
	if (load_device_tree(device_tree_name) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	bus_path = get_spi_bus_path_number(bus);
	if (bus_path == -1) {
		PyErr_SetString(PyExc_OverflowError,
			"Unable to find loaded spi bus path.");
		return NULL;
	}

	if (snprintf(path, MAXPATH, "/dev/spidev%d.%d", bus_path, device) >= MAXPATH) {
		PyErr_SetString(PyExc_OverflowError,
			"Bus and/or device number is invalid.");
		return NULL;
	}
	if ((self->fd = open(path, O_RDWR, 0)) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	if (ioctl(self->fd, SPI_IOC_RD_MODE, &tmp8) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	self->mode = tmp8;
	if (ioctl(self->fd, SPI_IOC_RD_BITS_PER_WORD, &tmp8) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	self->bpw = tmp8;
	if (ioctl(self->fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32) == -1) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	self->msh = tmp32;

	Py_INCREF(Py_None);
	return Py_None;
}

static int
SPI_init(SPI *self, PyObject *args, PyObject *kwds)
{
	int bus = -1;
	int client = -1;
	static char *kwlist[] = {"bus", "client", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ii:__init__",
			kwlist, &bus, &client))
		return -1;

	if (bus >= 0) {
		SPI_open(self, args, kwds);
		if (PyErr_Occurred())
			return -1;
	}

	return 0;
}


PyDoc_STRVAR(SPI_type_doc,
	"SPI([bus],[client]) -> SPI\n\n"
	"Return a new SPI object that is (optionally) connected to the\n"
	"specified SPI device interface.\n");

static PyMethodDef SPI_methods[] = {
	{"open", (PyCFunction)SPI_open, METH_VARARGS | METH_KEYWORDS,
		SPI_open_doc},
	{"close", (PyCFunction)SPI_close, METH_NOARGS,
		SPI_close_doc},
	{"readbytes", (PyCFunction)SPI_readbytes, METH_VARARGS,
		SPI_read_doc},
	{"writebytes", (PyCFunction)SPI_writebytes, METH_VARARGS,
		SPI_write_doc},
	{"xfer", (PyCFunction)SPI_xfer, METH_VARARGS,
		SPI_xfer_doc},
	{"xfer2", (PyCFunction)SPI_xfer2, METH_VARARGS,
		SPI_xfer2_doc},
	{NULL},
};

static PyTypeObject SPI_type = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size */
	"SPI",			/* tp_name */
	sizeof(SPI),			/* tp_basicsize */
	0,				/* tp_itemsize */
	(destructor)SPI_dealloc,	/* tp_dealloc */
	0,				/* tp_print */
	0,				/* tp_getattr */
	0,				/* tp_setattr */
	0,				/* tp_compare */
	0,				/* tp_repr */
	0,				/* tp_as_number */
	0,				/* tp_as_sequence */
	0,				/* tp_as_mapping */
	0,				/* tp_hash */
	0,				/* tp_call */
	0,				/* tp_str */
	0,				/* tp_getattro */
	0,				/* tp_setattro */
	0,				/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,		/* tp_flags */
	SPI_type_doc,			/* tp_doc */
	0,				/* tp_traverse */
	0,				/* tp_clear */
	0,				/* tp_richcompare */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter */
	0,				/* tp_iternext */
	SPI_methods,			/* tp_methods */
	0,				/* tp_members */
	SPI_getset,			/* tp_getset */
	0,				/* tp_base */
	0,				/* tp_dict */
	0,				/* tp_descr_get */
	0,				/* tp_descr_set */
	0,				/* tp_dictoffset */
	(initproc)SPI_init,		/* tp_init */
	0,				/* tp_alloc */
	SPI_new,			/* tp_new */
};

static PyMethodDef SPI_module_methods[] = {
	{NULL}
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initSPI(void)
{
	PyObject* m;

	if (PyType_Ready(&SPI_type) < 0)
		return;

	m = Py_InitModule3("SPI", SPI_module_methods, SPI_module_doc);
	Py_INCREF(&SPI_type);
	PyModule_AddObject(m, "SPI", (PyObject *)&SPI_type);
}


