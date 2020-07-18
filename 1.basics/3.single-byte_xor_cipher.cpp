#include "../utils/utils.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

static std::unordered_map<char, float> letter_frequency = {
    { 'e', 12.02 }, { 't', 9.10 }, { 'a', 8.12 }, { 'i', 7.31 }, { 'n', 6.95 },
    { 's', 6.28 },  { 'r', 6.02 }, { 'h', 5.92 }, { 'd', 4.32 }, { 'l', 3.98 },
    { 'u', 2.88 },  { 'c', 2.71 }, { 'm', 2.61 }, { 'f', 2.30 }, { 'y', 2.11 },
    { 'w', 2.09 },  { 'g', 2.03 }, { 'p', 1.82 }, { 'b', 1.49 }, { 'v', 1.11 },
    { 'k', 0.69 },  { 'x', 0.17 }, { 'q', 0.11 }, { 'j', 0.10 }, { 'z', 0.07 },
};

std::string xor_with( const bin_vector &data, char ch )
{
    std::string res( data.size(), '\0' );

    for( size_t n = 0; n < data.size(); n++ )
    {
        res[n] = data[n] ^ ch;
    }

    return res;
}

float calc_distance( const std::string &decoded, size_t total_ch )
{
    if( std::find_if( decoded.begin(), decoded.end(), []( auto ch ) {
            return !std::isprint( ch );
        } ) != decoded.end() )
    {
        return std::numeric_limits<float>::max();
    }
    std::unordered_map<char, float> storage;

    for( const auto &ch_orig : decoded )
    {
        auto ch = std::tolower( ch_orig );
        if( letter_frequency.find( ch ) != letter_frequency.end() )
        {
            storage[ch] = storage[ch] + 1;
        }
    }

    float res = 0;

    for( const auto &kv : storage )
    {
        float freq     = kv.second * 100 / total_ch;
        float exp_freq = letter_frequency[kv.first];
        res += std::abs( freq - exp_freq );
    }

    return res;
}

int main()
{
    auto data = to_bin( "1b37373331363f78151b7f2b783431333d78397828372d363c7837"
                        "3e783a393b3736" );
    auto total_ch = data.size();

    using record = std::tuple<float, char, std::string>;
    std::vector<record> decoding_res;

    for( char ch = 0; ch < std::numeric_limits<char>::max(); ch++ )
    {
        auto decoded_str = xor_with( data, ch );
        auto distance    = calc_distance( decoded_str, total_ch );
        decoding_res.emplace_back(
            std::make_tuple( distance, ch, decoded_str ) );
    }
    std::sort( decoding_res.begin(), decoding_res.end(), []( auto l, auto r ) {
        return std::get<0>( l ) < std::get<0>( r );
    } );

    for( auto it = decoding_res.begin(); it < decoding_res.begin() + 3; it++ )
    {
        std::cout << std::get<1>( *it ) << " : " << std::get<2>( *it )
                  << std::endl;
    }

    return 0;
}
