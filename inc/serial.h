
#ifdef ARDUINO

#define uint8_t byte
#define uint16_t word

#else

// ...

#endif

extern uint8_t serialInit(void);
extern uint8_t serialAvailable(void);
extern uint8_t serialRead(void);
extern void serialWrite(uint8_t data);
