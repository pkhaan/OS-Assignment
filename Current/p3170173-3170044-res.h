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


const int no_seats_per_row = 10;

const int no_telephones = 3;

const int no_zone_alpha= 10;

const int no_zone_beta = 20;

const float p_zone_alpha = 0.3;

const double cost_per_seat_zoneA = 30;

const double cost_per_seat_zoneB = 20;

const int no_seat_low = 1;

const int no_seat_high = 5;

const long time_res_high = 1;

const long time_res_high = 5;

const long time_seat_low = 5;

const long time_seat_high = 13;

const long time_cash_low = 4;

const long time_cash_high = 8;

const float p_card_success = 0.9;

void print_fail_no_available_seats(){
    printf("Your reservation failed due to no availability of seats.")
}

void print_fail_credit_card(){
    printf("Your reservation failed due to credit card issues.")
}

void print_success(){

}













