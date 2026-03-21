#include <8051.h>

#define PRZEKAZNIK P1_5
#define LED P1_6
#define PRZYCISK P3_4
#define MAX_CZAS 9

static volatile unsigned char czas; //czas w minutach

static void delay (void)
{
	unsigned int i;
	for (i = 0; i < 0x7000; i++);
}

void main (void)
{
	unsigned char c;
	//Inicjalizacja ukladow czasowo-licznikowych T0 i T1
	TMOD = 0x10;
	//Inicjalizacja sterownika przerwan
	ET1 = 1;
	EA = 1;

	TR1 = 1;
	czas = 0;

	PRZYCISK = 1;//wejœcie
	
	for (;;) {
		if (!PRZYCISK) {
			if (czas < MAX_CZAS) czas++;
			for (c = 0; c < czas; c++) {
				LED = 1; //zgas diode
				delay();
				LED = 0;
				delay();
			}
		}
	}
}

/*
	Program obslugi przerwania od ukladu czasowo-licznikowego T1
	co 0,065536 s @ fxtal=12MHz
*/
void t1isr (void) __interrupt 3
{
	static unsigned long l = 0;

	if (l >= 45776UL) { //50 minut
		l = 0;
		if (czas > 0) czas--;
	} else {
		l++;
	}

	if (czas) PRZEKAZNIK = 0; //zalacz przekaznik
	else {
		PRZEKAZNIK = 1; //wylacz przekaznik
		LED = 1;  //zgas diode
	}
}
