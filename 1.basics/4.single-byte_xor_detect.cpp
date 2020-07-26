#include <fstream>
#include <iostream>

#include "byte_xor.h"

int main()
{
    std::fstream f;

    f.open( "../4.txt", std::ios::in );
    if( !f.is_open() )
    {
        return -1;
    }
    std::string             buf;
    std::vector<var_record> all_lines;

    while( std::getline( f, buf ) )
    {
        auto data         = utils::to_bin( buf );
        auto decoding_res = gen_variant( data );
        if( decoding_res )
        {
            all_lines.emplace_back( *decoding_res );
        }
    }

    for( const auto &rec : all_lines )
    {
        std::cout << rec << std::endl;
    }

    return 0;
}