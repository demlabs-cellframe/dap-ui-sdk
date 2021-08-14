#ifndef DAPCMDPARSER_H
#define DAPCMDPARSER_H

/* INCLUDES */
#include <QObject>
#include <QTcpSocket>

/****************************************//**
 * @brief message receiver for GUI
 * @note testMode param is filled automaticaly and only
 * when required DEFINES is present. @see DapServiceClient.cpp
 * @note if testMode is TM_DUMP, class
 * will save messages into "/opt/kelvpn/dump"
 * @note if testMode is other than TM_NONE
 * or TM_DUMP, class will simulate socket
 * messages for unit test.
 * Messages will be sended by cmdReady() as
 * if it was service socket
 *******************************************/

class DapCmdParser : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum TestMode
  {
    TM_NONE,                    ///< no tests
    TM_DUMP,                    ///< dump messages for tests
    TM_COMMON,                  ///< test common messages from service to GUI
    TM_SERVICECONNECTINPROCESS, ///< test messages from service to GUI while service is connecting
  };
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdParser (
    QTcpSocket *sockCtl,
    QObject *parent = nullptr,
    TestMode m_testMode = TM_NONE);
  /// @}

  /****************************************//**
   * @name PRIVATE VARS
   *******************************************/
  /// @{
private:
  static const uint16_t MAX_BUFFER_SIZE = 10000;
  QTcpSocket *m_sockCtl;
  QByteArray readBuffer;
  TestMode m_testMode;
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
