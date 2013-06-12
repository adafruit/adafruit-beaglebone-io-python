#include "Python.h"
#include "common.h"

int setup_error = 0;
int module_setup = 0;

typedef struct pins_t { 
    const char *name; 
    const char *key; 
    int gpio;
    int pwm_mux_mode;
} pins_t;

//Table generated based on https://raw.github.com/jadonk/bonescript/master/node_modules/bonescript/bone.js
pins_t table[] = {
{ "USR0", "USR0", 53, -1},
{ "USR1", "USR1", 54, -1},
{ "USR2", "USR2", 55, -1},
{ "USR3", "USR3", 56, -1},
{ "DGND", "P8_1", 0, -1},
{ "DGND", "P8_2", 0, -1},
{ "GPIO1_6", "P8_3", 38, -1},
{ "GPIO1_7", "P8_4", 39, -1},
{ "GPIO1_2", "P8_5", 34, -1},
{ "GPIO1_3", "P8_6", 35, -1},
{ "TIMER4", "P8_7", 66, -1},
{ "TIMER7", "P8_8", 67, -1},
{ "TIMER5", "P8_9", 69, -1},
{ "TIMER6", "P8_10", 68, -1},
{ "GPIO1_13", "P8_11", 45, -1},
{ "GPIO1_12", "P8_12", 44, -1},
{ "EHRPWM2B", "P8_13", 23, 4},
{ "GPIO0_26", "P8_14", 26, -1},
{ "GPIO1_15", "P8_15", 47, -1},
{ "GPIO1_14", "P8_16", 46, -1},
{ "GPIO0_27", "P8_17", 27, -1},
{ "GPIO2_1", "P8_18", 65, -1},
{ "EHRPWM2A", "P8_19", 22, 4},
{ "GPIO1_31", "P8_20", 63, -1},
{ "GPIO1_30", "P8_21", 62, -1},
{ "GPIO1_5", "P8_22", 37, -1},
{ "GPIO1_4", "P8_23", 36, -1},
{ "GPIO1_1", "P8_24", 33, -1},
{ "GPIO1_0", "P8_25", 32, -1},
{ "GPIO1_29", "P8_26", 61, -1},
{ "GPIO2_22", "P8_27", 86, -1},
{ "GPIO2_24", "P8_28", 88, -1},
{ "GPIO2_23", "P8_29", 87, -1},
{ "GPIO2_25", "P8_30", 89, -1},
{ "UART5_CTSN", "P8_31", 10, -1},
{ "UART5_RTSN", "P8_32", 11, -1},
{ "UART4_RTSN", "P8_33", 9, -1},
{ "UART3_RTSN", "P8_34", 81, 2},
{ "UART4_CTSN", "P8_35", 8, -1},
{ "UART3_CTSN", "P8_36", 80, 2},
{ "UART5_TXD", "P8_37", 78, -1},
{ "UART5_RXD", "P8_38", 79, -1},
{ "GPIO2_12", "P8_39", 76, -1},
{ "GPIO2_13", "P8_40", 77, -1},
{ "GPIO2_10", "P8_41", 74, -1},
{ "GPIO2_11", "P8_42", 75, -1},
{ "GPIO2_8", "P8_43", 72, -1},
{ "GPIO2_9", "P8_44", 73, -1},
{ "GPIO2_6", "P8_45", 70, 3},
{ "GPIO2_7", "P8_46", 71, 3},
{ "DGND", "P9_1", 0, -1},
{ "DGND", "P9_2", 0, -1},
{ "VDD_3V3", "P9_3", 0, -1},
{ "VDD_3V3", "P9_4", 0, -1},
{ "VDD_5V", "P9_5", 0, -1},
{ "VDD_5V", "P9_6", 0, -1},
{ "SYS_5V", "P9_7", 0, -1},
{ "SYS_5V", "P9_8", 0, -1},
{ "PWR_BUT", "P9_9", 0, -1},
{ "SYS_RESETn", "P9_10", 0, -1},
{ "UART4_RXD", "P9_11", 30, -1},
{ "GPIO1_28", "P9_12", 60, -1},
{ "UART4_TXD", "P9_13", 31, -1},
{ "EHRPWM1A", "P9_14", 50, 6},
{ "GPIO1_16", "P9_15", 48, -1},
{ "EHRPWM1B", "P9_16", 51, 6},
{ "I2C1_SCL", "P9_17", 5, -1},
{ "I2C1_SDA", "P9_18", 4, -1},
{ "I2C2_SCL", "P9_19", 13, -1},
{ "I2C2_SDA", "P9_20", 12, -1},
{ "UART2_TXD", "P9_21", 3, 3},
{ "UART2_RXD", "P9_22", 2, 3},
{ "GPIO1_17", "P9_23", 49, -1},
{ "UART1_TXD", "P9_24", 15, -1},
{ "GPIO3_21", "P9_25", 117, -1},
{ "UART1_RXD", "P9_26", 14, -1},
{ "GPIO3_19", "P9_27", 115, -1},
{ "SPI1_CS0", "P9_28", 113, 4},
{ "SPI1_D0", "P9_29", 111, 1},
{ "SPI1_D1", "P9_30", 112, -1},
{ "SPI1_SCLK", "P9_31", 110, 1},
{ "VDD_ADC", "P9_32", 0, -1},
{ "AIN4", "P9_33", 0, -1},
{ "GNDA_ADC", "P9_34", 0, -1},
{ "AIN6", "P9_35", 0, -1},
{ "AIN5", "P9_36", 0, -1},
{ "AIN2", "P9_37", 0, -1},
{ "AIN3", "P9_38", 0, -1},
{ "AIN0", "P9_39", 0, -1},
{ "AIN1", "P9_40", 0, -1},
{ "CLKOUT2", "P9_41", 20, -1},
{ "GPIO0_7", "P9_42", 7, 0},
{ "DGND", "P9_43", 0, -1},
{ "DGND", "P9_44", 0, -1},
{ "DGND", "P9_45", 0, -1},
{ "DGND", "P9_46", 0, -1},
    { NULL, NULL, 0 }
};

int lookup_gpio_by_key(const char *key)
{
  pins_t *p;
  for (p = table; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int lookup_gpio_by_name(const char *name)
{
  pins_t *p;
  for (p = table; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int copy_pwm_key_by_key(const char *input_key, char *key)
{
    pins_t *p;
    for (p = table; p->key != NULL; ++p) {
        if (strcmp(p->key, input_key) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return 0;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;                
        }
    }
    return 0;
}

int get_pwm_key_by_name(const char *name, char *key)
{
    pins_t *p;
    for (p = table; p->name != NULL; ++p) {
        if (strcmp(p->name, name) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return 0;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_gpio_number(const char *key, unsigned int *gpio)
{
    *gpio = lookup_gpio_by_key(key);
    
    if (!*gpio) {
        *gpio = lookup_gpio_by_name(key);
    }

    return 0;
}

int get_pwm_key(const char *input, char *key)
{
    if (!copy_pwm_key_by_key(input, key)) {
        return get_pwm_key_by_name(input, key);
    }

    return 1;
}