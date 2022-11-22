#pragma once

#include "../core/core.h"
#include "../llist/llist.h"

#include "../auth/auth.h"

#include <errno.h>
#include <stdio.h>
#include <wchar.h>

#define ROUTE_NAME_MAX_LENGTH 20
#define ROUTE_DESCRIPTION_MAX_LENGTH 100
#define ROUTE_HORARIO_MAX_LENGTH 2 + 1 + 2

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

typedef struct
{
    int hour;
    int minute;
} ClockedTime;

typedef struct Time
{
    Weekday day;
    ClockedTime time;
} Time;

typedef struct Route
{
    wchar_t* name;
    wchar_t* destination;
    int state;
    LList scheduled_times;
} Route;

typedef enum _RouteState{
    REQUESTED,
    TAKEN,
    COMPLETED    
} RouteState;

typedef struct _UserRoute{
    // The UseRoute is a compressed version of the Route wich consists of the first 2 leters of the name and the destination and the time
    // the time is in the format "XX:XX" where XX is a maximum of 2 digits
    wchar_t ID[2 + 2 + 2 + 1 + 2 + 1];
    // First 2 letters of the name
    // First 2 letters of the destination
    // Day (0 - 7)
    // ':'
    // Hour (0 - 23)
    Route* SelectedRoute;
    Time* SelectedTime;
} UserRoute;

Result loadAllRoutes();

/**
 * @brief Frees all the routes in the system
 * 
 */
void freeRoutes();

/**
 * @brief Returns the route with the given information after appending it to the routes list
 * 
 * @param name The name of the route
 * @param destination The destination of the route
 * @param state Disabled or enabled
 * @return Result The result of the operation
 */
Result add_route(wchar_t* name, wchar_t* destination, int state);

/**
 * @brief Returns the number of routes in the system
 * 
 * @return Result The result of the operation
 */
Result number_of_routes();

/**
 * @brief Returns the route with the given index
 * 
 * @param id The index of the route to return
 * @return Result he result of the operation
 */
Result query_route_by_id(int id);