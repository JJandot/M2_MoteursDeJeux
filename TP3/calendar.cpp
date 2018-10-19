#include "calendar.h"

Calendar::Calendar()
{
    timer.start(1, this);
}

Calendar::~Calendar(){

}

void Calendar::timerEvent(QTimerEvent *)
{
    ++cpt;
    if(cpt % 1000 == 0)
        seasonChanged();
}
