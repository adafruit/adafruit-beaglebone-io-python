#ifndef	__BBduino_H__
#define	__BBduino_H__

#define BBNAME "BBDuino"
#define MAXGPIO 16

#define GPIO_DIR "/sys/class/gpio/"
#define GPIO_SUB "gpio"
#define GPIO_EXPORRT "export"
#define GPIO_DIRECTION "direction"
#define GPIO_VLUE "vlue"


// Threads

#define	PI_THREAD(X)	void *X (void *dummy)

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

#endif//end of __BBduino_H__
