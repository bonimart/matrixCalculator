#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Person {

public:
  string m_firstName;
  string m_lastName;
  string m_phoneNumber;

};

bool isValidNum(string pNum) {
  if(pNum.length() != 9 || pNum[0] == '0'){
    return false;
  }
  for(int i = 0; i < 9; i++) {
    if(pNum[i] < '0' || pNum[i] > '9') return false;
  }
  return true;
}

bool report ( const string & fileName, ostream & out )
{
  vector<Person*> people;
  ifstream inFile(fileName);
  string line;
  Person * p = nullptr;

  //cteni
  bool endReading = false;
  while(!endReading){
    getline(inFile, line);
    if(!inFile.good()){
      return false;
    }
    else if(line == ""){
      endReading = true;
      break;
    }

    p = new Person;
    stringstream s(line);
    s >> p->m_firstName;
    s >> p->m_lastName;
    s >> p->m_phoneNumber;
    if(s.fail()
      || (s >> line && !s.fail())
      || !isValidNum(p->m_phoneNumber))
    {
      out << p->m_phoneNumber << endl;
      delete p;
      for(size_t i = 0; i < people.size(); i++){
        delete people[i];
      }
      inFile.close();
      return false;
    }
    people.push_back(p);
  }

  string word;
  //dotazy
  while(getline(inFile, line) && !inFile.fail()){
    stringstream s(line);
    s >> word;
    if(s.fail() || s.bad()) word = "";
    int cnt = 0;
    for(size_t i = 0; i < people.size(); ++i){
      if(word == people[i]->m_firstName || word == people[i]->m_lastName){
        out << people[i]->m_firstName << ' ';
        out << people[i]->m_lastName << ' ';
        out << people[i]->m_phoneNumber << '\n';
        cnt++;
      }
    }
    out << "-> " << cnt << '\n';
  }

  for(size_t i = 0; i < people.size(); i++){
    delete people[i];
  }

  //delete p;
  inFile.close();
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test02.bin", oss ) == true );
  return 0;
}
#endif /* __PROGTEST__ */
