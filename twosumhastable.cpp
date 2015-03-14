#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>



long int hashFunction( long int x )
{
    return x % static_cast<long int>(124097);
}

int main()
{
    std::unordered_map<long int, long int> ht; // hash table
    std::ifstream numstream("algo1-programming_prob-2sum.txt");
    long int n;

    while( numstream >> n )
    {
        ht.insert({ n, hashFunction(n) });
    }

    std::cout<<"hashtable ready"<<std::endl;

    int howmany = 0; // how many t's are possible : final ans.

    std::unordered_map<long int, long int>::iterator findit ;
    for( long int t = -10000; t<=10000; ++t )
    {
        for( std::unordered_map<long int, long int>::iterator it = ht.begin(); it != ht.end(); ++it )
        {
            //std::cout<< t-it->first <<std::endl;
            findit = ht.find( static_cast<long int>(t - it->first) );
            //std::cout<<"findit done"<<std::endl;

            if ( (findit != ht.end()) && (findit->first != it->first) )
            {
                ++howmany;
                break;
            }
        }
        std::cout<<"t = "<<t<<" howmany = "<<howmany<<std::endl;
    }
}

