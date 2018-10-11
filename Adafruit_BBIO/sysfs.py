#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 MIT OpenCourseWare
# Copyright (c) 2017 Adafruit Industries
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Code originally published at http://stackoverflow.com/questions/4648792/ and
# subsequently forked at https://github.com/ponycloud/python-sysfs
#
# Original author: Benedikt Reinartz <filmor@gmail.com>
# Contributors:
#     - Jan Dvořák <mordae@anilinux.org>
#     - Jonathon Reinhart https://github.com/JonathonReinhart
#     - Ondřej Koch <o.koch@zerusnet.com>
#     - David Planella <david.planella@ubuntu.com>

"""
Simplistic Python SysFS interface. It enables access to the sys filesystem device
nodes and to get and set their exposed attributes.

Usage examples::
    from sysfs import sys

    # Print all block devices in /sys, with their sizes
    for block_dev in sys.block:
        print(block_dev, str(int(block_dev.size) / 1048576) + ' M')

    >>> import sysfs
    >>> # Read/write Beaglebone Black's eQEP module attributes
    >>> eqep0 = sysfs.Node("/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep")
    >>> # Read eqep attributes
    >>> eqep0.enabled
    '1'
    >>> eqep0.mode
    '0'
    >>> eqep0.period
    '1000000000'
    >>> eqep0.position
    '0'
    >>> # Write eqep attributes. They should be strings.
    >>> eqep0.position = str(2)
    >>> eqep0.position
    '2'
"""

from os import listdir
from os.path import isdir, isfile, join, realpath, basename

__all__ = ['sys', 'Node']


class Node(object):
    __slots__ = ['_path_', '__dict__']

    def __init__(self, path='/sys'):
        self._path_ = realpath(path)
        if not self._path_.startswith('/sys/') and not '/sys' == self._path_:
            raise RuntimeError('Using this on non-sysfs files is dangerous!')

        self.__dict__.update(dict.fromkeys(listdir(self._path_)))

    def __repr__(self):
        return '<sysfs.Node "%s">' % self._path_

    def __str__(self):
        return basename(self._path_)

    def __setattr__(self, name, val):
        if name.startswith('_'):
            return object.__setattr__(self, name, val)

        path = realpath(join(self._path_, name))
        if isfile(path):
            with open(path, 'w') as fp:
                fp.write(val)
        else:
            raise RuntimeError('Cannot write to non-files.')

    def __getattribute__(self, name):
        if name.startswith('_'):
            return object.__getattribute__(self, name)

        path = realpath(join(self._path_, name))
        if isfile(path):
            with open(path, 'r') as fp:
                return fp.read().strip()
        elif isdir(path):
            return Node(path)

    def __setitem__(self, name, val):
        return setattr(self, name, val)

    def __getitem__(self, name):
        return getattr(self, name)

    def __iter__(self):
        return iter(getattr(self, name) for name in listdir(self._path_))


sys = Node()
