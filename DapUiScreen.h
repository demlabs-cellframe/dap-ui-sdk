#ifndef DAPUISCREEN_H
#define DAPUISCREEN_H

#include <QtDebug>
#include <QObject>
#include <QWidget>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QStackedWidget>


class QStackedWidget;

class DapUiScreen : public QObject
{
    Q_OBJECT
public:
    enum ScreenVariant{VariantDefault=0,VariantNone=-1,VariantHor,VariantVer,VariantHorInput, VariantVerInput};
    enum ScreenSize{Small=0,Medium=10,Big=20};
    Q_ENUM(ScreenVariant);
    Q_ENUM(ScreenSize);
private:
    QStackedWidget* m_sw;
    QSet<ScreenVariant> m_variants;
    ScreenVariant m_currentVariant;
    ScreenVariant m_defaultVariant;
protected:

    /**
     * @brief variants
     * @return
     */
    const QSet<ScreenVariant>& variants(){ return  m_variants;}

    /**
     * @brief page
     * @param a_variant
     * @return
     */
    QWidget * page(ScreenVariant a_variant){ return m_page[a_variant];}

    QMap<ScreenVariant,QWidget*> m_page;


    /**
     * @brief initUiForm
     * @param a_w
     */
    template<class T,class TSmall, class TBig>
    void initUiForm(QWidget * a_w)
    {
        switch(getScreenSize()){
            case Big: {
                static TBig * l_ui = nullptr;
                if( l_ui) delete l_ui;
                l_ui = new TBig;
                l_ui->setupUi(a_w);

            } break;
            case Small: {
                static TSmall * l_ui = nullptr;
                if( l_ui)
                    delete l_ui;
                l_ui=new TSmall;
                l_ui->setupUi(a_w);
            } break;
            default: {
                static T * l_ui = nullptr;
                if( l_ui) delete l_ui;
                l_ui = new T;
                l_ui->setupUi(a_w);
            }
        }
    }

    virtual void initUi(QWidget * a_w,ScreenVariant a_variant)=0;

    QStackedWidget * sw(){ return m_sw;};

public:
    DapUiScreen(QObject *parent, QStackedWidget * a_sw);
    virtual ~DapUiScreen();

    /**
     * @brief create
     * @param a_variant
     * @param a_isDefault
     */
    template<class T,class TSmall, class TBig>
    inline void create(ScreenVariant a_variant = VariantDefault){
        QWidget *p;
        if(m_page.contains(a_variant)){
            qWarning() << "Screen variant" << a_variant<< " is already present in map. Deleting the old one";
            delete m_page[a_variant];
            m_page.remove(a_variant);
        }
        m_page.insert(a_variant, p = new QWidget() );
        sw()->addWidget(p);
        initUiForm<T,TSmall,TBig>(p);
        initUi(p,a_variant);

        m_defaultVariant = a_variant;
        m_variants.insert(a_variant);
        setCurrentVariant(a_variant);
    }

    /**
     * @brief create
     */
    template<class T> inline void create(){ create<T,T,T>(); }

    virtual void show();
    void setCurrentVariant(ScreenVariant a_newVariant);

    /**
     * @brief setCurrentVariantDefault
     */
    inline void setCurrentVariantDefault(){ setCurrentVariant(defaultVariant());};

    /**
     * @brief currentVariant
     * @return
     */
    inline ScreenVariant currentVariant(){ return m_currentVariant; };

    /**
     * @brief defaultVariant
     * @return
     */
    inline ScreenVariant defaultVariant(){ return m_defaultVariant; };

    static ScreenSize getScreenSize();

    /**
     * @brief currentPage
     * @return
     */
    QWidget * currentPage(){ return sw()->currentWidget(); }

    /**
     * @brief setVars
     * @param a_objName
     * @param a_varName
     * @param a_varValue
     */
    void setVars(const QString& a_objName, const QString& a_varName, const QVariant& a_varValue)
    {
        foreach (auto l_variant, variants()) {
            QWidget *w = getWidget(a_objName,l_variant);
            if(w)
                w->setProperty(a_varName.toLatin1().constData(),a_varValue);
        }
    }

    /**
     * @brief invokeMethods
     * @param a_objName
     * @param a_methodName
     * @param a_val0
     * @param a_val1
     * @param a_val2
     * @param a_val3
     * @param a_val4
     * @param a_val5
     * @param a_val6
     * @param a_val7
     * @param a_val8
     * @param a_val9
     */
    void invokeMethods(const QString& a_objName, const QString& a_methodName
                       , QGenericArgument a_val0=QGenericArgument(nullptr)
                       , QGenericArgument a_val1=QGenericArgument()
                       , QGenericArgument a_val2=QGenericArgument()
                       , QGenericArgument a_val3=QGenericArgument()
                       , QGenericArgument a_val4=QGenericArgument()
                       , QGenericArgument a_val5=QGenericArgument()
                       , QGenericArgument a_val6=QGenericArgument()
                       , QGenericArgument a_val7=QGenericArgument()
                       , QGenericArgument a_val8=QGenericArgument()
                       , QGenericArgument a_val9=QGenericArgument()
            )
    {
        foreach (auto variant, variants()) {
            QWidget *w = getWidget(a_objName,variant);
            QGenericReturnArgument ret;
            if(w){
               QMetaObject::invokeMethod(w, a_methodName.toLatin1().constData(),ret,a_val0,
                                         a_val1,a_val2,a_val3, a_val4, a_val5, a_val6, a_val7, a_val8,a_val9);
            }
        }
    }

    /**
     * @brief connectTo
     * @param a_objName
     * @param a_signal
     * @param a_func
     */
    template <class T, typename Func1,typename Func2>
    inline void
    connectTo(const QString& a_objName, Func1 a_signal, Func2 a_func  ){
        foreach (auto l_variant, variants()) {
            T * w=page(l_variant)->findChild<T*>(a_objName);
            if(w)
                connect(qobject_cast<T*>(w),a_signal,a_func);
        }
    }

    void connectTo(const QString& a_objName,const char *a_signal, const QObject *a_recv, const char *a_slot );

    //void connectTo(const QString& a_objName, PointerToMemberFunction a_signal,const QObject *a_recv,PointerToMemberFunction a_slot );
//    void connectTo(const QString& a_objName,const char *a_signal, const QObject *a_recv, const char *a_slot );

    /**
     * @brief getWidget
     * @param a_objName
     * @param a_variant
     * @return
     */
    QWidget* getWidget(const QString& a_objName,ScreenVariant a_variant) {  return page(a_variant)->findChild<QWidget*>(a_objName); }

    /**
     * @brief getWidgetCustom
     * @param a_objName
     * @param a_variant
     * @return
     */
    template <class T>
    T* getWidgetCustom(const QString& a_objName,ScreenVariant a_variant) {  return page(a_variant)->findChild<T*>(a_objName); }
signals:
public slots:

};

#endif // DAPUISCREEN_H
