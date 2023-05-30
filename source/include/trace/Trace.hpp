#pragma once

#include "trace/Logger.hpp"



#ifdef SYS_TRACE
   #define SYS_TYPE TRACE_LOG
#else
   #define SYS_TYPE( ... )
#endif

#ifdef MSG_TRACE
   #define MSG_TYPE TRACE_LOG
#else
   #define MSG_TYPE( ... )
#endif

#define SYS_VRB( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
#define SYS_DBG( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
#define SYS_INF( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::INFO,      USER_FORMAT, ##__VA_ARGS__ )
#define SYS_WRN( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::WARNING,   USER_FORMAT, ##__VA_ARGS__ )
#define SYS_ERR( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::ERROR,     USER_FORMAT, ##__VA_ARGS__ )
#define SYS_FTL( USER_FORMAT, ... )          SYS_TYPE( trace::eLogLevel::FATAL,     USER_FORMAT, ##__VA_ARGS__ )

#define MSG_VRB( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
#define MSG_DBG( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
#define MSG_INF( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::INFO,      USER_FORMAT, ##__VA_ARGS__ )
#define MSG_WRN( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::WARNING,   USER_FORMAT, ##__VA_ARGS__ )
#define MSG_ERR( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::ERROR,     USER_FORMAT, ##__VA_ARGS__ )
#define MSG_FTL( USER_FORMAT, ... )          MSG_TYPE( trace::eLogLevel::FATAL,     USER_FORMAT, ##__VA_ARGS__ )





#define TRACE_MARKER_LINE        "----------------------------------------------"

#define SYS_MARKER( VALUE )      SYS_ERR( TRACE_MARKER_LINE " MARKER: %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_MARKER( VALUE )      MSG_ERR( TRACE_MARKER_LINE " MARKER: %s " TRACE_MARKER_LINE, #VALUE )

#define SYS_DUMP_START( VALUE )  SYS_WRN( TRACE_MARKER_LINE " START DUMP: %s " TRACE_MARKER_LINE, #VALUE )
#define SYS_DUMP_END( VALUE )    SYS_WRN( TRACE_MARKER_LINE "  END DUMP:  %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_DUMP_START( VALUE )  MSG_WRN( TRACE_MARKER_LINE " START DUMP: %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_DUMP_END( VALUE )    MSG_WRN( TRACE_MARKER_LINE "  END DUMP:  %s " TRACE_MARKER_LINE, #VALUE )

#define SYS_SIZEOF( TYPE )       SYS_DBG( "sizeof( %s ) = %zu", #TYPE, sizeof( TYPE ) );
#define MSG_SIZEOF( TYPE )       MSG_DBG( "sizeof( %s ) = %zu", #TYPE, sizeof( TYPE ) );

#define SYS_TRACE_INFORMATION \
   SYS_VRB( "system verbose level message" ); \
   SYS_INF( "system information level message" ); \
   SYS_DBG( "system debug level message" ); \
   SYS_WRN( "system warning level message" ); \
   SYS_ERR( "system error level message" ); \
   SYS_FTL( "system fatal level message" );

#define MSG_TRACE_INFORMATION \
   MSG_VRB( "application verbose level message" ); \
   MSG_INF( "application information level message" ); \
   MSG_DBG( "application debug level message" ); \
   MSG_WRN( "application warning level message" ); \
   MSG_ERR( "application error level message" ); \
   MSG_FTL( "application fatal level message" );

