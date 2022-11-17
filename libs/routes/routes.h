#pragma once

#include "../core/core.h"
#include "../llist/llist.h"

#include "../auth/auth.h"

#include <errno.h>
#include <stdio.h>
#include <wchar.h>

typedef enum Weekday
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekday;

typedef struct Time
{
    Weekday day;
    int hour;
} Time;

typedef struct Route
{
    wchar_t* name;
    wchar_t* destination;
    int state;
    LList scheduled_times;
} Route;

typedef struct _UserRoute{
    // The UseRoute is a compressed version of the Route wich consists of the first 2 leters of the name and the destination and the time
    // the time is in the format "XX:XX" where XX is a maximum of 2 digits
    wchar_t ID[2 + 2 + 2 + 1 + 2 + 1];
    // First 2 letters of the name
    // First 2 letters of the destination
    // Day (0 - 7)
    // ':'
    // Hour (0 - 23)
} UserRoute;

Result loadAllRoutes();

/**
 * @brief Frees all the routes in the system
 * 
 */
void freeRoutes();
