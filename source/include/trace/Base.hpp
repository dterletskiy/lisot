#pragma once

#include "Colors.hpp"



//                                            +----------------------- Class Abbriviation
//                                            |  +-------------------- Function Name
//                                            |  |  +----------------- Line Number
//                                            |  |  |
//                                            |  |  |
//                                            |  |  |
//                                            |  |  |
#define PREFIX_FORMAT_CODE FG_LIGHT_YELLOW "[%s:%s:%d] -> "


#define PID_TID_DELIMITER "|"
//                                                                     +--------------------------- PID
//                                                                     |                 +--------- TID
//                                                                     |                 |   
//                                                                     |                 |   
#define PREFIX_FORMAT_PID_TID FG_YELLOW PID_TID_DELIMITER " " FG_RED "%#5x " FG_GREEN "%#10lx " FG_YELLOW PID_TID_DELIMITER

//                                        +-------------------------------- Foreground Colore
//                                        |            +------------------- Microseconds
//                                        |            |
//                                        |            |
#define PREFIX_FORMAT_MICROSECONDS FG_LIGHT_MAGENTA "%20lu"

//                                                               +------------------------------------------------------------ Year
//                                                               |    +------------------------------------------------------- Month
//                                                               |    |    +-------------------------------------------------- Day
//                                                               |    |    | 
//                                                               |    |    |                     +---------------------------- Hours
//                                                               |    |    |                     |    +----------------------- Minutes
//                                                               |    |    |                     |    |    +------------------ Seconds
//                                                               |    |    |                     |    |    |    +------------- Milliseconds
//                                                               |    |    |                     |    |    |    |
//                                                               |    |    |                     |    |    |    |
#define PREFIX_FORMAT_DATE_TIME_MILLISECONDS FG_LIGHT_MAGENTA "%04d-%02d-%02d " FG_LIGHT_CYAN "%02d:%02d:%02d:%06zu"

#define PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE PREFIX_FORMAT_MICROSECONDS " " PREFIX_FORMAT_PID_TID "   " PREFIX_FORMAT_CODE
#define PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE PREFIX_FORMAT_DATE_TIME_MILLISECONDS " " PREFIX_FORMAT_PID_TID "   " PREFIX_FORMAT_CODE

#define NEW_LINE RESET "\n"

#define DBG_NO( FG_MSG, BG_MSG, USER_FORMAT, ... )


