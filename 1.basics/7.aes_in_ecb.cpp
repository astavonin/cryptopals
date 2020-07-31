#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>

using namespace CryptoPP;

TEST_CASE( "AES in ECB mode", "[debug]" )
{
    std::fstream f;

    f.open( "../7.txt", std::ios::in );
    REQUIRE( f.is_open() );

    std::string tmp_res( ( std::istreambuf_iterator<char>( f ) ),
                         std::istreambuf_iterator<char>() );

    std::string cipher = base64_decode( tmp_res, true );

    const char *key_str = "YELLOW SUBMARINE";
    std::string recovered;

    ECB_Mode<AES>::Decryption d;
    d.SetKey( reinterpret_cast<const byte *>( key_str ), std::strlen( key_str ) );

    StringSource s( cipher, true,
                    new StreamTransformationFilter( d, new StringSink( recovered ) ) );

    std::string       line;
    std::stringstream ss( recovered );
    std::getline( ss, line, '\n' );

    REQUIRE( line == "I'm back and I'm ringin' the bell " );
}
