#ifndef DAPINDICATORSTATEE_H
#define DAPINDICATORSTATEE_H

#include <QObject>
#include <QHash>

class DapIndicator : public QObject
{
    Q_OBJECT
public:
    static const uint8_t TYPE_COUNT = 3;
    enum class Type{Session, Stream, Tunnel};

    /* Change enum State to enum class State
     * when old state machine will be removed */
    enum State{False, FalseToTrue, True, TrueToFalse};

    Q_ENUM(State) Q_ENUM(Type)

    explicit DapIndicator(QObject *parent = nullptr);

    explicit DapIndicator(Type t, State s, QObject *parent = nullptr) : QObject(parent)
    {
        _type = t;
        _state = s;
    }

    DapIndicator(const DapIndicator&di): QObject(Q_NULLPTR)
    {
        this->_state = di._state;
        this->_type = di._type;
    }

    DapIndicator & operator = (const DapIndicator & di)
    {
        this->_state = di._state;
        this->_type = di._type;
        return *this;
    }

    // operator== for use QCache. Thats why we use only _type
    bool operator== ( const DapIndicator & di ) const
    {
        return _type == di._type;
    }

    const QString getStringType() const;
    const QString& getStringState() const;
    Type getType() const { return _type; }
    State getState() const { return _state; }
    void setState(State s) { _state = s; }


    static const QString& toString(State i);
    static State fromString(const QString& state);
private:
    Type _type;
    State _state;
    static QMap<State, QString> i2s;
};

inline uint qHash(const DapIndicator &di, uint seed)
{
    return qHash(int(di.getType()), seed);
}

inline uint qHash(const DapIndicator::Type &t, uint seed)
{
    return qHash(int(t), seed);
}
#endif // DAPINDICATORSTATEE_H
