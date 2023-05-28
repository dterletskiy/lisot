#include <exception>

#include "parameters.hpp"



using namespace base;



const std::string Parameters::default_delimiter{ "=" };
const std::string Parameters::default_prefix{ "--" };

const Parameters::tValueOpt Parameters::invalid_value{ std::nullopt };



struct InvalidArgument : public std::exception
{
   InvalidArgument( const std::string& argument )
      : m_argument( argument )
   { }

   const char * what( ) const throw( )
   {
      static std::string message;
      message = std::string{ "Invalid argument: " } + m_argument;
      return message.c_str( );
   }

   std::string m_argument;
};



Parameters::Parameters( int argc, char* argv[ ], const tMap& default_parameters )
{
   for( size_t count = 1; count < argc; ++count )
   {
      std::string argument( argv[ count ] );
      argument.erase( 0, default_prefix.length( ) );

      std::size_t position = argument.find( default_delimiter );
      if( std::string::npos == position )
         m_map.emplace( argument, invalid_value );
      else
         m_map.emplace( argument.substr( 0, position ), argument.substr( position + 1 ) );
   }

   for( auto& pair: default_parameters )
   {
      m_map.emplace( pair.first, pair.second );
   }
}

Parameters::tSptr Parameters::create( int argc, char* argv[ ], const tMap& default_parameters )
{
   if( nullptr == argv )
      return nullptr;

   bool result = true;
   for( size_t count = 1; count < argc; ++count )
   {
      std::string argument( argv[ count ] );

      if( 0 != argument.rfind( Parameters::default_prefix, 0 ) )
      {
         result = false;
         printf( "Invalid argument: '%s'\n", argument.c_str( ) );
      }
   }

   struct make_shared_enabler : public Parameters
   {
      make_shared_enabler( int argc, char* argv[ ], const tMap& default_parameters )
         : Parameters( argc, argv, default_parameters )
      { }
   };

   return result ? std::make_shared< make_shared_enabler >( argc, argv, default_parameters ) : nullptr;
}

bool Parameters::is_exist( const tParameter& parameter ) const
{
   return m_map.end( ) != m_map.find( parameter );
}

std::pair< Parameters::tValueOpt, bool > Parameters::value( const tParameter& parameter ) const
{
   const auto iterator = m_map.find( parameter );
   if( m_map.end( ) == iterator )
      return std::make_pair( invalid_value, false );

   return std::make_pair( iterator->second, true );
}

std::pair< Parameters::tValue, bool > Parameters::value_or( const tParameter& parameter, const tValue& default_value ) const
{
   const auto iterator = m_map.find( parameter );
   if( m_map.end( ) == iterator )
      return std::make_pair( default_value, false );

   if( invalid_value == iterator->second )
      return std::make_pair( default_value, true );

   return std::make_pair( iterator->second.value( ), true );
}

void Parameters::dump( ) const
{
   printf( "--------------- PARAMETERS BEGIN ---------------\n" );
   for( auto iterator = m_map.begin( ); iterator != m_map.end( ); ++iterator )
   {
      if( invalid_value == iterator->second )
         printf( "'%s':\n", iterator->first.c_str( ) );
      else
         printf( "'%s': '%s'\n", iterator->first.c_str( ), iterator->second.value( ).c_str( ) );
   }
   printf( "---------------- PARAMETERS END ----------------\n" );
}
