/* ========================================
 *
 * SOCCER ROBOT EMBEDDED CODE
 * VERSION 1.0
 * COMPILED FOR THE PsOC 5LP PROTOTYPING KIT
 * CY8CKIT-059 WHICH IS A CY8C5888LTI-LP097 DEVICE
 *
 * AUTHORED BY CHAY322 ET. AL. IN THE SUMMER OF 2021
 *
 * ========================================
*/
/* TO FIND MORE ON DOUBLE BUFFERING UART SEE https://www.allaboutcircuits.com/technical-articles/uart-double-buffering-technique-interrupt-friendly/
   This source is potentially useful as well https://community.cypress.com/t5/PSoC-5-3-1-MCU/UART-Input-String-Parsing/td-p/70565?start=0 */
#include "project.h"
#include "encoder.h"
#include "robotmove.h"
#include "robot_ID.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/* *********************************************** */
/* ************USER DEFINED MACROS**************** */
/* *********************************************** */
#define ENCODER_TIMER_ARRAY_SIZE 5
#define STR_LEN_MAX 60 //Provides an extra 12 bytes in case Zigbee writes to the input buffer faster than main can process
#define TRANSMISSION_STR_LENGTH 48 //Length of each completed transmitted string from the PC
#define ANG_V_SCALING_FACTOR 64  //Transmitted bytes for the angular velocity are scaled up by this factor
#define LIN_V_SCALING_FACTOR 2048 //Transmitted bytes for the linear velocity are scaled up by this factor

//Hardware parameters
#define ENCODER_COUNTS 200 //Encoder pulses generated in a full 360 rotation
#define WHEEL_DIAMETER 10 //Measured in centimeters
#define GEAR_RATIO 8 //Large gear teeth / small gear teeth.  Number of turns of the motor shaft which will turn the wheels a full 360
#define MOTOR_MAX_RPM 11600 //Motor rated operating speed.  This is the maximum angv the motor will operate at with our supply voltage.


/* *********************************************** */
/* ***************GLOBAL VARIABLES**************** */
/* *********************************************** */
//Receive double buffering
//Initially buffer_1 will be pointed to by the receive pointer, and buffer 2
//will be pointed to by the processing buffer pointer
volatile char buffer_1[STR_LEN_MAX] = {};
volatile char buffer_2[STR_LEN_MAX]={};
volatile char *receiving_buffer_ptr;
volatile char *processing_buffer_ptr;
volatile uint8_t rx_buffer_size = 0;

//For indicating whether to start the timer or start a capture
volatile bool timerL_on = false;
volatile bool timerR_on = false;

//For storing multiple historic encoder timer values for accurate speed calculations
volatile uint16_t timerL_count[ENCODER_TIMER_ARRAY_SIZE] = {};
volatile uint16_t timerR_count[ENCODER_TIMER_ARRAY_SIZE] = {};
//Indicates which times to disregard if the timer has overflowed 
volatile bool timerL_overflows[ENCODER_TIMER_ARRAY_SIZE] = {};
volatile bool timerR_overflows[ENCODER_TIMER_ARRAY_SIZE] = {};
//For accessing the global arrays above
volatile uint8_t timerL_index = 0;
volatile uint8_t timerR_index = 0;


//For PID control
PID_DATA pid_linv;
PID_DATA pid_angv;
/* *********************************************** */
/* ***********INTERRUPT SERVICE ROUTINES********** */
/* *********************************************** */
//Encoder timer ISRS

/* ISR: ENCODER_R_OVF_ISR_vect
 * Purpose: Set the bit corresponding to the R encoder timer measurement that the timer has
 *          overflowed, allowing the get_velocity function to determine a speed of 0 rad/s
 * Modifies: bool timerR_overflows[] -  indicates which readings in timerR_count[] that didn't 
 *                                      measure an overflow of the counter
 *           uint8_t timerR_index - the index is updated
 *                      
 */
CY_ISR(ENCODER_R_OVF_ISR_vect){
    timerR_overflows[timerR_index] = true;
    if(timerR_index < ENCODER_TIMER_ARRAY_SIZE){
        timerR_index++;
    } else {
        timerR_index = 0;   
    }
}
/* ISR: ENCODER_L_OVF_ISR_vect
 * Purpose: Set the bit corresponding to the R encoder timer measurement that the timer has
 *          overflowed, allowing the get_velocity function to determine a speed of 0 rad/s
 * Modifies: bool timerL_overflows[] -  indicates which readings in timerR_count[] that didn't 
 *                                      measure an overflow of the counter
 *           uint8_t timerL_index - the index is updated
 *                      
 */
CY_ISR(ENCODER_L_OVF_ISR_vect){
    timerL_overflows[timerL_index] = true;
    if(timerL_index < ENCODER_TIMER_ARRAY_SIZE){
        timerL_index++;
    } else {
        timerL_index = 0;   
    }
}
//Encoder pin ISRS
/* ISR: ENC_R_ISR_vect
 * Purpose: Start the encoder timer or set the encoder capture registers to 
 *          generate a timer capture, depending on which has occured first
 * Modifies: bool timerL_overflows[] -  indicates which readings in timerR_count[] that didn't 
 *                                      measure an overflow of the counter
 *           uint8_t timerL_index - the index is updated
 *                      
 */
CY_ISR(ENC_R_ISR_vect){
    if(timerR_on){
        //This marks the end of a time measurement between encoder pulses
        //Force a capture on the encoder R timer and store it in the global array
        timerR_count[timerR_index] = Encoder_R_Timer_ReadCounter();
        //Clear the capture FIFO for the next reading
        Encoder_R_Timer_ClearFIFO();
        //Stop the timer until next pulse is detected
        Encoder_R_Timer_Stop();
        timerR_on = false;
        
        //Increment the timer index
        if(timerR_index < ENCODER_TIMER_ARRAY_SIZE){
            timerR_index++;
        } else {
            timerR_index = 0;   
        }
    } else {
        //Start the timer to begin a time sample
        Encoder_R_Timer_Start();
        timerR_on = true;
    }
}

/* ISR: ENC_L_ISR_vect
 * Purpose: Start the encoder timer or set the encoder capture registers to 
 *          generate a timer capture, depending on which has occured first
 * Modifies: bool timerL_overflows[] -  indicates which readings in timerL_count[] that didn't 
 *                                      measure an overflow of the counter
 *           uint8_t timerL_index - the index is updated
 *                      
 */
CY_ISR(ENC_L_ISR_vect){
    if(timerL_on){
        //This marks the end of a time measurement between encoder pulses
        //Force a capture on the encoder L timer and store it in the global array
        timerL_count[timerL_index] = Encoder_L_Timer_ReadCounter();
        //Clear the capture FIFO for the next reading
        Encoder_L_Timer_ClearFIFO();
        //Stop the timer until next pulse is detected
        Encoder_L_Timer_Stop();
        timerL_on = false;
        
        //Increment the timer index
        if(timerL_index < ENCODER_TIMER_ARRAY_SIZE){
            timerL_index++;
        }  else {
            timerL_index = 0;   
        }
    } else {
        //Start the timer to begin a time sample
        Encoder_L_Timer_Start();
        timerL_on = true;
    }
}
//USART ISRS
/* ISR: TX_ISR_vect
 * Purpose: Clear the buffer once the transmit has completed for the next transmission
 * Modifies: bool timerL_overflows[] -  indicates which readings in timerL_count[] that didn't 
 *                                      measure an overflow of the counter
 *           uint8_t timerL_index - the index is updated
 *                      
 */
CY_ISR(TX_ISR_vect){

    
}
/* ISR: RX_ISR_vect
 * Purpose: Store characters received from UART into the receive buffer
 * Modifies: char *receive_buffer_ptr[] - the input character buffer
 *                                         which is either buffer_1 or buffer_2
 *           uint8_t rx_buffer_size - the index of the rx buffer is updated
 *                      
 */
CY_ISR(RX_ISR_vect){
    char byte_received;
    //Check for the FIFO_NOEMPTY flag which indicates the FIFO has data available to read
    if(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
        byte_received = UART_GetChar();
        receiving_buffer_ptr[rx_buffer_size] = byte_received;
        rx_buffer_size++;
    }
}
/* *********************************************** */
/* ************USER DEFINED FUNCTIONS************* */
/* *********************************************** */
/* UART PROCESSING - ROBOT COMMAND PROCESSING */
typedef struct{
    double linv, angv;
} ROBOT_COMMAND;
/* *********************************************** */
/* **********************MAIN********************* */
/* *********************************************** */
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    //Ensure reset lines are low initially
    PWM_RESET_Write(0b00);
    ENCODER_TMR_RESET_Write(0b00);
    
    //Enable the USART
    UART_Start();
    UART_EnableRxInt();
    
    //Configure the initial pointers for the USART RX isr and the main function
    receiving_buffer_ptr = buffer_1;
    processing_buffer_ptr = buffer_2;
    uint8_t read_buffer_size = 0; //The size of the buffer the main function will read from
    
    //Enable and configure all interrupts
    tx_isr_StartEx(TX_ISR_vect);
    rx_isr_StartEx(RX_ISR_vect);
    ENCODER_R_OVF_ISR_StartEx(ENCODER_R_OVF_ISR_vect);
    ENCODER_L_OVF_ISR_StartEx(ENCODER_L_OVF_ISR_vect);
    ENC_R_ISR_StartEx(ENC_R_ISR_vect);
    ENC_L_ISR_StartEx(ENC_L_ISR_vect);
    
    //Get the robot id
    uint8_t robot_id = get_robot_ID();
    
    //Commands from USART.  Initialise to 0[m/s], 0[rad/s]
    ROBOT_COMMAND *current_command;
    current_command->angv = 0;
    current_command->linv = 0;
    
    bool transmission_corrupted = false;
    
    //Indicates whether a full transmission command has been processed before reading a new command
    //from the second input buffer
    bool main_rdy_to_read = true;
    
    //Target velocities which the command from the USART requires the robot to move at
    PWM_L_Start();
    PWM_R_Start();

    for(;;){
        /* Get transmission command from USART */
        if(rx_buffer_size > TRANSMISSION_STR_LENGTH && main_rdy_to_read){ //Full transmission string has been received in the buffer
            main_rdy_to_read = false;
            //****CRITICAL_SECTION
            //Swap buffers the pointers point to to read what the ISR has been writing to
            UART_DisableRxInt();    
                volatile char *temp = receiving_buffer_ptr;
                receiving_buffer_ptr = processing_buffer_ptr;
                processing_buffer_ptr = temp;
            
                //Save the size of the buffer written to by the UXART RX isr
                read_buffer_size = rx_buffer_size;
                //Reset the rx_buffer_size for when the RX isr writes to the new buffer
                rx_buffer_size = 0;
            UART_EnableRxInt();
            //****END OF CRITICAL SECTION
            char check_sum = 0;
            //Read the receiving buffer (from receiving_buffer_ptr), parse it and update the robot command
            for(int i = 0; i < read_buffer_size; ++i){
                
                //The start of each command string is 0xFF, 0xFF, 0x01
                if(receiving_buffer_ptr[0] == 0xFF && receiving_buffer_ptr[1] == 0xFF && receiving_buffer_ptr[2] == 0x01){
                    check_sum += receiving_buffer_ptr[i]; //Adding the bytes of the received data, including the transmitted checksum digit should have a result of 0
                    //Each byte for each robot begins at the nth multiple of 4 + 3 since the first three bytes are unrelated to speed transmission
                    int starting_index = 4 * (robot_id - 1) + 3;
                    
                    //The transission has the least significant bits followed by the most significant bits
                    //Concatenate these two bytes together to form the scaled version of the linear and angular velocity commanded from the PC
                    //Two bytes for the linear velocity followed by two bytes for the angular velocity
                    uint16_t scaled_linv = (uint16_t)(receiving_buffer_ptr[starting_index] | (receiving_buffer_ptr[starting_index+1] << 8));
                    uint16_t scaled_argv = (uint16_t)(receiving_buffer_ptr[starting_index + 2] | (receiving_buffer_ptr[starting_index + 3] << 8));
                    
                    current_command->linv = (double)scaled_linv / LIN_V_SCALING_FACTOR;
                    current_command->angv = (double)scaled_argv / ANG_V_SCALING_FACTOR;
                }
            }
            //Check the checksum - if it's zero - uncorrupted
            if(check_sum != 0){
                transmission_corrupted = true;
            } else {
                transmission_corrupted = false;   
            }
            main_rdy_to_read = true;
        }    
    }
}/* [] END OF FILE */