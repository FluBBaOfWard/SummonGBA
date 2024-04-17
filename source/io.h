#ifndef IO_HEADER
#define IO_HEADER

#ifdef __cplusplus
	extern "C" {
#endif

extern char sineTable[256];

extern int randomNumber(int max);
extern int clear16BitScreen();
extern int fill16BitScreen(int value);
extern int clear4BitScreen();
extern int fill4BitScreen(int value);
extern int fillWithCPU(void *dst, int value, int len);

#ifdef __cplusplus
	} // extern "C"
#endif

#endif // IO_HEADER
