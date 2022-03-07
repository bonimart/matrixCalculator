#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct t_Node
{
  t_Node * m_left;
  t_Node * m_right;
  string m_val;
  t_Node(){
    m_left = nullptr;
    m_right = nullptr;
    m_val = "";
  }
};

bool readBit(ifstream& ifs, uint8_t& bitCursor, char& byte){
  if(bitCursor == 0){
    bitCursor = 1 << 7;
    byte = ifs.get();
  }
  bool bit = byte & bitCursor;
  bitCursor = bitCursor >> 1;
  return bit;
}

string readUTF(ifstream& ifs, uint8_t& bitCursor, char& byte){
  string bytes = "";
  char c = '\0';
  bool bit;
  for(int i = 7; i >= 0; --i){
    bit = readBit(ifs, bitCursor, byte);
    if(!ifs.good()){
      ifs.setstate(ios_base::failbit);
      return "";
    }
    c = c + (bit << i);
  }
  int byteLength = 0;
  if((c & (1 << 7)) == 0){
    //ascii
    byteLength = 1;
    
  }
  else{
    if((c & (1 << 6)) == (1 << 6)){
      if((c & (1 << 5)) == 0){
        //2 byty
        byteLength = 2;
      }
      else{
        if((c & (1 << 4)) == 0){
          //3 byty
          byteLength = 3;
        }
        else{
          if((c & (1 << 3)) == 0){
            //4 byty
            byteLength = 4;
          }
          else{
            //fail
            ifs.setstate(ios_base::failbit);
            return "";
          }
        }
      }
    }
    else{
      //fail
      ifs.setstate(ios_base::failbit);
      return "";
    }
  }
  if(byteLength == 0){
    ifs.setstate(ios_base::failbit);
    return bytes;
  }
  bytes = c;
  for(int i = 1; i < byteLength; ++i){
    c = '\0';
    for(int i = 7; i >= 0; --i){
      bit = readBit(ifs, bitCursor, byte);
      if(!ifs.good()){
        ifs.setstate(ios_base::failbit);
        return "";
      }
      c = c + (bit << i);
    }
    if((c & (0b10 << 6)) != (0b10 << 6)){
      ifs.setstate(ios_base::failbit);
      return bytes;
    }
    bytes = bytes + c;
  }
  return bytes;
}

t_Node * mkTree(ifstream& ifs, uint8_t& bitCursor, char& byte){
  t_Node * n = new t_Node;
  bool bit = readBit(ifs, bitCursor, byte);
  if(!ifs.good()) return nullptr;
  else{
    if(bit == 1){
      string symbol = readUTF(ifs, bitCursor, byte);
      if(!ifs.good()) return nullptr;
      n -> m_val = symbol;
    }
    else if(bit == 0){
      //udelej node a pridej podstromy
      n ->m_left = mkTree(ifs, bitCursor, byte);
      n ->m_right = mkTree(ifs, bitCursor, byte);
    }
  }return n;
}

void delTree(t_Node * root){
  if(root->m_left){
    delTree(root->m_left);
  }
  if(root->m_right){
    delTree(root->m_right);
  }
  root->m_left = nullptr;
  root->m_right = nullptr;
  delete root;
}

class Tree
{
private:
  t_Node * m_root;
public:
  Tree(t_Node * root)
  : m_root(root) {}
  ~Tree(){
    delTree(m_root);
  }
  string readHuffman(ifstream& ifs, uint8_t& bitCursor, char& byte){
    t_Node * n = m_root;
    bool bit;
    while(n->m_val == ""){
      bit = readBit(ifs, bitCursor, byte);
      if(ifs.fail()) {
        return "";
      }
      else if(bit == 0 && n ->m_left){
        n = n->m_left;
      }
      else if(bit == 1 && n ->m_right){
        n = n->m_right;
      }
      else{
        ifs.setstate(ios_base::failbit);
        return "";
      }
    }
    return n->m_val;
  }
};

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  // read tree
  ifstream inFileStream(inFileName);
  if(!inFileStream.is_open()) return false;
  uint8_t bitCursor = 0;
  char byte = '\0';
  Tree huffmanTree(mkTree(inFileStream, bitCursor, byte));
  // decode input
  ofstream outFileStream(outFileName);
  bool endWriting = false;
  if(!outFileStream.good() || !inFileStream.good() || !outFileStream.is_open()) endWriting = true;
  int wordCount;
  bool bit;
  while(!endWriting){
    wordCount = 0;
    bit = readBit(inFileStream, bitCursor, byte);
    if(!inFileStream.good()) bit = 0;
    if(bit == 1){
      wordCount = 4096;
    }
    else{
      for (int i = 11; i >= 0; --i){
        bit = readBit(inFileStream, bitCursor, byte);
        if(!inFileStream.good()){
          if(wordCount != 0){
            endWriting = true;
            break;
          }
          inFileStream.close();
          outFileStream.close();
          return true;
          }
          wordCount = wordCount + (bit << i);
      }
    }
    for(int i = 0; i < wordCount; ++i){
      if(!inFileStream.good() || !outFileStream.good()){
        endWriting = true;
        break;
      }
      outFileStream << huffmanTree.readHuffman(inFileStream, bitCursor, byte);
    }
  }

  inFileStream.close();
  outFileStream.close();
  return false;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  ifstream f1(fileName1), f2(fileName2);
  char c1, c2;

  while(!f1.eof() && !f2.eof()){
    if(!f1.good() || !f2.good()) {
      return false;
    }
    c1 = f1.get();
    c2 = f2.get();
    if(c1 != c2) return false;
  }
  return true;
}

int main ( void )
{
  assert( identicalFiles("tests/test0.orig", "tests/test0.orig"));

  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );
  //tady
  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );

  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );
  
  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );
  /*
  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );
  */
  return 0;
}
#endif /* __PROGTEST__ */
