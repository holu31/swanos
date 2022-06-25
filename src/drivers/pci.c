#include <kernel.h>
#include <pci.h>
#include <ports.h>
#include <console.h>


uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000)); // Create configuration address as per Figure 1
 
    outl(0xCF8, address); // Write out the address
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}


uint16_t get_vendorID(uint16_t bus, uint16_t device, uint16_t function){
        uint32_t r0 = pci_read_word(bus,device,function,0);
        return r0;
}

uint16_t get_deviceID(uint16_t bus, uint16_t device, uint16_t function){
        uint32_t r0 = pci_read_word(bus,device,function,2);
        return r0;
}

void pci_getbuses(){
	for(uint32_t bus = 0; bus < 256; bus++){
        for(uint32_t slot = 0; slot < 32; slot++){
            for(uint32_t function = 0; function < 8; function++){
                switch (get_deviceID(bus, slot, function)) {
					case 0x7010:
						cputs("0x7010 | ");
						cputs("82371SB PIIX3 IDE [Natoma/Triton II]\n");
						break;
					case 0x7111:
						cputs("0x7111 | ");
						cputs("82371AB/EB/MB PIIX4 IDE\n");
						break;
					case 0x7113:
						cputs("0x7113 | ");
						cputs("82371AB/EB/MB PIIX4 ACPI\n");
						break;
					case 0x5513:
						cputs("0x5513 | ");
						cputs("5513 IDE Controller\n");
						break;
					case 0x7001:
						cputs("0x7001 | ");
						cputs("USB 1.1 Controller\n");
						break;
					case 0x7002:
						cputs("0x7002 | ");
						cputs("USB 2.0 Controller\n");
						break;
					case 0xAA28:
						cputs("0xAA28 | ");
						cputs("RV620 HDMI Audio [Radeon HD 3450/3470/3550/3570]\n");
						break;
					case 0xA012:
						cputs("0xA012 | ");
						cputs("Atom Processor D4xx/D5xx/N4xx/N5xx Integrated Graphics Controller\n");
						break;
					case 0x27D2:
						cputs("0x27D2 | ");
						cputs("NM10/ICH7 Family PCI Express Port 2\n");
						break;
					case 0x27D6:
						cputs("0x27D6 | ");
						cputs("NM10/ICH7 Family PCI Express Port 4\n");
						break;
					case 0x27C9:
						cputs("0x27C9 | ");
						cputs("NM10/ICH7 Family USB UHCI Controller #2\n");
						break;
					case 0x27CA:
						cputs("0x27CA | ");
						cputs("NM10/ICH7 Family USB UHCI Controller #3\n");
						break;
					case 0x27CB:
						cputs("0x27CB | ");
						cputs("NM10/ICH7 Family USB UHCI Controller #4\n");
						break;
					case 0x27CC:
						cputs("0x27CC | ");
						cputs("NM10/ICH7 Family USB2 EHCI Controller\n");
						break;
					case 0x27C1:
						cputs("0x27C1 | ");
						cputs("NM10/ICH7 Family SATA Controller [AHCI mode]\n");
						break;
					case 0x27DA:
						cputs("0x27DA | ");
						cputs("NM10/ICH7 Family SMBus Controller\n");
						break;
					case 0x1360:
						cputs("0x1360 | ");
						cputs("RTL8139 Ethernet\n");
						break;
					case 0x1300:
						cputs("0x1300 | ");
						cputs("RTL8139 Ethernet\n");
						break;
				}
				switch (get_vendorID(bus, slot, function)) {
					case 0x8086:
						cputs("0x8086 | ");
						cputs("Intel Corporation\n");
						break;
					case 0x1002:
						cputs("0x1002 | ");
						cputs("Advanced Micro Devices, Inc. [AMD/ATI]\n");
						break;
					case 0x1039:
						cputs("0x1039 | ");
						cputs("Silicon Integrated Systems [SiS]\n");
						break;
					case 0x0B05:
						cputs("0x0B05 | ");
						cputs("ASUSTek Computer, Inc.\n");
						break;
				}
            }
        }
    }
}
