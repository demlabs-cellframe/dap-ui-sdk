#ifndef DAPUIMAINWINDOW_H
#define DAPUIMAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QVariant>
#include <QtDebug>
class QStackedWidget;
#include "DapUiScreen.h"


class DapUiMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DapUiMainWindow(QWidget *parent = 0);
    virtual ~DapUiMainWindow();

    /**
     * @brief sw
     * @return
     */
    QStackedWidget * sw() { return m_sw; }

    /**
     * @brief currentScreen
     * @return
     */
    DapUiScreen * currentScreen() { return m_currentScreen; }

    /**
     * @brief currentScreenName
     * @return
     */
    QString currentScreenName(){ return QString::fromLatin1(currentScreen()?currentScreen()->metaObject()->className():"nullptr" ); }
    void deleteCurrentScreen(){
        if (m_currentScreen) delete m_currentScreen;
        m_currentScreen = nullptr;
    }
    void newScreen(DapUiScreen * a_screen);

    /**
     * @brief setUiProp
     * @param a_objName
     * @param a_varName
     * @param a_varValue
     */
    void setUiProp(const QString& a_objName, const QString& a_varName, const QVariant& a_varValue){
        if(currentScreen())
            currentScreen()->setVars(a_objName, a_varName, a_varValue);
    }

    /**
     * @brief currentScreenVariant
     * @return
     */
    DapUiScreen::ScreenVariant currentScreenVariant() { return currentScreen()? currentScreen()->currentVariant():DapUiScreen::VariantNone; }

    /**
     * @brief getInstance
     * @return
     */
    static DapUiMainWindow *getInstance() { return m_instance; }

    /**
     * @brief connectScreen
     * @param a_signal
     * @param a_receiver
     * @param a_slot
     */
    template <class T1, typename Func1, class T2, typename Func2>
    inline void
    connectScreen(Func1 a_signal, T2* a_receiver, Func2 a_slot  ){
        if(currentScreen())
            connect(qobject_cast<T1*>(currentScreen()),a_signal, a_receiver, a_slot );
        else
            qWarning() << "[DapUiMainWindow] no current screen to connecto to";
    }

    /**
     * @brief connectScreen
     * @param a_signal
     * @param a_lambda
     */
    template <class T1, typename Func1, typename Func2>
    inline void
    connectScreen(Func1 a_signal, Func2 a_lambda  ){
        if(currentScreen())
            connect(qobject_cast<T1*>(currentScreen()),a_signal, a_lambda );
        else
            qWarning() << "[DapUiMainWindow] no current screen to connecto to";
    }

signals:
    void sigScreenChanged();
protected:
    void onScreenOrientationChanged(Qt::ScreenOrientation a_orientation);
private:
    QStackedWidget * m_sw;

    DapUiScreen * m_currentScreen = nullptr;


    static DapUiMainWindow * m_instance;
private slots:
};

#endif // DAPUIMAINWINDOW_H
