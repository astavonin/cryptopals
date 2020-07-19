//
// Created by astavonin on 19/07/2020.
//

#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "byte_xor.h"

static std::unordered_map<char, float> letter_frequency = {
    { 'e', 12.02 }, { 't', 9.10 }, { 'a', 8.12 }, { 'i', 7.31 }, { 'n', 6.95 },
    { 's', 6.28 },  { 'r', 6.02 }, { 'h', 5.92 }, { 'd', 4.32 }, { 'l', 3.98 },
    { 'u', 2.88 },  { 'c', 2.71 }, { 'm', 2.61 }, { 'f', 2.30 }, { 'y', 2.11 },
    { 'w', 2.09 },  { 'g', 2.03 }, { 'p', 1.82 }, { 'b', 1.49 }, { 'v', 1.11 },
    { 'k', 0.69 },  { 'x', 0.17 }, { 'q', 0.11 }, { 'j', 0.10 }, { 'z', 0.07 },
};

std::string xor_with( const utils::bin_vector &data, uint8_t ch )
{
    std::string res( data.size(), '\0' );

    for( size_t n = 0; n < data.size(); n++ )
    {
        res[n] = data[n] ^ ch;
    }

    return res;
}

float calc_chi_squared( const std::string &decoded, float string_likelihood )
{
    if( std::find_if( decoded.begin(), decoded.end(), []( auto ch ) {
            return !std::isprint( ch ) && !std::isspace( ch );
        } ) != decoded.end() )
    {
        return std::numeric_limits<float>::max();
    }
    std::unordered_map<char, float> storage;
    size_t                          sym_count = 0;

    for( const auto &ch_orig : decoded )
    {
        auto ch = std::tolower( ch_orig );
        if( letter_frequency.find( ch ) != letter_frequency.end() )
        {
            storage[ch] = storage[ch] + 1;
            sym_count++;
        }
    }

    if( ( static_cast<float>( sym_count ) /
          static_cast<float>( decoded.length() ) ) < string_likelihood )
    {
        return std::numeric_limits<float>::max();
    }

    // https://crypto.stackexchange.com/questions/30209/developing-algorithm-for-detecting-plain-text-via-frequency-analysis
    float res = 0.;
    for( const auto &kv : storage )
    {
        float observed = kv.second;
        float expected = letter_frequency[kv.first] / 100 * sym_count;
        float diff     = observed - expected;
        res += diff * diff / expected;
    }

    return res;
}

std::vector<var_record> gen_variants( const std::vector<unsigned char> &data,
                                      size_t limit_variants,
                                      float  chi_square_threshold,
                                      float  string_likelihood )
{
    std::vector<var_record> decoding_res;

    for( uint8_t ch = 1; ch < std::numeric_limits<uint8_t>::max(); ch++ )
    {
        auto decoded_str = xor_with( data, ch );
        auto distance    = calc_chi_squared( decoded_str, string_likelihood );
        if( distance < chi_square_threshold )
        {
            decoding_res.emplace_back(
                std::make_tuple( distance, ch, decoded_str ) );
        }
    }
    auto real_limit = std::min( limit_variants, decoding_res.size() );
    std::partial_sort( decoding_res.begin(), decoding_res.begin() + real_limit,
                       decoding_res.end(), []( const auto &l, const auto &r ) {
                           return std::get<0>( l ) < std::get<0>( r );
                       } );

    return std::vector<var_record>( decoding_res.begin(),
                                    decoding_res.begin() + real_limit );
}

std::ostream &operator<<( std::ostream &os, const var_record &rec )
{
    os << "Chi-squared: " << std::get<0>( rec )
       << ", key: " << std::get<1>( rec ) << " : " << std::get<2>( rec );
    return os;
}
