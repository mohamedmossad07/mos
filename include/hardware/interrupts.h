#ifndef _MOS_HARDWARE_INTERRUPTS_H
#define _MOS_HARDWARE_INTERRUPTS_H
#include <common/types.h>
#include <gdt.h>
#include <multitasking.h>
#include <hardware/port.h>
using namespace mos::common;
using namespace mos;
using namespace mos::hardware;
namespace mos
{
  namespace hardware
  {
    class InterruptManager;
    class InterruptHandler
    {
    protected:
      uint8_t int_n;
      InterruptManager *int_mg;
      InterruptHandler(uint8_t int_n, InterruptManager *int_mg);
      ~InterruptHandler();

    public:
      virtual uint32_t handel(uint32_t esp);
    };

    class InterruptManager
    {
      friend class InterruptHandler;

    protected:
      InterruptHandler *handlers[256];
      TaskManager *taskMgr;
      static InterruptManager *instance;
      GlobalDescriptorTable *gdt;
      struct GateDescriptor
      {
        uint16_t handler_low;
        uint16_t cSeg; // code segment selector
        uint8_t zeros;
        uint8_t access;
        uint16_t handler_high;
      } __attribute__((packed));
      static GateDescriptor idt[256];

      struct IDTptr
      {
        uint16_t sz;
        uint32_t IDT;
      } __attribute__((packed));

      static void setIDTentry(
          uint8_t int_n,
          uint16_t codeSeg,
          void (*handler)(),
          uint8_t dpl,
          uint8_t type);
      port8BitSlow picMasterCMD;
      port8BitSlow picMasterData;
      port8BitSlow picSlaveCMD;
      port8BitSlow picSlaveData;

    public:
      InterruptManager(GlobalDescriptorTable *gdt, TaskManager *task);
      ~InterruptManager();
      void activate();
      void deactivate();
      static uint32_t handelInterrupt(uint8_t int_n, uint32_t esp);
      uint32_t handel(uint8_t int_n, uint32_t esp);
      static void ignoreInterruptRequest();

      static void handelInterruptRequest0x00();
      static void handelInterruptRequest0x01();
      static void handelInterruptRequest0x02();
      static void handelInterruptRequest0x03();
      static void handelInterruptRequest0x04();
      static void handelInterruptRequest0x05();
      static void handelInterruptRequest0x06();
      static void handelInterruptRequest0x07();
      static void handelInterruptRequest0x08();
      static void handelInterruptRequest0x09();
      static void handelInterruptRequest0x0A();
      static void handelInterruptRequest0x0B();
      static void handelInterruptRequest0x0C();
      static void handelInterruptRequest0x0D();
      static void handelInterruptRequest0x0E();
      static void handelInterruptRequest0x0F();
      // static void handelInterruptRequest0x31();
      static void handelInterruptRequest0x80();

      // static void handleException0x00();
      // static void handleException0x01();
      // static void handleException0x02();
      // static void handleException0x03();
      // static void handleException0x04();
      // static void handleException0x05();
      // static void handleException0x06();
      // static void handleException0x07();
      // static void handleException0x08();
      // static void handleException0x09();
      // static void handleException0x0A();
      // static void handleException0x0B();
      // static void handleException0x0C();
      // static void handleException0x0D();
      // static void handleException0x0E();
      // static void handleException0x0F();

      // static void handleException0x10();
      // static void handleException0x11();
      // static void handleException0x12();
      // static void handleException0x13();
    };
  };
};
#endif