/* INCLUDES */
#include "settingsmodel.h"

#include "kelguibutton.h"
#include <QUrl>
#include <QBoxLayout>

/* DEFS */
typedef QString TextStyle;
typedef void (*ItemCB) (int &index);

enum StyleId
{
  SI_TITLE,
  SI_BUTTON,
  SI_BUTTONRED,
  SI_BUTTONGRAY,
  SI_LINK,
};

struct Info
{
  TextStyle style[2];
};

struct _SItem
{
  StyleId sid;
  QString text[2];
  QUrl icon;
  ItemCB cb;
};

/* LINKS */
void defaultCb (int &index) { Q_UNUSED (index) }

/* VARS */
static QMap<StyleId, Info> s_presets =
{
  {SI_TITLE,      {"darkblue normalbold font24 margin28", ""}},
  {SI_BUTTON,     {"darkblue normal font16",  "darkblue normal font16"}},
  {SI_BUTTONRED,  {"darkblue normal font16",  "red bold font16"}},
  {SI_BUTTONGRAY, {"darkblue normal font16",  "darkblue gray font16"}},
  {SI_LINK,       {"darkblue normal font16",  ""}},
};

static QList<_SItem> s_items =
{
  _SItem{SI_TITLE,      {"Settings", ""}, QUrl(), defaultCb},

  _SItem{SI_BUTTONRED,  {"Get new licence key", "265 days left"}, QUrl ("qrc:/gui/ui/asset/ic_renew.png"), defaultCb},
  _SItem{SI_BUTTON,     {"Reset licence key"}, QUrl ("qrc:/gui/ui/asset/ic_key.png"), defaultCb},
  _SItem{SI_LINK,       {"Language"}, QUrl ("qrc:/gui/ui/asset/ic_language.png"), defaultCb},

  _SItem{SI_TITLE,      {"Support", ""}, QUrl(), defaultCb},

  _SItem{SI_BUTTON,     {"Send a bug report", ""}, QUrl ("qrc:/gui/ui/asset/ic_send-report.png"), defaultCb},
  _SItem{SI_BUTTON,     {"Telegram support bot", ""}, QUrl ("qrc:/gui/ui/asset/ic_bot.png"), defaultCb},

  _SItem{SI_TITLE,      {"Information", ""}, QUrl(), defaultCb},

  _SItem{SI_LINK,       {"Bug Report"}, QUrl ("qrc:/gui/ui/asset/ic_information_bug-report.png"), defaultCb},
  _SItem{SI_BUTTON,     {"Serial key history on this device"}, QUrl ("qrc:/gui/ui/asset/ic_key-history.png"), defaultCb},
  _SItem{SI_BUTTON,     {"Terms of use"}, QUrl ("qrc:/gui/ui/asset/ic_terms_policy.png"), defaultCb},
  _SItem{SI_BUTTON,     {"Privacy policy"}, QUrl ("qrc:/gui/ui/asset/ic_terms_policy.png"), defaultCb},
  _SItem{SI_BUTTONGRAY, {"Serial key history on this device", "@version"}, QUrl ("qrc:/gui/ui/asset/ic_version.png"), defaultCb},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SettingsModel::SettingsModel(QWidget *parent)
  : ModelBase (parent)
{
}

SettingsModel::~SettingsModel()
{

}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void SettingsModel::slotSetup()
{
  setupLayout();

  /* add every item */
  foreach(auto &item, s_items)
    {
      /* get item and preset */
      auto btn = new KelGuiButton;
      auto preset = s_presets.value (item.sid);

      /* setup new widget */
      btn->setBtnStyle(item.sid != SI_TITLE
          ? KelGuiButton::ButtonStyle::IconMainSub
          : KelGuiButton::ButtonStyle::TopMainBottomSub);

      btn->setMainText (item.text[0]);
      btn->setMainCssClass (preset.style[0]);

      btn->setSubText (item.text[1]);
      btn->setSubCssClass (preset.style[1]);

      btn->setSeparator (item.sid != SI_TITLE);
      btn->setLink (item.sid == SI_LINK);
      btn->setIcon (item.icon);

      btn->setMaximumWidth(392);

      /* add into list */
      lay->addWidget (btn);
    }
}

/*-----------------------------------------*/
