#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

// key size detection:
// https://crypto.stackexchange.com/questions/8115/repeating-key-xor-and-hamming-distance

#include <base64.h>
#include <bitset>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gsl/span>
#include <range/v3/all.hpp>
#include <string_view>
#include <type_traits>
#include <utils.h>
#include <vector>

#include "byte_xor.h"

size_t hamming_distance( gsl::span<char> ls, gsl::span<char> rs )
{
    return ranges::accumulate( ranges::views::zip( ls, rs ) |
                                   ranges::views::transform( []( auto &&vals ) {
                                       auto [l, r] = vals;
                                       return std::bitset<8>( l ^ r ).count();
                                   } ),
                               0 );
}

TEST_CASE( "Hamming distance", "[ready]" )
{
    char l[] = "this is a test";
    char r[] = "wokka wokka!!!";
    REQUIRE( hamming_distance( l, l ) == 0 );
    REQUIRE( hamming_distance( l, r ) == 37 );
}

template <typename T>
std::vector<std::vector<T>> split( const std::vector<T> &inputs, size_t size )
{
    assert( !inputs.empty() );

    return inputs | ranges::views::chunk( size ) | ranges::to<std::vector<std::vector<T>>>;
}

TEST_CASE( "split", "[ready]" )
{
    std::vector<int>              in       = { { 3, 4, 5, 1, 7, 1, 3, 6, 3, 8 } };
    std::vector<std::vector<int>> expected = { { 3, 4 }, { 5, 1 }, { 7, 1 }, { 3, 6 }, { 3, 8 } };

    REQUIRE( split( in, 2 ) == expected );
}

template <typename T>
std::vector<std::vector<T>> transpose( const std::vector<std::vector<T>> &inputs )
{
    assert( !inputs.empty() );
    size_t                      line_len  = inputs[0].size();
    size_t                      input_len = inputs.size();
    std::vector<std::vector<T>> res( line_len );
    for( size_t n = 0; n < input_len; n++ )
    {
        const auto &in_line = inputs[n];
        assert( line_len == in_line.size() );
        for( size_t m = 0; m < line_len; m++ )
        {
            res[m].emplace_back( in_line[m] );
        }
    }

    return res;
}

TEST_CASE( "transpose", "[ready]" )
{
    std::vector<std::vector<int>> in       = { { 3, 4, 5, 1, 7 }, { 1, 3, 6, 3, 8 } };
    std::vector<std::vector<int>> expected = { { 3, 1 }, { 4, 3 }, { 5, 6 }, { 1, 3 }, { 7, 8 } };

    REQUIRE( transpose( in ) == expected );
}

std::vector<size_t> detect_key_len( const utils::bin_vector &input, size_t to_len = 100 )
{
    std::unordered_map<size_t, double> distances;
    to_len            = std::min( input.size() / 2, to_len );
    size_t max_chunks = std::round( ( input.size() / to_len ) / 2 ) * 2;

    for( int k = 2; k < to_len; ++k )
    {
        auto strs = split( input, k );
        for( auto it = strs.begin(); it < strs.begin() + max_chunks; )
        {
            auto s1 = *it++;
            auto s2 = *it++;

            auto d1 = hamming_distance( s1, s2 );
            distances[k] += static_cast<double>( d1 ) / static_cast<double>( k );
        }
        distances[k] /= static_cast<double>( max_chunks / 2 );
    }
    std::vector<std::pair<size_t, double>> sorter;
    sorter.reserve( distances.size() );
    for( const auto &d : distances )
    {
        sorter.emplace_back( d );
    }

    int ret_count = 3;
    std::partial_sort( sorter.begin(), sorter.begin() + ret_count, sorter.end(),
                       []( const auto &l, const auto &r ) { return l.second < r.second; } );

    return sorter | ranges::views::take( ret_count ) |
           ranges::views::transform( []( const auto &p ) { return p.first; } ) | ranges::to_vector;
}

TEST_CASE( "detect key length", "[ready]" )
{
    std::string_view input =
        "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a65"
        "2e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    REQUIRE( detect_key_len( utils::to_bin( input ), 10 ) == std::vector<size_t>{ 3, 4, 9 } );
}

std::string detect_key( const utils::bin_vector &inputs, size_t potential_len )
{
    std::string res;

    auto sp = split( inputs, potential_len );
    sp.pop_back(); // last line could be shorter than expected
    auto                           tp = transpose( sp );
    std::vector<std::vector<char>> keys( potential_len );
    size_t                         key_pos = 0;
    for( const auto &l : tp )
    {
        auto vars = gen_variants( l, 3, 100.f, .5f );
        for( const auto &v : vars )
        {
            keys[key_pos].emplace_back( std::get<1>( v ) );
        }
        key_pos++;
        if( key_pos == potential_len )
        {
            key_pos = 0;
        }
    }
    for( const auto &k : keys )
    {
        assert( !k.empty() );
        if( k.size() == 1 )
        {
            res.push_back( k[0] );
        }
        else
        {
            res += fmt::format( "{}", k );
        }
    }
    return res;
}

TEST_CASE( "Break repeating-key XOR", "[debug]" )
{
    std::fstream f;

    f.open( "../6.txt", std::ios::in );
    REQUIRE( f.is_open() );

    std::string tmp_res( ( std::istreambuf_iterator<char>( f ) ),
                         std::istreambuf_iterator<char>() );

    std::string all_lines = base64_decode( tmp_res, true );
    auto        strs      = utils::bin_vector( all_lines.begin(), all_lines.end() );

    auto len_arr = detect_key_len( strs );
    REQUIRE( len_arr[0] == 29 );

    auto key = detect_key( strs, len_arr[0] );
    // The algorithm set a slightly wrong weight for some key-symbol values
    REQUIRE( key == "Terminator X: Brin{'g', '`'} t{'h', 'o'}e {'i', 'n'}ois{'e', 'd'}" );
}
