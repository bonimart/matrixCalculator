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
        for(uint32_t i = 0; i < curr -> m_bytes -> len; ++i){
            cout << (int) curr -> m_bytes -> data[i] << ", ";
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
                refCount = 1;
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
            void reallocate(){
                cap = cap * 2 + 10;
                uint8_t * tmp = new uint8_t[cap];
                for(uint32_t i = 0; i < len; ++i){
                    tmp[i] = data[i];
                }
                delete [] data;
                data = tmp;
            }

        };

    struct Node{
        Bytes * m_bytes;
        uint32_t pivot;
        uint32_t parents;

        Node(){
            m_bytes = new Bytes();
            pivot = 0;
            parents = 0;
        }
        Node(const Node &node){
            m_bytes = node.m_bytes;
            m_bytes -> refCount++;
            pivot = node.pivot;
            parents = 0;

        }
        ~Node(){
            if(--(m_bytes -> refCount)){
                delete m_bytes;
            }
            pivot = 0;
            parents = 0;
        }
        void reallocate(){

        }
    };

    CFile * prev;
    Node * curr;
};

CFile::CFile ( void ){
    curr = new Node();
    prev = nullptr;
}
CFile::CFile (const CFile &file){
    curr = file.curr;
    prev = file.prev;

    CFile * x = prev;
    CFile * y;
    while(x){
        y = x -> prev;
        x -> curr -> parents++;
        x = y;
    }
}
CFile CFile::operator=(const CFile &file){
    if(curr) delete curr;
    curr = nullptr;
    if(prev) delete prev;
    prev = nullptr;
    return CFile(file);
}
CFile::~CFile(){
    delete curr;
    curr = nullptr;
    if(prev) delete prev;
    prev = nullptr;
}

void CFile::addVersion ( void ){
    prev = new CFile(*this);
    prev -> curr -> parents = 1;
}
bool CFile::undoVersion ( void ){
    if(prev == nullptr){
        return false;
    }
    delete curr;
    curr = prev -> curr;
    if(--(prev -> curr -> parents)){
        curr -> m_bytes -> refCount++;
    }
    prev = prev -> prev;
    return true;
}
bool CFile::seek ( uint32_t offset ){
    if( offset < 0 || offset > curr -> m_bytes -> len){
        return false;
    }
    curr -> pivot = offset;
    return true;
}
uint32_t CFile::read ( uint8_t* dst, uint32_t bytes ){
    uint32_t idx = 0;
    while(curr -> pivot < curr -> m_bytes -> len && idx < bytes) {
        dst[idx++] = curr -> m_bytes -> data[curr -> pivot++];
    }
    return idx;
}
uint32_t CFile::write ( const uint8_t * src, uint32_t bytes ){
    if(curr->m_bytes->refCount > 1 && src != nullptr){
        curr -> m_bytes -> refCount--;
        Bytes * n_bytes = new Bytes(*curr->m_bytes);
        curr->m_bytes = n_bytes;
    }
    for(uint32_t i = 0; i < bytes; ++i){
        if(curr -> m_bytes -> len == curr -> m_bytes->cap){
           curr -> m_bytes->reallocate();
        }
        if(curr -> pivot == curr -> m_bytes -> len) curr -> m_bytes -> len++;
        curr -> m_bytes -> data[curr -> pivot++] = *src++;
    }
    return bytes;
}
void CFile::truncate ( void ){
    if(curr -> pivot != curr->m_bytes->len){
        if(curr->m_bytes->refCount > 1){
            curr->m_bytes -> refCount--;
            Bytes * tmp = new Bytes(*curr->m_bytes);
            curr->m_bytes = tmp;
        }
        curr->m_bytes -> len = curr->pivot;
    }
}
uint32_t CFile::fileSize ( void ) const{
    return curr -> m_bytes -> len;
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
