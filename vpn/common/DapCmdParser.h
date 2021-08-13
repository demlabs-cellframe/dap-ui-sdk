#ifndef DAPCMDPARSER_H
#define DAPCMDPARSER_H

/* INCLUDES */
#include <QObject>
#include <QTcpSocket>

/****************************************//**
 * @brief message receiver for GUI
 * @note if dumpMessages is true, class
 * will save messages into "/opt/kelvpn/dump"
 * @note if sockCtl is nullptr, class will
 * simulate socket messages for unit test.
 * Activate new messages by sending signal
 * to onCtlReadReady()
 *******************************************/

class DapCmdParser : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdParser (
    QTcpSocket *sockCtl,
    QObject *parent = nullptr,
    bool dumpMessages = false);
  /// @}

  /****************************************//**
   * @name PRIVATE VARS
   *******************************************/
  /// @{
private:
  static const uint16_t MAX_BUFFER_SIZE = 10000;
  QTcpSocket *m_sockCtl;
  QByteArray readBuffer;
  bool m_dumpMessages;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void cmdReady (QByteArray ba);
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void onCtlReadReady();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDPARSER_H
