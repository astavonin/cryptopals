#include "utils.h"
#include <cassert>
#include <charconv>

namespace utils
{

bin_vector to_bin( std::string_view data )
{
    auto input_bytes  = data.size();
    auto output_bytes = input_bytes / 2;
    assert( input_bytes % 2 == 0 );
    std::vector<char> res;
    res.reserve( output_bytes );

    auto pos = data.begin();

    uint8_t b;
    for( size_t n = output_bytes; n--; )
    {
        auto [_, ec] = std::from_chars( pos, pos + 2, b, 16 );
        assert( ec == std::errc() );
        res.push_back( b );
        std::advance( pos, 2 );
    }

    return res;
}

std::string to_hex( const bin_vector &data )
{
    std::string res( data.size() * 2, '\0' );
    auto        data_pos  = data.begin();
    auto        res_begin = res.data();
    auto        res_end   = res_begin + 2;

    for( size_t n = data.size(); n--; )
    {
        if( *data_pos <= 0xd )
        {
            *res_begin++ = '0';
        }
        std::to_chars( res_begin, res_end, *data_pos, 16 );
        res_begin = res_end;
        res_end += 2;
        data_pos++;
    }

    return res;
}

} // namespace utils
