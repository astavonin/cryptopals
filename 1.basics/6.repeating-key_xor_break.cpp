#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

// key size detection:
// https://crypto.stackexchange.com/questions/8115/repeating-key-xor-and-hamming-distance

#include <bitset>
#include <range/v3/all.hpp>
#include <string_view>
#include <type_traits>
#include <utils.h>
#include <vector>

size_t hamming_distance( std::string_view ls, std::string_view rs )
{
    return ranges::accumulate( ranges::views::zip( ls, rs ) |
                                   ranges::views::transform( []( auto &&vals ) {
                                       auto [l, r] = vals;
                                       return std::bitset<8>( l ^ r ).count();
                                   } ),
                               0 );
}

TEST_CASE( "Hamming distance" )
{
    std::string_view l = "this is a test";
    std::string_view r = "wokka wokka!!!";
    REQUIRE( hamming_distance( l, l ) == 0 );
    REQUIRE( hamming_distance( l, r ) == 37 );
}

std::vector<size_t> detect_key_len( const std::vector<std::string> &strs, size_t to_len = 10 )
{
    if( strs.empty() )
    {
        return {};
    }

    // 2*2 because of BASE64 encoding, extra *2 for having at least 4 samples per line
    if( strs[0].size() / 2 * 2 * 2 < to_len )
    {
        to_len = strs[0].size() / 2;
    }

    using distance_vec = std::vector<std::vector<size_t>>;
    distance_vec distances( strs.size() );

    for( int pos = 0; pos < strs.size(); pos++ )
    {
        auto  hex_str = utils::to_bin( strs[pos] );
        auto &dist    = distances[pos];
        for( int k = 2; k < to_len; ++k )
        {
            auto begin = hex_str.begin(), end = hex_str.begin() + k;
            auto sub_str1 = std::string( begin, end );
            begin += k;
            end += k;
            auto sub_str2 = std::string( begin, end );
            begin += k;
            end += k;
            auto sub_str3 = std::string( begin, end );
            begin += k;
            end += k;
            auto sub_str4 = std::string( begin, end );

            auto d1 = hamming_distance( sub_str1, sub_str2 );
            auto d2 = hamming_distance( sub_str1, sub_str2 );
            dist.push_back( ( d1 + d2 ) / 2 / k );
        }
        ranges::sort( dist );
    }

    auto only_min = distances | ranges::views::transform( []( const auto &v ) {
                        decltype( v ) res = { v.begin(), v.begin() + 3 };
                        return res;
                    } ) |
                    ranges::to<distance_vec>;

    auto filtered_mins = only_min | ranges::views::join | ranges::views::unique |
                         ranges::views::take( 3 ) | ranges::to<std::vector<size_t>>;

    return filtered_mins;
}

TEST_CASE( "detect key length" )
{
    std::vector<std::string> input = {
        "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a65"
        "2e2c652a3124333a653e2b2027630c692b20283165286326302e27282f",
        "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a65"
        "2e2c652a3124333a653e2b2027630c692b20283165286326302e27282f",
    };

    REQUIRE( detect_key_len( input ) == std::vector<size_t>{ 2 } );
}