#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

#include <QDebug>

#include "pointsandmatrix.h"

class PaintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PaintScene(QGraphicsScene* parent = 0);

    ~PaintScene();

signals:
    void signalGetPtrMatrix(PointsAndMatrix*& out_ptrMatrix);

private:
    // Вказівник на Matrix
    // Необхідний для побудови вершин і шляхів на QGraphicsScene
    PointsAndMatrix* ptrMatrix = nullptr;

    Qt::MouseButtons mouseButtonsState;

    // Вказівник на першу точку

    // Вказівник на останню створену точку

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void addItem_Point(int numb)
    {
        addItem(ptrMatrix->Points[numb]->zone);
        addItem(ptrMatrix->Points[numb]->point);
        addItem(ptrMatrix->Points[numb]->nameOfTop);
    }

    void addItem_Line(int firstTop, int secondTop)
    {
        ptrMatrix->WaysMatrix[firstTop][secondTop].line->setZValue(-1);
        addItem(ptrMatrix->WaysMatrix[firstTop][secondTop].line);
        addItem(ptrMatrix->WaysMatrix[firstTop][secondTop].text);
    }

    /*
    void deleteItem_Line(int firstTop, int secondTop)
    {
        removeItem(ptrMatrix->WaysMatrix[firstTop][secondTop].line);
        removeItem(ptrMatrix->WaysMatrix[firstTop][secondTop].text);
    }
    void deleteItem_Point(int index)
    {
        removeItem(ptrMatrix->Points[index]->nameOfTop);
        removeItem(ptrMatrix->Points[index]->point);
        removeItem(ptrMatrix->Points[index]->zone);
    }
    */


public slots:
    void deleteItem_Line(int firstTop, int secondTop)
    {
        removeItem(ptrMatrix->WaysMatrix[firstTop][secondTop].line);
        //delete ptrMatrix->WaysMatrix[firstTop][secondTop].line;
        removeItem(ptrMatrix->WaysMatrix[firstTop][secondTop].text);
    }
    void deleteItem_Point(int index)
    {
        removeItem(ptrMatrix->Points[index]->nameOfTop);
        removeItem(ptrMatrix->Points[index]->point);
        removeItem(ptrMatrix->Points[index]->zone);
    }

};

#endif // PAINTSCENE_H
