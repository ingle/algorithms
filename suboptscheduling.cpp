#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

#undef VERBOSE
//#define VERBOSE

class Cwtlen
{
    private:
        int m_nWt;
        int m_nLen;
        int m_nScore;
    public:
        Cwtlen() 
        {
            m_nWt  = -999;
            m_nLen = -999;
            m_nScore = -999;
        }
        Cwtlen(int wt, int len)
        {
            m_nWt = wt;
            m_nLen = len;
            m_nScore = wt - len;
        }
        Cwtlen( const Cwtlen &wl )
        {
            m_nWt = wl.getWeight();
            m_nLen = wl.getLength();
            m_nScore = wl.getScore();
        }
        ~Cwtlen() {};

        int getWeight() const { return m_nWt;    }
        int getLength() const { return m_nLen;   }
        int getScore()  const { return m_nScore; }

        friend bool operator<  (Cwtlen v1, Cwtlen v2);
        friend bool operator>  (Cwtlen v1, Cwtlen v2);
        friend bool operator== (Cwtlen v1, Cwtlen v2);
        friend bool operator>= (Cwtlen v1, Cwtlen v2);
        friend bool operator<= (Cwtlen v1, Cwtlen v2);

        friend std::ostream &operator<<( std::ostream &output, const Cwtlen &wl )
        {
            output << "(wt = "<<wl.getWeight()<<", length = "<<wl.getLength()<<"), score = "<<wl.getScore();
            return output;
        }
};

bool operator< (Cwtlen v1, Cwtlen v2)
{
    return ( (v1.getScore() < v2.getScore()) || 
            (v1.getScore()==v2.getScore()) && (v1.getWeight() < v2.getWeight()) ) ;
}
bool operator> (Cwtlen v1, Cwtlen v2)
{
    return ( (v1.getScore() > v2.getScore()) || 
            (v1.getScore()==v2.getScore()) && (v1.getWeight() > v2.getWeight()) ) ;
}
bool operator== (Cwtlen v1, Cwtlen v2)
{
    return ( (v1.getScore() == v2.getScore()) && 
            (v1.getWeight() == v2.getWeight()) ) ;
}
bool operator>= (Cwtlen v1, Cwtlen v2)
{
    return ((v1>v2)||(v1==v2)) ;
}
bool operator<= (Cwtlen v1, Cwtlen v2)
{
    return ((v1<v2)||(v1==v2)) ;
}




int main()
{
    std::vector<Cwtlen> wlvec;

    std::ifstream jobsfile("jobs.txt");
    int n;
    jobsfile>>n;

#ifdef VERBOSE
    n=9;
#endif

    int wcur, lcur;
    for( int i=0; i<n; i++ )
    {
        jobsfile>>wcur; 
        jobsfile>>lcur;

        wlvec.push_back( Cwtlen(wcur, lcur) );

#ifdef VERBOSE
        std::cout<<Cwtlen(wcur,lcur)<<std::endl;
#endif
    }
    jobsfile.close();
    

    std::sort( wlvec.begin(), wlvec.end(), operator> );

#ifdef VERBOSE
    std::cout<<std::endl;
    for(int i=0; i<n; i++)
    {
        std::cout<<wlvec[i]<<std::endl;
    }
#endif

    // now calc weighted sum of completion times
    unsigned long long curComplTime = 0;
    unsigned long long wtdsum = 0;
    for( int i=0; i<n; i++ )
    {
        curComplTime = curComplTime + wlvec[i].getLength();
        wtdsum = wtdsum + curComplTime * wlvec[i].getWeight();
    }

    std::cout<<"Weighted sum of completion times = "<<wtdsum<<std::endl;

    return 0;
}
