#include <drivers/driverManager.h>
#include <common/types.h> 
using namespace mos::common;
using namespace mos::drivers;
void printf(char * str);
void printfHex(int8_t hex);
DriverManager::DriverManager(){
    numDrivers=0;
}
void DriverManager::addDriver(Driver* driver){
    drivers[numDrivers]=driver;
    numDrivers++;
}
void DriverManager::installAll(){
    for (uint16_t i = 0; i < numDrivers; i++)
    { 
        drivers[i]->install();
    }
    
}
