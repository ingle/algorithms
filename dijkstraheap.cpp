#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <queue> // for priority queue (heap with deletions)
#include <functional>

class Vertex
{
    private: 
        int m_nId;
        int m_nGreedyScore;
    public:
        Vertex() { m_nId = -1; m_nGreedyScore = -1; }
        Vertex( int id, int score ) { m_nId = id; m_nGreedyScore = score; }
        ~Vertex() {};

        int getId() const { return m_nId; }
        int getScore() const { return m_nGreedyScore; }
        void updateScore(int newScore) { m_nGreedyScore = newScore; }

        friend bool operator< (Vertex v1, Vertex v2);
        friend bool operator> (Vertex v1, Vertex v2);
        friend bool operator== (Vertex v1, Vertex v2);
        friend bool operator>= (Vertex v1, Vertex v2);
        friend bool operator<= (Vertex v1, Vertex v2);
};

bool operator< (Vertex v1, Vertex v2)
{
    return (v1.getScore() < v2.getScore()) ;
}
bool operator> (Vertex v1, Vertex v2)
{
    return (v1.getScore() > v2.getScore()) ;
}
bool operator== (Vertex v1, Vertex v2)
{
    return (v1.getScore() == v2.getScore()) ;
}
bool operator>= (Vertex v1, Vertex v2)
{
    return (v1.getScore() >= v2.getScore()) ;
}
bool operator<= (Vertex v1, Vertex v2)
{
    return (v1.getScore() <= v2.getScore()) ;
}


// Parses a line from the adjacency info text file
std::vector< std::vector<int> > getDataLine( int lineNo )
{
    assert( lineNo >= 0 && lineNo <=200 );

    std::vector< std::vector<int> > pq;
    std::vector<int> p;
    std::vector<int> q;
    
    std::string line;
    std::ifstream adjfile("dijkstraData.txt");
    for(int i=0; i<lineNo; i++)
    {
        std::getline(adjfile, line);
    }
    //std::cout<<line<<std::endl;
    adjfile.close();

    std::stringstream stream(line);
    
    bool firstnum = true;
    bool oddpos = true;

    int n;
    while( stream>>n )
    {
        if (firstnum)
        {
            firstnum = false;
            continue;
        }

        if( stream.peek() == ',' || stream.peek() == ' ' )
            stream.ignore();

        //std::cout<<n<<std::endl;
        if(oddpos)
        {
            p.push_back( n );
            oddpos = !oddpos;
        }
        else
        {
            q.push_back( n );
            oddpos = !oddpos;
        }
    }

    pq.push_back(p);
    pq.push_back(q);

    return pq;
}

int main()
{
    std::priority_queue< Vertex, std::vector<Vertex>, std::greater<Vertex> > minheap;
    minheap.push(Vertex(1, 70));
    minheap.push(Vertex(2, 23));
    minheap.push(Vertex(3, 33));

    std::cout<< minheap.top().getScore() <<std::endl; minheap.pop();
    std::cout<< minheap.top().getScore() <<std::endl; minheap.pop();
    std::cout<< minheap.top().getScore() <<std::endl;

    return 0;

    std::vector<int> X; // maintain list of processed vertexes 
    X.push_back(1);

    std::vector<bool> processed(200, false); 
    processed[0] = true; // vertex 1 is already processed
    
    std::vector<int> A; // maintain shortest distanc to each processed vertex
    A.push_back(0);

    int XSizeCur = 0;
    while( X.size() > XSizeCur && X.size() <= 200 )
    {
        XSizeCur = X.size();

        int curMin = 1000000;
        int wstar = -1;
        for( int i=0; i < X.size(); i++ ) // for each v in X
        {
            std::vector< std::vector<int> > pq = getDataLine( X[i] );
            assert( pq[0].size() == pq[1].size() );
            for ( int j=0; j < pq[0].size(); j++ )
            {
                if( processed[ pq[0][j]-1 ] )        // and each w in V-X
                    continue;
                int curDist = A[i] + pq[1][j];
                if (curDist < curMin)
                {
                    curMin = curDist;
                    wstar = pq[0][j];
                }

            }
            
        }

        if( wstar > 0 )
        {
            std::cout<<"wstar = "<<wstar<<std::endl;
            X.push_back(wstar);
            A.push_back(curMin);
            processed[wstar-1] = true;
        }

        std::cout << X.size() <<" "<< A.size()<<" "<<processed.size();
    }

    std::cout<<std::endl;

    for( int i=0; i<X.size(); i++ )
    {
        std::cout<<"("<<X[i]<<","<<A[i]<<")  ";
    }

    std::cout<<std::endl;

}
