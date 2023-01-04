CC = g++
CXXFALGS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore  -c
ASFLAGS = --32
OBJECTS = obj/loader.o\
		  obj/gdt.o\
		  obj/drivers/driver.o\
		  obj/drivers/driverManager.o\
		  obj/common/helper.o \
		  obj/hardware/port.o\
		  obj/hardware/interruptstubs.o  \
		  obj/hardware/pci.o  \
		  obj/hardware/PCIDeviceDescriptor.o  \
		  obj/hardware/interrupts.o\
		  obj/hardware/BAR.o\
		  obj/drivers/Keyboard.o \
		  obj/drivers/keyboardEventHandler.o \
		  obj/drivers/Mouse.o\
		  obj/drivers/mouseEventHandler.o\
		  obj/drivers/vga.o\
		  obj/drivers/AMDNetCard.o\
		  obj/drivers/ATA.o\
		  obj/gui/widget.o\
		  obj/gui/CompositeWidget.o\
		  obj/gui/window.o\
		  obj/gui/desktop.o\
		  obj/multitasking.o\
		  obj/syscall.o\
		  obj/MemoryManager.o\
		  obj/MemoryChunk.o\
		  obj/net/DLLayer/etherframe.o\
		  obj/net/NetLayer/arp.o\
		  obj/net/NetLayer/ip4.o\
		  obj/net/NetLayer/icmp.o\
		  obj/net/TPLayer/udp.o\
		  obj/net/TPLayer/tcp.o\
		  obj/kernel.o
LDFLAGS = -Map map.map -melf_i386


obj/%.o :src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CXXFALGS) -o $@ $^
obj/%.o : src/%.s
	mkdir -p $(@D)
	as $(ASFLAGS)	-o $@ $^
kernel.bin : linker.ld $(OBJECTS)
	ld  $(LDFLAGS)	-T $< -o $@ $(OBJECTS)
kernel.iso : kernel.bin 
	mkdir iso
	mkdir iso/boot	
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set tiemout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My OS" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo 'boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
run : kernel.iso
	virtualboxvm --startvm "mos" 
clean :
	rm -rf obj kernel.iso *.o iso *.bin
