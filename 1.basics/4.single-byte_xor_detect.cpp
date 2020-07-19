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
    std::string                          buf;
    std::vector<std::vector<var_record>> all_lines;

    while( std::getline( f, buf ) )
    {
        auto data         = utils::to_bin( buf );
        auto decoding_res = gen_variants( data );
        if( !decoding_res.empty() )
        {
            all_lines.emplace_back( decoding_res );
        }
    }

    for( const auto &line : all_lines )
    {
        for( const auto &rec : line )
        {
            std::cout << "Chi-squared: " << std::get<0>( rec )
                      << ", key: " << std::get<1>( rec ) << " : "
                      << std::get<2>( rec ) << std::endl;
        }
    }

    return 0;
}