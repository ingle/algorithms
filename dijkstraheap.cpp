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
        Vertex() 
        { 
            m_nId = -1; m_nGreedyScore = -1; 
        }

        Vertex( int id, int score ) 
        { 
            m_nId = id; m_nGreedyScore = score; 
        }
        
        Vertex( const Vertex &v ) 
        { 
            m_nId = v.getId(); 
            m_nGreedyScore = v.getScore(); 
        }

        ~Vertex() {};

        int getId() const { return m_nId; }
        int getScore() const { return m_nGreedyScore; }
        void updateScore(int newScore) { m_nGreedyScore = newScore; }

        friend bool operator< (Vertex v1, Vertex v2);
        friend bool operator> (Vertex v1, Vertex v2);
        friend bool operator== (Vertex v1, Vertex v2);
        friend bool operator>= (Vertex v1, Vertex v2);
        friend bool operator<= (Vertex v1, Vertex v2);

        friend std::ostream &operator<<( std::ostream &output, const Vertex &v )
        {
            output << "(id = "<<v.getId()<<", score = "<<v.getScore()<<")";
            return output;
        }
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

// -1 if element is not in vector. Very stupid. Should not be doing this. Should use heap with delete method!
int findElemInVect( int elem, std::vector<int> vect )
{
    int result = -1;
    for( int i = 0; i<vect.size(); ++i )
    {
        if( vect[i] == elem )
        {
            result = i ;
            break;
        }
    }
    return result;
}

int main()
{
    //std::priority_queue< Vertex, std::vector<Vertex>, std::greater<Vertex> > minheap;
    //minheap.push( Vertex(1, 70) );
    //minheap.push( Vertex(2, 23) );
    //minheap.push( Vertex(3, 33) );
    //
    //std::cout<< minheap.top().getScore() <<std::endl; minheap.pop();
    //std::cout<< minheap.top().getScore() <<std::endl; minheap.pop();
    //std::cout<< minheap.top().getScore() <<std::endl;

    //return 0;

    // Initialization phase
    std::vector<int> X; // maintain list of processed vertexes 
    X.push_back(1);

    std::vector<bool> processed(200, false); 
    
    std::vector<int> A; // maintain shortest distance to each processed vertex
    A.push_back(0);

    std::priority_queue< Vertex, std::vector<Vertex>, std::greater<Vertex> > minheap; 
    // minheap will store vertexes from V-X
    std::vector< std::vector<int> > pq = getDataLine(1);
    for (int i = 0; i<pq[0].size(); ++i )
    {
        //vertexes directly attached to vertex 1
        minheap.push( Vertex( pq[0][i], pq[1][i] ) ); 
        processed[ pq[0][i]-1 ] = true;
    }
    
    for (int line=0; line<200; ++line)
    {
        if( processed[line] == false )
        {
            minheap.push( Vertex(line+1, 1000000) );
        }
    }


    
    // Main while loop
    int XSizeCur = 0;
    while( X.size() > XSizeCur && X.size() <= 200 )
    {
        XSizeCur = X.size();

        /*
        for( int i=0; i < X.size(); i++ ) // for each v in X
        {
            std::vector< std::vector<int> > pq = getDataLine( X[i] );
            assert( pq[0].size() == pq[1].size() );
            for ( int j=0; j < pq[0].size(); ++j )
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
            
        }*/

        //linear search for min can be replaced by heap operations below:

        Vertex topv = minheap.top();
        int wstar = topv.getId();
        int curMin = topv.getScore();

        std::cout<<topv.getId()<<" "<<topv.getScore()<<" "<< wstar <<" " <<curMin<<std::endl;

        X.push_back(wstar);
        A.push_back(curMin);
        minheap.pop();

        // recompute keys for vertexes in V-X reachable from wstar
        // I'm doing this stupid tmpheap technique because priority queue has no delete method
        std::priority_queue< Vertex, std::vector<Vertex>, std::greater<Vertex> > tmpminheap;
        std::vector< std::vector<int> > wstarpq = getDataLine( wstar );

        /*std::fill( processed.begin(), processed.end(), false );
        for( int p = 0; p < wstarpq[0].size(); ++p )
        {
            processed[wstarpq[0][p]-1] = true; // ones that are true and in minheap need key update
        }*/
        
        std::cout<<"pre minheap size = "<<minheap.size()<< std::endl;
        while( minheap.size()>0 )
        {
            Vertex vtmp = minheap.top();
            minheap.pop();

            int find = findElemInVect(vtmp.getId(), wstarpq[0]);
            if( find != -1 )
            {
                vtmp.updateScore( std::min(vtmp.getScore(), curMin+wstarpq[1][find]) );
            }

            tmpminheap.push(vtmp);

            /* if( processed[ vtmp.getId()-1 ] == false )
            {
                tmpminheap.push(vtmp);
                std::cout<<"inserted element not needing update "<<vtmp<<std::endl;
            }
            else
            {
                for( int q = 0; q < wstarpq[0].size(); ++q ) 
                {
                    if( !isElemInVect( wstarpq[0][q], X ) && (curMin + wstarpq[1][q] < vtmp.getScore()) )
                    {
                        vtmp.updateScore( curMin+wstarpq[1][q] );
                    }
                }
                tmpminheap.push(vtmp);
                std::cout<<"inserted element with update "<<vtmp<<std::endl;
            } */
        }
        std::cout<<"changing minheap"<<std::endl;
        minheap = tmpminheap;
        std::cout<<"minheap changed"<<std::endl;
        std::cout<<"post minheap size = "<<minheap.size()<< std::endl;

        std::cout << X.size() <<" "<< A.size()<<" "<<processed.size()<<std::endl;
    }

    std::cout<<std::endl;

    for( int i=0; i<X.size(); i++ )
    {
        std::cout<<"("<<X[i]<<","<<A[i]<<")  ";
    }

    std::cout<<std::endl;

    return 0;
}
