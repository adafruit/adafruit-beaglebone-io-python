#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

int gpio_mode;
int gpio_direction[120];
int pwm_pins[120];

char ctrl_dir[35];

int get_gpio_number(const char *key, unsigned int *gpio);
int get_pwm_key(const char *input, char *key);
int get_adc_ain(const char *key, unsigned int *ain);
int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
int load_device_tree(const char *name);
int unload_device_tree(const char *name);
int setup_error;
int module_setup;