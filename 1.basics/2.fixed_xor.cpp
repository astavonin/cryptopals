#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "utils.h"

std::string xor_hex( std::string_view a, std::string_view b )
{
    assert( a.size() == b.size() );

    std::vector<uint8_t> buf_a = utils::to_bin( a );
    std::vector<uint8_t> buf_b = utils::to_bin( b );
    std::vector<uint8_t> buf_out( a.size() / 2, 0 );

    for( size_t n = 0; n < a.size(); n++ )
    {
        buf_out[n] = buf_a[n] ^ buf_b[n];
    }

    return utils::to_hex( buf_out );
}

TEST_CASE( "to_hex" )
{
    std::string_view     trg = "0049276d00";
    std::vector<uint8_t> src = { 0x00, 0x49, 0x27, 0x6d, 0x00 };

    REQUIRE( utils::to_hex( src ) == trg );
}

TEST_CASE( "Fixed XOR" )
{
    std::string_view src      = "1c0111001f010100061a024b53535009181c";
    std::string_view xor_with = "686974207468652062756c6c277320657965";
    std::string_view exp      = "746865206b696420646f6e277420706c6179";

    REQUIRE( xor_hex( src, xor_with ) == exp );
}