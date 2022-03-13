#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


class Company{
      public:
        string m_name, m_addr, m_taxID;
        unsigned int m_sum;
        Company(const string & name, const string & addr, const string & taxID)
        : m_name(name)
        , m_addr(addr)
        , m_taxID(taxID) {
          m_sum = 0;
        }
};

class CVATRegister
{
  private:
    //trida pro firmu
    //pole castek z faktur, serazene od nejmensi hodnoty k nejvetsi
    vector<unsigned int> m_invoiceArr;
    //lexikograficky serazene pole dvojic (jmeno, adresa)
    vector<Company *> m_nameAdrArr;
    //lexikograficky serazene pole id firem
    vector<Company *> m_taxIDArr;
  
  public:
    //tady asi nic byt nemusi
    CVATRegister   ( void ) {}
    //smaz vsechny Company objekty
    ~CVATRegister  ( void ) {
      for(size_t i = 0; i < m_nameAdrArr.size(); ++i){
        delete m_nameAdrArr[i];
      }
      m_invoiceArr.clear();
      m_nameAdrArr.clear();
      m_taxIDArr.clear();
    }

    //prvne zkus  najit Company, pokud neni v seznamu, tak pridej a dej true, jinak false
    //musi se tu prohledavat oba seznamy
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );
    //najdi Company v jednom ze seznamu, pokud najdes, tak ji zrus (zase z obou seznamu), jinak false
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    bool          cancelCompany  ( const string    & taxID );
    //najdi Company, pridej k jeji sum "amount", pridej "amount" do auditArr a sesorti ji
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );
                              
    //najdi Company, pokudn najdes, do sumIncome pridej jeji sum, jinak vyhod false
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    //vrat name a addr prvni Company do name a addr, kdyz nenajdes vrat false
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    //najdi Company po Company s name a addr a nahraj do techto vystupnich parametru atributy nalezene Company
    //vyhod false pokud nenajdes
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    //vrat median z medianArray, pokud je medianArray prazdna, vrat 0
    unsigned int  medianInvoice  ( void ) const;

    auto findByNameAddr(const string & name, const string & addr) const;
    auto findByTaxID(const string & taxID) const;
};

string toLower(const string & str){
  string newStr = str;
  for( size_t i = 0; i < newStr.size(); ++i){
    if(newStr[i] >= 'A' && newStr[i] <= 'Z'){
      newStr[i] -= 'A' - 'a';
    }
  }
  return newStr;
}

bool compNameAddr(const Company * c1, const Company * c2){
      string n1 = toLower(c1 -> m_name);
      string n2 = toLower(c2 -> m_name);
      string a1 = toLower(c1 -> m_addr);
      string a2 = toLower(c2 -> m_addr);
      return n1 < n2 || ((n1 == n2) && (a1 < a2));
}

bool compTaxID(const Company * c1, const Company * c2){
  return c1->m_taxID < c2->m_taxID;
}

auto CVATRegister::findByNameAddr(const string & name, const string & addr) const{
  Company * tmp = new Company(name, addr, "");
  auto ix = equal_range(m_nameAdrArr.begin(), m_nameAdrArr.end(), tmp, compNameAddr);
  delete tmp;
  return ix.first;

}

auto CVATRegister::findByTaxID(const string & taxID) const{
  Company * tmp = new Company("", "", taxID);
  auto ix = equal_range(m_taxIDArr.begin(), m_taxIDArr.end(), tmp, compTaxID);
  delete tmp;
  return ix.first;
}

bool CVATRegister::newCompany(const string & name, const string & addr,const string & taxID ){
  
  Company * c = new Company(toLower(name), toLower(addr), taxID);
  if(binary_search(m_nameAdrArr.begin(), m_nameAdrArr.end(), c, compNameAddr)
    || binary_search(m_taxIDArr.begin(), m_taxIDArr.end(), c, compTaxID)){
      delete c;
      return false;
  }
  c -> m_name = name;
  c -> m_addr = addr;

  auto insertNameAddrIt = upper_bound(m_nameAdrArr.begin(), m_nameAdrArr.end(), c, compNameAddr);
  auto insertTaxIDIt = upper_bound(m_taxIDArr.begin(), m_taxIDArr.end(), c, compTaxID);
  m_nameAdrArr.insert(insertNameAddrIt, c);
  m_taxIDArr.insert(insertTaxIDIt, c);
  return true;
}


bool CVATRegister::cancelCompany(const string & name, const string & addr){
  auto ix = findByNameAddr(name, addr);
  if(ix == m_nameAdrArr.end() 
    || toLower((*ix) -> m_name) != toLower(name)
    || toLower((*ix) -> m_addr) != toLower(addr)){
      return false;
  }

  const string & taxID = (*ix) -> m_taxID;
  auto jx = findByTaxID(taxID);
  delete (*ix);
  m_nameAdrArr.erase(ix);
  m_taxIDArr.erase(jx);
  return true;
}

bool CVATRegister::cancelCompany(const string & taxID){
  auto ix = findByTaxID(taxID);
  if(ix == m_taxIDArr.end() || (*ix) -> m_taxID != taxID) return false;

  const string & name = (*ix) -> m_name;
  const string & addr = (*ix) -> m_addr;
  auto jx = findByNameAddr(name, addr);
  delete (*ix);
  m_taxIDArr.erase(ix);
  m_nameAdrArr.erase(jx);
  return true;
}

bool CVATRegister::invoice(const string & name, const string & addr, unsigned int amount){
  auto ix = findByNameAddr(name, addr);
  if(ix == m_nameAdrArr.end() 
    || toLower((*ix) -> m_name) != toLower(name)
    || toLower((*ix) -> m_addr) != toLower(addr)){
      return false;
  }

  (*ix) -> m_sum += amount;

  auto jx = upper_bound(m_invoiceArr.begin(), m_invoiceArr.end(), amount);
  m_invoiceArr.insert(jx, amount);
  return true;
}


bool CVATRegister::invoice(const string & taxID, unsigned int amount){
  auto ix = findByTaxID(taxID);
  if(ix == m_taxIDArr.end() || (*ix) -> m_taxID != taxID) return false;

  (*ix) -> m_sum += amount;
  auto jx = upper_bound(m_invoiceArr.begin(), m_invoiceArr.end(), amount);
  m_invoiceArr.insert(jx, amount);
  return true;

}

bool CVATRegister::audit(const string & name, const string & addr, unsigned int & sumIncome) const{
  auto ix = findByNameAddr(name, addr);
  if(ix == m_nameAdrArr.end() 
    || toLower((*ix) -> m_name) != toLower(name)
    || toLower((*ix) -> m_addr) != toLower(addr)){
      return false;
  }
  sumIncome = (*ix) -> m_sum;
  return true;
}

bool CVATRegister::audit(const string & taxID, unsigned int & sumIncome) const{
  auto ix = findByTaxID(taxID);
  if(ix == m_taxIDArr.end() || (*ix) -> m_taxID != taxID) return false;
  sumIncome = (*ix) -> m_sum;
  return true;
}

bool CVATRegister::firstCompany(string & name, string & addr) const{
  size_t companyCount = m_nameAdrArr.size();
  if(companyCount == 0) return false;
  name = m_nameAdrArr[0] -> m_name;
  addr = m_nameAdrArr[0] -> m_addr;
  return true;
}

bool CVATRegister::nextCompany(string & name, string & addr) const{
  auto ix = findByNameAddr(name, addr);
  if(ix == m_nameAdrArr.end()
    || ix == m_nameAdrArr.end() - 1
    || toLower((*ix) -> m_name) != toLower(name)
    || toLower((*ix) -> m_addr) != toLower(addr)){
      return false;
  }
  name = (*(ix+1)) -> m_name;
  addr = (*(ix+1)) -> m_addr;
  return true;
}


/**
 * @brief 
 * funkce pro hledani medianu v serazenem poli faktur
 * 
 * @return median ze vsech uspesne prijatych faktur
 */
unsigned int CVATRegister::medianInvoice(void) const{
  const size_t invoiceCount = m_invoiceArr.size();
  if(invoiceCount == 0){
    return 0;
  }
  else if(invoiceCount % 2){
    return m_invoiceArr[invoiceCount/2];
  }
  return m_invoiceArr[invoiceCount/2];
}

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b0;
  assert ( b0 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( ! b0 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b0 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b0 . invoice ( "666/666", 2000 ) );
  assert ( b0 . medianInvoice () == 2000 );
  assert ( b0 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b0 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( ! b0 . nextCompany ( name, addr ));

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
