/*
 * Convert32.c
 * 
 * Copyright 2014 Ricky
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdint.h>

int i; //used in for loops as iterator or a counter

/***Function Prototyping***/
void parseInput(const char* str);
void parseHex(const char* str);
void printHex(); 
void parseBin(const char* str);
void printBin();
void parseFloat(const char* str);
void printFloat();
void parseInt(const char* str);
void printInt();
void printParsedInput();
/*** End Function Prototyping***/
int32_t parsedInput = 0; 						//since this is a 32bit program, use an int32. This is where the input will be stored after it is parsed.

int main(int argc, char **argv)
{
	printf("Input: %s\n", argv[1]);			//print what the input is 
	parseInput(argv[1]);						//parse the input
	printParsedInput();							//print it in the different formats specified
	return 0;
}

void parseInput(const char* s){
	//This function is used to determine whehter the input is binary, float, hex, or int
	void (*parsingFunction)(const char* str);	//create a pointer to a function that that will further parse the string after it determins the string type
	parsingFunction = NULL;						//make the parsing function point to nothing
	
	const char* str = s;						//put s into a temporary variable str
	
	
	if(str[0] == '0'){							//a binary or hex will always start with a 0, if it does, check the second character for x, X, b, or B to determine whether to parse it as a hex or a binary
		if(str[1] == 'x' || str[1] == 'X'){
				parsingFunction = &parseHex;	//have the parsing funciton point to the address of the parseHex function because it determined the input was a hex string
		}else if(str[1] == 'b' || str[1] == 'B'){
			parsingFunction = &parseBin;		//have the parsing funciton point to the address of the parseBin function because it determined the input was a binary string
		}
	}
	
	if(!parsingFunction){						//checks whether a parsing function is defined. if not check the string for a decimal point									
		for(;*str != '\0';str++){				//loop through every character on the string and check for a decimal point	
			if(*str == '.'){					//if the current letter is decimal point	
				parsingFunction = &parseFloat;	//then set the parsing funciton to the parseFloat
			}	
		}
	}
	
	if(!parsingFunction){						//checks whether a parsing function is defined. if not
		parsingFunction = &parseInt;			//then set the parsing funciton to parseInt because we have no other options left	
	}
		
	parsingFunction(s);							//run the parsing function
}


void parseHex(const char* str){
	sscanf(str, "%x", &parsedInput);			//scan the string for a hex value and put it into the parsedInput variable	
}

void parseBin(const char* str){
	str++; str++; 								//move pointer twice to get rid of 0b

	for(;*str != '\0'; str++){					//loop until the end of the string
		int binVal = (int)*str - 48;			//the ascii value of 1 is 49 and 0 is 48, so we substract 48 to get a 1 or a 0 in the place	
		
		parsedInput <<= 1;						//shift the parsed input right by 1	
		
		if(binVal & 1){							//check if least significant bit is 1
			parsedInput++;						//if it is, then add 1 to parsed input
		}
	}
}

void parseFloat(const char* str){
	float temp;									//create a temporay variable to scan into
	sscanf(str, "%f", &temp);					//scan a float from str into the temporary variable
	parsedInput = *(int32_t*)&temp;			//cast the address of temp to be a pointer that points to an int32_t (instead of a float). essentially converts float to int32_t without changing bits
}

void parseInt(const char* str){
	sscanf(str, "%d", &parsedInput);			//scan an int into parsed int
}

void printParsedInput(){
	//This is the function where all of the printed values are called, formatted, and printed
	printBin();									
	
	printHex();
	
	printFloat();
	
	printInt();
}

void printBin(){
	printf("binary: 0B");								//print the formatting for binary
	
	int32_t temp = parsedInput;						//set temp = to parsed input because a shift operation is used and we don't want to modify parsedInput because it is used later	

	/*
	 * Loop through temp 
	 * it chekcks if most significant bit is 1
	 * if it is print a 1, if not print a 0
	 * then shift temp to the right
	 * This also adds 0 padding since it loops through the size of int32_t (32bits)
	 */
	for(i = 0; i < sizeof(temp) * 8; i++){
		if (temp & 0x80000000){
			printf("1");
		}else{
			printf("0");
		}
		temp <<= 1;
	}
	printf("\n");										//print a new line	
}

void printHex(){
	printf("hex: 0x%X\n", parsedInput);					//print the formatted hex value
}

void printFloat(){
	printf("float: %f\n", *(float*)&parsedInput);		//cast the address of parsedInput to be a pointer that points to a float (instead of a int32_t). essentially converts int32_t to float without changing bits, then print the float value
}

void printInt(){
	printf("int: %d", parsedInput);						//print the integer value of parsed input
}
