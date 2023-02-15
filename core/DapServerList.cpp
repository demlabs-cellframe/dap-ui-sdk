/* INCLUDES */
#include "DapServerList.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapServerList::DapServerList()
  : m_current (0)
{

}

/********************************************
 * METHODS
 *******************************************/

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

void DapServerList::insert(int a_index, DapServerInfo &&a_server)
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

void DapServerList::erase (DapServerList::iterator it)
{
  m_list.erase (it);
  emit sizeChanged();
}

DapServerList::iterator DapServerList::begin()
{
  return m_list.begin();
}

DapServerList::const_iterator DapServerList::cbegin() const
{
  return m_list.cbegin();
}

DapServerList::iterator DapServerList::end()
{
  return m_list.end();
}

DapServerList::const_iterator DapServerList::cend() const
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

const DapServerInfo &DapServerList::at (int a_index) const
{
  return m_list.at (a_index);
}

DapServerInfo DapServerList::value (int a_index) const
{
  return at (a_index);
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
    if (item.name.isEmpty())
      return QString();

    return _findInCountriesMap (item.name.toUpper());
  };

  /* get value */
  switch (FieldId (role))
    {
    case FieldId::name:     return item.name;
    case FieldId::country:  return country();
    case FieldId::quality:  return int (item.connection_quality);
    case FieldId::ping:     return item.ping;
    case FieldId::address:  return item.address;
    case FieldId::port:     return item.port;
    }

  /* return default value */
  return QVariant();
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
