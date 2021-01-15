#ifndef POINTSANDMATRIX_H
#define POINTSANDMATRIX_H

#include "point.h"
#include "SalesmansMethod.h"

#include <vector>
#include <QObject>
#include <functional>
#include <ctime>
#include <random>

// Структура, що описує комірку матриці
struct Way
{
    QGraphicsLineItem* line = nullptr;        // Лінія, що сполучатиме дві вершини
    QGraphicsTextItem* text = nullptr;        // Довжина шляху, яка відображатиметься на лінії
    int size = -1;                  // Фактичне значення довжини шляху
                                    //(за замовчуванням -1 - шлях відсутній)
};

class PointsAndMatrix : public QObject
{
    Q_OBJECT

public:
    std::vector<Point*> Points;
    Way** WaysMatrix;

    int activeZones = 0;

    explicit PointsAndMatrix(QObject *parent = Q_NULLPTR);

    ~PointsAndMatrix();

signals:
    void signalDeleteItem_Line(int index_i, int index_j);
    void signalDeleteItem_Point(int index);

    void signalSendMatrixSize(int size);

public:

    int addPoint(int x, int y);
    void addLine(int firstTop, int secondTop, int size);
    void deleteLine(int index_i, int index_j);
    int deletePoint(int index);
    void deleteAll();
    void coutMatrix();
    void setLineColor(int firstTop, int secondTop, int chose);
    void setPointColor(int firstTop, int chose);
    void setAllPointsOneColor(int chose);

    void setWaySize(int firstTop, int secondTop, int size);
    void setZones(int arg);
    void setAllLinesOneColor(int chose);
    int degreeOfTop(int numbOfTop);
    int gamilton(int k, int size, int* path, int* c, int** a, int v0);
    bool isHamiltonianGraph(int **ptr, int size);
    bool HamiltonianGraph(int index1);
    bool HamiltonianGraph(int index1, int index2);
    void ActivateSalesmansMethod();
};

#endif // POINTSANDMATRIX_H
