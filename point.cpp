#include "point.h"

Point::Point(int x, int y, unsigned int number, int activeZone)
{
    Number = number;
    X = x;
    Y = y;

    point = new QGraphicsEllipseItem(X-13,Y-13,26,26);
    point->setBrush(QColor(255,127,39));
    point->setPen(QPen(QColor(255,127,39)));

    nameOfTop = new QGraphicsTextItem(QString::number(Number));
    if(Number>=10){nameOfTop->setPos(X-9,Y-10);}
    else {nameOfTop->setPos(X-6,Y-10);}

    zone = new QGraphicsEllipseItem(X-50,Y-50,100,100);
    zone->setBrush(Qt::NoBrush);
    if(activeZone)
    {
        zone->setPen(QPen(Qt::black));
    }
    else
    {
        zone->setPen(QPen(Qt::NoPen));
    }
}

Point::~Point()
{
    delete point;
    delete nameOfTop;
    delete zone;
}
