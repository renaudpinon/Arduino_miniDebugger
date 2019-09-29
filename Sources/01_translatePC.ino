#include <Arduino.h>

// Prototypes:
uint16_t translatePC(uint16_t address);

void setup()
{
}

void loop()
{
    // TODO: appeler translatePC() avec l'adresse
    // de l'instruction de retour à transformer.
}

uint16_t translatePC(uint16_t address)
{
    // On doit inverser les octets pour
    // Obtenir le PC (ex: 0x2b1c devient 0x1c2b):
    uint8_t buff[2];
    buff[0] = (address >> 8);
    buff[1] = ((address << 8) >> 8);
    memcpy(&address, buff, sizeof(address));
    
    // Enfin on décale les bits de 1 vers la gauche:
    return (address << 1);
}
