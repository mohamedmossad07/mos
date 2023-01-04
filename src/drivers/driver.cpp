#include <drivers/driver.h>
#include <common/types.h>
using namespace mos::drivers;
using namespace mos::common;
Driver::Driver(){}
Driver::~Driver(){}
void Driver::install(){}
void Driver::uninstall(){}
int32_t Driver::reset(){
    return 0;
}