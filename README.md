# Microprocessors and Peripherals : 2nd Lab.
**Interupts and ISRs on the STM32 nulceo microcontroller**


Authors: Konstantinidis Paschalis, Tzouvaras Evangelos.

Team : 5 

### Lab Description: ###
The second lab contains the interrupts and ISR programming technics of the
STM32 microcontroller and the connectivity via the UART protocol. The
program requests from the user his AEM or to press the external button. On the
first case, the program takes the AEM number via UART and the receive
interrupt calls the ISR callback. On the second case, if the button was pressed,
the program calls the external interrupt ISR callback. Then the program is
responsible to transmit via UART appropriate messages to the user. The
program was developed by using the STM32 Cube MX and the HAL drivers with
cooperation of keil uVision, because we are more familiar with this packet.
