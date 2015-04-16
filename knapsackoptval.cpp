#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>


int main()
{
    std::ifstream objectsfile("knapsack1.txt");
    int W = 0;
    objectsfile>>W; // knapsack weight/size limit
    int n = 0;
    objectsfile>>n; // total number of objects

    std::cout<<W<<" "<<n<<" "<<std::endl;

    std::vector< std::vector<int> > A(n+1, std::vector<int>(W+1,0));
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

        for( int nWt=0; nWt<=W; nWt++ )
        {
            if( nWt-curWt>=0 )
            {
                A[nObj][nWt] = std::max( A[nObj-1][nWt], A[nObj-1][nWt-curWt]+curVal );
            }
            else
            {
                A[nObj][nWt] = A[nObj-1][nWt];
            }
        }
    }
    
    objectsfile.close();

    std::cout<<"optimal knapsack value = "<<A[n][W]<<std::endl;
    return 0;
}
