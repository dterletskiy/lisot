#include <cstring>

#include "trace/Types.hpp"



namespace trace {

   const eLogLevel log_level_from_number( const std::size_t& number )
   {
      if( number > static_cast< std::size_t >( eLogLevel::MAX ) )
         return eLogLevel::MAX;

      return static_cast< eLogLevel >( number );
   }

   const char* const to_color( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return FG_WHITE;
         case eLogLevel::DEBUG:     return FG_LIGHT_GREEN;
         case eLogLevel::INFO:      return FG_LIGHT_CYAN;
         case eLogLevel::WARNING:   return FG_LIGHT_BLUE;
         case eLogLevel::ERROR:     return FG_LIGHT_RED;
         case eLogLevel::FATAL:     return FG_RED;
         default:                   break;
      }
      return FG_WHITE;
   }

   #if OS_TARGET == OS_ANDROID
   int to_android( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return ANDROID_LOG_VERBOSE;
         case eLogLevel::DEBUG:     return ANDROID_LOG_DEBUG;
         case eLogLevel::INFO:      return ANDROID_LOG_INFO;
         case eLogLevel::WARNING:   return ANDROID_LOG_WARN;
         case eLogLevel::ERROR:     return ANDROID_LOG_ERROR;
         case eLogLevel::FATAL:     return ANDROID_LOG_FATAL;
         default:                   break;
      }
      return ANDROID_LOG_VERBOSE;
   }
   #endif

   #ifdef USE_DLT
   DltLogLevelType to_dlt( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return DLT_LOG_INFO; // return DLT_LOG_VERBOSE;
         case eLogLevel::DEBUG:     return DLT_LOG_INFO; // return DLT_LOG_DEBUG;
         case eLogLevel::INFO:      return DLT_LOG_INFO;
         case eLogLevel::WARNING:   return DLT_LOG_WARN;
         case eLogLevel::ERROR:     return DLT_LOG_ERROR;
         case eLogLevel::FATAL:     return DLT_LOG_FATAL;
      }
      return DLT_LOG_VERBOSE;
   }
   #endif

   const char* const log_strategy_to_string( const eLogStrategy& strategy )
   {
      switch( strategy )
      {
         case eLogStrategy::CONSOLE:         return "CONSOLE";
         case eLogStrategy::CONSOLE_EXT:     return "CONSOLE_EXT";
         case eLogStrategy::DLT:             return "DLT";
         case eLogStrategy::ANDROID_LOGCAT:  return "ANDROID";
         case eLogStrategy::UNDEFINED:       return "UNDEFINED";
         default:                            return "CONSOLE";
      }
      return "CONSOLE";
   }

   const eLogStrategy log_strategy_from_string( const char* const strategy )
   {
      if     ( 0 == std::strcmp( strategy, "CONSOLE" ) )       return eLogStrategy::CONSOLE;
      else if( 0 == std::strcmp( strategy, "CONSOLE_EXT" ) )   return eLogStrategy::CONSOLE_EXT;
      else if( 0 == std::strcmp( strategy, "DLT" ) )           return eLogStrategy::DLT;
      else if( 0 == std::strcmp( strategy, "ANDROID" ) )       return eLogStrategy::ANDROID_LOGCAT;
      else if( 0 == std::strcmp( strategy, "UNDEFINED" ) )     return eLogStrategy::UNDEFINED;
      else                                                     return eLogStrategy::CONSOLE;

      return eLogStrategy::CONSOLE;
   }

} // namespace trace



namespace trace {

   void local_time_of_date( tm*& _time_tm, size_t& _milliseconds )
   {
      timeval tv;
      gettimeofday( &tv, NULL );
      _time_tm = localtime( &tv.tv_sec );
      _milliseconds = tv.tv_usec;
   }

   uint64_t time( const eGranularity gran, clockid_t clk_id )
   {
      timespec time_timespec;
      clock_gettime( clk_id, &time_timespec );
      return time_timespec.tv_sec * static_cast< size_t >( gran ) + time_timespec.tv_nsec / ( 1000000000 / static_cast< size_t >( gran ) );
   }

} // namespace trace