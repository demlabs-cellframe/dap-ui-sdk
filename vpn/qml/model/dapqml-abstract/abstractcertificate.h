#ifndef ABSTRACTCRYPTOGRAPHY_H
#define ABSTRACTCRYPTOGRAPHY_H

/* INCLUDES */
#include <QtGlobal>
#include <QString>

/****************************************//**
 * @brief abstract certificate
 *
 * Allows to use template method to display
 * and certificate
 *
 * @ingroup groupUiModels
 * @date 23.08.2022
 * @author Mikhail Shilenko
 *******************************************/

class AbstractCertificate
{
  Q_DISABLE_COPY_MOVE(AbstractCertificate)

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// Crypto info struct
  struct Crypto
  {
    QString name; // TODO
  };

  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  AbstractCertificate() {}
  virtual ~AbstractCertificate() {}
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// append crypto
  void append (Crypto &&a_newCrypto)            { setCrypto (std::move (a_newCrypto)); }
  /// @}

  /****************************************//**
   * @name VIRTUAL METHODS
   *******************************************/
  /// @{
public:
  /// request const instance of crypto. otherwise will return static dummy item.
  virtual const Crypto &server (const QString &a_name) const = 0;

  /// set or replace crypto
  virtual void setCrypto (const QString &a_name, Crypto &&a_newCrypto) = 0;

  /// same as append crypto
  /// @note will replace crypto with the same name
  virtual void setCrypto (Crypto &&a_newCrypto) = 0;

  /// remove crypto by name
  virtual void remove (const QString &a_name) = 0;

  /// request crypto keys
  virtual const QStringList &keys() const = 0;

  /// request crypto list size
  virtual int size() const = 0;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  AbstractCertificate &operator<< (Crypto &&a_newCrypto)  { append (std::move (a_newCrypto)); return *this; }
  const Crypto operator[] (const QString &a_name) const
  {
    return server (a_name);
  }
  /// @}
};

/*-----------------------------------------*/
#endif // ABSTRACTCRYPTOGRAPHY_H
