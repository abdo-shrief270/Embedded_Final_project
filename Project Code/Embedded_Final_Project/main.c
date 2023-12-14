/*
 * Embedded_Final_Project.c
 *
 * Created: 11/16/2023 5:05:14 PM
 * Author : Ali Weeka
 */ 

// Include basic libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Include basic drivers
#include "Types.h"
#include "BIT_MATH.h"

// Include other drivers
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"
#include <stdbool.h>

// Define Micro-controller's frequency = 16MHz

#define F_CPU 16000000UL

// Include delay library
#include <util/delay.h>

// Declaring our global variables //
u8 BRAIN_TEST_OPTION ;
u8 BRAIN_TEST_RAND_OPTION ;
u8 tries;
////////////////////////////

void showLogicEquation(u8 num1[], u8 num2[], const char* gateName);
void performLogicGate(u8 gateType, u8 num1[], u8 num2[], u8 result[]);
// Prototypes of sum function will be used
void ENTRY_MODE(void);
void Init(void);
u8 getFromKeypad(void);
void ERROR_CHOICE(void);
void BRAIN_TEST(void);
void BRAIN_TEST_RANDOM(void);
void BRAIN_TEST_SPECIFIC(void);
void BRAIN_TEST_ARTH(void);
void BRAIN_TEST_LOGIC(void);
void BRAIN_TEST_CONV(void);
void CALCULATOR(void);
void END_CALCULATOR(s64 result);
void CONVERTER(void);
void CONVERTER_DEC_TO_BIN();
void CONVERTER_BIN_TO_DEC();
void ArthWrongAnswer(u32 res);
void LogicWrongAnswer(u8* res);
unsigned int binToDec(char *binary);
void decToBin(unsigned int decimal, char *binary);
int myPow(char n,char p);
u32 arrToInt(char* arr,char i);
u8 triesCount(u8 tries);



int main(void)
{
	// Initialization //
	DIO_vidSetPinMode(GPIOC,PIN0,OUTPUT);
	DIO_vidWritePin(GPIOC,PIN2,LOW);
	Init();
	LCD_voidDisplayString4bit("   Welcome :)   ");
	_delay_ms(500);
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("This Project is ");
	LCD_vidMoveCursor4bit(2,1);
	LCD_voidDisplayString4bit("Made By ASMAK :)");
	_delay_ms(1000);
	entry:ENTRY_MODE();
	u8 mode =getFromKeypad();
	
	switch(mode) {
		case '1' :
			BRAIN_TEST();
			break;
		case '2' :
			CALCULATOR();
			break;
		case '3' :
			CONVERTER();
			break;
		default :
			ERROR_CHOICE();
			goto entry;
			break;	
	}
	goto entry;
}


// Converter Functions

void CONVERTER(void){
	u8 conv_m;
	CONV_MODE :
	LCD_vidClearScreen4bit();
	LCD_vidMoveCursor4bit(1,1);
	LCD_voidDisplayString4bit("1_ Dec --> Bin ");
	LCD_vidMoveCursor4bit(2,1);
	LCD_voidDisplayString4bit("2_ Bin --> Dec ");
	conv_m = getFromKeypad();
	switch (conv_m){
		case '1' :
		CONVERTER_DEC_TO_BIN();
		break;
		case '2' :
		CONVERTER_BIN_TO_DEC();
		break;
		default:
		ERROR_CHOICE();
		goto CONV_MODE;
		break;
	}
}


void CONVERTER_DEC_TO_BIN()
{
	conv_dec_to_bin:
	LCD_vidClearScreen4bit();
	u8 numRes[5];
	u8 SEL;
	u8 i=0,counter=0;
	u16 num_dec;
	u8 num_bin[16];
	while(i <= 7)
	{
		
		SEL=getFromKeypad();
		if(SEL == '=')
		{
			numRes[i] = '\0';
			LCD_vidClearScreen4bit();
			break;
		}else if(SEL == 'C'){
			LCD_vidClearScreen4bit();
			 
			_delay_ms(500);
			return;
		}
		else
		{
			LCD_vidDisplayChar4bit(SEL);
			numRes[i] = SEL;
			counter++;
		}
		i++;
	}

	
	num_dec=arrToInt(numRes,counter);
	decToBin(num_dec,num_bin);
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Result in Bin ->");
	LCD_vidMoveCursor4bit(2,1);
	LCD_voidDisplayString4bit(num_bin);
	_delay_ms(2000);
	goto conv_dec_to_bin;
	
}


void CONVERTER_BIN_TO_DEC(){
	
	conv_bin_to_dec:
	LCD_vidClearScreen4bit();
	u8 SEL;
	u8 i=0,counter=0;
	s64 num_dec;
	u8 num_bin[17];
	while(i <= 15)
	{
		
		get_Bin_val :
		SEL=getFromKeypad();
		if (!(SEL == '0' || SEL == '1'))
		{
			if (SEL=='C')
			{
				BRAIN_TEST_RAND_OPTION=1;
				LCD_vidClearScreen4bit();
				 
				_delay_ms(500);
				return;
			}else if(SEL == '='){
				
				num_bin[i] = '\0';
				LCD_vidClearScreen4bit();
				break;
				
			}
			goto get_Bin_val;
		}

		LCD_vidDisplayChar4bit(SEL);
		num_bin[i] = SEL;
		counter++;
		
		i++;
		if (i==16)
		{
			num_bin[i]='\0';
		}
	}

	num_dec=binToDec(num_bin);
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Result in Dec ->");
	LCD_vidMoveCursor4bit(2,1);
	LCD_vidDisplayNumber4bit(num_dec);
	_delay_ms(2000);
	goto conv_bin_to_dec;
}


// Calculator Functions

void CALCULATOR(void)
{
	beginCAL :
	LCD_vidClearScreen4bit();
	u8 i=0,SEL,numChr[16],opr='F';
	s64 result=0;
	getEQ :
	i=0;
	SEL=getFromKeypad();
	while (!(SEL=='+' || SEL=='-' || SEL=='*' || SEL=='%' || SEL == 'C' || SEL == '='))
	{
		
		LCD_vidDisplayChar4bit(SEL);
		numChr[i]=SEL;
		i++;
		SEL=getFromKeypad();
	}
	numChr[i]='\0';
		switch (opr) {
			case '+' :
			result +=arrToInt(numChr,i);
			break;
			case '-' :
			result -=arrToInt(numChr,i);
			break;
			case '*' :
			result *=arrToInt(numChr,i);
			break;
			case '%' :
			result %=arrToInt(numChr,i);
			break;
			default:
			result=arrToInt(numChr,i);
			break;
		}
		
	opr=SEL;

	switch (SEL) {
		case '=' :
			END_CALCULATOR(result);
			goto beginCAL;
			break;
		case 'C' :
			LCD_vidClearScreen4bit();
			 
			_delay_ms(500);
			break;
		default :
			LCD_vidClearScreen4bit();
			LCD_vidDisplayRealNumber4bit(result);
			LCD_vidDisplayChar4bit(opr);
			goto getEQ;
			break;
	}
	
}

void END_CALCULATOR(s64 result){
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Result is");
	LCD_vidMoveCursor4bit(2,1);
	LCD_vidDisplayRealNumber4bit(result);
	_delay_ms(1000);
}


// Brain Test Functions

void BRAIN_TEST(){
	
	tries = 3;
	DIO_vidWriteLowNibble(GPIOA,tries);
	Brain_Test:
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Choose a mode");
	LCD_vidMoveCursor4bit(2,1);
	LCD_voidDisplayString4bit("1-rand 2-spec");
	u8 mode1 =getFromKeypad();
	switch(mode1) {
		case '1' :
			BRAIN_TEST_OPTION=1;
			BRAIN_TEST_RANDOM();
			break;
		case '2' :
			BRAIN_TEST_OPTION=0;
			BRAIN_TEST_SPECIFIC();
			break;
		case 'C' :
			return;
			break;
		default :
			ERROR_CHOICE();
			goto Brain_Test;
			break;
		
	}	
}

void BRAIN_TEST_SPECIFIC(){
	u8 eq_mode;
	getMode :
	LCD_vidClearScreen4bit();
	LCD_vidMoveCursor4bit(1,1);
	LCD_voidDisplayString4bit(" Choose Option : ");
	LCD_vidMoveCursor4bit(2,1);
	LCD_voidDisplayString4bit("1_ Logic 2_ Arth");
	eq_mode = getFromKeypad();
	switch (eq_mode){
		case '1' :
			BRAIN_TEST_LOGIC();
			break;
		case '2' :
			BRAIN_TEST_ARTH();
			break;
		default:
			ERROR_CHOICE();
			goto getMode;
		break;
	}
}

	
void BRAIN_TEST_RANDOM()
{
	u8 eq_mode;
	
	brain_test_rand:
	eq_mode = rand()%2;
	LCD_vidClearScreen4bit();
	switch (eq_mode){
		case 0 :
			BRAIN_TEST_RAND_OPTION=0;
			BRAIN_TEST_ARTH();
			if(!BRAIN_TEST_RAND_OPTION){
				goto brain_test_rand;
			}
			break;
		case 1 :
			BRAIN_TEST_RAND_OPTION=0;
			BRAIN_TEST_LOGIC();
			if(!BRAIN_TEST_RAND_OPTION){
				goto brain_test_rand;
			}
			break;
		default:
			break;
	}
	
}

void disBinNum(u8* num)
{
	for (u8 i = 0 ; i<4 ; i++)
	{
		LCD_vidDisplayNumber4bit(num[i]);
	}
	
}


void performLogicGate(u8 gateType, u8 num1[], u8 num2[], u8 result[]) {
	u8 i;
	for (i = 0; i < 4; i++) {
		switch (gateType) {
			case 0: // AND
			result[i] = num1[i] && num2[i];
			break;
			case 1: // OR
			result[i] = num1[i] || num2[i];
			break;
			case 2: // NAND
			result[i] = !(num1[i] && num2[i]);
			break;
			case 3: // NOR
			result[i] = !(num1[i] || num2[i]);
			break;
			case 4: // XOR
			result[i] = num1[i] ^ num2[i];
			break;
			case 5: // XNOR
			result[i] = !(num1[i] ^ num2[i]);
			break;
			default:
			break;
		}
	}
	
}

void showLogicEquation(u8 num1[], u8 num2[], const char* gateName){
	
	LCD_vidClearScreen4bit();
	disBinNum(num1);
	LCD_voidDisplayString4bit(" AND ");
	disBinNum(num2);
	LCD_vidMoveCursor4bit(2,1);

}

void getLogicResult(u8 result[],u8 * cond){
	u8 SEL;
	for (u8 i = 0 ; i<4 ; i++)
	{
		getBinAND :
		SEL=getFromKeypad();
		if (!(SEL == '0' || SEL == '1'))
		{
			if (SEL=='C')
			{
				BRAIN_TEST_RAND_OPTION=1;
				LCD_vidClearScreen4bit();
				_delay_ms(500);
				return;
			}
			goto getBinAND;
		}
		if(result[i]!=SEL-48)
		{
			*cond=FALSE;
		}
		LCD_vidDisplayChar4bit(SEL);
	}
	LCD_vidClearScreen4bit();
	LCD_vidMoveCursor4bit(1,1);

}

void BRAIN_TEST_LOGIC()
{
	LCD_vidClearScreen4bit();
	LCD_vidMoveCursor4bit(1,1);
	u8 temp,i,num1[4],num2[4],res[4],SEL,REAL_RES[4],count;
	Brain_Test_rand_logic:
	tries = 3;
	DIO_vidWriteLowNibble(GPIOA,tries);
	LCD_vidClearScreen4bit();
	LCD_vidMoveCursor4bit(1,1);
	temp = rand()%6;
	bool cond= true;
	i = 0;
	while (i<4)
	{
		num1[i] = rand()%2;
		num2[i] = rand()%2;
		i++;
	}
	
	// logic modes
	if(temp==0)    //AND
	{
		
		performLogicGate(0,num1,num2, REAL_RES);
		tryAND:
		
		cond = TRUE;
		showLogicEquation(num1,num2," AND ");
		getLogicResult(REAL_RES,&cond);
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
		}else{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();
			goto tryAND;
		}
		
	}
	else if (temp == 1)     //OR
	{
		
		performLogicGate(1,num1,num2, REAL_RES);
		tryOR:
		cond = TRUE;
		showLogicEquation(num1,num2," OR ");
		getLogicResult(REAL_RES,&cond);
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
			}else{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();

			goto tryOR;
		}
	}
	else if (temp == 2)     //NAND
	{
		
		performLogicGate(2,num1,num2,REAL_RES);
		tryNAND:
		cond = TRUE;
		showLogicEquation(num1,num2," NAND ");
		getLogicResult(REAL_RES,&cond);
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
			}else{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();

			goto tryNAND;
		}
	}
	else if (temp == 3)     //NOR
	{
		
		performLogicGate(3,num1,num2,REAL_RES);
		tryNOR:
		cond = TRUE;
		showLogicEquation(num1,num2," NOR ");
		getLogicResult(REAL_RES,&cond);
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
		}
		else
		{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();
			goto tryNOR;
		}
	}
	else if (temp == 4)     //XOR
	{
		
		performLogicGate(4,num1,num2,REAL_RES);
		tryXOR:
		cond = TRUE;
		showLogicEquation(num1,num2," XOR ");
		getLogicResult(REAL_RES,&cond);
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
			}else{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();

			goto tryXOR;
		}
	}
	else                    //XNOR
	{
		
		performLogicGate(5,num1,num2,REAL_RES);
		tryXNOR:
		cond = TRUE;
		showLogicEquation(num1,num2," XNOR ");
		getLogicResult(REAL_RES,&cond);
		
		if (cond)
		{
			LCD_voidDisplayString4bit(" Correct Ans :) ");
			DIO_vidWriteLowNibble(GPIOA,tries);
			}else{
			count = triesCount(tries);
			if(count == 0)
			{
				LogicWrongAnswer(REAL_RES);
				_delay_ms(1000);
				goto Brain_Test_rand_logic;
			}
			_delay_ms(500);
			LCD_vidClearScreen4bit();

			goto tryXNOR;
		}
	}
	_delay_ms(1000);
	if(BRAIN_TEST_OPTION)
	{
		return;
	}
	goto Brain_Test_rand_logic;
}

void BRAIN_TEST_ARTH() {
	// Clear the LCD screen
	LCD_vidClearScreen4bit();
	
	// Define variables
	u8 num1, num2, i = 0, *oprand, numRes[8], count, temp;
	s16 SEL, counter, POW;
	u16 sum;
	s16 res;

	// Label to allow going back to generate new numbers
	Brain_Test_rand_arth:
	tries = 3; // Number of tries allowed
	DIO_vidWriteLowNibble(GPIOA, tries);
	num1 = rand() % 101; // Random number generation for operand 1
	num2 = rand() % 101; // Random number generation for operand 2
	temp = rand() % 4; // Random operation (+, -, *, %)

	// Select the operation based on the generated random number
	if (temp == 0) {
		oprand = " + ";
		res = num1 + num2;
		} else if (temp == 1) {
		oprand = " - ";
		if (num1 < num2) {
			num1 = num1 + num2;
			num2 = num1 - num2;
			num1 = num1 - num2;
		}
		res = num1 - num2;
		} else if (temp == 2) {
		oprand = " * ";
		res = num1 * num2;
		} else {
		num1 = abs(num1);
		num2 = abs(num2);
		oprand = " % ";
		res = num1 % num2;
	}

	// Display the arithmetic operation on the LCD
	try:
	LCD_vidClearScreen4bit();
	LCD_vidDisplayNumber4bit(num1);
	LCD_voidDisplayString4bit(oprand);
	LCD_vidDisplayNumber4bit(num2);
	LCD_vidMoveCursor4bit(2, 1);
	LCD_voidDisplayString4bit("= ");

	// Initialize variables for input and calculation
	i = 0;
	counter = 0;
	sum = 0;

	// Get input from the user
	while (i <= 7) {
		SEL = getFromKeypad();
		if (SEL == '=') {
			numRes[i] = '\0'; // Null-terminate the input array
			LCD_vidClearScreen4bit();
			break;
		} else if (SEL == 'C') {
			BRAIN_TEST_RAND_OPTION = 1; // Return to menu if 'C' is pressed
			LCD_vidClearScreen4bit();
			_delay_ms(500);
			return;
			} else {
			LCD_vidDisplayChar4bit(SEL);
			numRes[i] = SEL;
			counter++;
		}
		i++;
	}

	// Convert input to integer for comparison
	sum = arrToInt(numRes, counter);

	// Check if the sum matches the result
	if (sum == res) {
		LCD_vidClearScreen4bit();
		LCD_voidDisplayString4bit("Correct Answer:)");
		} else {
		count = triesCount(tries);
		if (count == 0) {
			ArthWrongAnswer(res);
			_delay_ms(1000);
			goto Brain_Test_rand_arth;
		}
		_delay_ms(500);
		LCD_vidClearScreen4bit();
		goto try; // Try again if the answer is incorrect
	}

	_delay_ms(1000);
	if (BRAIN_TEST_OPTION) {
		return; // Return to menu if option is chosen
	}
	goto Brain_Test_rand_arth;
}
// GLOBAL FUNCTIONS USED IN ALL MODES

void ArthWrongAnswer(u32 res){
		
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Wrong Answer:(");
	DIO_vidWritePin(GPIOA,PIN4,HIGH);
	_delay_ms(500);
	DIO_vidWritePin(GPIOA,PIN4,LOW);
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Correct Ans is");
	LCD_vidMoveCursor4bit(2,1);
	LCD_vidDisplayNumber4bit(res);
	
}


void LogicWrongAnswer(u8* res){
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("  Wrong Ans :(  ");
	DIO_vidWritePin(GPIOA,PIN4,HIGH);
	_delay_ms(500);
	DIO_vidWritePin(GPIOA,PIN4,LOW);
	LCD_vidClearScreen4bit();
	LCD_voidDisplayString4bit("Correct Ans is");
	LCD_vidMoveCursor4bit(2,1);
	for (u8 i = 0 ; i<4 ; i++)
	{
		LCD_vidDisplayNumber4bit(res[i]);
	}

}


unsigned int binToDec(char *binary) {
	unsigned int decimal = 0;
	for (int i = 0; binary[i]!='\0'; i++) {
		decimal = decimal * 2 + (binary[i] - '0');
		LCD_vidClearScreen4bit();
		LCD_vidDisplayNumber4bit(decimal);
		_delay_ms(500);
	}
	return (unsigned)decimal;
}

void decToBin(unsigned int decimal, char *binary) {
	int index = 0;
	while (decimal > 0) {
		binary[index++] = decimal % 2 + '0';
		decimal /= 2;
	}
	binary[index] = '\0';
	strrev(binary);
}

u8 getFromKeypad(){
		u8 SEL;
		do
		{
			SEL = KPD_u8GetPressedKey(GPIOB,LOW_NIBBLE,HIGH_NIBBLE);
		} while (SEL == 0xFF);
		while (KPD_u8GetPressedKey(GPIOB,LOW_NIBBLE,HIGH_NIBBLE) == SEL);
		_delay_ms(100);
		return SEL;
}

u32 arrToInt(char* arr,char i){

	u32 num=0;
	char j=i-1;
	for (j ; j>=0 ; j--){
		if(arr[j]=='\0'){
			break;
		}
		
		num+= (arr[j]-48) *(myPow(10,i-j-1));
	}
	return num;

}

int myPow(char n,char p)
{
	int num=1;
	char i=0;
	for(i = 0 ; i < p ; i++)
	{
		num *=n;
	}
	return num;
}

void ERROR_CHOICE(){
	LCD_vidClearScreen4bit( );
	LCD_voidDisplayString4bit( "Choose valid one");
	_delay_ms(500);
	LCD_vidClearScreen4bit( );
}

// MAIN FUNCTIONS

void ENTRY_MODE(void)
{
	LCD_vidClearScreen4bit( );
	LCD_voidDisplayString4bit( "Choose a Mode :");
	LCD_vidMoveCursor4bit( 2,1);
	_delay_ms(100);
	LCD_voidDisplayString4bit("1_BT 2_Cal 3_Con");
}

void Init(void)
{
	srand(time(NULL));
	LCD_vidInit4bit();
	KPD_vidInit(GPIOB,LOW_NIBBLE,HIGH_NIBBLE);
	DIO_vidSetPinMode(GPIOC,PIN0,OUTPUT);
	DIO_vidWritePin(GPIOC,PIN0,LOW);
	DIO_vidSetPinMode(GPIOA,PIN4,OUTPUT);
	DIO_vidWritePin(GPIOA,PIN4,LOW);
	DIO_vidSetLowNibbleMode(GPIOA,OUTPUT);
}

u8 triesCount(u8 try)
{

    u8 returnVal = 1;
	tries--;
    DIO_vidWriteLowNibble(GPIOA,--try);
    if (try == 0)
    {
    	LCD_voidDisplayString4bit("You have no ");
    	LCD_vidMoveCursor4bit(2,1);
    	LCD_voidDisplayString4bit("tries left");
    	_delay_ms(500);
    	tries = 3;
        DIO_vidWriteLowNibble(GPIOA,tries);
    	returnVal = 0;
    }else{
		LCD_voidDisplayString4bit("Try Again !");
		DIO_vidWritePin(GPIOA,PIN4,HIGH);
		_delay_ms(500);
		DIO_vidWritePin(GPIOA,PIN4,LOW);
	}
    return returnVal;
}
