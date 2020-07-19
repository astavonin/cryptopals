//
// Created by astavonin on 19/07/2020.
//

#ifndef CRYPTOPALS_BYTE_XOR_H
#define CRYPTOPALS_BYTE_XOR_H

#include <string>
#include <tuple>
#include <utils.h>

using var_record = std::tuple<float, char, std::string>;

std::string             xor_with( const utils::bin_vector &data, uint8_t ch );
std::vector<var_record> gen_variants( const std::vector<unsigned char> &data,
                                      size_t limit_variants       = 3,
                                      float  chi_square_threshold = 60.f,
                                      float  string_likelihood    = .6f );

#endif // CRYPTOPALS_BYTE_XOR_H
