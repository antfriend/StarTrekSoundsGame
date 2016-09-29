//blink the led
#define F_CPU 8000000UL
#include <avr/io.h>		//header file
#include <util/delay.h>
#include <avr/eeprom.h>

// Typedefs
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

// Defines
#define forever	117
#define PHAZER_ON	PORTC |= (1<<PC5)	//PHAZER
#define PHAZER_OFF	PORTC &= ~(1<<PC5)
#define ENERGIZE_ON	PORTC |= (1<<PC4)	//ENERGIZE
#define ENERGIZE_OFF	PORTC &= ~(1<<PC4)
#define COMMUNICATOR_ON	PORTC |= (1<<PC3)	//COMMUNICATOR
#define COMMUNICATOR_OFF	PORTC &= ~(1<<PC3)
#define REDALERT_ON	PORTC |= (1<<PC2)		//REDALERT
#define REDALERT_OFF	PORTC &= ~(1<<PC2)
#define PHOTONTORPEDO_ON	PORTC |= (1<<PC1)//PHOTONTORPEDO
#define PHOTONTORPEDO_OFF	PORTC &= ~(1<<PC1)
#define INTERCOM_ON	PORTC |= (1<<PC0)		//INTERCOM
#define INTERCOM_OFF	PORTC &= ~(1<<PC0)
#define TRICORDER_ON	PORTB |= (1<<PB3)	//TRICORDER
#define TRICORDER_OFF	PORTB &= ~(1<<PB3)
#define DOOR_ON	PORTB |= (1<<PB2)		//DOOR
#define DOOR_OFF	PORTB &= ~(1<<PB2)

//LEDS
#define Led1R_on	PORTD |= (1<<PD2)
#define Led1R_off	PORTD &= ~(1<<PD2)
#define Led1G_on	PORTD |= (1<<PD0)
#define Led1G_off	PORTD &= ~(1<<PD0)
#define Led1B_on	PORTD |= (1<<PD1)
#define Led1B_off	PORTD &= ~(1<<PD1)

#define Led2R_on	PORTB |= (1<<PB6)
#define Led2R_off	PORTB &= ~(1<<PB6)
#define Led2G_on	PORTD |= (1<<PD3)
#define Led2G_off	PORTD &= ~(1<<PD3)
#define Led2B_on	PORTD |= (1<<PD4)
#define Led2B_off	PORTD &= ~(1<<PD4)

#define Led3R_on	PORTD |= (1<<PD6)
#define Led3R_off	PORTD &= ~(1<<PD6)
#define Led3G_on	PORTB |= (1<<PB7)
#define Led3G_off	PORTB &= ~(1<<PB7)
#define Led3B_on	PORTD |= (1<<PD5)
#define Led3B_off	PORTD &= ~(1<<PD5)

#define Led4R_on	PORTB |= (1<<PB1)
#define Led4R_off	PORTB &= ~(1<<PB1)
#define Led4G_on	PORTD |= (1<<PD7)
#define Led4G_off	PORTD &= ~(1<<PD7)
#define Led4B_on	PORTB |= (1<<PB0)
#define Led4B_off	PORTB &= ~(1<<PB0)

#define ALLRED_on	Led1R_on;Led2R_on;Led3R_on;Led4R_on;
#define ALLRED_off	Led1R_off;Led2R_off;Led3R_off;Led4R_off;
#define ALLGRN_on	Led1G_on;Led2G_on;Led3G_on;Led4G_on;
#define ALLGRN_off	Led1G_off;Led2G_off;Led3G_off;Led4G_off;
#define ALLBLU_on	Led1B_on;Led2B_on;Led3B_on;Led4B_on;
#define ALLBLU_off	Led1B_off;Led2B_off;Led3B_off;Led4B_off;


//Prototypes
void InitPorts (void);
void RefreshAll(void);
void PauseBy(int daPause);
void AllLEDsOff(void);
int RightButtonIsPushed(int daPause);
int LeftButtonIsPushed(int daPause);
void Intro(void);
void PhotonTorpedoFirstSteps(int PauseDelay);
void PhotonTorpedoLastSteps(int PauseDelay);
void ButtonPlayTest(void);
void RewardSequence(void);
void PlayThisOne(int theColumn, int theRow);
void PHAZER(void);
void ENERGIZE(void);
void COMMUNICATOR(void);
void REDALERT(void);
void PHOTONTORPEDO(void);
void INTERCOM(void);
void TRICORDER(void);
void DOOR(void);
void MakeTriangleWave(void);
void SetRowColumnColors(void);
void PLAY1(void);
void RECORD1(void);
void PLAY2(void);
void RECORD2(void);
uint8_t ReadByteFromEeprom(void);
void WriteByteToEeprom(uint8_t x);
void SetAll4(int daColor, int daBrightness);

//globals
int LEDmatrix[4][3];
const int R = 1;
const int G = 2;
const int B = 3;
int trianglewave[100];
int gtheRow=0;
int gtheColumn=4;
uint8_t EEMEM EEtheRow; //the address
uint8_t EgtheRow;//the variable to hold data
int daLetterCount=1;

uint8_t ReadByteFromEeprom(void)
{ 
   uint8_t temp; 
   temp = eeprom_read_byte(&EEtheRow);
   return(temp); 
}

void SetAll4(int daColor, int daBrightness)
{
	LEDmatrix[1][daColor]=daBrightness;
	LEDmatrix[2][daColor]=daBrightness;
	LEDmatrix[3][daColor]=daBrightness;
	LEDmatrix[4][daColor]=daBrightness;
}

void WriteByteToEeprom(uint8_t x)
{ 
   eeprom_write_byte (&EEtheRow, x); 
}

void delayms( uint16_t millis ) {
	while ( millis ) {
		_delay_ms( 1 );
		millis--;
	}
}

void delayus( int micrs ) {
	while ( micrs ) {
		_delay_us( 1 );
		micrs--;
	}
}

void InitPorts(void)
{
//Sounds all set as output
	DDRB |= 1<<PB2;
	DDRB |= 1<<PB3;
	DDRC |= 1<<PC0;
	DDRC |= 1<<PC1;
	DDRC |= 1<<PC2;
	DDRC |= 1<<PC3;
	DDRC |= 1<<PC4;
	DDRC |= 1<<PC5;

//Buttons 1 and 2 set as input
	//DDRB |= 0<<PB5;
	//DDRB |= 0<<PB4;	
	DDRB &=~_BV(5);//Set as input
	PORTB |=_BV(5);//Enable pull up
	DDRB &=~_BV(4);//Set as input
	PORTB |=_BV(4);//Enable pull up
//LED1 et all set as output
	DDRD |= 1<<PD0;
	DDRD |= 1<<PD1;
	DDRD |= 1<<PD2;
//LED2
	DDRD |= 1<<PD3;
	DDRD |= 1<<PD4;
	DDRB |= 1<<PB6;
//LED3
	DDRB |= 1<<PB7;
	DDRD |= 1<<PD5;
	DDRD |= 1<<PD6;
//LED4
	DDRD |= 1<<PD7;
	DDRB |= 1<<PB0;
	DDRB |= 1<<PB1;
//Phaser sound
	DDRC |= 1<<PC5;
//Set variables
	MakeTriangleWave();
}

void PauseBy(int daPause)
{
	for (int i=0; i < daPause; i++)
	{
		RefreshAll();
	}
}

void AllLEDsOff()
{
	for (int i=1; i <=4; ++i)
	{
		for (int y=1; y <=3; ++y)
		{
			LEDmatrix[i][y]=0;
		}
	}
	//RefreshAll();
}

int RightButtonIsPushed(int daPause)
{
	int daButtonState=0;
	for (int i=0; i < daPause; i++)
	{
		if (~PINB & (1<<4))
		{
			daButtonState=1;
		}
		RefreshAll();
	}
	return daButtonState;
}
int LeftButtonIsPushed(int daPause)
{
	int daButtonState=0;
	for (int i=0; i < daPause; i++)
	{
		if (~PINB & (1<<5))
		{
			daButtonState=1;
		}
		RefreshAll();
	}
	return daButtonState;
}

void RefreshAll(void)
{
	//brightness values can be 0 to iIterations
	int iIterations=300;
	int MeasureOfTime=2; //iIterations * MeasureOfTime = overall refresh time

	//turn all that should be on, on
	if (LEDmatrix[1][R] > 0) Led1R_on;//LED 1R should be on for at least one measure
	if (LEDmatrix[1][G] > 0) Led1G_on;
	if (LEDmatrix[1][B] > 0) Led1B_on;

	if (LEDmatrix[2][R] > 0) Led2R_on;
	if (LEDmatrix[2][G] > 0) Led2G_on;
	if (LEDmatrix[2][B] > 0) Led2B_on;

	if (LEDmatrix[3][R] > 0) Led3R_on;
	if (LEDmatrix[3][G] > 0) Led3G_on;
	if (LEDmatrix[3][B] > 0) Led3B_on;

	if (LEDmatrix[4][R] > 0) Led4R_on;
	if (LEDmatrix[4][G] > 0) Led4G_on;
	if (LEDmatrix[4][B] > 0) Led4B_on;

	//execute all values for iIterations * measures of time
	for (int i=0; i < iIterations; i=i+1)
	{
		//wait for one measure
		delayus(MeasureOfTime);
		//turn all that should be off, off
		if (LEDmatrix[1][R] <= i) Led1R_off; //LED 1R should be off from this count forward
		if (LEDmatrix[1][G] <= i) Led1G_off;
		if (LEDmatrix[1][B] <= i) Led1B_off;

		if (LEDmatrix[2][R] <= i) Led2R_off;
		if (LEDmatrix[2][G] <= i) Led2G_off;
		if (LEDmatrix[2][B] <= i) Led2B_off;

		if (LEDmatrix[3][R] <= i) Led3R_off;
		if (LEDmatrix[3][G] <= i) Led3G_off;
		if (LEDmatrix[3][B] <= i) Led3B_off;

		if (LEDmatrix[4][R] <= i) Led4R_off;
		if (LEDmatrix[4][G] <= i) Led4G_off;
		if (LEDmatrix[4][B] <= i) Led4B_off;
	}
}

void RewardSequence(void)
{
	//pretty and fancy
	AllLEDsOff();
	ALLRED_on;
	ENERGIZE();
	ALLRED_off;
	PauseBy(1);
	AllLEDsOff();PauseBy(1);
	for (int i=0; i < 50; i=i+5)
	{
		
		LEDmatrix[1][R]=i;
		LEDmatrix[2][R]=i;
		LEDmatrix[3][R]=i;
		LEDmatrix[4][R]=i;
		PauseBy(10);
	}
	for (int i=0; i < 50; i=i+5)
	{
		
		LEDmatrix[1][G]=i;
		LEDmatrix[2][G]=i;
		LEDmatrix[3][G]=i;
		LEDmatrix[4][G]=i;
		PauseBy(10);
	}
	for (int i=0; i < 50; i=i+5)
	{
		
		LEDmatrix[1][B]=i;
		LEDmatrix[2][B]=i;
		LEDmatrix[3][B]=i;
		LEDmatrix[4][B]=i;
		PauseBy(10);
	}
	for (int i=50; i > 0; i=i-5)
	{
		
		LEDmatrix[1][R]=i;
		LEDmatrix[2][R]=i;
		LEDmatrix[3][R]=i;
		LEDmatrix[4][R]=i;
		PauseBy(10);
	}
	for (int i=50; i > 0; i=i-5)
	{
		
		LEDmatrix[1][G]=i;
		LEDmatrix[2][G]=i;
		LEDmatrix[3][G]=i;
		LEDmatrix[4][G]=i;
		PauseBy(10);
	}
	AllLEDsOff();PauseBy(1);
	for (int i=80; i > 0; i=i-5)
	{
		
		LEDmatrix[1][B]=i;
		LEDmatrix[2][B]=i;
		LEDmatrix[3][B]=i;
		LEDmatrix[4][B]=i;
		PauseBy(10);
	}
	AllLEDsOff();PauseBy(1);
}
void SetRowColumnColors(void)
{

			switch (gtheColumn + gtheRow)
			{
				case 1 : //PHAZER();
					LEDmatrix[gtheColumn][R]=10; 
					LEDmatrix[gtheColumn][G]=20; 
					LEDmatrix[gtheColumn][B]=0; 
					break;
				case 2 : //ENERGIZE(); 
					LEDmatrix[gtheColumn][R]=0; 
					LEDmatrix[gtheColumn][G]=100; 
					LEDmatrix[gtheColumn][B]=10; 
					break;
				case 3 : //COMMUNICATOR(); 
					LEDmatrix[gtheColumn][R]=0; 
					LEDmatrix[gtheColumn][G]=0; 
					LEDmatrix[gtheColumn][B]=30; 
					break;
				case 4 : //REDALERT(); 
					LEDmatrix[gtheColumn][R]=30; 
					LEDmatrix[gtheColumn][G]=0; 
					LEDmatrix[gtheColumn][B]=0; 
					break;
				case 5 : //PHOTONTORPEDO(); 
					LEDmatrix[gtheColumn][R]=30; 
					LEDmatrix[gtheColumn][G]=0; 
					LEDmatrix[gtheColumn][B]=50; 
					break;
				case 6 : //INTERCOM(); 
					LEDmatrix[gtheColumn][R]=0; 
					LEDmatrix[gtheColumn][G]=30; 
					LEDmatrix[gtheColumn][B]=50; 
					break;
				case 7 : //TRICORDER(); 
					LEDmatrix[gtheColumn][R]=10; 
					LEDmatrix[gtheColumn][G]=10; 
					LEDmatrix[gtheColumn][B]=10; 
					break;
				case 8 : //DOOR(); 
					LEDmatrix[gtheColumn][R]=0; 
					LEDmatrix[gtheColumn][G]=10; 
					LEDmatrix[gtheColumn][B]=0; 
					break;
				case 9 : //PLAY1(); 
					LEDmatrix[1][G]=10; 
					LEDmatrix[2][B]=10; 
					LEDmatrix[3][B]=10; 
					LEDmatrix[4][B]=10; 
					break;
				case 10 : //RECORD1(); 
					LEDmatrix[1][B]=10; 
					LEDmatrix[2][R]=10; 
					LEDmatrix[3][B]=10; 
					LEDmatrix[4][B]=10; 
					break;
				case 11 : //PLAY2(); 
					LEDmatrix[1][B]=10; 
					LEDmatrix[2][B]=10; 
					LEDmatrix[3][G]=10; 
					LEDmatrix[4][B]=10; 
					break;
				case 12 : //RECORD2(); 
					LEDmatrix[1][B]=10; 
					LEDmatrix[2][B]=10; 
					LEDmatrix[3][B]=10; 
					LEDmatrix[4][R]=10; 
					break;
			}
			PauseBy(1);
}

void ButtonPlayTest(void)
{
	AllLEDsOff();
	LEDmatrix[1][G]=10;
	LEDmatrix[2][G]=10;
	LEDmatrix[3][G]=10;
	LEDmatrix[4][G]=10;
	PauseBy(10);
	AllLEDsOff();
	SetRowColumnColors();
	for (int x=0; x < 250; ++x)
	{
		//check the right button
		if (RightButtonIsPushed(2))
		{
			PlayThisOne(gtheColumn, gtheRow);
			//SetRowColumnColors();
		}
		//check the left button
		if (LeftButtonIsPushed(2))
		{
			--gtheColumn;
			if (gtheColumn<=0) 
			{
				gtheColumn=4;
				gtheRow=gtheRow+4;
				if (gtheRow>=8) gtheRow=0;//*** set the 8 to 12 to include the 3rd row ****
			}
			AllLEDsOff();
			WriteByteToEeprom(gtheColumn);
			SetRowColumnColors();
			daLetterCount=1;
			PauseBy(30);
		}
		PauseBy(1);		
	}
}


void Intro(void)
{
	//The Start Up Entertainment
	//initial blinky show
	AllLEDsOff();
	for (int i=0; i < 100; i=i+10)
	{
		
		LEDmatrix[1][R]=i;
		LEDmatrix[2][R]=i;
		LEDmatrix[3][R]=i;
		LEDmatrix[4][R]=i;
		PauseBy(10);
	}
	for (int i=100; i >= 1; i=i-10)
	{
		
		LEDmatrix[1][R]=i;
		LEDmatrix[2][R]=i;
		LEDmatrix[3][R]=i;
		LEDmatrix[4][R]=i;
		PauseBy(10);
	}
	AllLEDsOff();
	for (int Loopy=1; Loopy <= 3; ++Loopy)
	{
		for (int i=1; i <= 4; ++i)
		{
			LEDmatrix[i][Loopy]=90;
			PauseBy(20);
			LEDmatrix[i][Loopy]=0;
		}
		for (int i=4; i >= 1; --i)
		{
			LEDmatrix[i][Loopy]=90;
			PauseBy(20);
			LEDmatrix[i][Loopy]=0;
		}
	}
	AllLEDsOff();
	//ButtonPlayTest();
}

void PhotonTorpedoFirstSteps(int PauseDelay)
{
	PHOTONTORPEDO_ON;
	PauseBy(10);
	PHOTONTORPEDO_OFF;
	AllLEDsOff();PauseBy(1);
	//pulse left to right
	LEDmatrix[4][R]=30;
	LEDmatrix[4][B]=50;
	LEDmatrix[3][B]=1;
	PauseBy(PauseDelay);

	LEDmatrix[4][R]=0;
	LEDmatrix[4][B]=2;
	LEDmatrix[3][R]=30;
	LEDmatrix[3][B]=50;
	LEDmatrix[2][B]=1;

	LEDmatrix[4][B]=0;
	LEDmatrix[3][R]=0;
	LEDmatrix[3][B]=2;
	LEDmatrix[2][R]=30;
	LEDmatrix[2][B]=50;
	LEDmatrix[1][B]=1;
}
void PhotonTorpedoLastSteps(int PauseDelay)
{
	//last steps
	LEDmatrix[3][B]=0;
	LEDmatrix[2][R]=0;
	LEDmatrix[2][B]=2;
	LEDmatrix[1][R]=30;
	LEDmatrix[1][B]=50;
	PauseBy(PauseDelay);

	LEDmatrix[2][B]=0;
	LEDmatrix[1][R]=0;
	LEDmatrix[1][B]=2;
	PauseBy(PauseDelay);

	LEDmatrix[1][B]=0;
	PauseBy(PauseDelay);
}

int main(void)	//program start
{
	InitPorts();
	gtheRow=0;
	gtheColumn=ReadByteFromEeprom();;
	//Intro();

	while(forever)
	{
		ButtonPlayTest();
	}
	return (0);
}

void PlayThisOne(int theColumn, int theRow)
{
	switch (theColumn + theRow)
	{
		case 1 : PHAZER(); break;
		case 2 : ENERGIZE(); break;
		case 3 : COMMUNICATOR(); break;
		case 4 : REDALERT(); break;
		case 5 : PHOTONTORPEDO(); break;
		case 6 : INTERCOM(); break;
		case 7 : TRICORDER(); break;
		case 8 : DOOR(); break;
		case 9 : PLAY1();break;
		case 10 : RECORD1();break;
		case 11 : PLAY2();break;
		case 12 : RECORD2();break;
	}
}

void PLAY1(void)
{
	//PLAY
//	WriteByteToEeprom(3);
	COMMUNICATOR();
}
void RECORD1(void)
{
	//RECORD
	//WriteByteToEeprom(8);
	COMMUNICATOR();
}
void PLAY2(void)
{
	//PLAY
	COMMUNICATOR();
}

void RECORD2(void)
{
	//RECORD
	COMMUNICATOR();
	int disColor=1;
	int disBrightness=0;
	AllLEDsOff();
	PauseBy(10);
	while(forever)
	{
		if (LeftButtonIsPushed(10))//if both buttons are pushed
		{
			if (RightButtonIsPushed(10))
			{
				//both buttons were pushed
				INTERCOM_ON;
				PauseBy(10);
				INTERCOM_OFF;
				PauseBy(40);
				++disColor;
				if (disColor >= 4)
				{
					break;
				}
			}
		}
		if (LeftButtonIsPushed(20))
		{
			if (disBrightness <= 0)
			{
				disBrightness = 100;
			}
			--disBrightness;
	
		}
		if (RightButtonIsPushed(20))
		{
			if (disBrightness >= 100)
			{
				disBrightness = 0;
			}
			++disBrightness;
		}
		LEDmatrix[1][disColor]=disBrightness;
		LEDmatrix[2][disColor]=disBrightness;
		LEDmatrix[3][disColor]=disBrightness;
		LEDmatrix[4][disColor]=disBrightness;
		PauseBy(2);
	}
}

void ENERGIZE(void)
{
//	WriteByteToEeprom(2);
	ENERGIZE_ON;
	LEDmatrix[1][B]=50;
	LEDmatrix[2][B]=50;
	LEDmatrix[3][B]=50;
	LEDmatrix[4][B]=50;
	PauseBy(10);
	ENERGIZE_OFF; //sound plays on...

		//int daSpeed=30;
	for (int x=1; x <= 7; x=1+x)
	{
		for (int y=1; y <= 3; y=1+y)
		{
			SetAll4(G, 0);
			SetAll4(R, 300/x);
			PauseBy(8-x);
			//SetAll4(G, 300);
			//PauseBy(daSpeed);
			//SetAll4(G, y);
			//PauseBy(daSpeed);
			//SetAll4(G, 0);
			//PauseBy(daSpeed);
		}
		SetAll4(B, 0);
		SetAll4(G, x*30);
		PauseBy(x);
	}
	SetAll4(R, 300);
	PauseBy(5);

	AllLEDsOff();
	for (int x=250; x >= 0; x=x-3)
	{
		SetAll4(G, x);
		LEDmatrix[2][G]=300;
		PauseBy(1);
	}
		AllLEDsOff();
		LEDmatrix[2][G]=300;
		PauseBy(1);
}

void REDALERT(void)
{
//	WriteByteToEeprom(4);
	int z=1;
	int y=4;
	AllLEDsOff();
	REDALERT_ON;
	PauseBy(10);
	REDALERT_OFF;
	for (int w=0;w<=1;++w)
	{
		for (int x=0;x<=100;x=x+1)
		{
			LEDmatrix[y][R]=trianglewave[x]*3;
			PauseBy(2);
			++y;
			if (y==5) y=1;
			if (y==1)
			{
				z=4;
			}
			else
			{
				z=y-1;
			}
	}
		AllLEDsOff();
		PauseBy(100);
	}
	LEDmatrix[4][R]=30;
	PauseBy(1);
}
void PHAZER(void)
{
//	PHAZER_ON;
//	PauseBy(10);
//	PHAZER_OFF;
//	AllLEDsOff();
	int daDelay=1;
	switch (daLetterCount)
	{
		case 1 : //BLANK BLANK BLANK BLANK BLANK 
		PHAZER_ON;
		//PauseBy(5);
		//PHAZER_OFF;
		AllLEDsOff();//1
		LEDmatrix[4][B]=300;
		LEDmatrix[3][B]=300;
		LEDmatrix[2][B]=300;
		LEDmatrix[1][B]=300;
		PauseBy(7);
		AllLEDsOff();
		PHAZER_OFF;
		break;

		case 2 ://SSSSSSSSSSSSSSSSSSSSSSSSSSSSS
		AllLEDsOff();//1
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
//		LEDmatrix[4][B]=300;
//		LEDmatrix[3][B]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		case 3 ://TTTTTTTTTTTTTTTTTTTTTTTTTTTTT
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		case 4 : //AAAAAAAAAAAAAAAAAAAAAAAAAAAA
		AllLEDsOff();//1
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		case 5 ://RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(1);
		break;

		case 6 : //BLANK BLANK BLANK BLANK BLANK BLANK 
		AllLEDsOff();//1
		LEDmatrix[4][B]=300;
		LEDmatrix[3][B]=300;
		LEDmatrix[2][B]=300;
		LEDmatrix[1][B]=300;
		PauseBy(7);
		AllLEDsOff();
		break;

		case 7 ://TTTTTTTTTTTTTTTTTTTTTTTTT
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
//		LEDmatrix[4][B]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		case 8 ://RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(1);
		break;

		case 9 : //EEEEEEEEEEEEEEEEEEEEEEEEEEEE
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
		LEDmatrix[2][R]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		case 10 ://KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
		AllLEDsOff();//1
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();//2
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//3
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//4
		LEDmatrix[4][R]=300;
		LEDmatrix[3][R]=300;
//		LEDmatrix[2][B]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//5
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
		LEDmatrix[2][R]=300;
//		LEDmatrix[1][B]=300;
		PauseBy(daDelay);
		AllLEDsOff();//6
		LEDmatrix[4][R]=300;
//		LEDmatrix[3][B]=300;
//		LEDmatrix[2][B]=300;
		LEDmatrix[1][R]=300;
		PauseBy(daDelay);
		AllLEDsOff();
		PauseBy(daDelay);
		break;

		default :
		daLetterCount=0;
//		daLetterCount=1;
	}
	daLetterCount++;
}
void COMMUNICATOR(void)
{
//	WriteByteToEeprom(3);
	COMMUNICATOR_ON;
	PauseBy(10);
	COMMUNICATOR_OFF;
	for (int x=1; x <=2; ++x)
	{
		LEDmatrix[1][B]=0;
		LEDmatrix[2][B]=0;
		LEDmatrix[3][B]=0;
		LEDmatrix[4][B]=0;
		PauseBy(30);
		LEDmatrix[1][R]=50;//Blink Red
		LEDmatrix[2][R]=50;
		LEDmatrix[3][R]=50;
		LEDmatrix[4][R]=50;
		PauseBy(10);
		LEDmatrix[1][R]=0;
		LEDmatrix[2][R]=0;
		LEDmatrix[3][R]=0;
		LEDmatrix[4][R]=0;
		PauseBy(10);
		LEDmatrix[1][B]=0;
		LEDmatrix[2][B]=0;
		LEDmatrix[3][B]=0;
		LEDmatrix[4][B]=0;
		PauseBy(30);
		LEDmatrix[1][R]=50;//Blink Red
		LEDmatrix[2][R]=50;
		LEDmatrix[3][R]=50;
		LEDmatrix[4][R]=50;
		PauseBy(10);
		LEDmatrix[1][R]=0;
		LEDmatrix[2][R]=0;
		LEDmatrix[3][R]=0;
		LEDmatrix[4][R]=0;
		PauseBy(8);
	}
}
void PHOTONTORPEDO(void)
{
	int PauseDelay=40;
	PhotonTorpedoFirstSteps(PauseDelay);
	PauseBy(PauseDelay);
	if(RightButtonIsPushed(PauseDelay))
	{
		//You did It!
		//StageOne();
		//You did It!
		RewardSequence();
	}
	else
	{
		PhotonTorpedoLastSteps(PauseDelay);
	}
}
void INTERCOM(void)
{
	INTERCOM_ON;
	PauseBy(10);
	INTERCOM_OFF;
	for (int x=1; x <= 4; ++x)
	{
	LEDmatrix[1][R]=1;
	LEDmatrix[2][R]=1;
	LEDmatrix[3][R]=1;
	LEDmatrix[4][R]=1;
	LEDmatrix[x][R]=300;
	PauseBy(20);
	}
}
void TRICORDER(void)
{
	TRICORDER_ON;
	PauseBy(10);
	TRICORDER_OFF;
	int xR=0; //intensity
	int xG=5;
	int xB=50;
	int yR=10; //step
	int yG=20;
	int yB=30;
	int zR=4; //column
	int zG=1;
	int zB=2;
	int	InnerLooper=0;
	const int xMax=300;
	const int zMax=5;
	const int InnerLooperMax=10;

	for (int x=0; x <=650; ++x)
	{
		++InnerLooper; if (InnerLooper>=InnerLooperMax) InnerLooper=0;
		xR=xR+yR; if (xR>=xMax) xR=0;	
		xG=xG+yG; if (xG>=xMax) xG=0;	
		xB=xB+yB; if (xB>=xMax) xB=0;	
		if (InnerLooper>=(InnerLooperMax-1))
		{
			//Inner Loop
			++zR; if (zR>=zMax) zR=1;	
			--zG; if (zG<=0) zG=(zMax-1);	
			++zB; if (zB>=zMax) zB=1;
			//SetAll4(R, 300);
			//PauseBy(5);
			//SetAll4(R, 0);
			//PauseBy(1);
		}
	LEDmatrix[zR][R]=xR;
	LEDmatrix[zG][G]=xG;
	LEDmatrix[zB][B]=xB;
	PauseBy(1);
	AllLEDsOff();
	}
}
void DOOR(void)
{
	DOOR_ON;
	int y=100;
	int z=150;
	for (int x=0; x <= y; ++x)
	{
		LEDmatrix[1][G]=x;
		LEDmatrix[2][G]=x;
		LEDmatrix[3][G]=x;
		LEDmatrix[4][G]=x;
		PauseBy(1);
	}
	SetAll4(B, z);
	SetAll4(R, z);
	PauseBy(200);
	SetAll4(B, 0);
	SetAll4(R, 0);

	for (int x=y; x >= 0; x--)
	{
		LEDmatrix[1][G]=x;
		LEDmatrix[2][G]=x;
		LEDmatrix[3][G]=x;
		LEDmatrix[4][G]=x;
		PauseBy(1);
	}
	DOOR_OFF;
}

void MakeTriangleWave(void)
{
	//trianglewave[0 to 100] ranges
	int y=0;
	for (int x=0; x <= 50; ++x)
	{
		trianglewave[x]=x*2;
	}
	for (int x=100; x >= 50; --x)
	{
		trianglewave[x]=y*2;
		++y;
	}
}
