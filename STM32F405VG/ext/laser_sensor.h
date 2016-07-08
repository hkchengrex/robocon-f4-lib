#ifndef __LASER_SENSOR_H
#define __LASER_SENSOR_H

#include "adc.h"
#define ls_number					3				//number of sensors used

static const u16 min_adc[ls_number] = {5, 5, 5};
static const u16 max_adc[ls_number] = {2910, 2910, 2910};
static const u16 min_dis[ls_number] = {200, 200, 200};
static const u16 max_dis[ls_number] = {5000, 5000, 5000};

u32 get_ls_cal_reading(u8 device);
u32 get_ls_adc_reading(u8 device);
 
#endif	
