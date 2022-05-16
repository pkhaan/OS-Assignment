//
// Created by peter on 5/5/2022.
//

#ifndef OS_ASSIGNMENT_P3170173_3170044_RES_H
#define OS_ASSIGNMENT_P3170173_3170044_RES_H

#endif //OS_ASSIGNMENT_P3170173_3170044_RES_H

#pragma once
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>


//DECLARATION OF CONSTANT VALUES
/*
In here we store tha data needed for the assignment
*/

#define no_seats_per_row 10;
#define no_telephones 3;
#define no_zone_alpha 10;
#define no_zone_beta 20;
#define  p_zone_alpha 0.3F;
#define  p_card_success 0.9F;
#define cost_per_seat_zoneA 30D;
#define cost_per_seat_zoneB 20D;
#define no_seat_low 1;
#define no_seat_high 5;
#define time_res_high 1L;
#define time_res_high 5L;
#define time_seat_low 5L;
#define time_seat_high 13L;
#define  time_cash_low 4L;
#define time_cash_high 8L;





//function that makes the call
void *makeCall(void *threadId);






//DECLARATION OF ASSISTING METHODS IN THE PROGRAM
/*Prints the final message for the customer if everything was successful */
void  print_complete_transaction(int cid, const int* seats, int seats_total)
{
    printf("Your reservation has been completed. \nid customer is:  %d \nand your seats are: ", cid);
    for (int i = 0; i < no_seats; i++)
    {
        if (seats[i] == cid) printf("%d seat, ", i);
    }
    printf("\nThe total amount is  %.2f € \n\n", seats_total*cost_per_seat);

}

void print_fail_no_available_seats(){
    printf("Your reservation failed due to no availability of seats.")
}

void print_fail_credit_card(){
    printf("Your reservation failed due to credit card issues.")
}














