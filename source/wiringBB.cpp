#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include <pthread.h>
#include <sys/time.h>
//#include "common.h"
#include "event_gpio.h"

#include "wiringBB.h"

static uint64_t epochMilli, epochMicro;
bool error;
int* myGPIO;
int myNumOfPin;

void testPrint() {
	printf("Hallo BBduino !\n");
}

void initialBBduino(int* GPIO, int NumOfPin) {
	printf("BBduino initial! Number of GPIOs is %d\n", NumOfPin);
	error = false;
	myGPIO = GPIO;
	myNumOfPin = NumOfPin;
	printf("GPIO is %d\n", myGPIO[0]);

	if (NumOfPin > MAXGPIO) {
		printf(
				"BBduino error! the max number of GPIOs if %d \nNumber of GPIOs is %d\n",
				MAXGPIO, NumOfPin);
		error = true;
		return;
	}
	for (int i = 0; i < NumOfPin; i++) {
		printf("GPIO is %d\n", myGPIO[i]);
		gpio_export(myGPIO[i]);
//		gpio_set_direction(myGPIO[i], OUTPUT);
	}
}

void exitBBduino() {
	if (myGPIO == NULL)
		return;
	if (!error) {
		for (int i = 0; i < myNumOfPin; i++) {
			gpio_unexport(myGPIO[i]);
		}
	}
}

void pinMode(int pin, int mode) {
	gpio_set_direction(pin, mode);
}

int digitalRead(int pin) {
	unsigned int returnval;
	gpio_get_value(pin, &returnval);
	return returnval;
}

void digitalWrite(int pin, int value) {
	gpio_set_value(pin, value);
}

/*
 * initialiseEpoch:
 *	Initialise our start-of-time variable to be the current unix
 *	time in milliseconds and microseconds.
 *********************************************************************************
 */

static void initialiseEpoch(void) {
	struct timeval tv;

	gettimeofday(&tv, NULL);
	epochMilli = (uint64_t) tv.tv_sec * (uint64_t) 1000
			+ (uint64_t)(tv.tv_usec / 1000);
	epochMicro = (uint64_t) tv.tv_sec * (uint64_t) 1000000
			+ (uint64_t)(tv.tv_usec);
}

/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */
void delay(unsigned int howLong) {
	struct timespec sleeper, dummy;

	sleeper.tv_sec = (time_t)(howLong / 1000);
	sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;

	nanosleep(&sleeper, &dummy);
}

/*
 * delayMicroseconds:
 *	This is somewhat intersting. It seems that on the Pi, a single call
 *	to nanosleep takes some 80 to 130 microseconds anyway, so while
 *	obeying the standards (may take longer), it's not always what we
 *	want!
 *
 *	So what I'll do now is if the delay is less than 100uS we'll do it
 *	in a hard loop, watching a built-in counter on the ARM chip. This is
 *	somewhat sub-optimal in that it uses 100% CPU, something not an issue
 *	in a microcontroller, but under a multi-tasking, multi-user OS, it's
 *	wastefull, however we've no real choice )-:
 *
 *      Plan B: It seems all might not be well with that plan, so changing it
 *      to use gettimeofday () and poll on that instead...
 *********************************************************************************
 */

void delayMicrosecondsHard(unsigned int howLong) {
	struct timeval tNow, tLong, tEnd;

	gettimeofday(&tNow, NULL);
	tLong.tv_sec = howLong / 1000000;
	tLong.tv_usec = howLong % 1000000;
	timeradd(&tNow, &tLong, &tEnd);

	while (
		timercmp (&tNow, &tEnd, <))
		gettimeofday(&tNow, NULL);
}

void delayMicroseconds(unsigned int howLong) {
	struct timespec sleeper;
	unsigned int uSecs = howLong % 1000000;
	unsigned int wSecs = howLong / 1000000;

	/**/if (howLong == 0)
		return;
	else if (howLong < 100)
		delayMicrosecondsHard(howLong);
	else {
		sleeper.tv_sec = wSecs;
		sleeper.tv_nsec = (long) (uSecs * 1000L);
		nanosleep(&sleeper, NULL);
	}
}

/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int millis(void) {
	struct timeval tv;
	uint64_t now;

	gettimeofday(&tv, NULL);
	now = (uint64_t) tv.tv_sec * (uint64_t) 1000
			+ (uint64_t)(tv.tv_usec / 1000);

	return (uint32_t)(now - epochMilli);
}

/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int micros(void) {
	struct timeval tv;
	uint64_t now;

	gettimeofday(&tv, NULL);
	now = (uint64_t) tv.tv_sec * (uint64_t) 1000000 + (uint64_t) tv.tv_usec;

	return (uint32_t)(now - epochMicro);
}
