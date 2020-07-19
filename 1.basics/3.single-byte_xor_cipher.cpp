#include <algorithm>
#include <iostream>
#include <utils.h>

#include "byte_xor.h"

int main()
{
    auto data = utils::to_bin( "1b37373331363f78151b7f2b783431333d78397828372d3"
                               "63c78373e783a393b3736" );

    auto decoding_res = gen_variants( data );

    for( const auto &rec : decoding_res )
    {
        std::cout << rec << std::endl;
    }

    return 0;
}
