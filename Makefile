
all: build3 # build2 build3

time:
	/usr/bin/ntpdate -b -s -u pool.ntp.org

publish: clean
	python setup.py sdist upload

clean: cpp-clean
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
	python3 setup.py build # --force

install3: build3
	python3 setup.py install # --force

################################################################################
# c++ library
#
# sudo apt-get install automake
# make cpp
# cd build && sudo make install
#
# libgtest-dev is needed for UT
#
# sudo sh -c 'echo cape-universaln > /sys/devices/platform/bone_capemgr/slots'
# sudo sh -c 'echo pwm > /sys/devices/platform/ocp/ocp\:P9_16_pinmux/state'
# sudo sh -c 'echo pwm > /sys/devices/platform/ocp/ocp\:P8_19_pinmux/state'
################################################################################
configure: configure.ac
	rm -f configure && \
	autoreconf --install -I m4

build/Makefile: configure
	mkdir -p build && \
	cd build && \
	../configure

cpp: build/Makefile
	cd build && \
	$(MAKE) build

cpp-check: cpp
	cd build && \
	$(MAKE) check

cpp-install: cpp
	cd build && \
	$(MAKE) install

cpp-clean:
	rm -rf \
	  Makefile.in \
	  aclocal.m4 \
	  autom4te.cache/ \
	  compile \
	  configure \
	  config.guess \
	  config.h.in \
	  config.sub \
	  depcomp \
	  install-sh \
	  ltmain.sh \
	  m4/libtool.m4 \
	  m4/ltoptions.m4 \
	  m4/ltsugar.m4 \
	  m4/ltversion.m4 \
	  m4/lt~obsolete.m4 \
	  missing \
	  source/Makefile.in \
	  test-driver

################################################################################

.PHONY: all clean
.PHONY: tests
.PHONY: build  install
.PHONY: build2 install2
.PHONY: build3 install3
.PHONY: cpp cpp-install cpp-clean
