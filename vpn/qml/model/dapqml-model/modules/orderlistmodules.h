#ifndef ORDERLISTMODULES_H
#define ORDERLISTMODULES_H

/* INCLUDES */
#include <QAbstractListModel>
#include <QSharedPointer>

#include "dapqml-model/dapqmlmodelorderlist.h"

/****************************************//**
 * @brief orders model list model modules
 * @ingroup groupUiModels
 * @date 09.08.2023
 * @author Mikhail Shilenko
 *******************************************/

/* NAMESPACE */
namespace OrderListModule {

/* DEFS */

struct OrderItem
{
  QString location;
  QString price;
  QString units;
  QString units_value;
  QString server;
  QString node_addr;
  QString hash;
  QString ipAddress;
};

struct NameValueItem
{
  QString name;
  QString value;
};

/****************************************//**
 * @brief interface class for private model modes
 *******************************************/

class ModuleInterface
{
  /* VARS */
protected:
  int _currentIndex;

  /* CONSTRUCT/DESTRUCT */
public:
  ModuleInterface();
  virtual ~ModuleInterface();

  /* VIRTUAL METHODS */
  virtual int size() const = 0;
  virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
  virtual bool setCurrentIndex (int a_value);

  virtual const QString &name() const = 0;
  virtual const QString &value() const = 0;

  /* METHODS */
  int currentIndex() const;

  /// @note ! method can throw std::runtime_error !
  template<class T> T *as()
  {
    return dynamic_cast<T *> (this);
  }
};

/****************************************//**
 * @brief Name+Value List Module
 *
 * Basic list type.
 * Used for displaying Name + Value items
 *******************************************/

class NameValueModule : public ModuleInterface
{
  /* VARS */
protected:
  QVector<NameValueItem> _items;

  /* CONSTRUCT/DESTRUCT */
public:
  NameValueModule() {}
  ~NameValueModule() override {};

  /* METHODS */
  const QVector<NameValueItem> &items() const;
  void setItems (const QVector<NameValueItem> &a_items);
  void setItems (QVector<NameValueItem> &&a_items);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

/****************************************//**
 * @brief Order List Module
 *
 * Complex list.
 * Used to display orders information.
 *******************************************/

class OrdersModule : public ModuleInterface
{
  /* VARS */
protected:
  QVector<OrderItem> _items;

  /* CONSTRUCT/DESTRUCT */
public:
  OrdersModule() {}
  ~OrdersModule() override {};

  /* METHODS */
  const QVector<OrderItem> &items() const;
  void setItems (const QVector<OrderItem> &a_items);
  void setItems (QVector<OrderItem> &&a_items);

  OrderItem &operator[] (int a_index);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
  void installAdressMap (const QHash<QString, QString> &a_map);
};

/****************************************//**
 * @brief Network List Module
 *
 * Based on basic Name+Value list.
 *******************************************/

class NetworksModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  NetworksModule() {}
  ~NetworksModule() override {};

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
};

/****************************************//**
 * @brief Wallet List Module
 *
 * Based on basic Name+Value list.
 *******************************************/

class WalletsModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  WalletsModule() {}
  ~WalletsModule() override {};

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
};

/****************************************//**
 * @brief Token List Module
 *
 * Based on basic Name+Value list.
 *******************************************/

class TokensModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  TokensModule() {}
  ~TokensModule() override {};

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
};

/****************************************//**
 * @brief Unit List Module
 *
 * Basic unit select list.
 *******************************************/

class UnitsModule : public ModuleInterface
{
  /* VARS */
protected:
  QStringList _items;

  /* CONSTRUCT/DESTRUCT */
public:
  UnitsModule();
  ~UnitsModule() override {};

  /* METHODS */
  const QStringList &items() const;
  void setItems (const QStringList &a_items);
  void setItems (QStringList &&a_items);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setCurrentIndex (int a_value) override;
  const QString &name() const override;
  const QString &value() const override;
};

/****************************************//**
 * @brief ModuleInterface pointer wrapper
 *
 * Simply creates module by provided mode
 * and share's it by operator->
 *******************************************/

class ModuleContainer
{
  /* DEFS */
public:
  typedef QSharedPointer<ModuleInterface> ModuleInterfacePtr;

  /* VARS */
protected:
  DapQmlModelOrderList::Mode _mode;
  ModuleInterfacePtr _module;

  //ModuleInterfacePtr _orders;
  ModuleInterfacePtr _networks;
  ModuleInterfacePtr _wallets;
  ModuleInterfacePtr _tokens;
  ModuleInterfacePtr _units;

public:
  ModuleContainer (DapQmlModelOrderList::Mode a_mode = DapQmlModelOrderList::Invalid);

  DapQmlModelOrderList::Mode mode() const;
  bool setMode (DapQmlModelOrderList::Mode a_value);

  //ModuleInterfacePtr orders() const    { return _orders;   }
  ModuleInterfacePtr networks() const  { return _networks; }
  ModuleInterfacePtr wallets() const   { return _wallets;  }
  ModuleInterfacePtr tokens() const    { return _tokens;   }
  ModuleInterfacePtr units() const     { return _units;    }

  ModuleInterface *operator->();
  ModuleInterface &operator&();
};

/*-----------------------------------------*/
}
/*-----------------------------------------*/
#endif // ORDERLISTMODULES_H
