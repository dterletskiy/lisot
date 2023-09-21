#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <memory>
#include <optional>



namespace base {

   class Parameters
   {
         // Argument name must be split with it's value by this delimiter
         static const std::string default_delimiter;
         // Each argument name must start with this prefix
         static const std::string default_prefix;

      public:
         using tArgument = std::string;
         using tParameter = std::string;
         using tValue = std::string;
         using tValueOpt = std::optional< std::string >;
         using tMap = std::map< tParameter, tValueOpt >;
         using tSptr = std::shared_ptr< Parameters >;

         static const tValueOpt invalid_value;

      private:
         Parameters( ) = default;
         Parameters( int argc, char* argv[ ], const tMap& default_parameters );
      public:
         ~Parameters( ) = default;

         static tSptr create( int argc, char* argv[ ], const tMap& default_parameters = { } );

         bool is_exist( const tParameter& parameter ) const;

         std::pair< tValueOpt, bool > value( const tParameter& parameter ) const;
         template< typename T >
            std::pair< std::optional< T >, bool > value( const tParameter& parameter ) const;

         std::pair< tValue, bool > value_or( const tParameter& parameter, const tValue& default_value ) const;
         template< typename T >
            std::pair< T, bool > value_or( const tParameter& parameter, const T& default_value ) const;

         void dump( ) const;

      private:
         tMap    m_map;
   };



   template< typename T >
   std::pair< std::optional< T >, bool > Parameters::value( const tParameter& parameter ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return std::make_pair( invalid_value, false );

      if( invalid_value == iterator->second )
         return std::make_pair( invalid_value, true );

      std::stringstream ss( iterator->second.value( ) );
      T value;
      ss >> value;

      return std::make_pair( std::make_optional< T >( value ), true );
   }

   template< typename T >
   std::pair< T, bool > Parameters::value_or( const tParameter& parameter, const T& default_value ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return std::make_pair( default_value, false );

      if( invalid_value == iterator->second )
         return std::make_pair( default_value, true );

      std::stringstream ss( iterator->second.value( ) );
      T value;
      ss >> value;

      return std::make_pair( value, true );
   }

}
