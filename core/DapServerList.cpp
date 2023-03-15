/* INCLUDES */
#include "DapServerList.h"

#include <QMetaEnum>

/* FUNCTIONS */
static const QHash<QString, QString> s_countryMap =
{
  {"Andorra", "AD"},
  {"United arab emirates", "AE"},
  {"Afganistan", "AF"},
  {"Antigua and barbuda", "AG"},
  {"Anguilla", "AI"},
  {"Albania", "AL"},
  {"Armenia", "AM"},
  {"Nederlandse antillen", "AN"},
  {"Angola", "AO"},
  {"Argentina", "AR"},
  {"Austria", "AT"},
  {"Australia", "AU"},
  {"Aruba", "AW"},
  {"Aland islands", "AX"},
  {"Azerbaijan", "AZ"},
  {"Bosnia and herzegovina", "BA"},
  {"Barbados", "BB"},
  {"Bangladesh", "BD"},
  {"Belgium", "BE"},
  {"Burkina faso", "BF"},
  {"Bulgaria", "BG"},
  {"Bahrain", "BH"},
  {"Burundi", "BI"},
  {"Benin", "BJ"},
  {"Bermuda", "BM"},
  {"Brunei darussalam", "BN"},
  {"Bolivia", "BO"},
  {"Brazil", "BR"},
  {"Bahamas", "BS"},
  {"Brutan", "BT"},
  {"Botswana", "BW"},
  {"Belarus", "BY"},
  {"Belize", "BZ"},
  {"Canada", "CA"},
  {"Congo", "CD"},
  {"Central african republic", "CF"},
  {"Congo", "CG"},
  {"Switzerland", "CH"},
  {"Cote d'ivoire", "CI"},
  {"Chile", "CL"},
  {"Cameroon", "CM"},
  {"China", "CN"},
  {"Colombia", "CO"},
  {"Costa rica", "CR"},
  {"Cuba", "CU"},
  {"Cape verde", "CV"},
  {"Cyprus", "CY"},
  {"Czech republic", "CZ"},
  {"Germany", "DE"},
  {"Djibouti", "DJ"},
  {"Denmark", "DK"},
  {"Dominica", "DM"},
  {"Dominican republic", "DO"},
  {"Algeria", "DZ"},
  {"Ecuador", "EC"},
  {"Estonia", "EE"},
  {"Egipt", "EG"},
  {"Eritrea", "ER"},
  {"Spain", "ES"},
  {"Ethiopia", "ET"},
  {"Europe", "EU"},
  {"Finland", "FI"},
  {"Fiji", "FJ"},
  {"Falkland islands", "FK"},
  {"Malvinas", "FK"},
  {"Micronesia", "FM"},
  {"France", "FR"},
  {"Gabon", "GA"},
  {"United kingdom", "GB"},
  {"England", "GB"},
  {"Grenada", "GD"},
  {"Georgia", "GE"},
  {"Guernsey", "GG"},
  {"Ghana", "GH"},
  {"Gibraltar", "GI"},
  {"Gambia", "GM"},
  {"Guinea", "GN"},
  {"Equatorial guinea", "GQ"},
  {"Greece", "GR"},
  {"Guatemala", "GT"},
  {"Guinea-bissau", "GW"},
  {"Guyana", "GY"},
  {"Hong kong", "HK"},
  {"Honduras", "HN"},
  {"Croatia", "HR"},
  {"Haiti", "HT"},
  {"Hungary", "HU"},
  {"Indonesia", "ID"},
  {"Ireland", "IE"},
  {"Israel", "IL"},
  {"Isle of man", "IM"},
  {"India", "IN"},
  {"Iraq", "IQ"},
  {"Iran", "IR"},
  {"Iceland", "IS"},
  {"Italy", "IT"},
  {"Jersay", "JE"},
  {"Jamaika", "JM"},
  {"Jordan", "JO"},
  {"Japan", "JP"},
  {"Kenia", "KE"},
  {"Kyrgyzstan", "KG"},
  {"Cambodia", "KH"},
  {"Comoros", "KM"},
  {"Saint kitts and nevis", "KN"},
  {"Korea", "KP"},
  {"Republic of korea", "KR"},
  {"Kuwait", "KW"},
  {"Cayman islands", "KY"},
  {"Kazakhstan", "KZ"},
  {"Lao", "LA"},
  {"Lebanon", "LB"},
  {"Saint lusia", "LC"},
  {"Leichtenstein", "LI"},
  {"Sri lanka", "LK"},
  {"Liberia", "LR"},
  {"Lesatho", "LS"},
  {"Lithuania", "LT"},
  {"Luxembourg", "LU"},
  {"Latvia", "LV"},
  {"Libyan arab jamahiriya", "LY"},
  {"Morocco", "MA"},
  {"Monaco", "MC"},
  {"Moldova", "MD"},
  {"Montenegro", "ME"},
  {"Madagaskar", "MG"},
  {"Macedonia", "MK"},
  {"Mali", "ML"},
  {"Burma", "MM"},
  {"Mongolia", "MN"},
  {"Macao", "MO"},
  {"Mauritania", "MR"},
  {"Montserrat", "MS"},
  {"Malta", "MT"},
  {"Mauritius", "MU"},
  {"Maldives", "MV"},
  {"Malawi", "MW"},
  {"Mexico", "MX"},
  {"Malaysia", "MY"},
  {"Mozambique", "MZ"},
  {"Namibia", "NA"},
  {"Niger", "NE"},
  {"Nigeria", "NG"},
  {"Nicaragua", "NI"},
  {"Netherlands", "NL"},
  {"Norway", "NO"},
  {"Nepal", "NP"},
  {"New zealand", "NZ"},
  {"Oman", "OM"},
  {"Panama", "PA"},
  {"Peru", "PE"},
  {"French polynesia", "PF"},
  {"Papua new guinea", "PG"},
  {"Philippines", "PH"},
  {"Pakistan", "PK"},
  {"Poland", "PL"},
  {"Puerto rico", "PR"},
  {"Portugal", "PT"},
  {"Palau", "PW"},
  {"Paraguay", "PY"},
  {"Qatar", "QA"},
  {"Romania", "RO"},
  {"Serbia", "RS"},
  {"Russia", "RU"},
  {"Rwanda", "RW"},
  {"Saudi arabia", "SA"},
  {"Solomon islands", "SB"},
  {"Seychelles", "SC"},
  {"Sudan", "SD"},
  {"Sweden", "SE"},
  {"Singapur", "SG"},
  {"Saint helena", "SH"},
  {"Slovenia", "SI"},
  {"Slovakia", "SK"},
  {"Sierra lione", "SL"},
  {"San mario", "SM"},
  {"Senegal", "SN"},
  {"Somalia", "SO"},
  {"Suriname", "SR"},
  {"Sao tome and principe", "ST"},
  {"El salvador", "SV"},
  {"Salvador", "SV"},
  {"Syrian arab republic", "SY"},
  {"Swaziland", "SZ"},
  {"Turks and caicos islands", "TC"},
  {"Chad", "TD"},
  {"Togo", "TG"},
  {"Thailand", "TH"},
  {"Tajikistan", "TJ"},
  {"Timor-leste", "TL"},
  {"Turkmenistan", "TM"},
  {"Tunisia", "TN"},
  {"Tonga", "TO"},
  {"Turkey", "TR"},
  {"Trinidad and tobago", "TT"},
  {"Trinidad", "TT"},
  {"Taiwan", "TW"},
  {"Tanzania", "TZ"},
  {"Ukraine", "UA"},
  {"Uganda", "UG"},
  {"United states", "US"},
  {"Usa", "US"},
  {"Uruguay", "UY"},
  {"Uzbekistan", "UZ"},
  {"Saint vincent and the grinadines", "VC"},
  {"Venezuela", "VE"},
  {"Virgin island", "VG"},
  {"Vietnam", "VN"},
  {"Vanuatu", "VU"},
  {"Samoa", "WS"},
  {"Yemen", "YE"},
  {"South africa", "ZA"},
  {"Zambia", "ZM"},
  {"Zimbabwe", "ZW"}
};

static const QString _findInCountriesMap (const QString &string)
{
  QStringList list = string.split (".", QString::SkipEmptyParts);
  QString code;
  for (const QString &s : qAsConst (list))
    {
      auto it = s_countryMap.find (s);
      if (it != s_countryMap.end())
        code = it.value();
    }
  return code.isEmpty()
         ? code
         : "://country/" + code + ".png";
}


/**##########################################
 *
 * DapServerList
 *
 ##########################################*/


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapServerList::DapServerList()
  : DapAbstractServerList (DapAbstractServerList::Type::ServerList)
  , m_current (-1)
{

}

DapServerList::~DapServerList()
{

}

/********************************************
 * METHODS
 *******************************************/

DapServerList *DapServerList::instance()
{
  static DapServerList i;
  return &i;
}

int DapServerList::append (const DapServerInfo &a_server)
{
  int result  = size();
  m_list.append (a_server);
  emit sizeChanged();
  return result;
}

int DapServerList::append (DapServerInfo &&a_server)
{
  int result  = size();
  m_list.append (std::move (a_server));
  emit sizeChanged();
  return result;
}

void DapServerList::insert (int a_index, const DapServerInfo &a_server)
{
  m_list.insert (a_index, a_server);
  emit sizeChanged();
}

void DapServerList::insert (int a_index, DapServerInfo &&a_server)
{
  m_list.insert (a_index, std::move (a_server));
  emit sizeChanged();
}

void DapServerList::remove (int a_index)
{
  m_list.removeAt (a_index);
  emit sizeChanged();
}

int DapServerList::size() const
{
  return m_list.size();
}

bool DapServerList::empty() const
{
  return m_list.empty();
}

int DapServerList::indexOf (const DapServerInfo &a_item) const
{
  return m_list.indexOf (a_item);
}

int DapServerList::indexOfName (const QString &a_name) const
{
  int index = 0;
  for (auto &server : m_list)
    {
      if (server.name() == a_name)
        return index;
      index++;
    }
  return -1;
}

int DapServerList::indexOfAddress (const QString &a_address) const
{
  int index = 0;
  for (auto &server : m_list)
    {
      if (server.address() == a_address)
        return index;
      index++;
    }
  return -1;
}

void DapServerList::erase (DapServerList::Iterator it)
{
  m_list.erase (it);
  emit sizeChanged();
}

DapServerList::Iterator DapServerList::begin()
{
  return m_list.begin();
}

DapServerList::ConstIterator DapServerList::begin() const
{
  return cbegin();
}

DapServerList::ConstIterator DapServerList::cbegin() const
{
  return m_list.cbegin();
}

DapServerList::Iterator DapServerList::end()
{
  return m_list.end();
}

DapServerList::ConstIterator DapServerList::end() const
{
  return cend();
}

DapServerList::ConstIterator DapServerList::cend() const
{
  return m_list.cend();
}

const DapServerInfo &DapServerList::first() const
{
  return m_list.first();
}

const DapServerInfo &DapServerList::last() const
{
  return m_list.last();
}

DapServerInfo &DapServerList::at (int a_index)
{
  return m_list [a_index];
}

const DapServerInfo &DapServerList::at (int a_index) const
{
  return m_list.at (a_index);
}

DapServerInfo DapServerList::value (int a_index) const
{
  return at (a_index);
}

QVariant DapServerList::qValue (int a_index) const
{
  return DapServerType (m_list.at (a_index)).asVariantMap();
}

int DapServerList::current() const
{
  return m_current;
}

void DapServerList::setCurrent (int a_index)
{
  m_current = a_index;
  emit currentChanged();
}

const DapServerInfo &DapServerList::currentServer() const
{
  static DapServerInfo dummy;
  if (current() < 0
      || current() >= size())
    return dummy;
  return at (current());
}

void DapServerList::move (int a_source, int a_dest)
{
  m_list.move (a_source, a_dest);
}

void DapServerList::clear()
{
  beginResetModel();
  m_list.clear();
  endResetModel();
}

//void DapServerList::sortByPing()
//{
//  DapServerInfoList availableServerList, notAvailableServerList;

//  beginResetModel();

//  {
//    qSort (m_list.begin(), m_list.end());

//    /* unavailable at the end of the list */
//    for (auto &server : m_list)
//      {
//        //      if (server.m_name == "Auto")
//        //        {
//        //          notAvailableServerList.push_front (server);
//        //          continue;
//        //        }

//        if (server.ping() == -1)
//          availableServerList.push_back (server);
//        else
//          notAvailableServerList.push_back (server);
//      }

//    m_list.clear();
//    m_list = notAvailableServerList += availableServerList;
//  }

//  endResetModel();
//}

/********************************************
 * OVERRIDE
 *******************************************/

int DapServerList::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return m_list.size();
}

QVariant DapServerList::data (const QModelIndex &index, int role) const
{
  /* checks */
  if (!index.isValid())
    return QVariant();
  if (index.row() < 0 || index.row() >= m_list.size())
    return QVariant();

  /* get item */
  auto &item    = at (index.row());
  auto country  = [item]
  {
    if (item.name().isEmpty())
      return QString();

    return _findInCountriesMap (item.name().toUpper());
  };

  /* get value */
  return item.value (role);
}

QHash<int, QByteArray> DapServerList::roleNames() const
{
  return DapAbstractServerList::serverRoleNames();
}

/********************************************
 * OPERATORS
 *******************************************/

DapServerInfo &DapServerList::operator[] (int a_index)
{
  return m_list[a_index];
}

const DapServerInfo &DapServerList::operator[] (int a_index) const
{
  return m_list[a_index];
}

DapServerList &DapServerList::operator<< (const DapServerInfo &a_server)
{
  append (a_server);
  return *this;
}

DapServerList &DapServerList::operator<< (DapServerInfo &&a_server)
{
  append (std::move (a_server));
  return *this;
}

/*-----------------------------------------*/



/**##########################################
 *
 * DapSortedServerList
 *
 ##########################################*/

/*-----------------------------------------*/

DapSortedServerListIterator::DapSortedServerListIterator() : p (nullptr), i() {}
DapSortedServerListIterator::DapSortedServerListIterator (DapSortedServerListIterator::Iterator n, DapSortedServerList *p) : p (p), i (n) {}
DapSortedServerListIterator::DapSortedServerListIterator (const DapSortedServerListIterator &o): p (o.p), i (o.i) {}
DapServerInfo &DapSortedServerListIterator::operator*()        { return p->_list [*i]; }
DapServerInfo *DapSortedServerListIterator::operator->()       { return & (p->_list) [*i]; }
DapServerInfo &DapSortedServerListIterator::operator[] (int j) { return p->_list [*i + j]; }
bool DapSortedServerListIterator::operator== (const DapSortedServerListIterator &o) const      { return i == o.i; }
bool DapSortedServerListIterator::operator!= (const DapSortedServerListIterator &o) const      { return i != o.i; }
bool DapSortedServerListIterator::operator< (const DapSortedServerListIterator &other) const   { return *i < *other.i; }
bool DapSortedServerListIterator::operator<= (const DapSortedServerListIterator &other) const  { return *i <= *other.i; }
bool DapSortedServerListIterator::operator> (const DapSortedServerListIterator &other) const   { return *i > *other.i; }
bool DapSortedServerListIterator::operator>= (const DapSortedServerListIterator &other) const  { return *i >= *other.i; }
DapSortedServerListIterator DapSortedServerListIterator::operator++ (int) { DapSortedServerListIterator n (i, p); ++i; return n; }
DapSortedServerListIterator DapSortedServerListIterator::operator-- (int) { DapSortedServerListIterator n (i, p); i--; return n; }
DapSortedServerListIterator &DapSortedServerListIterator::operator+= (int j)     { i += j; return *this; }
DapSortedServerListIterator &DapSortedServerListIterator::operator-= (int j)     { i -= j; return *this; }
int DapSortedServerListIterator::operator- (DapSortedServerListIterator j) const { return *i - *j.i; }
DapSortedServerListIterator::operator DapServerInfo *()                          { return operator->(); }
DapSortedServerListIterator::operator int() const                                { return *i; }
DapSortedServerListIterator::operator Iterator() const                           { return i; }
DapSortedServerListIterator DapSortedServerListIterator::operator- (int j) const { return DapSortedServerListIterator (i - j, p); }
DapSortedServerListIterator DapSortedServerListIterator::operator+ (int j) const { return DapSortedServerListIterator (i + j, p); }
DapSortedServerListIterator &DapSortedServerListIterator::operator--()           { i--; return *this; }
DapSortedServerListIterator &DapSortedServerListIterator::operator++()           { ++i; return *this; }
bool DapSortedServerListIterator::isNull() const                                 { return p == nullptr; }

/*-----------------------------------------*/

DapSortedServerListConstIterator::DapSortedServerListConstIterator() : p (nullptr), i() {}
DapSortedServerListConstIterator::DapSortedServerListConstIterator (DapSortedServerListConstIterator::ConstIterator n, const DapSortedServerList *p) : p (p), i (n) {}
DapSortedServerListConstIterator::DapSortedServerListConstIterator (const DapSortedServerListConstIterator &o): p (o.p), i (o.i) {}
DapSortedServerListConstIterator::DapSortedServerListConstIterator (const DapSortedServerListIterator &o): p (o.p), i (o.i) {}
const DapServerInfo &DapSortedServerListConstIterator::operator*() const        { return p->_list [*i]; }
const DapServerInfo *DapSortedServerListConstIterator::operator->() const       { return &p->_list [*i]; }
const DapServerInfo &DapSortedServerListConstIterator::operator[] (int j) const { return p->_list [*i + j]; }
bool DapSortedServerListConstIterator::operator== (const DapSortedServerListConstIterator &o) const     { return i == o.i; }
bool DapSortedServerListConstIterator::operator!= (const DapSortedServerListConstIterator &o) const     { return i != o.i; }
bool DapSortedServerListConstIterator::operator< (const DapSortedServerListConstIterator &other) const  { return *i < *other.i; }
bool DapSortedServerListConstIterator::operator<= (const DapSortedServerListConstIterator &other) const { return *i <= *other.i; }
bool DapSortedServerListConstIterator::operator> (const DapSortedServerListConstIterator &other) const  { return *i > *other.i; }
bool DapSortedServerListConstIterator::operator>= (const DapSortedServerListConstIterator &other) const { return *i >= *other.i; }
DapSortedServerListConstIterator DapSortedServerListConstIterator::operator++ (int)         { DapSortedServerListConstIterator n (i, p); ++i; return n; }
DapSortedServerListConstIterator DapSortedServerListConstIterator::operator-- (int)         { DapSortedServerListConstIterator n (i, p); i--; return n; }
DapSortedServerListConstIterator &DapSortedServerListConstIterator::operator+= (int j)      { i += j; return *this; }
DapSortedServerListConstIterator &DapSortedServerListConstIterator::operator-= (int j)      { i -= j; return *this; }
int DapSortedServerListConstIterator::operator- (DapSortedServerListConstIterator j) const  { return *i - *j.i; }
DapSortedServerListConstIterator::operator const DapServerInfo *() const                    { return operator->(); }
DapSortedServerListConstIterator::operator int() const                                      { return *i; }
DapSortedServerListConstIterator::operator ConstIterator() const                            { return i; }
DapSortedServerListConstIterator DapSortedServerListConstIterator::operator- (int j) const  { return DapSortedServerListConstIterator (i - j, p); }
DapSortedServerListConstIterator DapSortedServerListConstIterator::operator+ (int j) const  { return DapSortedServerListConstIterator (i + j, p); }
DapSortedServerListConstIterator &DapSortedServerListConstIterator::operator--()            { i--; return *this; }
DapSortedServerListConstIterator &DapSortedServerListConstIterator::operator++()            { ++i; return *this; }
bool DapSortedServerListConstIterator::isNull() const                                       { return p == nullptr; }

/*-----------------------------------------*/

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapSortedServerList::DapSortedServerList()
  : DapAbstractServerList (DapAbstractServerList::Type::SortedServerList)
{
  //_sort();
}

DapSortedServerList::~DapSortedServerList()
{

}

/********************************************
 * METHODS
 *******************************************/

DapSortedServerList *DapSortedServerList::instance()
{
  static DapSortedServerList i;
  return &i;
}

int DapSortedServerList::append (const DapServerInfo &a_server)
{
  /* append new item index */
  int result  = _list.size();
  _appendServerIndex (a_server, result);

  /* store new item */
  _list.append (a_server);
  emit sizeChanged();
  return result;
}

int DapSortedServerList::append (DapServerInfo &&a_server)
{
  /* append new item index */
  int result  = _list.size();
  _appendServerIndex (a_server, result);

  /* store new item */
  _list.append (std::move (a_server));
  emit sizeChanged();
  return result;
}

void DapSortedServerList::insert (int a_index, const DapServerInfo &a_server)
{
  int newIndex  = _list.size();
  _sortedIndexes.insert (_sortedIndexes.begin() + a_index, newIndex);
  _list.append (a_server);
  emit sizeChanged();
}

void DapSortedServerList::insert (int a_index, DapServerInfo &&a_server)
{
  int newIndex  = _list.size();
  _sortedIndexes.insert (_sortedIndexes.begin() + a_index, newIndex);
  _list.append (std::move (a_server));
  emit sizeChanged();
}

void DapSortedServerList::remove (int a_index)
{
  _decreaseAllIndexes (a_index);
  _list.remove (a_index);
  emit sizeChanged();
}

int DapSortedServerList::size() const
{
  return _sortedIndexes.size();
}

bool DapSortedServerList::empty() const
{
  return _sortedIndexes.isEmpty();
}

int DapSortedServerList::indexOf (const DapServerInfo &a_item) const
{
  int index = 0;
  for (auto i = begin(), e = end(); i != e; i++, index++)
    if (*i == a_item)
      return index;
  return -1;
}

int DapSortedServerList::indexOfName (const QString &a_name) const
{
  int index = 0;
  for (auto i = begin(), e = end(); i != e; i++, index++)
    if (i->name() == a_name)
      return index;
  return -1;
}

int DapSortedServerList::indexOfAddress (const QString &a_address) const
{
  int index = 0;
  for (auto i = begin(), e = end(); i != e; i++, index++)
    if (i->address() == a_address)
      return index;
  return -1;
}

void DapSortedServerList::erase (DapSortedServerList::Iterator it)
{
  int actualIndex = it;
  _decreaseAllIndexes (actualIndex);
  _list.remove (actualIndex);
  emit sizeChanged();
}

DapSortedServerList::Iterator DapSortedServerList::begin()
{
  return Iterator (_sortedIndexes.begin(), this);
}

DapSortedServerList::ConstIterator DapSortedServerList::begin() const
{
  return ConstIterator (_sortedIndexes.begin(), this);
}

DapSortedServerList::ConstIterator DapSortedServerList::cbegin() const
{
  return ConstIterator (_sortedIndexes.cbegin(), this);
}

DapSortedServerList::Iterator DapSortedServerList::end()
{
  return Iterator (_sortedIndexes.end(), this);
}

DapSortedServerList::ConstIterator DapSortedServerList::end() const
{
  return ConstIterator (_sortedIndexes.end(), this);
}

DapSortedServerList::ConstIterator DapSortedServerList::cend() const
{
  return ConstIterator (_sortedIndexes.cend(), this);
}

const DapServerInfo &DapSortedServerList::first() const
{
  return *begin();
}

const DapServerInfo &DapSortedServerList::last() const
{
  return * (--end());
}

DapServerInfo &DapSortedServerList::at (int a_index)
{
  return operator[] (a_index); //*(begin() + a_index);
}

const DapServerInfo &DapSortedServerList::at (int a_index) const
{
  return operator[] (a_index); //*(begin() + a_index);
}

DapServerInfo DapSortedServerList::value (int a_index) const
{
  return operator[] (a_index); //*(begin() + a_index);
}

QVariant DapSortedServerList::qValue (int a_index) const
{
  return DapServerType (at (a_index)).asVariantMap();
}

int DapSortedServerList::current() const
{
  return _list.current();
}

void DapSortedServerList::setCurrent (int a_index)
{
  _list.setCurrent (a_index);
  emit currentChanged();
}

const DapServerInfo &DapSortedServerList::currentServer() const
{
  return at (_list.current());
}

//void DapSortedServerList::move (int a_source, int a_dest)
//{

//}

void DapSortedServerList::clear()
{
  beginResetModel();

  _list.clear();
  _sortedIndexes.clear();

  endResetModel();
}

void DapSortedServerList::update (const QList<int> &a_indexes)
{
//  /* copy indexes */
//  QList<int> sortedIndexes;
//  for (int v : qAsConst (_sortedIndexes))
//    sortedIndexes << v;

////  /* get actual indexes */
////  QList<int> actualIndexes;
////  for (int index : a_indexes)
////    actualIndexes << sortedIndexes.at (index);

//  /* sort every index */
//  QModelIndex dummyIndex;
//  for (auto i = a_indexes.cbegin(), e = a_indexes.cend(); i != e; i++)
//    {
//      /* vars */
//      auto it       = _sortedIndexes.begin();
//      int index     = *i;               // get index from sorted list
//      auto pos      = it + index;       // calc pos inside chain
//      int old       = *pos;             // store old server index
//      auto &server  = _list.at (old);   // get server pointed by sorted chain

//      /* take index from chain */
//      _sortedIndexes.erase (pos);       // erase chain item

//      /* insert to proper place */
//      int newIndex  = _appendServerIndex (server, old);

//      /* signal about rows moving */
//      if (newIndex != old)
//        {
//          beginMoveRows (dummyIndex, index, index, dummyIndex, newIndex);
//          endMoveRows();
//        }
//    }

  /* sort provided list */
  QList<int> indexes  = a_indexes;
  qSort (indexes);

  /* take indexes from chain */
  QList<int> takenIndexes;
  for (auto i = indexes.crbegin(), e = indexes.crend(); i != e; i++)
    {
      /* calc iterator */
      auto pos  = _sortedIndexes.begin() + *i;

      /* store value */
      takenIndexes << *pos;

      /* take from chain */
      _sortedIndexes.erase (pos);
    }

  /* insert to proper places */
  for (auto i = takenIndexes.cbegin(), e = takenIndexes.cend(); i != e; i++)
    {
      auto &server  = _list[*i];
      _appendServerIndex (server, *i);
    }

  /* collect new indexes */
  QList<int> newIndexes;
  for (int takenIndex : qAsConst (takenIndexes))
    newIndexes << indexOf (_list[takenIndex]);

  /* send moving signals */
  auto oldIt  = takenIndexes.cbegin();
  auto newIt  = newIndexes.cbegin();
  QModelIndex dummyIndex;
  for (int i = 0; i < takenIndexes.size(); i++, oldIt++, newIt++)
    {
      beginMoveRows (dummyIndex, *oldIt, *oldIt, dummyIndex, *newIt);
      endMoveRows();
    }
}

const QLinkedList<int> &DapSortedServerList::getSortedIndexes() const
{
  return _sortedIndexes;
}

void DapSortedServerList::_sort()
{
//  /* defs */
//  struct Item
//  {
//    int index;
//    int ping;
//    bool operator< (const Item &o) const { return ping < o.ping; }
//  };

//  /* collect all indexes */
//  QVector<Item> items (size());
//  QVector<Item> unavItems (size());
//  int index = 0;
//  for (const auto &server : _list)
//    {
//      if (server.ping() != -1)
//        items << Item {index++, server.ping()};
//      else
//        unavItems << Item {index++, server.ping()};
//    }

//  beginResetModel();

//  /* sort by ping */
//  std::sort (items.begin(), items.end());

//  /* combine result */
//  items += unavItems;

//  /* store result */
//  _sortedIndexes.clear();
//  for (const auto &item : qAsConst (items))
//    _sortedIndexes << item.index;

//  endResetModel();

  beginResetModel();

  int index = 0;
  _sortedIndexes.clear();
  for (auto i = _list.cbegin(), e = _list.cend(); i != e; i++, index++)
    _appendServerIndex (*i, index);

  endResetModel();
}

int DapSortedServerList::_appendServerIndex (const DapServerInfo &a_server, int a_index)
{
  /* lambdas */
  auto appendToEnd = [this](int a_index) -> int
    {
      _sortedIndexes.append (a_index);
      return _sortedIndexes.size() - 1;
    };

  /* add non ping at the end */
  if (a_server.ping() == -1)
    return appendToEnd (a_index);

  /* insert ping to proper place */
  int pos = 0;
  for (auto i = begin(), e = end(); i != e; i++, pos++)
    {
      auto &item = *i;
      if (item.ping() > a_server.ping())
        {
          _sortedIndexes.insert (i, a_index);
          return pos;
        }
    }

  /* place not found, store to the end */
  return appendToEnd (a_index);
}

// 1234567
// 123+4567
// 12344567
// 12345678
void DapSortedServerList::_increaseAllIndexes (int a_index)
{
  for (auto i = begin(), e = end(); i != e; i++)
    if (int (i) >= a_index)
      (*DapSortedServerListIterator::Iterator (i))++;
}

// 1234567
// 123-567
// 123567
// 123456
void DapSortedServerList::_decreaseAllIndexes (int a_index)
{
  for (auto i = begin(), e = end(); i != e; i++)
    {
      /* decrease */
      if (int (i) > a_index)
        (*DapSortedServerListIterator::Iterator (i))--;

      /* remove if same */
      else if (int (i) == a_index)
        {
          auto c = i--;
          _sortedIndexes.erase (c);
        }
    }
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapSortedServerList::rowCount (const QModelIndex &parent) const
{
  return _list.rowCount (parent);
}

QVariant DapSortedServerList::data (const QModelIndex &index, int role) const
{
  int actualIndex = begin() + index.row();
  return _list.data (_list.index (actualIndex, index.column(), index.parent()), role);
}

/********************************************
 * OPERATORS
 *******************************************/

DapServerInfo &DapSortedServerList::operator[] (int a_index)
{
  return _list[begin() + a_index];
}

const DapServerInfo &DapSortedServerList::operator[] (int a_index) const
{
  return _list[begin() + a_index];
}

DapSortedServerList &DapSortedServerList::operator<< (const DapServerInfo &a_server)
{
  append (a_server);
  return *this;
}

DapSortedServerList &DapSortedServerList::operator<< (DapServerInfo &&a_server)
{
  append (std::move (a_server));
  return *this;
}

/*-----------------------------------------*/

const QHash<int, QByteArray> &DapAbstractServerList::serverRoleNames()
{
  static QHash<int, QByteArray> result;

  if (result.isEmpty())
    {
      int enumIndex  = DapServerType::staticMetaObject.indexOfEnumerator ("FieldId");
      auto metaEnum  = DapServerType::staticMetaObject.enumerator (enumIndex);
      int enumSize   = metaEnum.keyCount();

      for (int i = 0; i < enumSize; i++)
        result.insert (metaEnum.value (i), metaEnum.key (i));
    }

  return result;
}

const QHash<QString, QString> &DapAbstractServerList::countryMap()
{
  return s_countryMap;
}

/*-----------------------------------------*/
