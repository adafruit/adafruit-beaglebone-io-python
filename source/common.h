#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

int gpio_mode;
int gpio_direction[120];
int pwm_pins[120];

int get_gpio_number(const char *key, unsigned int *gpio);
int get_pwm_key(const char *input, char *key);
int get_adc_ain(const char *key, unsigned int *ain);
int setup_error;
int module_setup;