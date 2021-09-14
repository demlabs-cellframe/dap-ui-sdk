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
  QString iconCss;
  ItemCB cb;
};

/* LINKS */
void defaultCb (int &index) { Q_UNUSED (index) }

/* VARS */
static QMap<StyleId, Info> s_presets =
{
  {SI_TITLE,      {"darkblue normalbold font24 margin28 lato", ""}},
  {SI_BUTTON,     {"darkblue normal font16 lato",  "darkblue normal font16 lato"}},
  {SI_BUTTONRED,  {"darkblue normal font16 lato",  "red bold font16 lato"}},
  {SI_BUTTONGRAY, {"darkblue normal font16 lato",  "darkblue gray font16 lato"}},
  {SI_LINK,       {"darkblue normal font16 lato",  ""}},
};

static QList<_SItem> s_items =
{
  _SItem{SI_TITLE,      {"Settings", ""}, "settings_icon", defaultCb},

  _SItem{SI_BUTTONRED,  {"Get new licence key", "265 days left"}, "settings_icon ic_renew", defaultCb},
  _SItem{SI_BUTTON,     {"Reset licence key"}, "settings_icon ic_key", defaultCb},
  _SItem{SI_LINK,       {"Language"}, "settings_icon ic_language", defaultCb},

  _SItem{SI_TITLE,      {"Support", ""}, "settings_icon", defaultCb},

  _SItem{SI_BUTTON,     {"Send a bug report", ""}, "settings_icon ic_send-report", defaultCb},
  _SItem{SI_BUTTON,     {"Telegram support bot", ""}, "settings_icon ic_bot", defaultCb},

  _SItem{SI_TITLE,      {"Information", ""}, "settings_icon", defaultCb},

  _SItem{SI_LINK,       {"Bug Report"}, "settings_icon ic_information_bug-report", defaultCb},
  _SItem{SI_BUTTON,     {"Serial key history on this device"}, "settings_icon ic_key-history", defaultCb},
  _SItem{SI_BUTTON,     {"Terms of use"}, "settings_icon ic_terms_policy", defaultCb},
  _SItem{SI_BUTTON,     {"Privacy policy"}, "settings_icon ic_terms_policy", defaultCb},
  _SItem{SI_BUTTONGRAY, {"Serial key history on this device", "@version"}, "settings_icon ic_version", defaultCb},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SettingsModel::SettingsModel (QWidget *parent)
  : ModelBase (parent)
{
  setCssStyle ("screenarea sett-scroll-area backgroundcolor");
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
  foreach (auto &item, s_items)
    {
      /* get item and preset */
      auto btn = new KelGuiButton;
      auto preset = s_presets.value (item.sid);

      /* setup new widget */
      btn->setBtnStyle (item.sid != SI_TITLE
                        ? KelGuiButton::ButtonStyle::IconMainSub
                        : KelGuiButton::ButtonStyle::TopMainBottomSub);

      btn->setMainText (item.text[0]);
      btn->setMainCssClass (preset.style[0]);

      btn->setSubText (item.text[1]);
      btn->setSubCssClass (preset.style[1]);

      btn->setSeparator (item.sid != SI_TITLE);
      btn->setLink (item.sid == SI_LINK);
      btn->setIconCssClass (item.iconCss); //btn->setIcon (item.icon);

      btn->setCssStyle (
        item.sid != SI_TITLE
        ? "sett-scroll-btn"
        : "set-scroll-title"
      );

      /* add into list */
      lay->addWidget (btn);
    }
}

/*-----------------------------------------*/
