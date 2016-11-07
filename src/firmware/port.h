//Macro's
#define SetPin(Port, Bit)    *Port |= (1 << Bit)
#define ClearPin(Port, Bit)    *Port &= ~(1 << Bit)
