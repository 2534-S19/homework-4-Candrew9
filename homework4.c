#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";



    // TODO: Declare the variables that main uses to interact with your state machine.
    typedef enum myStates{first, second, third, fourth} charStates;
    //charStates currState = first;
    bool flag = false;

    // Stops the Watchdog timer.
    initBoard();
    // Declare a UART config struct as defined in uart.h.
    //

    eUSCI_UART_ConfigV1 uartConfig = {
              EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
              19,                                           // UCBR = 19
              8,                                            // UCBRF = 8
              0x55,                                         // UCBRS = 0x55
              EUSCI_A_UART_NO_PARITY,                     // Even Parity
              EUSCI_A_UART_LSB_FIRST,                       // LSB First
              EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
              EUSCI_A_UART_MODE,                            // UART mode
              EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,// oversampling
              EUSCI_A_UART_8_BIT_LEN                         // 8 bit payload
    };


    // Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    // Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);
    while(1)
    {
        // Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            rChar = UART_receiveData(EUSCI_A0_BASE);
        else
            rChar = 0xFF;

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != 0xFF) {
            flag = charFSM(rChar);
            //Checking interrupt here
            while(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 1)
            {}
            UART_transmitData(EUSCI_A0_BASE, rChar);
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if (flag) {

            while(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0) {
            }
            char *c;

            for (c = response; *c != '\0'; c++) {
                while(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0)
                    {}
                    UART_transmitData(EUSCI_A0_BASE, *c);
            }

            flag = false;
        }


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;

    switch(currState)
    {
        case first:
        {
            if(rChar == '2'){
                currState = second;
            }
            else
                currState = first;
        }
        break;
        case second:
        {
            if(rChar == '5'){
                currState = third;
            }
            else
                currState = first;
        }
        break;
        case third:
        {
            if(rChar == '3') {
                currState = fourth;
            }
            else
                currState = first;
        }
        break;
        case fourth:
        {
            if(rChar == '4') {
                finished = true;
                currState = first;
            }
            else
                currState = first;

        }
        break;
    }


    return finished;
}
