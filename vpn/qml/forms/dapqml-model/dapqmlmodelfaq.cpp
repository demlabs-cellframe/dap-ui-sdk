/* INCLUDES */
#include "dapqmlmodelfaq.h"

/* DEFS */
enum Role
{
  Question  = Qt::UserRole,
  Answer,
};

struct Item
{
  const char *q;
  const char *a;
};

/* VARS */
static DapQmlModelFaq *__inst = nullptr;
static const QVector<Item> s_items =
{
  {
    "Q: I have an issue using KelVPN. How can I get help?",
    "A: Please send a report using the in-app \"Send bug report\" feature. Describe as much of the details as possible and leave an email address for us to contact you. If you don’t want to give your email address also you can reach out for technical assistance on Telegram using the KelVPN Support Bot (<a href=\"https://t.me/kelvpn_tech_support_bot\">https://t.me/kelvpn_tech_support_bot</a>). In some cases, the tech support specialist may ask you to send a bug report to discover the issue."
  },
  {
    "Q: Is it possible to use one key on multiple devices?",
    "A: At the moment, one license key allows the user to use the VPN service on one device. However, if you need to switch to another device, you just need to reset the serial key on the “Settings” page. After you reset the serial key, the expiration time for your subscription freezes for until you switch to another device. This allows users to input the same serial key on another device without wasting any subscription time.<br>"
    "An upcoming feature will offer the possibility of using one key on three devices without the resetting while also freezing the expiration timer."
  },
  {
    "Q: Is the output in KelVPN IP addresses resident or stored on the server?",
    "A: In the current testnet, the addresses are not residential. NoCBD mode is under development at the moment. Soon users will have the opportunity to use KelVPN in a decentralized or normal mode as they wish."
  },
  {
    "Q: When will KelVPN be available on Playstore or F-Droid?",
    "A: The team is working on publishing KelVPN on Google Play. You can see this in the current roadmap. Stay tuned for updates!"
  },
  {
    "Q: Is there a free trial?",
    "A: No, not at the moment. The 3-day trial period feature is under development and its release is on our nearest development roadmap."
  },
  {
    "Q: I don't have access to the device where I used KelVPN last time. How can I reset my license key without it?",
    "A: Right now, the only way to reset it is through our support team. Resetting and restoring keys using a seed phrase is one of the upcoming features on our development roadmap."
  },
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelFaq::DapQmlModelFaq()
  : QAbstractListModel (nullptr)
{
  /* vars */
  __inst  = this;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelFaq *DapQmlModelFaq::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelFaq;
  return __inst;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelFaq::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent)
  return s_items.size();
}

QVariant DapQmlModelFaq::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= s_items.size())
    return QVariant();

  switch (role)
    {
    case Role::Question:  return QString (s_items.at (index.row()).q);
    case Role::Answer:    return QString (s_items.at (index.row()).a);
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelFaq::roleNames() const
{
  static QHash<int, QByteArray> names;

  if (names.isEmpty())
    {
      names.insert (Role::Question, "question");
      names.insert (Role::Answer,   "answer");
    }

  return names;
}

/*-----------------------------------------*/
