#ifndef	__WIRINGBB_H__
#define	__WIRINGBB_H__



#define MAXGPIO 16

// Threads

#define	BB_THREAD(X)	void *X (void *dummy)

extern void testPrint(void);

#ifdef __cplusplus
extern "C" {
#endif

//core

extern void initialBBduino(int* GPIO,int NumOfPin);
extern void exitBBduino(void);

extern void pinMode(int pin, int mode);
extern int digitalRead(int pin);
extern void digitalWrite(int pin, int value);


// Schedulling priority

extern int piHiPri (const int pri) ;

// Extras from arduino land
extern void delay(unsigned int howLong);
extern void delayMicroseconds(unsigned int howLong);
extern unsigned int millis(void);
extern unsigned int micros(void);

#ifdef __cplusplus
}
#endif

#endif//end of __WIRINGBB_H__
