#include "lpc17xx.h"

#define USB_DEV_ADDR    1
#define USB_EP_IN       0x81
#define USB_EP_OUT      0x01

void USB_Init()
{
    LPC_SC->PCONP |= (1UL << 31);           // Enable USB device clock
    LPC_USB->OTGClkCtrl = 0x1F;             // Enable USB device clock
    LPC_USB->USBClkCtrl = 0x12;             // Enable USB controller clock
    LPC_USB->IntSt = 0x7FF;                 // Clear all USB interrupts
    LPC_USB->DevIntClr = 0xFFFFFFFF;        // Clear all device interrupts
    LPC_USB->DevIntPri = 0x00000000;        // Set all device interrupts to low priority
    LPC_USB->EpIntClr[0] = 0xFFFFFFFF;      // Clear all endpoint 0 interrupts
    LPC_USB->EpIntClr[1] = 0xFFFFFFFF;      // Clear all endpoint 1 interrupts
    LPC_USB->EpIntPri[0] = 0x00000000;      // Set all endpoint 0 interrupts to low priority
    LPC_USB->EpIntPri[1] = 0x00000000;      // Set all endpoint 1 interrupts to low priority
    LPC_USB->EpInd = 0;                     // Select endpoint 0
    LPC_USB->EpMaxPSize = 64;               // Set maximum packet size for endpoint 0
    LPC_USB->EpCtrl[0] = 0x0000001D;        // Enable endpoint 0 and set as control endpoint
    LPC_USB->EpInd = 1;                     // Select endpoint 1
    LPC_USB->EpMaxPSize = 8;                // Set maximum packet size for endpoint 1
    LPC_USB->EpCtrl[1] = 0x0000001B;        // Enable endpoint 1 and set as interrupt endpoint
    LPC_USB->CmdCode = 0;                   // Clear USB command code
    LPC_USB->CmdData = 0;                   // Clear USB command data
    LPC_USB->DMARSt = 0x00000000;           // Clear DMA request status
    LPC_USB->DMARClr = 0xFFFFFFFF;          // Clear DMA request status
    LPC_USB->DMARSet = 0x00000000;           // Disable DMA
}

void USB_Connect()
{
    LPC_USB->DevIntPri |= (1UL << 1);        // Enable USB connect interrupt
    LPC_USB->DevIntEn |= (1UL << 1);         // Enable USB connect interrupt
    LPC_USB->USBCtrl |= (1UL << 2);          // Enable USB device
}

void USB_Disconnect()
{
    LPC_USB->DevIntEn &= ~(1UL << 1);        // Disable USB connect interrupt
    LPC_USB->USBCtrl &= ~(1UL << 2);         // Disable USB device
}

void USB_SendData(uint8_t *data, uint32_t size)
{
    while (LPC_USB->EpIntSt & (1UL << 1));   // Wait until endpoint 1 is not busy
    for (uint32_t i = 0; i < size; i += 8) {
        uint32_t packet_size = size - i;
        if (packet_size > 8) {
            packet_size = 8;
        }
        uint32_t *packet = (uint32_t *)&data[i];
        LPC_USB->EpBuf[1] = packet[0];
        LPC_USB->EpBuf[1] = packet[1];
        LPC_USB->EpBuf[1] = packet[2];
        LPC_USB->EpBuf[1] = packet[3];
        LPC_USB->EpBuf[1] = packet[4];
        LPC_USB->EpBuf[1] = packet[5];
        LPC_USB->EpBuf[1] = packet[6];
        LPC_USB->EpBuf[1] = packet[7];
        LPC_USB->EpCtrl[1] = (packet_size << 2) | 0x02;
        while (LPC_USB->EpIntSt & (1UL << 1));   // Wait until endpoint 1 is not busy
    }
}

int main()
{
    USB_Init();
    USB_Connect();
    uint8_t keycode;
    while (1) {
        if (LPC_USB->EpIntSt & (1UL << 0)) {  // Check if endpoint 0 has received a SETUP packet
            uint32_t packet[2];
            packet[0] = LPC_USB->EpBuf[0];
            packet[1] = LPC_USB->EpBuf[0];
            LPC_USB->EpIntClr[0] = (1UL << 0);    // Clear endpoint 0 interrupt
            if (packet[0] == 0x0B0100FE && packet[1] == 0x0300) {   // Check if SETUP packet is for Get_Report request
                uint8_t data[8] = {0, keycode, 0, 0, 0, 0, 0, 0};   // Prepare data to be sent in response
                USB_SendData(data, 8);  // Send data in response to SETUP packet
            }
        }
        if (LPC_USB->EpIntSt & (1UL << 1)) {  // Check if endpoint 1 has received an interrupt IN packet
            uint32_t packet[2];
            packet[0] = LPC_USB->EpBuf[1];
            packet[1] = LPC_USB->EpBuf[1];
            LPC_USB->EpIntClr[1] = (1UL << 1);    // Clear endpoint 1 interrupt
            keycode = packet[0] & 0xFF;   // Extract keycode from packet
            // Do something with the keycode here
        }
    }
}

           
