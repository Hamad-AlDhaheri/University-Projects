/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>

union u{
    int binaryVal;
    float floatVal;

};

int main(int argc, char* argv[])
{
//variables
char mode[1];
float flo;
char bin[32];
//unsigned int mantissaB;
int mantissa;
unsigned int bVal;
int sign;
unsigned int exponent;
//float value;
union u floatCast;
float mantissaFloat = 1.0f;
    //retrieve arguments
    strcpy(mode, argv[1]);
    
    if(strcmp(mode, "f") == 0){
        flo = strtof(argv[2], NULL) ; 
        
        
        floatCast.floatVal = flo;
        //printf("%d", floatCast.binaryVal);
        
        printf("Binary: ");
        
        for (int i = 31; i >= 0; i--){
            unsigned int mask = (1 << i);
            if(floatCast.binaryVal & mask ){
                printf("1");
            } else { 
                printf("0");
            }
        }
        
        
        sign = floatCast.binaryVal >> 31;
        if(sign == 0){
            sign = 0;
        } else {
            sign = 1;
        } // print2
        
        printf("\nsign: %d", sign);
        
        exponent = (((floatCast.binaryVal >> 23) - 127) & 0xFF);
        printf("\nExponent: ");
        printf("%d", exponent);
        
        
        
        
        
        
        // mantissaB = ((floatCast.binaryVal) & 0x7FFFFF);
        //     int j = -1;
        //     for(int i = 22; i > 0; i--){
        //     mantissa = mantissa + (((mantissaB >> i) & 1) * pow(2, j));
        //     //printf("%d", j);
        //     j = j - 1;
        //     } //print4
            
             mantissa = floatCast.binaryVal & 0x7FFFFF;

             for(int i = 0; i < 23; i++){
                mantissaFloat += ((mantissa >> (22-i)) & 1) * (1.0f / (1 << (i + 1)));
             }

        printf("\nmantissa: %f", mantissaFloat);
        
        printf("\nValue: %.7g", floatCast.floatVal);
      
      
    } else if (strcmp(mode, "b") == 0){
        
    strcpy(bin, argv[2]); //print1
    
    bVal = atoi(argv[2]);
    
    
    floatCast.binaryVal = bVal;
            
    
     sign = bVal >> 31; 
        if(sign == 0){
            sign = 0;
        } else {
            sign = 1;
        } // print2
    
     exponent = (((bVal >> 23) & 0xFF)-127); //print3
        
        
            // mantissaB = ((bVal) & 0x7FFFFF);
            // int j = -1;
            // for(int i = 22; i > 0; i--){
            // mantissa = mantissa + (((mantissaB >> i) & 1) * pow(2, j));
            // //printf("%d", j);
            // j = j - 1;
            // } //print4

           // mantissa = bVal & 0x7FFFFF;
            //value = mantissa;

            // for(int i = exponent; i >= 0 ; i--){
            //     value = value * value;
            // }

            mantissa = bVal & 0x7FFFFF;

             for(int i = 0; i < 23; i++){
                mantissaFloat += ((mantissa >> (22-i)) & 1) * (1.0f / (1 << (i + 1)));
             }
            
    printf("Binary: %s\n", bin);
    printf("Sign: %d\n", sign);
    printf("Exponent: %d\n", exponent);
    printf("Mantissa: %f\n", mantissaFloat);
    printf("Value: %.7g\n", floatCast.floatVal);
    
    }
    
    
    
    
    
        
    //find mantissa
    
    
    
    
    //test print line
   // printf("%f", mantissa);

    return 0;
}


