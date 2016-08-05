#include <stdio.h>
#include <string.h>

#include "c_pinmux.h"
#include "common.h"


BBIO_err set_pin_mode(const char *key, const char *mode)
{
	// char ocp_dir[30] "/sys/devices/platform/ocp"
	char path[60]; // "/sys/devices/platform/ocp/ocp:P#_##_pinmux/state"
	char pinmux_dir[20]; // "ocp:P#_##_pinmux"
	FILE *f = NULL;

	build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));

	snprintf(pinmux_dir, sizeof(pinmux_dir), "ocp:%s_pinmux", key);
	snprintf(path, sizeof(path), "%s/%s/state", sizeof(path));

	f = fopen(path, "w");
	if (NULL == f) {
		return BBIO_ACCESS;
	} 

	fprintf(f, "%s", mode);
	fclose(f);

	return BBIO_OK;
}
