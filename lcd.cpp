#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <unistd.h>

using namespace std;

// *****************************************************************************************
// Incomplete TEST Code, Not production ready!!!!!!
//
// Compile
// g++ lcd.cpp  -lwiringPi -o lcdtest
// Test
// sudo ./lcdtest
// *****************************************************************************************

#define INS 0xF8

// ******************************************************************************************
// LCD

class Lcd {
   int fd;
   unsigned char buffer[10];
   int CHANNEL;

   public:
	Lcd()
	{
		CHANNEL=1;
		// CHANNEL is the wiringPi name for the chip select (or chip enable) pin.
		// Set this to 0 or 1, depending on how it's connected.
   		fd = wiringPiSPISetup(CHANNEL, 100000); // TODO fd????
	}

	// Destructor ???? TODO

	void off()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0x80;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void clr()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0x10;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(5000);
	}

	void entry_mode()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0x60;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void on()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0xc0;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void cursor_blink()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0xd0;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void cursor_on()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0xe0;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void cursor_all()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0xf0;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void home()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x00;
   		buffer[2] = 0x20;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void fn8()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x30;
   		buffer[2] = 0x00;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void fn8_1()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x30;
   		buffer[2] = 0x20;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void fn8_3()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x30;
   		buffer[2] = 0x60;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	void fn_ext()
	{
   		buffer[0] = INS;
   		buffer[1] = 0x20;
   		buffer[2] = 0x40;
   		wiringPiSPIDataRW(CHANNEL, buffer, 3);
   		usleep(1000);
	}

	// TODO
	// line_no  1,2 OK
	// line_no  3,4 NG
	void ddr_addr(int line_no)
	{
   		int index = 0x80;
   		if (line_no == 1) index = 0x90;
   		else if (line_no == 2) index = 0xA0;
   		else if (line_no == 3) index = 0xb0;
   		buffer[0] = 0xF8;
   		buffer[1] = index & 0xF0;
   		buffer[2] = (0xF0)&(index<<4);
   		wiringPiSPIDataRW(CHANNEL, buffer,3);
   		usleep(1000);
	}

	void pos(int index)
	{
   		index |= 0x80;
   		buffer[0] = 0xF8;
   		buffer[1] = index & 0xF0;
   		buffer[2] = (0xF0)&(index<<4);
   		wiringPiSPIDataRW(CHANNEL, buffer,3);
   		usleep(1000);
	}

	void pos_graphic(unsigned char index)
	{
   		buffer[0] = 0xF8;
   		buffer[1] = index & 0xF0;
   		buffer[2] = (0xF0)&(index<<4);
   		wiringPiSPIDataRW(CHANNEL, buffer,3);
   		usleep(1000);
	}

	void char_code( unsigned char index)
	{
   		buffer[0] = 0xFA;
   		buffer[1] = index & 0xF0;
   		buffer[2] = (0xF0)&(index<<4);
   		wiringPiSPIDataRW(CHANNEL, buffer,3);
   		usleep(1000);
	}

	void char_code_ext( unsigned int index)
	{
   		buffer[0] = 0xFA;
   		buffer[1] = (index & 0xF000) >> 8;
   		buffer[2] = (index & 0x0F00) >> 4;
   		buffer[3] = (index & 0x00F0) >> 0;
   		buffer[4] = (index & 0x000F) << 4;
   		wiringPiSPIDataRW(CHANNEL, buffer,5);
   		usleep(1000);
	}

	void char_code_graphic(unsigned char index)
	{
   		buffer[0] = 0xFA;
   		buffer[1] = (index & 0xF0);
   		buffer[2] = (index & 0x0F) << 4;
   		wiringPiSPIDataRW(CHANNEL, buffer,3);
   		usleep(100);
	}

};

// ***********************************************************************************************
// LED
void led_init()
{
	pinMode(4, OUTPUT);
}
	
void led_on()
{
	digitalWrite(4, LOW);
}

void led_off()
{
	digitalWrite(4, HIGH);
}

// ***********************************************************************************************
// Interrupt

static int inter_1 = 0;
static int inter_2 = 0;

void interrupt1()
{
   inter_1 =1;
}

void interrupt2()
{
   inter_2 =1;
}

void setup_interrupts()
{
	wiringPiISR(18, INT_EDGE_RISING, interrupt1);
	wiringPiISR(17, INT_EDGE_RISING, interrupt2);
}

// ********************************************************************************************
// Test Code
static int line_no = 0;

static void lcd_init_simple1(Lcd *lcd)
{
   lcd->off();
   lcd->fn8();
   lcd->clr();
   lcd->entry_mode();
   lcd->on();
   lcd->cursor_blink();
   lcd->cursor_on();
   lcd->cursor_all();
   lcd->home();
   lcd->entry_mode();
   lcd->entry_mode();
}

static void lcd_simple_test1(Lcd *lcd)
{
   lcd->ddr_addr(line_no);
   for (unsigned char i=0 ; i< 64; i++)
   {
	// Char
        lcd->char_code( i);
   }
}

static void lcd_simple_test2(Lcd *lcd)
{
   int pos = 0;
   for (unsigned int i=0xB141 ; i< 0xB161 ; i=i+0x1)
   {
	// Char
        lcd->pos((pos++)%32);
        lcd->char_code_ext( i);
	usleep(1000);
   }
}

static void lcd_simple_test4(Lcd *lcd)
{
   lcd->fn8_3();
   lcd->clr();
   // FILL TOP HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x80);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0x0);
	}
   }
   // FILL BOTTOM HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x88);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0x0);
	}
    }
}

static void lcd_simple_test5(Lcd *lcd)
{
   lcd->fn8_3();
   lcd->clr();
   // FILL TOP HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x80);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( j);
	}
   }
   // FILL BOTTOM HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x88);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0xFF-j);
	}
    }
}

static void lcd_simple_test6(Lcd *lcd)
{
   lcd->fn8_3();
   lcd->clr();
   // FILL TOP HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x80);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0x40 );
	}
   }
   // FILL BOTTOM HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x88);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0x04);
	}
    }
}

static void lcd_simple_test3(Lcd *lcd)
{
   lcd->fn8_3();
   lcd->clr();

   // FILL TOP HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x80);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0xFF);
	}
	usleep(1000);
   }
   // FILL BOTTOM HALF
   for (unsigned char i=0 ; i< 32; i++)
   {
	// Char
        lcd->pos_graphic(0x80|i);
        lcd->pos_graphic(0x88);
   	for (unsigned int j=0 ; j< 16; j++)
	{
        	lcd->char_code_graphic( 0x00);
	}
	usleep(1000);
   }
}
	
// ********************************************************************************************
// Main Test

static int test_no = 0;

int main()
{
   int fd, result;
   Lcd *lcd = new Lcd();

   fd = wiringPiSetupGpio();

   led_init();
   lcd_init_simple1(lcd);
   setup_interrupts();
   led_on();

   while (1) {
	usleep(1000);
   	if (inter_1 ==1 )
	{
   		lcd_init_simple1(lcd);
		if (test_no == 0)
		{
			led_on();
			lcd_simple_test1(lcd);
		}
		else if (test_no == 1)
		{
			led_off();
			lcd_simple_test2(lcd);
		}
		test_no = (++test_no)%2;
   		inter_1 =0;
	}
   	if (inter_2 ==1 )
	{
		led_off();
   		lcd_init_simple1(lcd);
		lcd->on();
		if (test_no ==0)
		{
			led_on();
			lcd_simple_test3(lcd);
		}
		else if (test_no ==1)
		{
			led_off();
			lcd_simple_test4(lcd);
		}
		else if (test_no ==2)
		{
			led_on();
			lcd_simple_test5(lcd);
		}
		else
		{
			led_off();
			lcd_simple_test6(lcd);
		}
   		inter_2 =0;
   		test_no = (++test_no)%4;
	}
   }
}
