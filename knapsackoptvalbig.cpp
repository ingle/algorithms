#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>


int main()
{
    std::ifstream objectsfile("knapsack_big.txt");
    int W = 0;
    objectsfile>>W; // knapsack weight/size limit
    int n = 0;
    objectsfile>>n; // total number of objects

    std::cout<<W<<" "<<n<<" "<<std::endl;

    // to reduce usage from 15GB to 15MB,
    // we will only maintain 2 rows instead of all 2000, because at
    // any step we really only require the previous and current rows
    // 
    //std::vector< std::vector<int> > A(n+1, std::vector<int>(W+1,0)); 
    //
    
    std::vector< std::vector<int> > A(2, std::vector<int>(W+1,0));
    // however, note that we now lose the ability to reconstruct the
    // actual path taken, we can only give the total final optimal value

    for( int i=0; i<W+1; i++ )
    {
        A[0][i] = 0;
    }
    std::cout<<"2d A matrix initialized"<<std::endl;
    
    for( int nObj=1; nObj<=n; nObj++ )
    {
        int curVal, curWt;
        curVal = 0; curWt = 0;
        objectsfile >> curVal >> curWt;

        std::cout<<"examining obj "<<nObj<<"/"<<n<<std::endl;

        for( int nWt=0; nWt<=W; nWt++ )
        {
            if( nWt-curWt>=0 )
            {
                A[nObj%2][nWt] = std::max( A[(nObj-1)%2][nWt], A[(nObj-1)%2][nWt-curWt]+curVal );
            }
            else
            {
                A[(nObj)%2][nWt] = A[(nObj-1)%2][nWt];
            }
        }
    }
    
    objectsfile.close();

    std::cout<<"optimal knapsack value = "<<A[0][W]<<std::endl;
    return 0;
}
