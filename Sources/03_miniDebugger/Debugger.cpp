/**
 *  Proj5_debugger by Reno.
 *  Debugger.cpp.cpp
 *
 *  Source file created on 22/09/2019.
 *  (c) All rights reserved.
 */

#include <Arduino.h>

#include "Debugger.h"

String      _dbgBuffer = "";
uint16_t    _dbgCurrentLine = 0;
String      _dbgCurrentFile = "";
uint32_t    _dbgReturnAddress = 0;
DebugState    _dbgState = DebugStateContinue;
uint32_t    _dbgStopMin = 0;
uint32_t    _dbgStopMax = 0;
uint8_t     _dbgCurrentFrameSize = 0;

// Fonctions


void DebugInterpretCommand(String command)
{
    int pos = command.indexOf(' ');
    String part1 = (pos < 0) ? command : command.substring(0, pos);
    String part2 = (pos < 0 || pos >= command.length()) ? "" : command.substring(pos + 1);
    
    if (part1.compareTo(F("state")) == 0)
    {
        String state = "en execution";
        switch (_dbgState)
        {
            case DebugStatePause:
                state = "en pause";
              break;
                
            case DebugStateStopNext:
                state = "arret a la prochaine instruction";
              break;
            
            default:
              break;
        }
        
        if (_dbgStopMax != 0)
        {
            state = String(F("arret à l'instruction [0x")) + String(_dbgStopMin, HEX) + String(F(",0x")) + String(_dbgStopMax, HEX) + String(F("]"));
        }
        
        Serial.println(String(F("Etat: ")) + state);
        
    }
    else if (part1.compareTo(F("stop")) == 0 && part2.compareTo("") == 0)
    {
        _dbgState = DebugStateStopNext;
        _dbgStopMin = 0;
        _dbgStopMax = 0;
        Serial.println(String(F("executed: ")) + command);
        
    }
    else if (part1.compareTo(F("stop")) == 0 && part2.length() > 0)
    {
        pos = part2.indexOf(' ');
        _dbgStopMin = StrHexToInt(part2.substring(0, pos));
        _dbgStopMax = StrHexToInt(part2.substring(pos+1));

        Serial.println("executed: " + command);
    }
    else if (part1.compareTo(F("play")) == 0)
    {
        _dbgState = DebugStateContinue;
        _dbgStopMin = 0;
        _dbgStopMax = 0;
    }
    else if (part1.compareTo(F("next")) == 0)
    {
        _dbgState = DebugStateStopNext;
        _dbgStopMin = 0;
        _dbgStopMax = 0;
    }
    else if (part1.compareTo(F("return")) == 0)
    {
        // obtenir l'adresse de retour (seulement si stoppé):
        if (_dbgState != DebugStatePause)
        {
            Serial.println(F("Le debugger doit être arrete pour RETURN."));
        }
        else
        {
            Serial.println(String(F("Adresse retour: 0x")) + String(_dbgReturnAddress, HEX));
        }
    }
    else
    {
        Serial.println(F("Commande inconnue"));
    }
}

void DebugBP(uint16_t spValue, uint16_t previousSpValue, String file, uint16_t line)
{
    uint32_t pc = DebugGetPC(SP, spValue);
    
    do {
        if (_dbgState == DebugStateStopNext || (pc >= _dbgStopMin && pc <= _dbgStopMax))
        {
            if (_dbgState != DebugStatePause)
            {
                _dbgState = DebugStatePause;
                
                Serial.println(String(F("Stop fichier [")) + file + String(F("] : ")) + String(line));
                Serial.println(String(F("PC = 0x") ) + String(pc, HEX)); 
                
                // Adresse retour:
                _dbgReturnAddress = DebugGetPC(spValue, previousSpValue);
                
                Serial.println(F("Stack frame: "));
                int stackSize = (int)(previousSpValue - spValue + 1);
                DebugPrintVariable(spValue, stackSize);
            }
        }
        DebugSerialRead();
    } while (_dbgState == DebugStatePause);
}

void DebugSerialRead()
{
    while (Serial.available() > 0) {
        char letter = (char)Serial.read();
        if (letter != '\r' && letter != '\n')
        {
            _dbgBuffer += String(letter);
        }
        else if (letter == '\n')
        {
            // Fin de commande (\n = séparateur de commmandes):
            _dbgBuffer.trim();
            _dbgBuffer.toLowerCase();
            DebugInterpretCommand(_dbgBuffer);
            
            _dbgBuffer = ""; // réinitialisation du buffer.
        }
    }
}

uint32_t DebugGetPC(uint16_t spValue, uint16_t previousSpValue)
{
    uint16_t diff = previousSpValue - spValue;
    
    uint32_t tmpAddress = 0;
    uint8_t* pointer = (uint8_t*)(spValue + diff - 1);   
    
    memcpy(&tmpAddress, pointer, sizeof(DBG_RET_SIZE));
    
    uint8_t buff[2];
    buff[0] = (tmpAddress >> 8);
    buff[1] = ((tmpAddress << 8) >> 8);
    memcpy(&tmpAddress, buff, 2);
    
    return (tmpAddress << 1);
}

void DebugPrintVariable(uint16_t address, int len) 
{
    // On place un pointeur à l'adresse:
    uint8_t* pointer = (uint8_t*)address; 
    for (int i = 0 ; i < len ; i+= 16)
    {
        for (int j = 0 ; j < 16 && i + j < len ; j++)
        {
            uint8_t val = *(pointer+i+j); // valeur d'1 octet à imprimer.
            Serial.print( (val < 16 ? "0" : "") + String(val, HEX) + " ");
        }
        Serial.println(""); // CRLF;
    } 
    Serial.println(""); // CRLF
}

uint32_t StrHexToInt(String value)
{
    return (uint32_t) strtol(value.c_str(), 0, 16);
}


