#pragma once



#define CLR_PREFIX   "\x1B["
#define CLR_SUFFIX   "m"

#define BLACK        "0"
#define RED          "1"
#define GREEN        "2"
#define YELLOW       "3"
#define BLUE         "4"
#define MAGENTA      "5"
#define CYAN         "6"
#define GRAY         "7"

#define FG           "3"
#define FG_LIGHT     "9"
#define BG           "4"
#define BG_LIGHT     "10"




#ifdef COLORED_TRACE

   #define RESET              CLR_PREFIX  "0"                  CLR_SUFFIX
   #define BOLD               CLR_PREFIX  "1"                  CLR_SUFFIX
   #define DIM                CLR_PREFIX  "2"                  CLR_SUFFIX
   #define CURSIVE            CLR_PREFIX  "3"                  CLR_SUFFIX
   #define UNDERLINE          CLR_PREFIX  "4"                  CLR_SUFFIX
   #define BLINK              CLR_PREFIX  "5"                  CLR_SUFFIX
   #define REVERSE            CLR_PREFIX  "7"                  CLR_SUFFIX
   #define HIDDEN             CLR_PREFIX  "8"                  CLR_SUFFIX

   #define FG_BLACK           CLR_PREFIX  FG          BLACK    CLR_SUFFIX
   #define FG_RED             CLR_PREFIX  FG          RED      CLR_SUFFIX
   #define FG_GREEN           CLR_PREFIX  FG          GREEN    CLR_SUFFIX
   #define FG_YELLOW          CLR_PREFIX  FG          YELLOW   CLR_SUFFIX
   #define FG_BLUE            CLR_PREFIX  FG          BLUE     CLR_SUFFIX
   #define FG_MAGENTA         CLR_PREFIX  FG          MAGENTA  CLR_SUFFIX
   #define FG_CYAN            CLR_PREFIX  FG          CYAN     CLR_SUFFIX
   #define FG_LIGHT_GRAY      CLR_PREFIX  FG          GRAY     CLR_SUFFIX
   #define FG_DARK_GRAY       CLR_PREFIX  FG_LIGHT    BLACK    CLR_SUFFIX
   #define FG_LIGHT_RED       CLR_PREFIX  FG_LIGHT    RED      CLR_SUFFIX
   #define FG_LIGHT_GREEN     CLR_PREFIX  FG_LIGHT    GREEN    CLR_SUFFIX
   #define FG_LIGHT_YELLOW    CLR_PREFIX  FG_LIGHT    YELLOW   CLR_SUFFIX
   #define FG_LIGHT_BLUE      CLR_PREFIX  FG_LIGHT    BLUE     CLR_SUFFIX
   #define FG_LIGHT_MAGENTA   CLR_PREFIX  FG_LIGHT    MAGENTA  CLR_SUFFIX
   #define FG_LIGHT_CYAN      CLR_PREFIX  FG_LIGHT    CYAN     CLR_SUFFIX
   #define FG_WHITE           CLR_PREFIX  FG_LIGHT    GRAY     CLR_SUFFIX

   #define BG_BLACK           CLR_PREFIX  BG          BLACK    CLR_SUFFIX
   #define BG_RED             CLR_PREFIX  BG          RED      CLR_SUFFIX
   #define BG_GREEN           CLR_PREFIX  BG          GREEN    CLR_SUFFIX
   #define BG_YELLOW          CLR_PREFIX  BG          YELLOW   CLR_SUFFIX
   #define BG_BLUE            CLR_PREFIX  BG          BLUE     CLR_SUFFIX
   #define BG_MAGENTA         CLR_PREFIX  BG          MAGENTA  CLR_SUFFIX
   #define BG_CYAN            CLR_PREFIX  BG          CYAN     CLR_SUFFIX
   #define BG_LIGHT_GRAY      CLR_PREFIX  BG          GRAY     CLR_SUFFIX
   #define BG_DARK_GRAY       CLR_PREFIX  BG_LIGHT    BLACK    CLR_SUFFIX
   #define BG_LIGHT_RED       CLR_PREFIX  BG_LIGHT    RED      CLR_SUFFIX
   #define BG_LIGHT_GREEN     CLR_PREFIX  BG_LIGHT    GREEN    CLR_SUFFIX
   #define BG_LIGHT_YELLOW    CLR_PREFIX  BG_LIGHT    YELLOW   CLR_SUFFIX
   #define BG_LIGHT_BLUE      CLR_PREFIX  BG_LIGHT    BLUE     CLR_SUFFIX
   #define BG_LIGHT_MAGENTA   CLR_PREFIX  BG_LIGHT    MAGENTA  CLR_SUFFIX
   #define BG_LIGHT_CYAN      CLR_PREFIX  BG_LIGHT    CYAN     CLR_SUFFIX
   #define BG_WHITE           CLR_PREFIX  BG_LIGHT    GRAY     CLR_SUFFIX

#else

   #define RESET              ""
   #define BOLD               ""
   #define DIM                ""
   #define CURSIVE            ""
   #define UNDERLINE          ""
   #define BLINK              ""
   #define REVERSE            ""
   #define HIDDEN             ""

   #define FG_BLACK           ""
   #define FG_RED             ""
   #define FG_GREEN           ""
   #define FG_YELLOW          ""
   #define FG_BLUE            ""
   #define FG_MAGENTA         ""
   #define FG_CYAN            ""
   #define FG_LIGHT_GRAY      ""
   #define FG_DARK_GRAY       ""
   #define FG_LIGHT_RED       ""
   #define FG_LIGHT_GREEN     ""
   #define FG_LIGHT_YELLOW    ""
   #define FG_LIGHT_BLUE      ""
   #define FG_LIGHT_MAGENTA   ""
   #define FG_LIGHT_CYAN      ""
   #define FG_WHITE           ""

   #define BG_BLACK           ""
   #define BG_RED             ""
   #define BG_GREEN           ""
   #define BG_YELLOW          ""
   #define BG_BLUE            ""
   #define BG_MAGENTA         ""
   #define BG_CYAN            ""
   #define BG_LIGHT_GRAY      ""
   #define BG_DARK_GRAY       ""
   #define BG_LIGHT_RED       ""
   #define BG_LIGHT_GREEN     ""
   #define BG_LIGHT_YELLOW    ""
   #define BG_LIGHT_BLUE      ""
   #define BG_LIGHT_MAGENTA   ""
   #define BG_LIGHT_CYAN      ""
   #define BG_WHITE           ""

#endif

