#define CATCH_CONFIG_MAIN

#include "utils.h"
#include <catch2/catch.hpp>

std::string hex_to_base64( std::string_view hex )
{
    static char constexpr tab[] = {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
    size_t      input_bytes = hex.size() / 2;
    std::string res( input_bytes / 3 * 4, '\0' );
    auto        pos_out = res.data();
    auto        bin     = to_bin( hex );
    auto        pos_in  = bin.begin();

    for( size_t n = input_bytes / 3; n--; )
    {
        *pos_out++ = tab[( pos_in[0] & 0xfc ) >> 2];
        *pos_out++ =
            tab[( ( pos_in[0] & 0x03 ) << 4 ) + ( ( pos_in[1] & 0xf0 ) >> 4 )];
        *pos_out++ =
            tab[( ( pos_in[2] & 0xc0 ) >> 6 ) + ( ( pos_in[1] & 0x0f ) << 2 )];
        *pos_out++ = tab[pos_in[2] & 0x3f];
        std::advance( pos_in, 3 );
    }

    switch( input_bytes % 3 )
    {
    case 2:
        *pos_out++ = tab[( pos_in[0] & 0xfc ) >> 2];
        *pos_out++ =
            tab[( ( pos_in[0] & 0x03 ) << 4 ) + ( ( pos_in[1] & 0xf0 ) >> 4 )];
        *pos_out++ = tab[( pos_in[1] & 0x0f ) << 2];
        *pos_out++ = '=';
        break;
    case 1:
        *pos_out++ = tab[( pos_in[0] & 0xfc ) >> 2];
        *pos_out++ = tab[( ( pos_in[0] & 0x03 ) << 4 )];
        *pos_out++ = '=';
        *pos_out++ = '=';
        break;
    case 0:
        break;
    }

    return res;
}

TEST_CASE( "to_bin check" )
{
    std::string_view     src = "49276d00";
    std::vector<uint8_t> trg = { 0x49, 0x27, 0x6d, 0x00 };

    REQUIRE( to_bin( src ) == trg );
}

TEST_CASE( "Convert hex to base64" )
{
    std::string_view src = "49276d206b696c6c696e6720796f757220627261696e206c696"
                           "b65206120706f69736f6e6f7573206d757368726f6f6d";
    std::string_view trg =
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    REQUIRE( hex_to_base64( src ) == trg );
}