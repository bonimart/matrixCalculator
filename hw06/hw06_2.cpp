#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
class CDataType{
public:
  //helper method for printing into a given stream
  virtual void print(ostream & os) const = 0;
  virtual size_t getSize() const = 0;
  //polymorphic copy constructor
  virtual shared_ptr<CDataType> clone() const = 0;
  //default comparison
  virtual bool operator==(const CDataType & other) const {
    return typeid(*this) == typeid(other);
  }
  //polymorphism 8)
  virtual bool operator!=(const CDataType & other) const { return !(*this == other); }
};
//--------------------------------------------------------------------
/** function for printing into a given stream */
ostream & operator<<(ostream & os, const CDataType & dtype){
  //polymorphism 8)
  dtype.print(os);
  return os;
}
//--------------------------------------------------------------------
class CDataTypeInt : public CDataType
{
  public:
    virtual void print(ostream & os) const override { os << "int"; }
    virtual size_t getSize() const override { return 4; }

    virtual shared_ptr<CDataType> clone () const override { 
      return make_shared<CDataTypeInt>(*this); 
    }
};
//--------------------------------------------------------------------
class CDataTypeDouble : public CDataType
{
  public:
    virtual void print(ostream & os) const override { os << "double"; }
    virtual size_t getSize() const override { return 8; }

    virtual shared_ptr<CDataType> clone() const override { 
      return make_shared<CDataTypeDouble>(*this); 
    }
};
//--------------------------------------------------------------------
class CDataTypeEnum : public CDataType
{
  public:
    virtual void print(ostream & os) const override;
    virtual size_t getSize() const override { return 4; }

    virtual shared_ptr<CDataType> clone() const override { 
      return make_shared<CDataTypeEnum>(*this);
    }

    virtual bool operator==(const CDataType & other) const override;
    //enum specific methods
    CDataTypeEnum & add(const string & state);
    CDataTypeEnum & add(const char * state){
      return add(string(state));
    }
  
  protected:
    //constant look-up of elements used in add function
    unordered_set<string> states;
    //printing order is based on insertion history
    vector<string> insertHistory;
};
//-------------------
void CDataTypeEnum::print(ostream & os) const {
  os << "enum" << "{";
  for(auto d : insertHistory){
    //polymorphism 8)
    os << d;
    //no ',' after the last element
    if(d != insertHistory[insertHistory.size() - 1]) os << ",";
  }
  os << "}";
}
//-------------------
bool CDataTypeEnum::operator==(const CDataType & other) const {
  //check if other is CDataTypeEnum with the same number of states
  shared_ptr<CDataTypeEnum> ptr = dynamic_pointer_cast<CDataTypeEnum>(other.clone());
  if(ptr == nullptr || ptr -> insertHistory.size() != insertHistory.size()) return false;
  //other is CDataTypeEnum, we have to check state by state
  auto it1 = insertHistory.begin();
  auto it2 = ptr -> insertHistory.begin();
  while(it1 != insertHistory.end() && it2 != ptr -> insertHistory.end()){
    if(*it1++ != *it2++) {
      return false;
    }
  }
  return true;
  
}
//-------------------
CDataTypeEnum & CDataTypeEnum::add(const string & state){
  //check if there already is a state with given name
  auto it = states.find(state);
  if(it != states.end()) throw invalid_argument("Duplicate enum value: " + state);
  //there is no state with given name, let's add it
  states.insert(state);
  insertHistory.push_back(state);

  return *this;
}
//--------------------------------------------------------------------
class CDataTypeStruct : public CDataType
{
  public:
    virtual void print(ostream & os) const override;
    virtual size_t getSize() const override;

    virtual shared_ptr<CDataType> clone() const override { 
      return make_shared<CDataTypeStruct>(*this);
    }

    virtual bool operator==(const CDataType & other) const override;
    //struct specific methods
    CDataTypeStruct & addField(const string & name, const CDataType & dataType);
    CDataTypeStruct &  addField(const char * name, const CDataType & dataType){
      return addField(string(name), dataType);
    }
    CDataType & field(const string & name) const;
    CDataType & field(const char * name) const{
      return field(string(name));
    }    
  
  protected:
    //fast look-up and access of elements
    unordered_map<string, shared_ptr<CDataType>> fields;
    //printing order is based on insertion history
    vector<pair<string, shared_ptr<CDataType>>> insertHistory;
};
//-------------------
void CDataTypeStruct::print(ostream & os) const {
  os << "struct" << "{";
  for(const auto & [name, dtype] : insertHistory){
    //polymorphism 8)
    dtype -> print(os);
    os << name << ";";
  }
  os << "}";
}
//-------------------
size_t CDataTypeStruct::getSize() const {
  size_t sum = 0;
  //get size of every element recursively
  //polymorphism 8)
  for(const auto & [name, dtype] : insertHistory){
    sum += dtype -> getSize();
  }
  return sum;
}
//-------------------
bool CDataTypeStruct::operator==(const CDataType & other) const {
  //check if other is CDataTypeStruct with the same number of fields
  shared_ptr<CDataTypeStruct> ptr = dynamic_pointer_cast<CDataTypeStruct>(other.clone());
  if(ptr == nullptr || ptr -> insertHistory.size() != insertHistory.size()) return false;
  //other is CDataTypeStruct, we have to check every element recursively
  auto it1 = insertHistory.begin();
  auto it2 = ptr -> insertHistory.begin();
  while(it1 != insertHistory.end() && it2 != ptr -> insertHistory.end()){
    //polymorphism 8)
    if(*(it1++ -> second) != *(it2++ -> second)) return false;
  }

  return true;
}
//-------------------
CDataTypeStruct & CDataTypeStruct::addField(const string & name, const CDataType & dtype){
  //check if there already is a field with given name
  auto it = fields.find(name);
  if(it != fields.end()) throw invalid_argument("Duplicate field: " + name);
  //there is no such field, let's add it
  //polymorphism 8)
  shared_ptr<CDataType> ptr = dtype.clone();
  fields[name] = ptr;
  insertHistory.emplace_back(name, ptr);

  return *this;
}
//-------------------
CDataType & CDataTypeStruct::field(const string & name) const{
  //check if there is a field with given name
  auto it = fields.find(name);
  if(it == fields.end()){
    //there is no such field
    throw invalid_argument("Unknown field: " + name);
  }
  return *fields.at(name);
}
//--------------------------------------------------------------------
class CDataTypeArray
{
  // todo
};
class CDataTypePtr
{
  // todo
};
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
  auto it_a = a.begin();
  auto it_b = b.begin();
  while(isspace(*it_a)) it_a++;
  while(isspace(*it_b)) it_b++;

  while(it_a != a.end() && it_b != b.end()){
    if(*it_a++ != *it_b++) return false;
    while(isspace(*it_a)) it_a++;
    while(isspace(*it_b)) it_b++;

  }
  if(it_a == a.end() && it_b == b.end()) return true;
  return false;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{

  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  b . addField ( "m_Other", a );

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "  } m_Other;\n"
    "}") );

  assert ( whitespaceMatch ( b . field ( "m_Other" ), "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b . field ( "m_Other" ) . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 32 );
  b . addField ( "m_Other1", b );
  b . addField ( "m_Other2", b );
  b . addField ( "m_Other3", b );
  assert ( b . field ( "m_Other3" ) . field ( "m_Other2" ) . field ( "m_Other1" ) == b . field ( "m_Other1" ) );

  assert ( b . getSize () == 256);

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "  } m_Other;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "  } m_Other1;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "    } m_Other1;\n"
    "  } m_Other2;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "    } m_Other1;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          READY\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "        struct\n"
    "        {\n"
    "          int m_Length;\n"
    "          enum\n"
    "          {\n"
    "            NEW,\n"
    "            FIXED,\n"
    "            BROKEN,\n"
    "            DEAD\n"
    "          } m_Status;\n"
    "          double m_Ratio;\n"
    "        } m_Other;\n"
    "      } m_Other1;\n"
    "    } m_Other2;\n"
    "  } m_Other3;\n"
    "}" ) );

  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate field: m_Status"sv );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Unknown field: m_Fail"sv );
  }

  try
  {
    cout << a . field ( "m_Length" ) . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int" ));
  }

  try
  {
    cout << a . field ( "m_Status" ) . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}" ));
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"sv );
  }

  CDataTypeArray ar1 ( 10, CDataTypeInt () );
  assert ( whitespaceMatch ( ar1, "int[10]") );
  assert ( whitespaceMatch ( ar1 . element (), "int") );
  CDataTypeArray ar2 ( 11, ar1 );
  assert ( whitespaceMatch ( ar2, "int[11][10]") );
  assert ( whitespaceMatch ( ar2 . element (), "int[10]") );
  assert ( whitespaceMatch ( ar2 . element () . element (), "int") );
  CDataTypeArray ar3 ( 10, CDataTypeArray ( 20, CDataTypePtr ( CDataTypeInt () ) ) );
  assert ( whitespaceMatch ( ar3, "int*[10][20]") );
  assert ( whitespaceMatch ( ar3 . element (), "int*[20]") );
  assert ( whitespaceMatch ( ar3 . element () . element (), "int*") );
  assert ( whitespaceMatch ( ar3 . element () . element () . element (), "int") );
  CDataTypePtr ar4  ( ar1 . element () );
  assert ( whitespaceMatch ( ar4, "int*") );
  assert ( whitespaceMatch ( ar4 . element (), "int") );
  CDataTypePtr ar5  ( b . field ( "m_Status" ) );
  assert ( whitespaceMatch ( ar5, "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  READY\n"
    "}*") );
  assert ( whitespaceMatch ( ar5 . element (), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  READY\n"
    "}") );
  CDataTypePtr ar6 ( ar3 . element () . element () );
  assert ( whitespaceMatch ( ar6, "int**") );
  assert ( whitespaceMatch ( ar6 . element (), "int*") );
  assert ( whitespaceMatch ( ar6 . element () . element (), "int") );
  CDataTypePtr ar7 ( CDataTypeArray ( 50, ar6 ) );
  assert ( whitespaceMatch ( ar7, "int**(*)[50]") );
  assert ( whitespaceMatch ( ar7 . element (), "int**[50]") );
  assert ( whitespaceMatch ( ar7 . element () . element (), "int**") );
  assert ( whitespaceMatch ( ar7 . element () . element () . element (), "int*") );
  assert ( whitespaceMatch ( ar7 . element () . element () . element () . element (), "int") );
  CDataTypeArray ar8 ( 25, ar7 );
  assert ( whitespaceMatch ( ar8, "int**(*[25])[50]") );
  CDataTypePtr ar9 ( ar8 );
  assert ( whitespaceMatch ( ar9, "int**(*(*)[25])[50]") );
  a . addField ( "m_Ar1", ar1 ) .
      addField ( "m_Ar2", ar2 ) .
      addField ( "m_Ar3", ar3 ) .
      addField ( "m_Ar4", ar4 ) .
      addField ( "m_Ar5", ar5 ) .
      addField ( "m_Ar6", ar6 ) .
      addField ( "m_Ar7", ar7 ) .
      addField ( "m_Ar8", ar8 ) .
      addField ( "m_Ar9", ar9 );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "  int m_Ar1[10];\n"
    "  int m_Ar2[11][10];\n"
    "  int* m_Ar3[10][20];\n"
    "  int* m_Ar4;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  }* m_Ar5;\n"
    "  int** m_Ar6;\n"
    "  int**(* m_Ar7)[50];\n"
    "  int**(* m_Ar8[25])[50];\n"
    "  int**(*(* m_Ar9)[25])[50];\n"
    "}") );
  a . addField ( "m_Madness", CDataTypeArray ( 40, CDataTypePtr ( a ) ) );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "  int m_Ar1[10];\n"
    "  int m_Ar2[11][10];\n"
    "  int* m_Ar3[10][20];\n"
    "  int* m_Ar4;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  }* m_Ar5;\n"
    "  int** m_Ar6;\n"
    "  int**(* m_Ar7)[50];\n"
    "  int**(* m_Ar8[25])[50];\n"
    "  int**(*(* m_Ar9)[25])[50];\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    int m_Sum;\n"
    "    int m_Ar1[10];\n"
    "    int m_Ar2[11][10];\n"
    "    int* m_Ar3[10][20];\n"
    "    int* m_Ar4;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    }* m_Ar5;\n"
    "    int** m_Ar6;\n"
    "    int**(* m_Ar7)[50];\n"
    "    int**(* m_Ar8[25])[50];\n"
    "    int**(*(* m_Ar9)[25])[50];\n"
    "  }* m_Madness[40];\n"
    "}") );
  assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) == a . field ( "m_Ar9" ));
  assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) != a . field ( "m_Ar8" ));
  try
  {
    cout << ar2 . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int[11][10]" ));
  }

  try
  {
    cout << c . element () << endl;
    assert ( "element: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use element() for type: struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}" ));
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
