#include <nuvoton/functions.h>
#include <nuvoton/N76E003.h>
#include <nuvoton/Common.h>
#include <nuvoton/Delay.h>
#include <nuvoton/SFR_Macro.h>

__bit BIT_TMP;

#define RELAY P15

#define RED P02
#define GREEN P03
#define BLUE P04

#define PWM_RELAY PWM5L
#define PWM_BLUE PWM2L


#define BUTTON1 P01
#define BUTTON2 P00

int getchar(void)
{
    UINT8 c;
    while (!RI);
    c = SBUF;
    RI = 0;
    return (c);
}

int putchar (int c)
{
	if (c == '\n')
		putchar('\r');
    TI = 0;
    SBUF = c;
    while(TI==0);
	return 0;
}

/**
 *  Stock protocol
 *  Open relay:A0 01 01 A2
 *  Close relay:A0 01 00 A1
 */

void receive_packet(unsigned char *tmp)
{
	unsigned char checksum;
	tmp[0]=0;
	while (1) {
		do {
			tmp[0] = getchar();
		} while (tmp[0] != 0xA0);
		tmp[1] = getchar();
		tmp[2] = getchar();
		tmp[3] = getchar();
		checksum = tmp[0] + tmp[1] + tmp[2];
		if (checksum == tmp[3])
			break;
	}
}

void uart_loop() {
	unsigned char tmp[4];
	receive_packet(tmp);
	BLUE = 1;
	switch(tmp[1]) {
		case 1:
			RELAY = tmp[2] ? 255 : 0; //100% PWM if payload is not 0
			if ( RELAY > 1) {
				Timer0_Delay1ms(100); //wait 100ms for relay to stabilize
				RELAY = 130; //51% PWM to reduce relay current draw
			}
			break;
		case 0x11:
			RED = tmp[2] ? 1 : 0;
			break;
		case 0x12:
			GREEN = tmp[2] ? 1 : 0;
			break;
		case 0x13:
			PWM_BLUE = tmp[2];
			LOAD = 1;
			break;
		/* Pollin' ain't a good idea here */
		case 0x21:
			printf("S1:%x\n", BUTTON1);
			break;
		case 0x22:
			printf("S2:%x\n", BUTTON2);
			break;
	}
}

void uart_init(UINT32 u32Baudrate) //T1M = 1, SMOD = 1
{
	P06_PushPull_Mode;
	P07_Input_Mode;

	SCON = 0x50;  //UART0 Mode1,REN=1,TI=1
	TMOD |= 0x20; //Timer1 Mode1

	set_SMOD; //UART0 Double Rate Enable
	set_T1M;
	clr_BRCK; //Serial port 0 baud rate clock source = Timer1

#ifdef FOSC_160000
	TH1 = 256 - (1000000 / u32Baudrate + 1); /*16 MHz */
#endif
#ifdef FOSC_166000
	TH1 = 256 - (1037500 / u32Baudrate); /*16.6 MHz */
#endif
	set_TR1;
	set_TI; //For printf function must setting TI = 1
}

void gets(char *tmp) {
	do {
		*tmp = getchar();
	} while (*tmp++ != '\n');
}

int main()
{
	uart_init(19200);
	printf("\n\n\nHello world. I'm a hacky opensource firmware for LC-tech modules\n");
	/* Relay */
	P15_PushPull_Mode;

	/* LEDS */
	P02_PushPull_Mode;
	P03_PushPull_Mode;
	P04_PushPull_Mode;

	/* Buttons */
	P01_Input_Mode;
	P00_Input_Mode;

	/* buttonz */
	PWMPH = 0;
	PWMPL = 255;
	PWM_CLOCK_FSYS;
	PWM_CLOCK_DIV_128;
	PM_EDGE_TYPE;
	PWM3_P04_OUTPUT_ENABLE;
	PWM5_P15_OUTPUT_ENABLE;

	RED = 0;
	GREEN = 0;
	BLUE = 0;

	Timer0_Delay1ms(500);
	PWM3H = 0;
	PWM5H = 0;

	RED = 1;
	PWMRUN = 1;
	while (1)
	{
		uart_loop();
	}
}
