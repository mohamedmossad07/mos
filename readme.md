
# `MOS`

**MOS** (My Own Operating System) is a text-mode kernel that includes the essential components required to boot up and interact with motherboard devices and drivers.

**MOS** is more than just an operating system—it's a platform for learning, innovation, and experimentation. Whether you're interested in OS development, or simply exploring the inner workings of your computer, **MOS** provides an environment to bring your ideas to life.

## Key Features:

- **Lightweight and Minimalistic**: `MOS` focuses on minimal resource usage.

- **Direct Hardware Access**: Provides low-level access to system hardware, offering a deeper understanding of how operating systems interact with hardware at the core level.

- **Built-in Security**: Features security measures, including user-space separation, memory protection, and privilege levels to safeguard the system and user data.

- **Task Management**: Supports efficient multitasking with preemptive scheduling to manage real-time tasks, ensuring smooth performance even in resource-constrained environments.

- **Virtual Memory Support**: Implements memory paging and segmentation (including support for the GDT), enabling advanced memory management and protection.

- **Local Device Drivers**: Includes a list of local device drivers such as **ATA**, **Mouse**, **Keyboard**, **Network Card**, and **VGA** drivers.

- **Built-in Network Layers**: Implements basic networking protocols, including **EthernetFrame**, **ARP**, **ICMP**, **IPv4**, and **TCP**.

## Designed For:

- **Learning and Development**: `MOS` is designed to provide deep insight into how OS components work together, making it an excellent tool for learning low-level programming, kernel design, and system architecture.

- **Custom Workloads**: If you need a custom OS for experimenting with operating system concepts, `MOS` can be tailored to meet your needs.

## Development Environment:

- **Written in C++**: The core system is written in C++ for efficiency, low-level hardware control, and object-oriented features.

### **Requirements**:
- Linux operating system
- GNU tools (Assembler, G++, Linker, [binutils, libc6-dev-i386])
- VirtualBox (VM)
- GRUB Bootloader
- Make tool

## Core Components:

- **Kernel**: The heart of the operating system, responsible for process scheduling, memory management, hardware abstraction, and system calls.

- **Device Drivers**: Provide support for a range of hardware components, including storage, networking, and input/output devices, offering direct access to hardware resources.

- **Network Protocols**:
  - Data Link Layer (Ethernet)
  - Network Layer (IP)
  - Transport Layer (TCP, UDP)

