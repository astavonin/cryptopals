//
// Created by astavonin on 19/07/2020.
//

#ifndef CRYPTOPALS_BYTE_XOR_H
#define CRYPTOPALS_BYTE_XOR_H

#include <optional>
#include <ostream>
#include <string>
#include <tuple>
#include <utils.h>

using var_record = std::tuple<float, char, std::string>;

std::ostream &operator<<( std::ostream &os, const var_record &rec );

std::string               xor_with( const utils::bin_vector &data, char ch );
std::optional<var_record> gen_variant( const std::vector<char> &data,
                                       float                    chi_square_threshold = 60.f,
                                       float                    string_likelihood    = .6f );
std::vector<var_record>   gen_variants( const std::vector<char> &data, size_t limit_variants = 3,
                                        float chi_square_threshold = 60.f,
                                        float string_likelihood    = .6f );

#endif // CRYPTOPALS_BYTE_XOR_H
