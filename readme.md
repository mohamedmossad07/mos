# My Operating System [MOS]

If you want to backward glance on operating system architecture, `This project for you`. 

**Features**
- Implementation of basic kernel components
    * Interrupting system
    * Peripheral component interconnect (PCI)
    * Drivers 
        * Mouse driver
        * Keyboard driver
        * ATA driver
        * AMD Network card
        * VGA driver
    * Multitasking 
    * Dynamic Memory manager
    * Network protocols
        * DataLink layer (Ethernet)
        * Network layer (IP)
        * Transport layer (TCP, UDP)    

- Custom linker script to combine objects files    
- Custom loader to load the kernel

**Limitations**
> We implemented the basic core features like GUI, Network protocols, but We didn't activate it.
---
> ⚠️ This code is'nt for production , `just for practice` .
<br>
<br>

```bash
$ git clone https://github.com/mohamedmossad07/mos
```

**Requirements**
- Linux operating system.
- GNU Tools like Assembler, G++, Linker or Install Packages [ binutils,  libc6-dev-i386 ]
* VM Virtualbox
* Grub Boot loader 
* Make Tool
---
### Are you ready? `Lets start`
 ## 1) Creating VM named mos in Virtualbox
 ## 2) Build `MOS`  
 ```
 $ make kernel.iso
 ```
 ## 3) Import produced ISO file into Virtualbox VM mos
 ```
 $ make run
 ```
 > `Kernel Output should be in the text mode .`
 ---

