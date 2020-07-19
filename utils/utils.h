#ifndef CRYPTOPALS_UTILS_H
#define CRYPTOPALS_UTILS_H

#include <string>
#include <string_view>
#include <vector>

namespace utils
{

using bin_vector = std::vector<uint8_t>;

bin_vector  to_bin( std::string_view data );
std::string to_hex( const bin_vector &data );

} // namespace utils

#endif // CRYPTOPALS_UTILS_H
