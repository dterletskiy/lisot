#include "trace/Logger.hpp"



namespace {

   #ifdef USE_DLT
      std::size_t NAMELEN = 16;
   #endif

}



using namespace trace;



Logger* Logger::mp_instance = nullptr;

Logger::Logger( )
   : mp_application_name( "APPL" )
{
   log_strategy( eLogStrategy::CONSOLE );
}

Logger::Logger(
         const eLogStrategy& _log_strategy,
         const std::string& app_name,
         const std::size_t buffer_size,
         const eLogLevel& max_log_level
      )
   : mp_application_name( app_name )
   , m_buffer_size( buffer_size )
   , m_max_log_level( max_log_level )
{
   log_strategy( _log_strategy );

   #ifdef USE_DLT
      if( eLogStrategy::DLT == m_log_strategy )
      {
         const auto registerAppStatus = dlt_register_app( mp_application_name.c_str( ), "Application" );
         if( DLT_RETURN_OK != registerAppStatus )
            return;
      }
   #endif
}

Logger::~Logger( )
{
   #ifdef USE_DLT
      if( eLogStrategy::DLT == m_log_strategy )
      {
         for( auto& pair : m_context_map )
            dlt_unregister_context( &(pair.second) );
         dlt_unregister_app( );
      }
   #endif

   // Set mp_instance to nullptr for using default logging strategy
   if( this == mp_instance )
      mp_instance = nullptr;
}

#ifdef USE_DLT
DltContext& Logger::dlt_context( )
{
   const auto [iterator, success] = m_context_map.insert( { pthread_self( ), DltContext{ } } );
   if( true == success )
   {
      static thread_local char* const thread_name = new char [ NAMELEN ];
      if ( 0 != pthread_getname_np( iterator->first, thread_name, NAMELEN ) )
      {
         char thread_name_id[ 8 ] = {0};
         sprintf( thread_name_id, "%ld", iterator->first );
         dlt_register_context( &(iterator->second), thread_name_id, thread_name_id );
      }
      else
      {
         dlt_register_context( &(iterator->second), thread_name, thread_name );
      }
   }

   return iterator->second;
}
#endif

Logger& Logger::instance( )
{
   if( nullptr == mp_instance )
   {
      static Logger dummy_logger;
      return dummy_logger;
   }

   return *mp_instance;
}

Logger& Logger::instance(
      const eLogStrategy& log_strategy,
      const std::string& app_name,
      const std::size_t buffer_size,
      const eLogLevel& max_log_level
   )
{
   if( nullptr == mp_instance )
      mp_instance = new Logger( log_strategy, app_name, buffer_size, max_log_level );

   return *mp_instance;
}

bool Logger::init(
      const eLogStrategy& log_strategy,
      const std::string& app_name,
      const std::size_t buffer_size,
      const eLogLevel& max_log_level
   )
{
   instance( log_strategy, app_name, buffer_size, max_log_level );

   return true;
}

bool Logger::deinit( )
{
   if( nullptr != mp_instance )
      delete mp_instance;
   return true;
}
