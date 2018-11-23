#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include "c_pinmux.h"
#include "common.h"


BBIO_err set_pin_mode(const char *key, const char *mode)
{
	// char ocp_dir[] defined in common.h
	char path[60]; // "/sys/devices/platform/ocp/ocp:P#_##_pinmux/state"
	char pinmux_dir[20]; // "ocp:P#_##_pinmux"
	char pin[6]; //"P#_##"
	FILE *f = NULL;
	
	if (strlen(key) == 4)	// Key P#_# format, must inject '0' to be P#_0#
		snprintf(pin, sizeof(pin), "%.3s0%c", key,key[3]);
	else	//copy string
		snprintf(pin, sizeof(pin), "%s", key);

#ifndef BBBVERSION41
	BBIO_err err;
	err = build_path("/sys/devices/platform", "ocp", ocp_dir, sizeof(ocp_dir));
	if (err != BBIO_OK) {
		return err;
	}
#else
	strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#endif

	snprintf(pinmux_dir, sizeof(pinmux_dir), "ocp:%s_pinmux", pin);
	snprintf(path, sizeof(path), "%s/%s/state", ocp_dir, pinmux_dir);

	/* beaglebone blue has complete dtb file and does not need overlays */
	if(beaglebone_blue()) {
		fprintf(stderr, "DEBUG: Adafruit_BBIO: set_pin_mode() :: Pinmux file: %s, mode: %s", path, mode); 
		fprintf(stderr, "DEBUG: Adafruit_BBIO: set_pin_mode(): beaglebone_blue() is TRUE; return BBIO_OK\n");
		return BBIO_OK;
	}


	f = fopen(path, "w");
	if (NULL == f) {
		return BBIO_ACCESS;
	}
	syslog(LOG_DEBUG, "Adafruit_BBIO: set_pin_mode() :: Pinmux file %s access OK", path); 
	fprintf(f, "%s", mode);
	fclose(f);
	syslog(LOG_DEBUG, "Adafruit_BBIO: set_pin_mode() :: Set pinmux mode to %s for %s", mode, pin);
	return BBIO_OK;
}
