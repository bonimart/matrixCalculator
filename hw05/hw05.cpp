#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
    int m_year, m_month, m_day;
    CDate ( const int y, const int m, const int d )
      : m_year(y)
      , m_month(m)
      , m_day(d) {}

    bool isValid() const{
      return !(m_year < 0 || m_month < 1 || m_month > 12 || m_day < 1 || m_day > monthLength(m_year, m_month));
    }
    
    bool operator==(const CDate & other) const {
      return tie(m_year, m_month, m_day) == tie(other.m_year, other.m_month, other.m_day);
    }
    bool operator<(const CDate & other) const{
      return tie(m_year, m_month, m_day) < tie(other.m_year, other.m_month, other.m_day);
    }
    bool operator<=(const CDate & other) const{
      return (*this == other) || (*this < other);
    }
    bool operator>(const CDate & other) const{
      return !(*this <= other);
    }
    bool operator>=(const CDate & other) const{
      return !(*this < other);
    }
    bool operator!=(const CDate & other) const{
      return !(*this == other);
    }
  private:

    //y = year; m = month; d = day
    int monthLength(const int y, const int m) const{
      switch(m){
      case 2:
        return 28 + isLeap(y);
      case 4:
        return 30;
      case 6:
        return 30;
      case 9:
        return 30;
      case 11:
        return 30;
      default:
        return 31;
      }
    }
    bool isLeap(const int y) const{
      return (!(y % 4) && y % 100) || !(y % 400);
    }
};

struct DateHash{
  size_t operator()(const CDate & date) const{
    //return (hash<int>{}(date.m_year) ^ hash<int>{}(date.m_month)) ^ hash<int>{}(date.m_day);
    return ((size_t)date.m_year)*10000 + date.m_month*100 + date.m_day;
  }
};

struct PairHash{
  size_t operator()(const pair<size_t, string> & p) const{
    return p.first ^ hash<string>{}(p.second);
  }
};

class CSupermarket
{
  public:
    CSupermarket & store(const string & name, const CDate & expiryDate, const int count);
    void sell(list<pair<string, int>> & shoppingList);
    list<pair<string,int>> expired(const CDate & date) const;

  private:
    unordered_map<string, map<CDate, int>> goodsByName;
    unordered_map<CDate, unordered_map<string, int>, DateHash> goodsByDate;
    unordered_map<size_t, unordered_set<string>> namesByLen;
    unordered_map<pair<size_t, string>, unordered_set<string>, PairHash> substrMap;

    void storeByName(const string & name, const CDate & expiryDate, const int count);
    void storeByDate(const CDate & expiryDate, const string & name, const int count);
    bool canBeSold(const pair<string, int> & item, string & alias);
    int differingNames(const string & itemName, string & name);
};
//--------------------------------------------------------------------------
/*STORE
  hleda se v unordered_mape - konstantni
  hleda se v mape - log n
  __________
  celkem log n
-----------------------
*/
void CSupermarket::storeByName(const string & name, const CDate & expiryDate, const int count){
  const auto & itName = goodsByName.find(name);
  //goods with such name aren't stored yet
  if(itName == goodsByName.end()){
    goodsByName[name] = map<CDate, int>();
    goodsByName[name][expiryDate] = count;
    const auto & itNameLen = namesByLen.find(name.length());
    if(itNameLen == namesByLen.end()){
      namesByLen[name.length()] = {name};
    }
    else{
      namesByLen[name.length()].emplace(name);
    }
    for(size_t i = 0; i < name.length(); ++i){
      string start = name.substr(0, i);
      string end = name.substr(i+1, name.length());
      //cout << start + end << endl;
      substrMap[{i, start+end}].insert(name);
    }
    return;
  }
  //goods are already stored
  const auto & itDate = goodsByName[name].find(expiryDate);
  //instance with such expiry date isn't stored yet
  if(itDate == goodsByName[name].end()){
    goodsByName[name].emplace(expiryDate, count);
    return;
  }
  //add count to an existing instance with such expiry date
  goodsByName[name][expiryDate] += count;
}

void CSupermarket::storeByDate(const CDate & expiryDate, const string & name, const int count){
  const auto & itDate = goodsByDate.find(expiryDate);
  //no goods have their expiration date same as the given date
  if(itDate == goodsByDate.end()){
    goodsByDate[expiryDate][name] = count;
    return;
  }
  //goods that expire on given date
  //auto & expiringOnDate = goodsByDate[expiryDate];
  const auto & itName = goodsByDate[expiryDate].find(name);
  if(itName == goodsByDate[expiryDate].end()){
    goodsByDate[expiryDate][name] = count;
    return;
  }
  goodsByDate[expiryDate][name] += count;
}

CSupermarket & CSupermarket::store(const string & name, const CDate & expiryDate, const int count){
  if(count < 1 || !expiryDate.isValid()){
    return *this;
  }
  storeByName(name, expiryDate, count);
  storeByDate(expiryDate, name, count);
  return *this;
}
//--------------------------------------------------------------------------
/*SELL
  iterace skrz list - n
    hledani v unordered_mape - konstantni
    hledani rozdilneho jmena - n*(delka jmena ~ 30)
  n**2
  +
  iterace skrz list - n
    iterace mapou - n * log n
    

  __________
  celkem
//--------------------------------------------------------------------------
*/
int CSupermarket::differingNames(const string & itemName, string & name){
  //count of names that differ in one character
  int count = 0;
  //int diffs;
  const size_t nameLen = itemName.length();

  //considering only names of the same length
  if(namesByLen.find(nameLen) == namesByLen.end()){
    return 0;
  }
  for(size_t i = 0; i < nameLen; ++i){
    string start = itemName.substr(0, i);
    string end = itemName.substr(i+1, nameLen);
    string substring = start + end;
    const auto & it = substrMap.find({i, substring});
    if(it != substrMap.end()){
      if(substrMap[{i, substring}].size() == 1){
        name = *substrMap[{i, substring}].begin();
        count++;
      }
      else{
        return 0;
      }
    }
  }
  /*
  for(const auto & stored : namesByLen[nameLen]){
    diffs = 0;
    //character by character comparison
    for(size_t i = 0; i < itemName.length(); ++i){
      if(stored[i] != itemName[i]){
        diffs++;
      }
      if(diffs > 1){
        break;
      }
    }
    //only one character mismatch was found, let's update name accordingly
    if(diffs == 1){
      name = stored;
      count++;
    }
    //2 or more names means we cannot determine which one to use
    if(count > 1){
      return count;
    }
  }
  */
  return count;
}

bool CSupermarket::canBeSold(const pair<string, int> & item, string & alias){
  const auto & it = goodsByName.find(item.first);
  string name = item.first;
  //even if the name is incorrect, there could be a typo, so we have to check
  if(it == goodsByName.end()){
    //there is less or more than one items differing in one character
    if(differingNames(item.first, name) != 1){
      return false;
    }
  }
  //we don't have to set a different name if there's not typo
  if(item.first != name){
    alias = name;
  }
  else{
    alias = item.first;
  }
  return true;
}

void CSupermarket::sell(list<pair<string, int>> & shoppingList){
  //to determine which items to sell and with which alias
  unordered_map<string, pair<bool, string>> toBeSold;
  string alias;
  for(const auto & item : shoppingList) {
    //it item is to be sold we have to pass in alias in case typo took place
    if(canBeSold(item, alias) && item.second > 0){
      toBeSold[item.first].first = true;
      toBeSold[item.first].second = alias;
      //cout << alias << endl;
    }
    else{
      toBeSold[item.first].first = false;
    }
  }

  for(auto it = shoppingList.begin(); it != shoppingList.end();) {
    //readable with these variables
    string & givenName = it -> first;
    int & demandedAmount = it -> second;

    //if item is to be sold, let's sell it
    if(toBeSold[givenName].first == true){
      //item alias, may differ from given name
      alias = toBeSold[givenName].second;
      if(!goodsByName[alias].empty()){
        //for(auto & [expiryDate, count] : goodsByName[alias]){
        for(auto it = goodsByName[alias].begin(); it != goodsByName[alias].end();){
          const CDate & expiryDate = it -> first;
          int & count = it -> second;
          if(demandedAmount < count){
            count -= demandedAmount;
            goodsByDate[expiryDate][alias] -= demandedAmount;
            demandedAmount = 0;
            break;
          }
          else{
            demandedAmount -= count;
            count = 0;
            goodsByDate[expiryDate].erase(alias);
            if(goodsByDate[expiryDate].empty()){
              goodsByDate.erase(expiryDate);
            }
            it = goodsByName[alias].erase(it);
          }
        }
        //demanded amount was too much for our limited storage
        if(demandedAmount > 0){
          goodsByName.erase(alias);
          namesByLen[alias.length()].erase(alias);
          for(size_t i = 0; i < alias.length(); ++i){
            string start = alias.substr(0, i);
            string end = alias.substr(i+1, alias.length());
            substrMap[{i, start+end}].erase(alias);
            if(substrMap[{i, start + end}].empty()){
              substrMap.erase({i, start+end});
            }
          }
          it++;
        }
        else{
          it = shoppingList.erase(it);
        }
      }
      else{
        it++;
      }
    }
    else{
      it++;
    }
  }
}
//--------------------------------------------------------------------------
/*EXPIRED
  RATTY MUF
  __________
  celkem
//--------------------------------------------------------------------------
*/
struct pairGreater{
  bool operator()(const pair<string, int> & a, const pair<string, int> & b) const{
    return a.second > b.second;
}
};

list<pair<string,int>> CSupermarket::expired(const CDate & date) const{
  list<pair<string, int>> goodsExpired;
  //let's just quit trying if we are given invalid date
  if(!date.isValid()){
    return goodsExpired;
  }

  vector<pair<string, int>> expiredVect;
  map<string, int> expirationMap;

  for(const auto & [expiryDate, items] : goodsByDate){
    if(expiryDate < date){
      for(const auto & [name, count] : items){
        if(expirationMap.find(name) == expirationMap.end()){
          expirationMap[name] = count;
        }
        else{
          expirationMap[name] += count;
        }
      }
    }
  }

  for(const auto & [name, count] : expirationMap){
    expiredVect.emplace_back(name, count);
  }

  sort(expiredVect.begin(), expiredVect.end(), [](const pair<string, int> & a, const pair<string, int> & b) 
    {return a.second > b.second;});

  for(const auto & item : expiredVect){
    goodsExpired.emplace_back(item);
  }

  return goodsExpired;
}

#ifndef __PROGTEST__

int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
