#include <stdio.h>
#include <string.h>

#include "c_pinmux.h"
#include "common.h"


BBIO_err set_pin_mode(const char *key, const char *mode)
{
	// char ocp_dir[] defined in common.h
	char path[60]; // "/sys/devices/platform/ocp/ocp:P#_##_pinmux/state"
	char pinmux_dir[20]; // "ocp:P#_##_pinmux"
	FILE *f = NULL;

#ifndef BBBVERSION41
	BBIO_err err;
	err = build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
	if (err != BBIO_OK) {
		return err;
	}
#else
	strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#endif

	snprintf(pinmux_dir, sizeof(pinmux_dir), "ocp:%s_pinmux", key);
	snprintf(path, sizeof(path), "%s/%s/state", ocp_dir, pinmux_dir);

	f = fopen(path, "w");
	if (NULL == f) {
		return BBIO_ACCESS;
	} 

	fprintf(f, "%s", mode);
	fclose(f);

	return BBIO_OK;
}
