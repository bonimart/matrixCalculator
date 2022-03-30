#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class CFile
{
public:
    CFile ( void );
    CFile (const CFile &file);
    CFile operator=(const CFile &file);
    ~CFile();
    bool seek ( uint32_t offset );
    uint32_t read ( uint8_t* dst, uint32_t bytes );
    uint32_t write ( const uint8_t * src, uint32_t bytes );
    void truncate ( void );
    uint32_t fileSize ( void ) const;
    void addVersion ( void );
    bool undoVersion ( void );
    void print(){
        for(uint32_t i = 0; i < m_bytes -> len; ++i){
            cout << (int) m_bytes -> data[i] << ", ";
        }
        cout << endl;
    }
    void recursivePrint(){
        CFile * x = this;
        cout << "recursive print" << endl;
        while(x){
            x -> print();
            x = x -> prev;
        }
    }
    void recursiveDelete();
private:
    struct Bytes
        {
            uint32_t refCount;
            uint32_t len;
            uint32_t cap;
            uint8_t * data;

            
            Bytes(){
                refCount = 0;
                len = 0;
                cap = 0;
                data = nullptr;
            }
            Bytes(const Bytes & bytes){
                refCount = 1;
                len = bytes.len;
                cap = bytes.cap;
                data = new uint8_t[bytes.cap];
                for(uint32_t i = 0; i < len; ++i){
                    data[i] = bytes.data[i];
                }
            }
            ~Bytes(){
                refCount = 0;
                len = 0;
                cap = 0;
                delete [] data;
            }

        };
    
    struct Data{

    };

    Bytes * m_bytes;
    uint32_t pivot;
    CFile * prev;
    int parents;
};

CFile::CFile ( void ){
    m_bytes = new Bytes();
    m_bytes -> refCount += 1;
    pivot = 0;
    parents = 0;
    prev = nullptr;
}
CFile::CFile (const CFile &file){
    cout << "Called cpy constructor" << endl;
    m_bytes = file.m_bytes;
    m_bytes -> refCount++;
    pivot = file.pivot;
    prev = file.prev;
    parents = 0;

    CFile * x = prev;
    CFile * y;
    while(x){
        y = x -> prev;
        x -> parents++;
        x = y;
    }
}
CFile CFile::operator=(const CFile &file){
    if(m_bytes -> refCount-- == 1){
        delete m_bytes;
    }
    if(prev) delete prev;
    return CFile(file);
}
CFile::~CFile(){
    cout << m_bytes -> refCount << endl;
    if(!--(m_bytes -> refCount)){
        delete m_bytes;
    }
    m_bytes = nullptr;
    pivot = 0;
    if(prev) delete prev;
    prev = nullptr;
}
/*
void CFile::recursiveDelete(){
    CFile * x = prev;
    CFile * y;
    while(x){
        y = x -> prev;
        if(!--(x->parents)){
            delete x;
        }
        x = y;
    }
}
*/
void CFile::addVersion ( void ){
    //CFile * tmp = this -> prev;
    prev = new CFile(*this);
    /*
    prev -> pivot = this -> pivot;
    prev -> m_bytes = this -> m_bytes;
    */
    prev -> parents++;
    //m_bytes -> refCount++;
    //prev -> prev = tmp;
    
}
bool CFile::undoVersion ( void ){
    if(prev == nullptr){
        return false;
    }

    if(!--(m_bytes -> refCount)){
        delete m_bytes;
    }
    m_bytes = prev -> m_bytes;
    if(--(prev -> parents)){
        cout << m_bytes -> refCount << endl;
        m_bytes -> refCount++;
    }
    pivot = prev -> pivot;
    prev = prev -> prev;

    return true;
}
bool CFile::seek ( uint32_t offset ){
    if( offset < 0 || offset > m_bytes -> len){
        return false;
    }
    pivot = offset;
    return true;
}
uint32_t CFile::read ( uint8_t* dst, uint32_t bytes ){
    uint32_t idx = 0;
    while(pivot < m_bytes -> len && idx < bytes) {
        dst[idx++] = m_bytes -> data[pivot++];
    }
    return idx;
}
uint32_t CFile::write ( const uint8_t * src, uint32_t bytes ){
    if(m_bytes->refCount > 1 && src != nullptr){
        m_bytes -> refCount--;
        Bytes * n_bytes = new Bytes(*m_bytes);
        m_bytes = n_bytes;
    }
    for(uint32_t i = 0; i < bytes; ++i){
        if(m_bytes -> len == m_bytes->cap){
            m_bytes -> cap = (m_bytes -> cap * 2) + 10;
            uint8_t * tmp = new uint8_t[m_bytes -> cap];
            for(uint32_t i = 0; i < m_bytes -> len; ++i){
                tmp[i] = m_bytes -> data[i];
            }
            delete [] m_bytes -> data;
            m_bytes -> data = tmp;
        }
        if(pivot == m_bytes -> len) m_bytes -> len++;
        m_bytes -> data[pivot++] = *src++;

    }
    return bytes;
}
void CFile::truncate ( void ){
    if(pivot != m_bytes->len){
        if(m_bytes->refCount > 1){
            m_bytes -> refCount--;
            Bytes * tmp = new Bytes(*m_bytes);
            m_bytes = tmp;
        }
        m_bytes -> len = pivot;
    }
}
uint32_t CFile::fileSize ( void ) const{
    return m_bytes -> len;
}

#ifndef __PROGTEST__
bool writeTest (CFile & x,
                const initializer_list<uint8_t> & data,
                uint32_t wrLen )
{
 return x . write ( data . begin (), data . size () ) == wrLen;
}

bool readTest ( CFile & x, 
                const initializer_list<uint8_t> & data,
                uint32_t rdLen )
{
    uint8_t tmp[100];
    uint32_t idx = 0;

    if ( x . read ( tmp, rdLen ) != data . size ())
    {
        cout << x . read ( tmp, rdLen ) << 'x' << data.size() << endl;
        return false;
    }
    for ( auto v : data )
    if ( tmp[idx++] != v )
    return false;
    return true;
}

int main ( void )
{
    CFile f0;   
    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    f0 . addVersion(); //potom smaz
    assert ( f0 . undoVersion () ); //potom smaz
    /*
    assert ( f0 . fileSize () == 3 );
    assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 2 ));
    assert ( writeTest ( f0, { 5, 4 }, 2 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 1 ));
    assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
    assert ( f0 . seek ( 3 ));
    f0 . addVersion();
    f0.recursivePrint();
    assert ( f0 . seek ( 6 ));
    assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
    f0 . addVersion();
    assert ( f0 . seek ( 5 ));
    //assert ( f0 . undoVersion () ); //potom smaz
    
    CFile f1 ( f0 );
    f0 . truncate ();
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
    assert ( !f0 . seek ( 100 ));
    assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
    assert ( f1 . seek ( 0 ));
    assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
    f1.recursivePrint();
    assert ( f1 . undoVersion () );
    assert ( f1 . undoVersion () );
    assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
    assert ( !f1 . undoVersion () );
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
