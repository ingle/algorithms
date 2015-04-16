#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

//#define VERBOSE
#undef VERBOSE


class UF    {
        int *id, cnt, *sz;
    public:
        // Create an empty union find data structure with N isolated sets.
        UF(int N)   {
            cnt = N;
            id = new int[N];
            sz = new int[N];
            for(int i=0; i<N; i++)  {
                id[i] = i;
                sz[i] = 1;
            }
        }
        ~UF()   {
            delete [] id;
            delete [] sz;
        }
        // Return the id of component corresponding to object p.
        int find(int p) {
            int root = p;
            while (root != id[root])
                root = id[root];
            while (p != root) {
                int newp = id[p];
                id[p] = root;
                p = newp;
            }
            return root;
        }
        // Replace sets containing x and y with their union.
        void merge(int x, int y)    {
            int i = find(x);
            int j = find(y);
            if (i == j) return;

            // make smaller root point to larger one
            if   (sz[i] < sz[j])    { 
                id[i] = j; 
                sz[j] += sz[i]; 
            } else  { 
                id[j] = i; 
                sz[i] += sz[j]; 
            }
            cnt--;
        }
        // Are objects x and y in the same set?
        bool connected(int x, int y)    {
            return find(x) == find(y);
        }
        // Return the number of disjoint sets.
        int count() {
            return cnt;
        }
};



/*
 * Union Find implemenation:
 * Copyright 2011-2014 Lars Buitinck   
 */
 
//Disjoint set forest
class DisjSets
{
    struct Set
    {
        unsigned int parent;  // index of parent in vector
        unsigned rank;

        // Construct the i'th of an array of disjoint sets
        Set(unsigned int i) : parent(i), rank(0) { }
    };

    std::vector<Set> forest;

    public:
    DisjSets(unsigned int n)
    {
        forest.reserve(n);
        for (unsigned int i=0; i<n; i++)
            forest.push_back(Set(i));
    }

    // Find set that element i belongs to, represented as an index
    unsigned int find(unsigned int i)
    {
        if (forest[i].parent == i)
            return i;
        else {
            forest[i].parent = find(forest[i].parent);
            return forest[i].parent;
        }
    }

    // Merge. Returns true iff i and j were
    // initially in different sets.
    bool merge(unsigned int i, unsigned int j)
    {
        unsigned int root_i = find(i);
        unsigned int root_j = find(j);
        if (root_i != root_j) {
            if (forest[root_i].rank < forest[root_j].rank)
                forest[root_i].parent = root_j;
            else if (forest[root_i].rank > forest[root_j].rank)
                forest[root_j].parent = root_i;
            else {
                forest[root_i].parent = root_j;
                forest[root_j].rank += 1;
            }
            return true;
        }
        return false;
    }
};



class Cedges
{
    private:
        std::pair<unsigned int, unsigned int> m_nodes;
        unsigned long long  m_weight;
    public:
        Cedges() 
        {
            m_nodes.first = 0;
            m_nodes.second = 0;
            m_weight = 999999;
        }

        Cedges(std::pair<unsigned int, unsigned int> nodes, unsigned long long weight)
        {
            m_nodes = nodes;
            m_weight = weight;
        }
        
        Cedges( const Cedges &wl )
        {
            m_nodes = wl.getNodes();
            m_weight = wl.getWeight();
        }

        Cedges( unsigned int node1, unsigned int node2, unsigned long long weight )
        {
            m_nodes.first = node1;
            m_nodes.second = node2;
            m_weight = weight;
        }

        ~Cedges() {};

        std::pair<unsigned int, unsigned int> getNodes() const { return m_nodes;  }
        unsigned long long getWeight() const { return m_weight; }

        friend std::ostream &operator<<( std::ostream &output, const Cedges &wl )
        {
            output << "("<<wl.getNodes().first<<", "
                <<wl.getNodes().second<<"), weight= "
                <<wl.getWeight();
            return output;
        }
};

inline bool operator<  (const Cedges &v1, const Cedges &v2)
{
    return ( v1.getWeight() < v2.getWeight() ) ;
}
inline bool operator<=  (const Cedges &v1, const Cedges &v2)
{
    return ( v1.getWeight() <= v2.getWeight() ) ;
}

int main()
{
    unsigned int k = 4; // target number of clusters

    std::ifstream graphfile("clustering1.txt");
    unsigned int n;
    graphfile >> n;

    unsigned int nEdges = (n*(n-1)) >> 1;

    std::cout<<"nEdges = "<<nEdges<<std::endl;
    
#ifdef VERBOSE
    n = 10;
#endif
    
    std::vector<Cedges> allEdges;

    for ( unsigned int i=0; i<nEdges; i++ )
    {
        unsigned int node1, node2;
        unsigned long long weight;
        graphfile >> node1 >> node2 >> weight;
        allEdges.push_back( Cedges( node1, node2, weight ) );
    }

    std::cout<<"allEdges read"<<std::endl;

#ifdef VERBOSE
    for( unsigned int i=0; i<n; i++ )
    {
        std::cout << allEdges[i] << std::endl;
    }
#endif

    std::sort( allEdges.begin(), allEdges.end(), operator< );

    std::cout<<"allEdges sorted"<<std::endl;

    //DisjSets *ds = new DisjSets(n);

    UF *uf = new UF(n);

    unsigned int nMergers = 0;
    unsigned int nExam; // number of edges examined so far
    for( nExam=0; nExam<nEdges; nExam++ )
    {
        
        std::cout<< allEdges[nExam].getNodes().first << " "
            << allEdges[nExam].getNodes().second << std::endl;
        //std::cout << ds->find( allEdges[nExam].getNodes().first ) 
        //    <<" "<< ds->find( allEdges[nExam].getNodes().second ) <<std::endl;

        std::cout << uf->find( allEdges[nExam].getNodes().first ) 
            <<" "<< uf->find( allEdges[nExam].getNodes().second ) <<std::endl;

        if( uf->find( allEdges[nExam].getNodes().first ) != uf->find( allEdges[nExam].getNodes().second ) )
        //if( ds->find( allEdges[nExam].getNodes().first ) != ds->find( allEdges[nExam].getNodes().second ) )
        {
            uf->merge(allEdges[nExam].getNodes().first, allEdges[nExam].getNodes().second);
            //ds->merge(allEdges[nExam].getNodes().first, allEdges[nExam].getNodes().second);
            nMergers++;
            std::cout<<"merged! nmergers = "<<nMergers<<std::endl;
        }

        if (n - nMergers == k)
        {
            break;
        }
    }

    unsigned long long minsep = 999999;
    Cedges minedge; 

    for (nExam = 0 ; nExam<nEdges; nExam++ )
    {

        if( 
            (uf->find( allEdges[nExam].getNodes().first ) != uf->find(allEdges[nExam].getNodes().second) )
            && ( allEdges[nExam].getWeight() < minsep )
          )
        //if( 
        //    (ds->find( allEdges[nExam].getNodes().first ) != ds->find(allEdges[nExam].getNodes().second) )
        //    && ( allEdges[nExam].getWeight() < minsep )
        //  )
        {
            minsep = allEdges[nExam].getWeight();
            minedge = allEdges[nExam];
        }
    }


    //delete ds;
    delete uf;

    std::cout<<"min k sep = "<<minsep<<std::endl;

    std::cout<<minedge<<std::endl;

    return 0;
}
