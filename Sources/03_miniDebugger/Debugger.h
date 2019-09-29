/**
 *  Proj5_debugger by Reno.
 *  Debugger.h
 *
 *  Source file created on 22/09/2019.
 *  (c) All rights reserved.
 */

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <Arduino.h>

typedef enum : byte 
{
    DebugStateContinue,
    DebugStatePause,
    DebugStateStopNext
} DebugState;

extern String       _dbgBuffer;
extern uint16_t   _dbgCurrentLine;
extern String     _dbgCurrentFile;
extern uint16_t     _dbgLastSP;
extern uint32_t   _dbgReturnAddress;
extern DebugState     _dbgState;
extern uint32_t       _dbgStopMin;
extern uint32_t       _dbgStopMax;

//extern uint16_t   _dbgStartFrame;
extern uint8_t      _dbgCurrentFrameSize;       

// Prototypes:

void    DebugInterpretCommand(String command);
uint32_t  DebugGetPC(uint16_t spValue, uint16_t previousSpValue);
void    DebugBP(uint16_t spValue, uint16_t previousSpValue, String file, uint16_t line);
void    DebugPrintVariable(uint16_t address, int len);
void    DebugSerialRead();
uint32_t  StrHexToInt(String value);

/* Obtient le nombre d'octets du PC suivant le modèle de carte */
#ifdef __AVR_3_BYTE_PC__
  #define DBG_RET_SIZE     3
#elif defined(__AVR_2_BYTE_PC__)
  #define DBG_RET_SIZE     2
#else
  #define DBG_RET_SIZE    0  /* on pourrait retourner une erreur ici */
#endif

#define BP()    (DebugBP(SP, DBGStackStart, __FILE__, __LINE__))
#define STACKLEN()   __asm__ __volatile__("ldi %0, .L__stack_usage" : "=r"(_dbgCurrentFrameSize))
#define STACKGETSTART()  uint16_t DBGStackStart = SP + _dbgCurrentFrameSize + DBG_RET_SIZE

#endif


