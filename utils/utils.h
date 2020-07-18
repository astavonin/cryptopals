#ifndef CRYPTOPALS_UTILS_H
#define CRYPTOPALS_UTILS_H

#include <string>
#include <string_view>
#include <vector>

using bin_vector = std::vector<uint8_t>;

bin_vector  to_bin( std::string_view data );
std::string to_hex( const bin_vector &data );

#endif // CRYPTOPALS_UTILS_H
