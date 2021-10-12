/* INCLUDES */
#include "settingsmodel.h"
#include "../settings.h"

#include "kelguibutton.h"
#include <QUrl>
#include <QBoxLayout>
#include <QEvent>
#include <QScrollBar>

/* DEFS */
typedef QString TextStyle;
typedef void (*ItemCB) ();

enum StyleId
{
  SI_TITLE,
  SI_TITLETOP,
  SI_BUTTON,
  SI_BUTTONRED,
  SI_BUTTONGRAY,
  SI_LINK,
  SI_SPACER
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
static void defaultCb () {}

/* settings */
static void cbLicenceGet();
static void cbLicenceReset();
static void cbLanguage();

/* support */
static void cbBugSend();
static void cbTelegramBot();

/* info */
static void cbBugReport();
static void cbLicenceHistory();
static void cbTermsOfUse();
static void cbPrivacyPolicy();
static void cbVersion();

/* VARS */
static QMap<StyleId, Info> s_presets =
{
  {SI_TITLETOP,   {"darkblue font24 lato bold", ""}},
  {SI_TITLE,      {"darkblue font24 margin28 lato bold", ""}},
  {SI_BUTTON,     {"darkblue font16 lato normal",  "darkblue font16 lato normal"}},
  {SI_BUTTONRED,  {"darkblue font16 lato normal",  "red font16 lato bold"}},
  {SI_BUTTONGRAY, {"darkblue font16 lato normal",  "darkblue gray font16 lato"}},
  {SI_LINK,       {"darkblue font16 lato normal",  ""}},
  {SI_SPACER,     {"",  ""}},
};

static QList<_SItem> s_items =
{
  _SItem{SI_SPACER,     {"", ""}, "1", defaultCb},
  _SItem{SI_TITLETOP,   {"Settings", ""}, "settings_icon", defaultCb},
  _SItem{SI_SPACER,     {"", ""}, "2", defaultCb},

  _SItem{SI_BUTTONRED,  {"Get new licence key", /*"265 days left"*/" "}, "settings_icon ic_renew", cbLicenceGet},
  _SItem{SI_BUTTON,     {"Reset licence key"}, "settings_icon ic_key", cbLicenceReset},
  //_SItem{SI_LINK,       {"Language"}, "settings_icon ic_language", cbLanguage},

  _SItem{SI_TITLE,      {"Support", ""}, "settings_icon", defaultCb},

  _SItem{SI_BUTTON,     {"Send a bug report", ""}, "settings_icon ic_send-report", cbBugSend},
  _SItem{SI_BUTTON,     {"Telegram support bot", ""}, "settings_icon ic_bot", cbTelegramBot},

  _SItem{SI_TITLE,      {"Information", ""}, "settings_icon", defaultCb},

  //_SItem{SI_LINK,       {"Bug Reports"}, "settings_icon ic_information_bug-report", cbBugReport},
  //_SItem{SI_BUTTON,     {"Serial key history on this device"}, "settings_icon ic_key-history", cbLicenceHistory},
  _SItem{SI_BUTTON,     {"Terms of use"}, "settings_icon ic_terms_policy", cbTermsOfUse},
  _SItem{SI_BUTTON,     {"Privacy policy"}, "settings_icon ic_terms_policy", cbPrivacyPolicy},
  _SItem{SI_BUTTONGRAY, {"Version", "@version"}, "settings_icon ic_version", cbVersion},
};

static QMap<KelGuiButton*, ItemCB> s_btnCallbacks;
static KelGuiButton* s_licenceKey;
static KelGuiButton* s_version;

/* VARS */
Settings *s_settings = nullptr;

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
 * PUBLIC METHODS
 *******************************************/

void SettingsModel::setInterface(Settings *s)
{
  /* store */
  s_settings = s;
}

void SettingsModel::setVersionText(const QString &a_text)
{
  if(!s_version)
    return;
  s_version->setSubText (a_text);
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void SettingsModel::slotSetup()
{
  setupLayout();

  /* add every item */
  int i = 0;
  foreach (auto &item, s_items)
    {
      /* get item and preset */
      auto btn = new KelGuiButton;
      auto preset = s_presets.value (item.sid);

      /* setup new widget */
      btn->setBtnStyle ((item.sid != SI_TITLE && item.sid != SI_TITLETOP)
                        ? KelGuiButton::ButtonStyle::IconMainSub
                        : KelGuiButton::ButtonStyle::TopMainBottomSub);

      btn->setMainText (item.text[0]);
      btn->setMainCssClass (preset.style[0]);

      btn->setSubText (item.text[1]);
      btn->setSubCssClass (preset.style[1]);

      btn->setSeparator (
        item.sid != SI_TITLE
        && item.sid != SI_TITLETOP
        && item.sid != SI_SPACER);
      btn->setLink (item.sid == SI_LINK);
      btn->setIconCssClass (item.iconCss); //btn->setIcon (item.icon);

      switch (item.sid)
        {
        case SI_TITLETOP: btn->setCssStyle ("sett-scroll-title-top"); break;
        case SI_TITLE:    btn->setCssStyle ("sett-scroll-title"); break;
        case SI_SPACER:   btn->setCssStyle (
            (item.iconCss == "1")
            ? ("sett-top-spacer")
            : ("sett-bottom-spacer")
          ); break;
        default:          btn->setCssStyle ("sett-scroll-btn"); break;
        }

      /* add into list */
      lay->addWidget (btn);

      /* store callback */
      s_btnCallbacks.insert (btn, item.cb);

      /* store licence key button */
      if (i == 3)
        s_licenceKey = btn;
      if (item.text[1] == "@version")
        s_version = btn;

      /* connect signal */
      connect (btn, &KelGuiButton::clicked,
               this, &SettingsModel::slotClicked,
               Qt::QueuedConnection);

      i++;
    }
}

void SettingsModel::slotSetDaysLeft(QString days)
{
  if (s_licenceKey)
    s_licenceKey->setSubText (days);
}

void SettingsModel::slotClicked()
{
  auto btn  = qobject_cast<KelGuiButton*> (sender());
  auto cb   = s_btnCallbacks.value (btn, defaultCb);
  cb();
}

/********************************************
 * OVERRIDE
 *******************************************/

bool SettingsModel::eventFilter(QObject *o, QEvent *e)
{
  // This works because QScrollArea::setWidget installs an eventFilter on the widget
  if(o && o == widget() && e->type() == QEvent::Resize)
    setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

  return QScrollArea::eventFilter(o, e);
}

/********************************************
 * STATIC FUNCTIONS
 *******************************************/

/* settings */
void cbLicenceGet()     { emit s_settings->sigLicenceGet(); }
void cbLicenceReset()   { emit s_settings->sigLicenceReset(); }
void cbLanguage()       { emit s_settings->sigLanguage(); }

/* support */
void cbBugSend()        { emit s_settings->sigBugSend(); }
void cbTelegramBot()    { emit s_settings->sigTelegramBot(); }

/* info */
void cbBugReport()      { emit s_settings->sigBugReport(); }
void cbLicenceHistory() { emit s_settings->sigLicenceHistory(); }
void cbTermsOfUse()     { emit s_settings->sigTermsOfUse(); }
void cbPrivacyPolicy()  { emit s_settings->sigPrivacyPolicy(); }
void cbVersion()        { emit s_settings->sigVersion(); }

/*-----------------------------------------*/
