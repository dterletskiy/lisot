#pragma once

#include <string.h>
#include <map>
#include "trace/Types.hpp"



namespace trace {

   const std::size_t s_buffer_size = 2 * 1024;
   const char* const s_build_msg_error = "----- build message error -----\n";
   const std::size_t s_build_msg_error_len = strlen( s_build_msg_error );



   class Logger
   {
      private:
         Logger( );
         Logger(
               const eLogStrategy&,
               const std::string& app_name,
               const std::size_t buffer_size,
               const eLogLevel& max_log_level
            );
         static Logger* mp_instance;
      public:
         ~Logger( );
         static Logger& instance( );
         static Logger& instance(
               const eLogStrategy&,
               const std::string& app_name,
               const std::size_t buffer_size,
               const eLogLevel& max_log_level
            );

         static bool init(
               const eLogStrategy&,
               const std::string& app_name,
               const std::size_t buffer_size,
               const eLogLevel& max_log_level
            );
         static bool deinit( );

         template< typename... Args >
         void message( const eLogLevel& log_level, const char* const format, Args... args )
         {
            if( log_level > m_max_log_level )
               return;

            char message_buffer[ m_buffer_size ];
            std::size_t size = ::snprintf( message_buffer, m_buffer_size, format, args... );
            char* p_buffer = const_cast< char* >( message_buffer );
            if( 0 >= size )
            {
               size = s_build_msg_error_len;
               p_buffer = const_cast< char* >( s_build_msg_error );
            }
            else if( size >= m_buffer_size )
            {
               size = m_buffer_size;
               p_buffer[ size - 2 ] = '\n';
               p_buffer[ size - 1 ] = '\0';
            }

            switch( m_log_strategy )
            {
               case eLogStrategy::CONSOLE:
               case eLogStrategy::CONSOLE_EXT:
               {
                  ::write( STDOUT_FILENO, p_buffer, size );
                  break;
               }
               case eLogStrategy::DLT:
               {
                  #ifdef USE_DLT
                     // DLT_LOG( dlt_context( ), to_dlt( log_level ), DLT_SIZED_CSTRING( p_buffer, size ) );
                     DLT_LOG( dlt_context( ), to_dlt( log_level ), DLT_STRING( p_buffer ) );
                  #endif // USE_DLT
                  break;
               }
               case eLogStrategy::ANDROID_LOGCAT:
               {
                  #if OS_TARGET == OS_ANDROID
                     __android_log_print( to_android( log_level ), "TAG", "%s", p_buffer );
                  #endif // OS_TARGET == OS_ANDROID
                  break;
               }
               default: break;
            }
         }

         template< typename... Args >
         void message_format(
                     const eLogLevel& log_level,
                     const char* const file, const char* const function, const int line,
                     const char* const format, Args... args
            )
         {
            const std::size_t full_format_max_size = 1024;

            switch( m_log_strategy )
            {
               case eLogStrategy::CONSOLE:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE );
                  strcat( full_format, to_color( log_level ) );
                  strcat( full_format, format );
                  strcat( full_format, RESET );
                  strcat( full_format, "\n" );

                  message(
                        log_level, full_format,
                        time( eGranularity::microseconds ), getpid( ), pthread_self( ),
                        file, function, line,
                        args...
                     );
                  break;
               }
               case eLogStrategy::CONSOLE_EXT:
               {
                  tm* time_tm;
                  std::size_t milliseconds = 0;
                  local_time_of_date( time_tm, milliseconds );

                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE );
                  strcat( full_format, to_color( log_level ) );
                  strcat( full_format, format );
                  strcat( full_format, RESET );
                  strcat( full_format, "\n" );

                  message(
                        log_level, full_format,
                        time_tm->tm_year + 1900, time_tm->tm_mon + 1, time_tm->tm_mday,
                        time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec, milliseconds,
                        getpid( ), pthread_self( ),
                        file, function, line,
                        args...
                     );
                  break;
               }
               case eLogStrategy::DLT:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, "[%s:%s:%d] -> " );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message( log_level, full_format, file, function, line, args... );
                  break;
               }
               case eLogStrategy::ANDROID_LOGCAT:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, "[%s:%s:%d] -> " );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message( log_level, full_format, file, function, line, args... );
                  break;
               }
               default: break;
            }
         }

         template< typename... Args >
         static void verbose( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::VERBOSE, format, args... );
         }

         template< typename... Args >
         static void debug( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::DEBUG, format, args... );
         }

         template< typename... Args >
         static void info( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::INFO, format, args... );
         }

         template< typename... Args >
         static void warning( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::WARNING, format, args... );
         }

         template< typename... Args >
         static void error( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::ERROR, format, args... );
         }

         template< typename... Args >
         static void fatal( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::FATAL, format, args... );
         }

      private:
         std::string mp_application_name = nullptr;
         std::size_t m_buffer_size = s_buffer_size;
         eLogLevel m_max_log_level = eLogLevel::MAX;

      public:
         const eLogStrategy& log_strategy( ) const;
         void log_strategy( const eLogStrategy& );
      private:
         #if OS_TARGET == OS_ANDROID
            eLogStrategy m_log_strategy = eLogStrategy::ANDROID_LOGCAT;
         #else
            eLogStrategy m_log_strategy = eLogStrategy::CONSOLE;
         #endif

      private:
         #ifdef USE_DLT
            DltContext& dlt_context( );
            std::map< pthread_t, DltContext > m_context_map;
         #endif
   };



   inline
   const eLogStrategy& Logger::log_strategy( ) const
   {
      return m_log_strategy;
   }

   inline
   void Logger::log_strategy( const eLogStrategy& _log_strategy )
   {
      m_log_strategy = _log_strategy;

      #ifndef USE_DLT
         // Prevent DLT logging startegy in case of DLT is not used
         if( eLogStrategy::DLT == m_log_strategy )
         {
            #if OS_TARGET == OS_ANDROID
               m_log_strategy = eLogStrategy::ANDROID_LOGCAT;
            #else
               m_log_strategy = eLogStrategy::CONSOLE;
            #endif
         }
      #endif

      #if OS_TARGET != OS_ANDROID
         if( eLogStrategy::ANDROID_LOGCAT == m_log_strategy )
            m_log_strategy = eLogStrategy::CONSOLE;
      #endif
   }

} // namespace trace



#define CLASS_ABBR "MAIN"

#define TRACE_LOG( LOG_LEVEL, USER_FORMAT, ... ) \
   do { \
      ::trace::Logger::instance( ).message_format( \
         LOG_LEVEL, \
         CLASS_ABBR, __FUNCTION__, __LINE__, \
         "" USER_FORMAT, ##__VA_ARGS__ \
      ); \
   } while( false )

#undef CLASS_ABBR
