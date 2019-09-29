#include <Arduino.h>

#define DBG_RET_SIZE 	2
#ifdef __AVR_3_BYTE_PC__
  #define  DBG_RET_SIZE     3
#elif defined(__AVR_2_BYTE_PC__)
  #define  DBG_RET_SIZE     2
#endif

// Prototypes:
int FuncTest(int a, int b);
void PrintVariable(uint16_t address, int len);


void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int x = FuncTest(4, 5);
    
    delay(2000);
}

int FuncTest(int a, int b)
{
    // début : on stocke l'adresse de départ:
    uint8_t stack_usage;
    __asm__ __volatile__("ldi %0, .L__stack_usage" : "=r"(stack_usage));
    uint16_t topSP = SP + stack_usage + DBG_RET_SIZE;
    
    int ret = a + b;
    // TODO: actions...    
    PrintVariable(SP, topSP - SP + 1);
    
    return ret;
}

void PrintVariable(uint16_t address, int len) 
{
    // On place un pointeur à l'adresse:
    uint8_t* pointer = (uint8_t*)address; 
    for (int i = 0 ; i < len ; i+= 16)
    {
        for (int j = 0 ; j < 16 && i + j < len ; j++)
        {
            uint8_t val = *(pointer+i+j); // valeur d'1 octet.
            Serial.print( (val < 16 ? "0" : "") + String(val, HEX) + " ");
        }
        Serial.println(""); // CRLF;
    } 
    Serial.println(""); // CRLF
}

