#!/bin/bash
# useful for testing changes against all versions of python

cd test
echo "Testing Python 2"
python2 -mpytest
echo "Testing Python 3"
python3 -mpytest
cd ..
