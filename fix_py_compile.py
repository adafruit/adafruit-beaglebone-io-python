#!/usr/bin/env python2
# Some Angstrom images are missing the py_compile module; get it if not
# present:
# Fix credit:https://github.com/alexanderhiam/PyBBIO/blob/master/setup.py
import random, os
python_lib_path = random.__file__.split('random')[0]
if not os.path.exists(python_lib_path + 'py_compile.py'):
  print "py_compile module missing; installing to %spy_compile.py" %\
                                                          python_lib_path
  import urllib2
  url = "http://hg.python.org/cpython/raw-file/4ebe1ede981e/Lib/py_compile.py"
  py_compile = urllib2.urlopen(url)
  with open(python_lib_path+'py_compile.py', 'w') as f:
    f.write(py_compile.read())
  print "testing py_compile..."
  try:
    import py_compile
    print "py_compile installed successfully"
  except Exception, e:
    print "*py_compile install failed, could not import"
    print "*Exception raised:"
    raise e
