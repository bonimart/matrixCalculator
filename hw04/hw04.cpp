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
        Node * x = this -> curr;
        cout << "Vyhazuji rekurzivni print:" << endl;
        while(x){
            cout << "Rodice: " << x ->parents << "; ";
            for(uint32_t i = 0; i < curr -> m_bytes -> len; ++i){
            cout << (int) curr -> m_bytes -> data[i] << ", ";
            }
            cout << endl;
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
        Node * prev;

        Node(){
            m_bytes = nullptr;
            pivot = 0;
            parents = 0;
            prev = nullptr;
        }
        Node(const Node &node){
            m_bytes = node.m_bytes;
            m_bytes -> refCount++;
            pivot = node.pivot;
            parents = 0;
            prev = node.prev;

        }
        ~Node(){
            //cout << "Volam node destructor, s refcount: ";
            //cout << m_bytes -> refCount << endl;
            if(!--m_bytes -> refCount) delete [] m_bytes -> data;
            pivot = 0;
            parents = 0;
        }
    };

    Node * curr;
};

CFile::CFile ( void ){
    curr = new Node();
    curr -> m_bytes = new Bytes();
}
CFile::CFile (const CFile &file){
    //cout << "Copying CFile" << endl;
    curr = new Node();
    curr -> m_bytes = file.curr -> m_bytes;
    curr -> m_bytes ->refCount++;
    cout << "REFS: " << (int) curr -> m_bytes ->refCount << endl;
    curr -> pivot = file.curr -> pivot;
    curr -> prev = file.curr -> prev;

    Node * x = curr -> prev;
    Node * y;
    while(x){
        y = x -> prev;
        x -> parents++;
        x = y;
    }
}
CFile CFile::operator=(const CFile &file){
    Node * nxt = nullptr; 
    while(curr){
        nxt = curr -> prev;
        if(curr -> parents <= 1){
            delete curr;
        }
        else{
            curr -> parents--;
        }
        curr = nxt;
    }
    curr = nullptr;
    return CFile(file);
}
CFile::~CFile(){
    Node * nxt = nullptr; 
    while(curr){
        nxt = curr -> prev;
        cout << "Parents: " << curr -> parents << endl;
        if(curr -> parents <= 1){
            delete curr;
        }
        else{
            curr -> parents--;
        }
        curr = nxt;
    }
    curr = nullptr;
}

void CFile::addVersion ( void ){
    Node * tmp = curr -> prev;
    curr -> prev = new Node();
    //curr -> prev -> m_bytes = new Bytes();
    curr -> prev -> m_bytes = curr -> m_bytes;
    curr -> prev -> m_bytes ->refCount++;
    curr -> prev -> parents = 1;
    curr -> prev -> pivot = curr -> pivot;
    curr -> prev -> prev = tmp;

}
bool CFile::undoVersion ( void ){
    if(curr -> prev == nullptr){
        return false;
    }

    Node * tmp = curr -> prev;

    if(!curr -> parents){
        delete curr;      
    }
    else{
        curr -> m_bytes -> refCount--;
    }

    if(--(tmp -> parents)){
        curr = new Node();
        curr -> m_bytes = tmp -> m_bytes;
        curr -> m_bytes ->refCount++;
        curr -> parents = 0;
        curr -> pivot = tmp -> pivot;
    }
    else{
        curr = tmp;
    }
    curr -> prev = tmp -> prev;
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
        if(!src) break;
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
    assert ( f0 . seek ( 6 ));
    assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
    f0 . addVersion();
    assert ( f0 . seek ( 5 ));
    
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
    assert ( f1 . undoVersion () );
    assert ( f1 . undoVersion () );
    assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
    assert ( !f1 . undoVersion () );

    CFile f2 = f1;
    assert ( writeTest ( f2, { 60, 70, 80 }, 3 ) );
    f2.addVersion();
    CFile f3(f2);
    assert ( f2 . undoVersion () );
    assert ( writeTest ( f2, { 60, 70, 80 }, 3 ) );
    assert ( f3 . undoVersion () );

    CFile f4;
    f4.addVersion();
    CFile f5(f4);
    assert(!f4.seek(2));
    assert ( writeTest ( f4, { 60, 70, 80 }, 3 ) );
    assert(f4.seek(1));
    f4.truncate();
    assert(f5.undoVersion());
    assert(!f5.undoVersion());

    CFile f6;   
    assert ( writeTest ( f6, { 1 }, 1 ) );
    f6.addVersion();
    f6.recursivePrint();
    assert ( writeTest ( f6, { 2, 3, 4, 5}, 4 ) );
    f6.addVersion();
    f6.recursivePrint();
    CFile f7 = f6;
    f6.recursivePrint();
    //CFile f8 = f7;
    //assert(f7.undoVersion());
    //assert ( writeTest ( f8, { 2, 3, 4, 5}, 4 ) );
    

    
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
