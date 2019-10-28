#include "DapIndicator.h"
#include <QMap>

DapIndicator::DapIndicator(QObject *parent)
    : QObject(parent) {

}

const QString DapIndicator::getStringType() const {
    switch (_type) {
    case Type::Session:
        return "session";
    case Type::Stream:
        return "stream";
    case Type::Tunnel:
        return "tunnel";
    }
    throw "Cant get string type in DapIndicator";
}

const QString& DapIndicator::getStringState() const {
    return i2s[_state];
}

QMap<DapIndicator::State, QString> DapIndicator::i2s{
    {True,"true"},
    {False,"false"},
    {TrueToFalse,"true_to_false"},
    {FalseToTrue,"false_to_true"},
};

DapIndicator::State DapIndicator::fromString(const QString& state) {
    return DapIndicator::i2s.key(state);
}

const QString& DapIndicator::toString(State i) {
    return i2s[i];
}
