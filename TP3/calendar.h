#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QBasicTimer>
#include <iostream>

class Calendar : public QObject
{
    Q_OBJECT

public:
    explicit Calendar();
    ~Calendar();

protected:
    void timerEvent(QTimerEvent *e) override;


public slots:

signals:
    void seasonChanged();

private:
    QBasicTimer timer;
    int cpt;

};

#endif // CALENDAR_H
