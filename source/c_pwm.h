int pwm_enable(const char *key);
int pwm_disable(const char *key);
int pwm_set_frequency(const char *key, float freq);
int pwm_set_duty_cycle(const char *key, float duty);
void pwm_cleanup(void);