#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <range/v3/all.hpp>

#include "utils.h"

utils::bin_vector rep_xor( std::string_view input, std::string_view key )
{
    return ranges::views::zip( input, key | ranges::views::cycle ) |
           ranges::views::transform( []( auto &&vals ) {
               auto const &[l, r] = vals;
               return l ^ r;
           } ) |
           ranges::to<utils::bin_vector>();
}

TEST_CASE( "Implement repeating-key XOR" )
{
    std::string_view input = "Burning 'em, if you ain't quick and nimble\n"
                             "I go crazy when I hear a cymbal";
    std::string_view output =
        "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a65"
        "2e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
    std::string_view key = "ICE";

    REQUIRE( utils::to_hex( rep_xor( input, key ) ) == output );
}
