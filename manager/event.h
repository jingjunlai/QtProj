#ifndef EVENT_H
#define EVENT_H

#include <QEvent>
#include <QVariant>


#define Q_CUSTOMEVENT_CLONE(T) 	\
public:							\
    virtual CustomEvent* clone()\
    {							\
        return new T(*this);	\
    }

class CustomEvent : public QEvent
{
public:
    explicit CustomEvent(QEvent::Type type) : QEvent(type)
    {
    }

    virtual ~ CustomEvent()
    {
    }

    CustomEvent(const CustomEvent& other)
        : QEvent(other)
    {
        sender = other.sender;
    }

    virtual CustomEvent* clone() = 0;

    QString sender;
};

struct VariantEv : public CustomEvent
{
    Q_CUSTOMEVENT_CLONE(VariantEv)

    VariantEv(QEvent::Type type) : CustomEvent(type)
    {

    }

    VariantEv(const VariantEv &obj)
        : CustomEvent(obj)
    {
        var = obj.var;
    }

    QVariant var;
};

#endif // EVENT_H
