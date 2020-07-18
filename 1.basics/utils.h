#ifndef CRYPTOPALS_UTILS_H
#define CRYPTOPALS_UTILS_H

#include <string>
#include <string_view>
#include <vector>

std::vector<uint8_t> to_bin( std::string_view data );
std::string          to_hex( const std::vector<uint8_t> &data );

#endif // CRYPTOPALS_UTILS_H
