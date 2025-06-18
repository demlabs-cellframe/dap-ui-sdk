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
  QStringList list = string.split (".", Qt::SkipEmptyParts);
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

/* DEFS */

class InsertServerOperation
{
  DapSortedServerList &_list;
  QList<int> &_sortedIndexes;
  int _insertedIndex;
  int _destination;
  bool _finished;
  enum
  {
      Insert,
      Append,
  } _result;
  public:
  InsertServerOperation (DapSortedServerList &a_list, QList<int> &a_sortedIndexes, const DapServerInfo &a_server, int a_insertedIndex);
  ~InsertServerOperation(); ///< will call finish
  void finish();
  operator int() const;
};


/**##########################################
 *
 * DapServerList
 *
 ##########################################*/


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapServerList::DapServerList()
  : m_current (-1)
{

}

DapServerList::DapServerList (const DapServerInfoList &a_src)
  : m_current (-1)
{
  operator = (a_src);
}

DapServerList::DapServerList (const DapServerList &a_src)
  : m_current (-1)
{
  operator = (a_src);
}

DapServerList::DapServerList (DapServerList &&a_src)
  : m_current (-1)
{
  operator = (std::move (a_src));
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

  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, result, result);
  {
    m_list.append (a_server);
  }
  endInsertRows();

  emit sizeChanged();
  return result;
}

int DapServerList::append (DapServerInfo &&a_server)
{
  int result  = size();

  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, result, result);
  {
    m_list.append (std::move (a_server));
  }
  endInsertRows();

  emit sizeChanged();
  return result;
}

void DapServerList::insert (int a_index, const DapServerInfo &a_server)
{
  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, a_index, a_index);
  {
    m_list.insert (a_index, a_server);
  }
  endInsertRows();

  emit sizeChanged();
}

void DapServerList::insert (int a_index, DapServerInfo &&a_server)
{
  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, a_index, a_index);
  {
    m_list.insert (a_index, std::move (a_server));
  }
  endInsertRows();

  emit sizeChanged();
}

void DapServerList::remove (int a_index)
{
  QModelIndex dummyIndex;
  beginRemoveRows (dummyIndex, a_index, a_index);
  {
    m_list.removeAt (a_index);
  }
  endRemoveRows();

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
  int sortedIndex = _iteratorIndex (it);

  QModelIndex dummyIndex;
  beginRemoveRows (dummyIndex, sortedIndex, sortedIndex);
  {
    m_list.erase (it);
  }
  endRemoveRows();

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
  if (m_current == a_index)
    return;

  m_current = a_index;
  emit currentChanged();
}

const DapServerInfo &DapServerList::currentServer() const
{
  static DapServerInfo dummy;
  if (current() < 0 || current() >= size() || size() == 0)
    return dummy;
  return at (current());
}

void DapServerList::move (int a_source, int a_dest)
{
  QModelIndex dummyIndex;
  beginMoveRows (dummyIndex, a_source, a_source, dummyIndex, a_dest);
  {
    m_list.move (a_source, a_dest);
  }
  endMoveRows();
}

void DapServerList::clear()
{
  beginResetModel();

  m_list.clear();

  endResetModel();
}

int DapServerList::_iteratorIndex (DapServerList::Iterator &a_it) const
{
  int sortedIndex = 0;
  for (auto i = cbegin(), e = cend(); i != e; i++, sortedIndex++)
    if (i == a_it)
      break;
  return (sortedIndex < size()) ? sortedIndex : -1;
}

int DapServerList::_iteratorIndex (DapServerList::ConstIterator &a_it) const
{
  int sortedIndex = 0;
  for (auto i = cbegin(), e = cend(); i != e; i++, sortedIndex++)
    if (i == a_it)
      break;
  return (sortedIndex < size()) ? sortedIndex : -1;
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

int DapServerList::rowCount (const QModelIndex &) const
{
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

DapServerList &DapServerList::operator = (const DapServerInfoList &a_src)
{
  m_list  = a_src;
  return *this;
}

DapServerList &DapServerList::operator = (const DapServerList &a_src)
{
  if (this == &a_src)
    return *this;

  m_list    = a_src.m_list;
  m_current = a_src.m_current;

  return *this;
}

DapServerList &DapServerList::operator = (DapServerList &&a_src)
{
  if (this == &a_src)
    return *this;

  m_list    = std::move (a_src.m_list);
  m_current = a_src.m_current;

  a_src.m_current = -1;
  return *this;
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
DapSortedServerListIterator::operator Iterator() const                           { return i; }
int DapSortedServerListIterator::internalIndex() const                           { return *i; }
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
DapSortedServerListConstIterator::operator ConstIterator() const                            { return i; }
int DapSortedServerListConstIterator::internalIndex() const                                 { return *i; }
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
  QModelIndex dummyIndex;

////  /* insert to a proper index */
////  int result = _appendServerIndex (a_server, _list.size());
////  _list.append (a_server);

////  /* signal */
////  beginInsertRows (dummyIndex, result, result);
////  endInsertRows();

//  /* figure out a proper place */
//  int result = _beginInsertServer (a_server);//_appendServerIndex (a_server, _list.size());

//  /* perform inserting */
//  beginInsertRows (dummyIndex, result, result);
//  {
//    _list.append (a_server);
//    _endInsertServer (_list.size() - 1, result);
//  }
//  endInsertRows();

  /* start insert operation */
  InsertServerOperation isop (*this, _sortedIndexes, a_server, _list.size());

  /* perform appending */
  beginInsertRows (dummyIndex, isop, isop);
  _list.append (a_server);
  isop.finish();
  endInsertRows();

  emit sizeChanged();
  return isop;
}

int DapSortedServerList::append (DapServerInfo &&a_server)
{
  QModelIndex dummyIndex;

////  /* insert to a proper index */
////  int result = _appendServerIndex (a_server, _list.size());
////  _list.append (std::move (a_server));

////  beginInsertRows (dummyIndex, result, result);
////  endInsertRows();

//  /* figure out a proper place */
//  int result = _beginInsertServer (a_server);//_appendServerIndex (a_server, _list.size());

//  /* perform inserting */
//  beginInsertRows (dummyIndex, result, result);
//  {
//    _list.append (std::move (a_server));
//    _endInsertServer (_list.size() - 1, result);
//  }
//  endInsertRows();

  /* start insert operation */
  InsertServerOperation isop (*this, _sortedIndexes, a_server, _list.size());

  /* perform appending */
  beginInsertRows (dummyIndex, isop, isop);
  _list.append (std::move (a_server));
  isop.finish();
  endInsertRows();

  emit sizeChanged();
  return isop;
}

void DapSortedServerList::insert (int a_index, const DapServerInfo &a_server)
{
  int newIndex  = _list.size();

  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, a_index, a_index);
  {
    _sortedIndexes.insert (_sortedIndexes.begin() + a_index, newIndex);
    _list.append (a_server);
    _fixCurrent (a_index, Inserted);
  }
  endInsertRows();

  emit sizeChanged();
}

void DapSortedServerList::insert (int a_index, DapServerInfo &&a_server)
{
  int newIndex  = _list.size();

  QModelIndex dummyIndex;
  beginInsertRows (dummyIndex, a_index, a_index);
  {
    _sortedIndexes.insert (_sortedIndexes.begin() + a_index, newIndex);
    _list.append (std::move (a_server));
    _fixCurrent (a_index, Inserted);
  }
  endInsertRows();

  emit sizeChanged();
}

void DapSortedServerList::remove (int a_index)
{
  int removeIndex = * (_sortedIndexes.begin() + a_index);

  QModelIndex dummyIndex;
  beginRemoveRows (dummyIndex, a_index, a_index);
  {
    _decreaseAllIndexes (removeIndex);
    _list.remove (removeIndex);
    _fixCurrent (a_index, Removed);
  }
  endRemoveRows();

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

int DapSortedServerList::indexOfOrderHash (const QString &a_hash) const
{
  int index = 0;
  for (auto i = begin(), e = end(); i != e; i++, index++)
    if (i->orderHash() == a_hash)
      return index;
  return -1;
}

void DapSortedServerList::erase (DapSortedServerList::Iterator it)
{
  int actualIndex = it.internalIndex(),
      sortedIndex = _iteratorIndex (it);

  QModelIndex dummyIndex;
  beginRemoveRows (dummyIndex, sortedIndex, sortedIndex);
  {
    _decreaseAllIndexes (actualIndex);
    _list.remove (actualIndex);
    if (sortedIndex != -1)
      _fixCurrent (sortedIndex, Removed);
  }
  endRemoveRows();

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
  if (_list.current() == a_index)
    return;

  /* vars */
  IndexData oldData, newData;

  /* store old index data */
  oldData.index = current();
  if (current() != -1)
    oldData.name  = currentServer().name();

  _list.setCurrent (a_index);
  emit currentChanged();

  /* store new index data */
  newData.index = a_index;
  if (a_index != -1)
    newData.name  = currentServer().name();

  /* save */
  m_sortIndexData = { std::move (oldData), std::move (newData) };
}

const DapServerInfo &DapSortedServerList::currentServer() const
{
  static const DapServerInfo dummy;
  if (current() < 0 || current() >= size() || size() == 0)
    return dummy;
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
  QModelIndex dummyIndex;
  QList<int> sortedIndexes = a_indexes;
  QList<int> actualIndexes;

  /* resort */
  std::sort (sortedIndexes.begin(), sortedIndexes.end());

  /* take items from list */
  for (auto i = sortedIndexes.crbegin(), e = sortedIndexes.crend(); i != e ; i++)
    {
      auto it = _sortedIndexes.begin() + *i;
      actualIndexes << *it;

      beginRemoveRows (dummyIndex, *i, *i);
      _sortedIndexes.erase (it);
      endRemoveRows();
    }

  /* insert to a proper place */
  for (const auto i : qAsConst (actualIndexes))
    {
      auto &server  = _list[i];

      /* start insert operation */
      InsertServerOperation isop (*this, _sortedIndexes, server, i);

      /* perform inserting */
      beginInsertRows (dummyIndex, isop, isop);
      isop.finish();
      endInsertRows();
    }
}

const QList<int> &DapSortedServerList::getSortedIndexes() const
{
  return _sortedIndexes;
}

QPair<DapSortedServerList::IndexData, DapSortedServerList::IndexData> DapSortedServerList::sortIndexData() const
{
  return m_sortIndexData;
}

DapSortedServerList::operator DapServerInfoList() const
{
  DapServerInfoList result;

  for (auto i = begin(), e = end(); i != e; i++)
    result << *i;

  return result;
}

DapSortedServerList::operator DapServerList() const
{
  DapServerList result;

  for (auto i = begin(), e = end(); i != e; i++)
    result << *i;

  return result;
}

void DapSortedServerList::_sort()
{
//  auto printServers = [this] (const char *a_name, int a_current, int a_newCurrent = -1)
//    {
//      QStringList names, indexes;
//      QString currentName = currentServer().name();

//      for (auto &server : qAsConst (_list))
//        names << server.name();

//      for (auto index : qAsConst (_sortedIndexes))
//        indexes << QString::number (index);

//      qDebug() << a_name
//               << ((a_newCurrent == -1)
//                  ? QString ("Current: %1 - %2").arg (currentName).arg (a_current)
//                  : QString ("Current: %1 - %2 -> %3").arg (currentName).arg (a_current).arg (a_newCurrent));
//      qDebug() << a_name << QString ("Names:   %1").arg (names.join(','));
//      qDebug() << a_name << QString ("Indexes: %1").arg (indexes.join(','));
//    };

  beginResetModel();

  int index = 0,
      current = (this->current() != -1) ? * (_sortedIndexes.begin() + this->current()) : -1,
      newCurrent = current;

  /* vars */
  IndexData oldData, newData;

  /* store old index data */
  oldData.index = current;
  if (current != -1)
    oldData.name  = currentServer().name();

//  printServers (__PRETTY_FUNCTION__, current);

  /* sort */
  _sortedIndexes.clear();
  for (auto i = _list.cbegin(), e = _list.cend(); i != e; i++, index++)
    {
//      int pos = _beginInsertServer (*i);//_appendServerIndex (*i, index);
//      _endInsertServer (index, pos);
      InsertServerOperation (*this, _sortedIndexes, *i, index);
    }

  /* find current */
  index = 0;
  if (current != -1)
    {
      for (auto j = _sortedIndexes.cbegin(), k = _sortedIndexes.cend(); j != k; j++, index++)
        {
          if (*j == current)
            {
              newCurrent = index;
              break;
            }
        }
    }

  //if (newCurrent != current)
  setCurrent (newCurrent);

  /* store new index data */
  newData.index = newCurrent;
  if (newCurrent != -1)
    newData.name  = currentServer().name();

  /* save */
  m_sortIndexData = { std::move (oldData), std::move (newData) };

//  printServers (__PRETTY_FUNCTION__, current, newCurrent);

  endResetModel();
}

////int DapSortedServerList::_appendServerIndex (const DapServerInfo &a_server, int a_index)
////{
////  /* lambdas */
////  auto appendToEnd = [this] (int a_index) -> int
////  {
////    _sortedIndexes.append (a_index);
////    return _sortedIndexes.size() - 1;
////  };

////  /* add non ping at the end */
////  if (a_server.ping() == -1)
////    return appendToEnd (a_index);

////  /* insert ping to proper place */
////  int pos = 0;
////  for (auto i = begin(), e = end(); i != e; i++, pos++)
////    {
////      auto &item = *i;
////      if (item.ping() > a_server.ping())
////        {
////          _sortedIndexes.insert (i, a_index);
////          return pos;
////        }
////    }

////  /* place not found, store to the end */
////  return appendToEnd (a_index);
////}

//int DapSortedServerList::_beginInsertServer (const DapServerInfo &a_server) const
//{
//  /* lambdas */
//  auto appendToEnd = [this] { return _sortedIndexes.size() - 1; };

//  /* add non ping at the end */
//  if (a_server.ping() == -1)
//    return appendToEnd();

//  /* insert ping to proper place */
//  int pos = 0;
//  for (auto i = begin(), e = end(); i != e; i++, pos++)
//    {
//      auto &item = *i;
//      if (item.ping() > a_server.ping())
//        return pos;
//    }

//  /* place not found, store to the end */
//  return appendToEnd();
//}

//void DapSortedServerList::_endInsertServer (int a_index, int a_dest)
//{
//  if (a_dest == -1)
//    return _sortedIndexes.append (a_index);
//  _sortedIndexes.insert (begin() + a_dest, a_index);
//}

// 1234567
// 123+4567
// 12344567
// 12345678
void DapSortedServerList::_increaseAllIndexes (int a_index)
{
  for (auto i = begin(), e = end(); i != e; i++)
    if (i.internalIndex() >= a_index)
      (*DapSortedServerListIterator::Iterator (i))++;
}

// 1234567
// 123-567
// 123567
// 123456
void DapSortedServerList::_decreaseAllIndexes (int a_index)
{
  for (auto i = _sortedIndexes.begin(); i != _sortedIndexes.end(); i++)
    {
      /* decrease */
      if (*i > a_index)
        *i = *i - 1;

      /* remove if same */
      else if (*i == a_index)
        {
          auto c = i;
          //if (i != _sortedIndexes.begin())
          i--;
          _sortedIndexes.erase (c);
        }
    }
}

void DapSortedServerList::_fixCurrent (int a_index, DapSortedServerList::OperationType a_operationType)
{
  /* when lower or equal */
  {
    int magic[2] =
    {
      1, // Inserted
      -1 // Removed
    };

    if (a_index <= _list.current())
      {
#ifdef QT_DEBUG
        qDebug ("%s : %s + lower or equal : old [n:%s,i:%d] new [i:%d]",
                "DapSortedServerList::_fixCurrent",
                (a_operationType == OperationType::Inserted ? "insert" : "remove"),
                currentServer().name().toUtf8().data(),
                _list.current(),
                _list.current() + magic[a_operationType]);
#endif // QT_DEBUG
        _list.setCurrent (_list.current() + magic[a_operationType]);
      }
  }

  /* when current bigger and it's removing */
  if (a_operationType == Removed)
    {
      if (_list.current() >= _list.size())
        {
#ifdef QT_DEBUG
          qDebug ("%s : %s + bigger or equal the size : old [n:%s,i:%d] new [i:%d]",
                  "DapSortedServerList::_fixCurrent",
                  (a_operationType == OperationType::Inserted ? "insert" : "remove"),
                  currentServer().name().toUtf8().data(),
                  _list.current(),
                  _list.size() - 1);
#endif // QT_DEBUG
          _list.setCurrent (_list.size() - 1);
        }
    }
}

int DapSortedServerList::_iteratorIndex (DapSortedServerList::Iterator &a_it)
{
  int actualIndex = a_it.internalIndex(), sortedIndex = 0;
  for (auto i = _sortedIndexes.cbegin(), e = _sortedIndexes.cend(); i != e; i++, sortedIndex++)
    if (*i == actualIndex)
      break;
  return (sortedIndex < _sortedIndexes.size()) ? sortedIndex : -1;
}

int DapSortedServerList::_iteratorIndex (DapSortedServerList::ConstIterator &a_it)
{
  int actualIndex = a_it.internalIndex(), sortedIndex = 0;
  for (auto i = _sortedIndexes.cbegin(), e = _sortedIndexes.cend(); i != e; i++, sortedIndex++)
    if (*i == actualIndex)
      break;
  return (sortedIndex < _sortedIndexes.size()) ? sortedIndex : -1;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapSortedServerList::rowCount (const QModelIndex &) const
{
  return size();
}

QVariant DapSortedServerList::data (const QModelIndex &index, int role) const
{
  auto it         = begin() + index.row();
  int actualIndex = it.internalIndex();
  return _list.data (_list.index (actualIndex, index.column(), index.parent()), role);
}

QHash<int, QByteArray> DapSortedServerList::roleNames() const
{
  return DapAbstractServerList::serverRoleNames();
}

/********************************************
 * OPERATORS
 *******************************************/

DapServerInfo &DapSortedServerList::operator[] (int a_index)
{
  auto it = begin() + a_index;
  return _list[it.internalIndex()];
}

const DapServerInfo &DapSortedServerList::operator[] (int a_index) const
{
  auto it = begin() + a_index;
  return _list[it.internalIndex()];
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

InsertServerOperation::InsertServerOperation (
  DapSortedServerList &a_list,
  QList<int> &a_sortedIndexes,
  const DapServerInfo &a_server,
  int a_insertedIndex)
  : _list (a_list)
  , _sortedIndexes (a_sortedIndexes)
  , _insertedIndex (a_insertedIndex)
  , _destination (-1)
  , _finished (false)
  , _result (Insert)
{
  /* lambdas */
  auto appendToEnd = [this]
  {
    _result = Append;
    return _sortedIndexes.size();
  };

  /* add non ping at the end */
  if (a_server.ping() == -1)
    {
      _destination = appendToEnd();
      return;
    }

  /* insert ping to proper place */
  int pos = 0;
  for (auto i = _list.begin(), e = _list.end(); i != e; i++, pos++)
    {
      auto &item    = *i;
      int itemPing  = item.ping() == -1 ? 10000 : item.ping();
      if (itemPing > a_server.ping())
        {
          _destination  = pos;
          return;
        }
    }

  /* place not found, store to the end */
  _destination = appendToEnd();
}

InsertServerOperation::~InsertServerOperation()
{
  finish();
}

void InsertServerOperation::finish()
{
  if (_finished)
    return;

  switch (_result)
    {
    case Insert: _sortedIndexes.insert (_destination, _insertedIndex); break;
    case Append: _sortedIndexes.append (_insertedIndex); break;
    }

//  if (_destination == -1)
//    _sortedIndexes.append (_insertedIndex);
//  else
//    _sortedIndexes.insert (_list.begin() + _destination, _insertedIndex);

  _finished = true;
}

InsertServerOperation::operator int() const
{
  return _destination;//_destination == -1 ? 0 : _destination;
}

/*-----------------------------------------*/
