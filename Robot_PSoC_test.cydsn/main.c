/* ========================================
 *
 * SOCCER ROBOT EMBEDDED CODE
 * VERSION 1.0
 * COMPILED FOR THE PsOC 5LP PROTOTYPING KIT
 * CY8CKIT-059 WHICH IS A CY8C5888LTI-LP097 DEVICE
 *
 * AUTHORED BY CURTIS HAYDON, JUSTIN YE 
 * January / February 2021
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
#include <stdlib.h>
/* *********************************************** */
/* ************USER DEFINED MACROS**************** */
/* *********************************************** */
//Parameters for global variables
#define ENCODER_TIMER_ARRAY_SIZE 5
#define STR_LEN_MAX 100 //Provides an extra 52 bytes in case Zigbee writes to the input buffer faster than main can process
#define TRANSMISSION_STR_LENGTH 48 //Length of each completed transmitted string from the PC
#define ANG_V_SCALING_FACTOR 64  //Transmitted bytes for the angular velocity are scaled up by this factor
#define LIN_V_SCALING_FACTOR 2048 //Transmitted bytes for the linear velocity are scaled up by this factor
#define TRANSMISSION_PKT_SIZE 48 //The transmitted packet is this many bytes long

//Robot Hardware parameters
#define ROBOT_WIDTH 0.1 //Measured in meters
#define ENCODER_COUNTS 11 //Encoder pulses generated in a full 360 rotation
#define WHEEL_DIAMETER 0.1 //Measured in meters
#define GEAR_RATIO 8 //Large gear teeth / small gear teeth.  Number of turns of the motor shaft which will turn the wheels a full 360
#define MOTOR_MAX_RPM 11600 //Motor rated operating speed.  This is the maximum angv the motor will operate at with our supply voltage.

//Encoder parameters
#define CLOCKWISE 1
#define ANTICLOCKWISE 0

//PID parameters
#define Pgain_linv 1 //Proportional gain for linear velocity
#define Dgain_linv 0 //Differential gain for linear velocity
#define Igain_linv 0 //Integral gain for linear velocity

#define Pgain_angv 1 //Proportional gain for angular velocity
#define Dgain_angv 0 //Differential gain for angular velocity
#define Igain_angv 0 //Integral gain for angular velocity

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

//Encoder variables
//For storing multiple historic encoder timer values for accurate speed calculations
volatile uint16_t timerL0_count[ENCODER_TIMER_ARRAY_SIZE] = {};
volatile uint16_t timerR0_count[ENCODER_TIMER_ARRAY_SIZE] = {};

//Indicates which times to disregard if the timer has overflowed 
volatile bool timerL0_overflows[ENCODER_TIMER_ARRAY_SIZE] = {};
volatile bool timerR0_overflows[ENCODER_TIMER_ARRAY_SIZE] = {};

//For accessing the global count and overflow arrays above
volatile uint8_t timerL0_index = 0;
volatile uint8_t timerR0_index = 0;

//For indicating the measured wheel turning direction
volatile bool R_direction = CLOCKWISE;
volatile bool L_direction = CLOCKWISE;

//For PID control
PID_DATA pid_linv;
PID_DATA pid_angv;
/* *********************************************** */
/* ************USER DEFINED DATATYPES************* */
/* *********************************************** */
typedef struct{
    double linv, angv;
} ROBOT_COMMAND;
//Note: PID_DATA is defined in robotmove.h

/* *********************************************** */
/* ************USER DEFINED FUNCTIONS************* */
/* *********************************************** */

CY_ISR_PROTO(ENCODER_R0_OVF_ISR_vect);
CY_ISR_PROTO(ENCODER_L0_OVF_ISR_vect);
CY_ISR_PROTO(ENC_R0_ISR_vect);
CY_ISR_PROTO(ENC_L0_ISR_vect);
CY_ISR_PROTO(ENC_L1_ISR_vect);
CY_ISR_PROTO(ENC_R1_ISR_vect);
CY_ISR_PROTO(TX_ISR_vect);
CY_ISR_PROTO(RX_ISR_vect);

/* USBUART INTERFACE */
/* Function: usbPutString
 * Purpose: Puts the input string into the USBUART data buffer once the CDC is ready
 *          If the string is longer than 64 characters it is truncated at 63 as USB restricts 
 *          data packets to 64 characters long (inserting the NULL character at the end)
 *          The final character [63] is a ! to warn the user
 * Inputs:  char *string - the input character string
 * Outputs: None             
 */
void usbPutString(char *string){
    while(USBUART_CDCIsReady() == 0);
    string[63] = '\0';
    string[62] = '!';
    USBUART_PutData((uint8*)string,strlen(string));    
}

/* Function: usbPutChar
 * Purpose: Puts the input character into the USBUART data buffer once the CDC is ready
 * Inputs:  char c - the input character
 * Outputs: None             
 */
void usbPutChar(char c){
    while(USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);   
}

/* *********************************************** */
/* ***********INTERRUPT SERVICE ROUTINES********** */
/* *********************************************** */
//Encoder timer ISRS

/* ISR: ENCODER_R0_OVF_ISR_vect
 * Purpose: Set the bit corresponding to the R encoder timer measurement that the timer has
 *          overflowed, allowing the get_velocity function to determine a speed of 0 rad/s
 * Modifies: bool timerR0_overflows[] -  indicates which readings in timerR0_count[] that didn't 
 *                                      measure an overflow of the counter
 *                      
 */
CY_ISR(ENCODER_R0_OVF_ISR_vect){
    //Ensure R0 index is in correct range
    if(timerR0_index > ENCODER_TIMER_ARRAY_SIZE - 1){
        timerR0_index = 0;    
    }
    timerR0_overflows[timerR0_index] = true;
    //Stop the timer until the next pulse is detected
    Encoder_R0_Timer_Stop();
}

/* ISR: ENCODER_L0_OVF_ISR_vect
 * Purpose: Set the bit corresponding to the R encoder timer measurement that the timer has
 *          overflowed, allowing the get_velocity function to determine a speed of 0 rad/s
 * Modifies: bool timerL0_overflows[] -  indicates which readings in timerR0_count[] that didn't 
 *                                      measure an overflow of the counter
 *                      
 */
CY_ISR(ENCODER_L0_OVF_ISR_vect){
    //Ensure L0 index is in correct range
    if(timerL0_index > ENCODER_TIMER_ARRAY_SIZE - 1){
        timerL0_index = 0;    
    }
    timerL0_overflows[timerL0_index] = true;
    //Stop the timer until the next pulse is detected
    Encoder_L0_Timer_Stop();
}

//Encoder pin ISRS
/* ISR: ENC_R0_ISR_vect
 * Purpose: Store the capture value in the timer FIFO into timerR0_count
 *          If the timer had overflowed during the time the period is stored
 *          Restart the timer once this is complete
 * Modifies: bool timerR0_count[] - the timer count recorded between adjacent encoder
 *                                  pulse rising edges on the left wheel.
 *           uint8_t timerR0_index - the index is updated                      
 */
CY_ISR(ENC_R0_ISR_vect){
    if(timerR0_index > ENCODER_TIMER_ARRAY_SIZE - 1){
        timerR0_index = 0;   
    }
    uint16_t timer_period = Encoder_R0_Timer_ReadPeriod();
    if(timerR0_overflows[timerR0_index] == false){
        //If the timer has not overflowed, store the capture value
        timerR0_count[timerR0_index] = timer_period - Encoder_R0_Timer_ReadCapture();
    } else {
        //Otherwise store the period time of the timer
        timerR0_count[timerR0_index] = timer_period;
    }
    //Clear the FIFO timer buffer
    Encoder_R0_Timer_ClearFIFO();
    //Re-load the timer period into the count register to restart the timer
    Encoder_R0_Timer_Stop();
    Encoder_R0_Timer_WriteCounter(timer_period);
    
    if(timerR0_index < ENCODER_TIMER_ARRAY_SIZE - 1){
        timerR0_index++;   
    } else {
        timerR0_index = 0;   
    }
    //Initialise the overflow entry and start the timer
    timerR0_overflows[timerR0_index] = false;    
    Encoder_R0_Timer_Start();
}

/* ISR: ENC_L0_ISR_vect
 * Purpose: Store the capture value in the timer FIFO into timerL0_count
 *          If the timer had overflowed during the time the period is stored
 *          Restart the timer once this is complete
 * Modifies: bool timerL0_count[] - the timer count recorded between adjacent encoder
 *                                  pulse rising edges on the left wheel.
 *           uint8_t timerL0_index - the index is updated                      
 */
CY_ISR(ENC_L0_ISR_vect){
    if(timerL0_index > ENCODER_TIMER_ARRAY_SIZE - 1){
        timerL0_index = 0;   
    }
    uint16_t timer_period = Encoder_L0_Timer_ReadPeriod();
    if(timerL0_overflows[timerL0_index] == false){
        //If the timer has not overflowed, store the capture value
        timerL0_count[timerL0_index] = timer_period - Encoder_L0_Timer_ReadCapture();
    } else {
        //Otherwise store the period time of the timer
        timerL0_count[timerL0_index] = timer_period;
    }
    //Clear the FIFO timer buffer
    Encoder_L0_Timer_ClearFIFO();
    //Re-load the timer period into the count register to restart the timer
    Encoder_L0_Timer_Stop();
    Encoder_L0_Timer_WriteCounter(timer_period);
    
    if(timerL0_index < ENCODER_TIMER_ARRAY_SIZE - 1){
        timerL0_index++;   
    } else {
        timerL0_index = 0;   
    }
    //Initialise the overflow entry and start the timer
    timerL0_overflows[timerL0_index] = false;    
    Encoder_L0_Timer_Start();
}

/* ISR: ENC_R1_ISR_vect
 * Purpose: Use this to identify which direction the right wheel is turning
 *          This assumes that if R0 has a rising edge before R1, motor is turning clockwise
 *                               R1 has a rising edge before R0, motor is turning anticlockwise
 * Modifies: bool R_direction - 0 - clockwise
 *                              1 - anticlockwise
 */
CY_ISR(ENC_R1_ISR_vect){
    uint8_t encoder_R0_pin_state = Encoder_R0_Read();
    if(encoder_R0_pin_state == 0u){
        //R1 ISR has been triggered before R0.  We will interpret this as the wheel moving 
        //anticlockwise (negative/backwards)
        R_direction = ANTICLOCKWISE;
    } else {
        //R1 was triggered after R0.  Wheel is moving clockwise
        R_direction = CLOCKWISE;
    }
    //Clear the latched pin events on encoder R1 pin to enable detection of future events
    Encoder_R1_ClearInterrupt();
}

/* ISR: ENC_L1_ISR_vect
 * Purpose: Use this to identify which direction the left wheel is turning
 *          This assumes that if L0 has a rising edge before L1, motor is turning clockwise
 *                               L1 has a rising edge before L0, motor is turning anticlockwise
 * Modifies: bool L_direction - 0 - clockwise
 *                              1 - anticlockwise
 */
CY_ISR(ENC_L1_ISR_vect){
    uint8_t encoder_L0_pin_state = Encoder_L0_Read();
    if(encoder_L0_pin_state == 0u){
        //L1 ISR has been triggered before L0.  We will interpret this as the wheel moving 
        //anticlockwise (negative/backwards)
        L_direction = ANTICLOCKWISE;
    } else {
        //L1 was triggered after L0.  Wheel is moving clockwise
        L_direction = CLOCKWISE;
    }
    //Clear the latched pin events on encoder L1 pin to enable detection of future events
    Encoder_L1_ClearInterrupt();
}

//USART ISRS
//Currently unused
/* ISR: TX_ISR_vect
 * Purpose: May be used in future to send strings to the host PC with the transmission module
 *          Currently not used.
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
        usbPutChar(byte_received);
        //USBUART_PutChar(byte_received);
        receiving_buffer_ptr[rx_buffer_size] = byte_received;
        rx_buffer_size++;
    }
}

/* *********************************************** */
/* **********************MAIN********************* */
/* *********************************************** */
int main(void)
{
    /* ********INITIALISE CODE******** */
    CyGlobalIntEnable; /* Enable global interrupts. */
    //Ensure reset lines are low initially
    PWM_RESET_Write(0b0000);
    ENCODER_TMR_RESET_Write(0b0000);
    
    //Enable the USART
    UART_Start();
    UART_EnableRxInt();
    UART_EnableTxInt();
    USBUART_Start(0,USBUART_5V_OPERATION);
    
    //Enable the PWM wheel outputs
    PWM_L_Start();
    PWM_R_Start();

    //Enable the encoder timers
    Encoder_L0_Timer_Start();
    Encoder_R0_Timer_Start();
    
    //Configure the initial pointers for the USART RX isr and the main function
    receiving_buffer_ptr = buffer_1;
    processing_buffer_ptr = buffer_2;
    uint8_t read_buffer_size = 0; //The size of the buffer the main function will read from

    //Get the robot id
    uint8_t robot_id = get_robot_ID();
    
    //Commands from USART.  Initialise to 0[m/s], 0[rad/s]
    ROBOT_COMMAND *current_command = malloc(sizeof(*current_command));
    current_command->angv = 0;
    current_command->linv = 0;
    uint8_t starting_index = 0; //Starting index of the starting bytes of the received data
    bool transmission_corrupted = false;
	
	//Initialise PID with the gain values specfied
	PID_initialise(&pid_linv,Pgain_linv,Dgain_linv,Igain_linv);
	PID_initialise(&pid_angv,Pgain_angv,Dgain_angv,Igain_angv);
    
    char *string = (char*)malloc(STR_LEN_MAX);

    //Enable and configure all interrupts
    tx_isr_StartEx(TX_ISR_vect);
    rx_isr_StartEx(RX_ISR_vect);
    ENCODER_R0_OVF_ISR_StartEx(ENCODER_R0_OVF_ISR_vect);
    ENCODER_L0_OVF_ISR_StartEx(ENCODER_L0_OVF_ISR_vect);
    ENC_R0_ISR_StartEx(ENC_R0_ISR_vect);
    ENC_L0_ISR_StartEx(ENC_L0_ISR_vect);
    ENC_R1_ISR_StartEx(ENC_R1_ISR_vect);
    ENC_L1_ISR_StartEx(ENC_L1_ISR_vect);
    
    usbPutString("Welcome to the ROBOT SOCCER TEST DEBUGGING TERMINAL\n\r");    
    sprintf(string,"Robot ID: %d\r\n",robot_id);
    usbPutString(string);
    
    /* *************RUN CODE************* */
    for(;;){
        /* Get transmission command from USART */
        //Full transmission string has been received in the buffer.  This condition needs to be tested with the Xbee module.
        if(rx_buffer_size > TRANSMISSION_STR_LENGTH){ 
            
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
            
            //Obtain the starting index of the command string in the receive buffer
            //The start bytes of the transmission are 0xFF, 0xFF and 0x01
            for(uint8_t i = 0; i < read_buffer_size; ++i){
                //To avoid reading bad memory outside of what has been allocated to the buffer check the index
                if(i + 2 < STR_LEN_MAX){
                    if(receiving_buffer_ptr[i] == (char)0xFF && receiving_buffer_ptr[i+1] == (char)0xFF && receiving_buffer_ptr[i+2] == (char)0x01){
                        starting_index = i;
                    }
                }
            }
            
            char check_sum = 0;
            //Read the receiving buffer (from receiving_buffer_ptr), parse it and update the robot command
            //Recall the packet is 48 bytes long
            //Check for accuracy in the transmission
            //The sum of the bytes plus the last checksum byte should be 0 if receieved in completion without errors
            for(uint8_t i = starting_index; i < starting_index + TRANSMISSION_PKT_SIZE - 1; ++i){
                if(i < STR_LEN_MAX){
                    check_sum += (uint8_t)receiving_buffer_ptr[i];
                }
            }
            
            //Check the checksum - if it's zero - uncorrupted
            if(check_sum != 0){
                transmission_corrupted = true;
            } else {
                transmission_corrupted = false;   
            }
            /* *Parse input command* */          
            //Each byte for each robot begins at three plus the nth multiple of 4 after the starting index of the transission.
            //The first three bytes are unrelated to speed transmission as these are the starting bytes
            //Since robot_id begins at 1 for the 1st robot, the starting index needs to begin at the (robot_id - 1) multpile of 4.
            uint8_t robot_data_index = 4 * (robot_id - 1) + 3 + starting_index;
            
            //The transmission has the least significant bits followed by the most significant bits
            //Concatenate these two bytes together to form the scaled version of the linear and angular velocity commanded from the PC
            //Two bytes for the linear velocity followed by two bytes for the angular velocity
            uint8_t linv_LSB, argv_LSB;
            int16_t linv_MSB, argv_MSB, scaled_linv, scaled_argv;
            
            //If the transmission has been corrupted, don't update the command
            if(robot_data_index + 3 < STR_LEN_MAX && transmission_corrupted == false){
                linv_MSB = (int16_t)(receiving_buffer_ptr[robot_data_index + 1] << 8);
                linv_LSB = (uint8_t)receiving_buffer_ptr[robot_data_index];
                
                argv_MSB = (int16_t)(receiving_buffer_ptr[robot_data_index + 3] << 8);
                argv_LSB = (uint8_t)receiving_buffer_ptr[robot_data_index + 2];
                
                scaled_linv = (int16_t)(linv_MSB | linv_LSB);
                scaled_argv = (int16_t)(argv_MSB | argv_LSB);
                
                current_command->linv = (double)scaled_linv / LIN_V_SCALING_FACTOR;
                current_command->angv = (double)scaled_argv / ANG_V_SCALING_FACTOR;
            } else {
                usbPutString("Robot data received for the robot in question is over the length of the receive buffer size\n\r");   
            }
            
			/* Move robot with PID control */
			//Gettting the actual angular, linear velocities of the robot through the encoder
		    double motor_angv_R = count_to_angv(timerR0_count[timerR0_index],ENCODER_COUNTS,timerR0_overflows[timerR0_index], R_direction);	
            double motor_angv_L = count_to_angv(timerL0_count[timerL0_index],ENCODER_COUNTS,timerL0_overflows[timerL0_index], L_direction);
		
			double linv_Lw = motor_angv_to_wheel_linv(motor_angv_L,GEAR_RATIO,WHEEL_DIAMETER);
			double linv_Rw = motor_angv_to_wheel_linv(motor_angv_R,GEAR_RATIO,WHEEL_DIAMETER);
			
            //Linear velocity of rebot measured from encoder
			double robot_linv = get_robot_linv(linv_Rw,linv_Lw);
            //Angular velocity of robot measured from encoder
			double robot_angv = get_robot_angv(linv_Rw,linv_Lw,ROBOT_WIDTH); 
			
			//Using PID control to adjust any velocity errors of the robot
			double new_linv = PID_update(&pid_linv,robot_linv,current_command->linv);
			double new_angv = PID_update(&pid_angv,robot_angv,current_command->angv);
			
			//Updating the new robot commands/movement
			move_robot(new_linv,new_angv,ROBOT_WIDTH,WHEEL_DIAMETER,MOTOR_MAX_RPM,GEAR_RATIO);
        }

        /* **************************************TESTING INDIVIDUAL FUNCTIONS CODE ********************************************** */
        /* Comment out when not in use */
        
        //For testing PWM output     
            
//        double vleft_1, vright_1, linv, angv;
//        
//        vleft_1 = RPM_to_radps(-0.9*MOTOR_MAX_RPM);
//        vright_1 = RPM_to_radps(-0.9*MOTOR_MAX_RPM);
//        move_left_motor(vleft_1, MOTOR_MAX_RPM);
//        move_right_motor(vright_1,MOTOR_MAX_RPM);
//        
//        linv = 8;
//        angv = 0;
//        move_robot(linv,angv,ROBOT_WIDTH,WHEEL_DIAMETER,MOTOR_MAX_RPM,GEAR_RATIO);
//        move_robot(3.0,50.0,ROBOT_WIDTH,WHEEL_DIAMETER,MOTOR_MAX_RPM,GEAR_RATIO);
//        
//        //For testing the encoder R0   
//        //Print the timer measurement to the screen
//        usbPutString("R0 index is: ");
//        sprintf(string,"%d",timerR0_index);
//        usbPutString(string);
//        usbPutString("\r\n");
//        usbPutString("The encoder count from the right wheel is: [");
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; ++i){
//            sprintf(string,"%d, ",timerR0_count[i]);
//            usbPutString(string);
//        }
//        
//        usbPutString("]\r\n");
//        usbPutString("The average encoder count from the right wheel is: ");
//        int sum = 0;
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
//                sum += timerR0_count[i];
//        }
//        sum /= ENCODER_TIMER_ARRAY_SIZE;
//        sprintf(string,"%d",sum);
//        usbPutString(string);
//        double average_ang_velocity = count_to_angv(sum,ENCODER_COUNTS,R_direction);
//        usbPutString("\r\n");
//        usbPutString("The encoder overflow from the right wheel is: [");
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; ++i){
//            sprintf(string,"%d, ",timerR0_overflows[i]);
//            usbPutString(string);
//        }
//        usbPutString("]\r\n");
//        usbPutString("The average speed of the right wheel is: ");
//        sprintf(string,"%.2f",average_ang_velocity);
//        usbPutString(string);
//        usbPutString("[rad/s]\r\n\r\n");
//        
//
//        //For testing the encoder L0 
//        //Print the timer measurement to the screen 
//        usbPutString("L0 index is: ");
//        sprintf(string,"%d",timerL0_index);
//        usbPutString(string);
//        usbPutString("\r\n");
//        usbPutString("The encoder count from the left wheel is: [");
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; ++i){
//            sprintf(string,"%d, ",timerL0_count[i]);
//            usbPutString(string);
//        }
//        usbPutString("]\r\n");
//        usbPutString("The average encoder count from the left wheel is: ");
//        sum = 0;
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
//                sum += timerL0_count[i];
//        }
//        sum /= ENCODER_TIMER_ARRAY_SIZE;
//        sprintf(string,"%d",sum);
//        usbPutString(string);
//        average_ang_velocity = count_to_angv(sum,ENCODER_COUNTS,L_direction);
//        usbPutString("\r\n");
//        usbPutString("The encoder overflow from the left wheel is: [");
//        for(uint8_t i = 0; i < ENCODER_TIMER_ARRAY_SIZE; ++i){
//            sprintf(string,"%d, ",timerL0_overflows[i]);
//            usbPutString(string);
//        }
//        usbPutString("]\r\n");
//        usbPutString("The average speed of the left wheel is: ");
//        sprintf(string,"%.2f",average_ang_velocity);
//        usbPutString(string);
//        usbPutString("[rad/s]\r\n\r\n");
//        CyDelay(200);
    }  
}/* [] END OF FILE */