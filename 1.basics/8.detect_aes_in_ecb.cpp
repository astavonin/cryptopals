#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <fstream>
#include <unordered_set>
#include <utils.h>

using dup_info = std::pair<int, int>;

size_t check_for_duplicates( const utils::bin_vector &data )
{
    constexpr size_t block_size = 16;
    assert( data.size() % block_size == 0 );

    std::unordered_set<size_t> chunks;
    size_t                     begin = 0, end = block_size;
    while( begin < data.size() )
    {
        chunks.emplace(
            std::hash<std::string_view>{}( std::string_view( data.data() + begin, end - begin ) ) );
        begin = end;
        end += block_size;
    }

    return data.size() / block_size - chunks.size();
}

TEST_CASE( "Detect AES in ECB mode" )
{
    std::fstream f;

    f.open( "../8.txt", std::ios::in );
    REQUIRE( f.is_open() );
    std::string           buf;
    std::vector<dup_info> dup_arr;
    size_t                line_nr = 0;
    while( std::getline( f, buf ) )
    {
        line_nr++;

        auto dup = check_for_duplicates( utils::to_bin( buf ) );
        if( dup > 0 )
        {
            dup_arr.emplace_back( std::make_pair( dup, line_nr ) );
        }
    }
    REQUIRE( dup_arr.size() == 1 );
    REQUIRE( dup_arr[0] == std::make_pair( 3, 133 ) );
}