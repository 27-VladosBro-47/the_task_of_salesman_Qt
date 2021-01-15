#ifndef POINT_H
#define POINT_H

#include<QGraphicsItem>
#include<QGraphicsEllipseItem>
#include<QGraphicsTextItem>
#include<QGraphicsLineItem>
#include<QBrush>
#include<QPen>
#include<QColor>
#include<QFont>
#include<QString>

#include<QDebug>

// Клас, що описує вершину у задачі комівояжера
class Point
{
public:
    unsigned int Number;                        // Номер вершини
    int X = 0, Y = 0;                           // Координати вершини

    QGraphicsEllipseItem* zone = nullptr;       // Коло, яке буде сигналізувати, що в даній області
                                                // не можна будувати інші вершини
    QGraphicsEllipseItem* point = nullptr;      // Коло, яке відображає саму вершину графа
    QGraphicsTextItem* nameOfTop = nullptr;     // Текст, що містить номер вершини і розмішується в середині ellipse

    Point(int x, int y, unsigned int number, int activeZone);

    ~Point();
};

#endif // POINT_H
