#!/bin/bash
# useful for testing changes against all versions of python

cd test
echo "Testing Python 2.7"
python2.7 -mpytest
echo "Testing Python 3.5"
python3.5 -mpytest
echo "Testing Python 3.6"
python3.6 -mpytest
cd ..
