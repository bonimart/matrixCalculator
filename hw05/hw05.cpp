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

class InvalidDateException : public invalid_argument {
  public:
    InvalidDateException()
      : invalid_argument("Invalid date") {}
};

class CDate
{
  public:

    CDate ( const int y, const int m, const int d ) {
      if(!isValid(y, m, d)){
        throw InvalidDateException();
      }
      m_year = y;
      m_month = m;
      m_day = d;
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
    int m_year, m_month, m_day;

    //y = year; m = month; d = day
    bool isValid(const int y, const int m, const int d) const{
      return !(y < 0 || m < 1 || m > 12 || d < 1 || d > monthLength(y, m));
    }
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
class CSupermarket
{
  public:
    // default constructor
    // store   ( name, expiryDate, count )
    // sell    ( shoppingList )
    // expired ( date ) const

  private:
    // todo
    //nejaka mapa, klic je nazev, hodnota bude nejakej STL kontejner serazen podle data
    /*STORE
    checkne jestli zbozi se stejnym nazvem uz naskladneno neni, pokud ne prida nove, jinak pridava ke stavajicimu
      kdyz pridavam asi by bylo smart rovnou delat nejakej binary search podle expiryDate a potom tam vlozit
      ? mozna se trochu nabizi ukladat zbozi jako unordered mapu z ordered map, tak se vyhnu tomu hledani
      celkovej cost - log n (s tim ze by chtelo jeste nastudovat tu mapu)
    
    -----------------------
    */
    /*SELL
    canBeSold - checkuje jestli pujde prodat jeden item z listu
      pokud projde pro vsechny prvky, vrati sell true a odebere zbozi
      pokud nenajde primou shodu, validni je i nazev odlisny v jednom pismeni, pokud se ale najdou dva takove je to fail
      asi by bylo fajn mit tu unordered set se jmeny, aby se snadno hledaly chyby v spellingu
      celkovej cost - n
    celkovej cost n**2
    */
    /*EXPIRED
    asi ordered set s lower bound - chceme zbozi, ktere projde pred zadanym datem
    celkovej cost - n * log n
    */

};
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
