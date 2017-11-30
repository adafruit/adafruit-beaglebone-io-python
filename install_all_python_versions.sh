#!/bin/bash
# useful for testing changes against all versions of python
make clean
echo "Install Python 2.7"
python2.7 ./setup.py install
echo "Install Python 3.5"
python3.5 ./setup.py install
echo "Install Python 3.6"
python3.6 ./setup.py install
