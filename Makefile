
all: build # build2 build3

time:
	/usr/bin/ntpdate -b -s -u pool.ntp.org

publish: clean
	python setup.py sdist upload

clean:
	rm -rf Adafruit_BBIO.* build dist
	rm -f *.pyo
	rm -f *.egg
	rm -f overlays/*.pyo overlays/*.pyc

tests:
	py.test

build:
	python setup.py build --force

install: build
	python setup.py install --force

build2:
	python2 setup.py build --force

install2: build2
	python2 setup.py install --force

build3:
	python3 setup.py build --force

install3: build3
	python3 setup.py install --force

.PHONY: all clean
.PHONY: tests
.PHONY: build  install
.PHONY: build2 install2
.PHONY: build3 install3
