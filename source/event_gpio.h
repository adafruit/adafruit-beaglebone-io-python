#define NO_EDGE      0
#define RISING_EDGE  1
#define FALLING_EDGE 2
#define BOTH_EDGE    3

#define INPUT  0
#define OUTPUT 1
#define ALT0   4

#define HIGH 1
#define LOW  0

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2

int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
void exports_cleanup(void);
int gpio_set_direction(unsigned int gpio, unsigned int in_flag);
int gpio_get_direction(unsigned int gpio, unsigned int *value);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);

int add_edge_detect(unsigned int gpio, unsigned int edge);
void remove_edge_detect(unsigned int gpio);
int add_edge_callback(unsigned int gpio, void (*func)(unsigned int gpio));
int event_detected(unsigned int gpio);
int gpio_event_added(unsigned int gpio);
int event_initialise(void);
void event_cleanup(void);
int blocking_wait_for_edge(unsigned int gpio, unsigned int edge);
