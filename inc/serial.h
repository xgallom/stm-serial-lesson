#ifndef SERIAL_H_
#define SERIAL_H_

#ifdef ARDUINO

#define uint8_t byte
#define uint16_t word

#else

// ...

#endif

extern void serialInit(void);
extern uint8_t serialAvailable(void);
extern uint8_t serialRead(void);
extern void serialWrite(uint8_t data);

extern void USART2_IRQHandler(void);

#endif
