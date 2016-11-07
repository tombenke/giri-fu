#define SECOND (1L)
#define MINUTE (60L*SECOND)
#define HOUR (60L*MINUTE)
#define DAY (24L*HOUR)
#define TIME(h,m,s) (h*HOUR+m*MINUTE+s*SECOND)

extern long rtc_tick(void);
extern long int rtc_getCurrentTime(void);
extern void rtc_setCurrentTime(long int time);
extern void rtc_setTurnOver(long int newTurnOver);
