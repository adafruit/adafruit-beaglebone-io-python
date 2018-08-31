#!/bin/bash
# useful for testing changes against all versions of python
make clean
echo "Install Python 2"
python2 ./setup.py install
echo "Install Python 3"
python3 ./setup.py install
