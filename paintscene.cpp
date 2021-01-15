#include "paintscene.h"


PaintScene::PaintScene(QGraphicsScene* parent) : QGraphicsScene(parent)
{

}

PaintScene::~PaintScene()
{

}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //Якщо у місці, де розташований курсор миші,
        // вже наявний якийсь елемент сцени, то не створюємо вершину
        if(items(QPointF(event->scenePos().x(), event->scenePos().y())).size()==0)
        {
            // Отримуємо вказівник на об'єкт класа PointsAndMatrix
            if(ptrMatrix==nullptr) {emit signalGetPtrMatrix(ptrMatrix);}

            // Створюємо і додаємо у контейнер вершину
            // buffNumbOfCreatedPoint - індекс останньої створеної вершини
            int buffNumbOfCreatedPoint = ptrMatrix->addPoint(event->scenePos().x(), event->scenePos().y());

            // Рисуємо вершину
            addItem_Point(buffNumbOfCreatedPoint);

            for(int i = 0; i < buffNumbOfCreatedPoint; i++)
            {
            // Рисуємо шляхи від новоствореної вершини до інших вершин
            addItem_Line(i, buffNumbOfCreatedPoint);
            }


            qDebug() << "\n";
            QDebug aaa = qDebug();
            aaa << "    ";
            for(int i = 0; i < ptrMatrix->Points.size(); i++)
            {
                aaa << i << " ";
            }
            aaa << "\n";
            qDebug() = aaa;
            for(int i = 0; i < ptrMatrix->Points.size(); i++)
            {
                for(int j = 0; j < ptrMatrix->Points.size(); j++)
                {
                    if(j==0) {aaa << i << " ";}
                    aaa << ptrMatrix->WaysMatrix[i][j].size << " ";
                }
                aaa << "\n";
                qDebug() = aaa;
            }


        }

    }

    if(event->button() == Qt::RightButton)
    {
        /*
        for(int i = 0; i < ptrMatrix->Points.size(); i++)
        {
            for(int j = 0; j < ptrMatrix->Points.size(); j++)
            {
                deleteItem_Line(i, j);
                ptrMatrix->deleteLine(i,j);
            }
        }
        */
        ptrMatrix->deletePoint(0);
    }

    if(event->button() == Qt::MiddleButton)
    {
        /*
        for(int i = 0; i < ptrMatrix->Points.size(); i++)
        {
            for(int j = 0; j < ptrMatrix->Points.size(); j++)
            {
                deleteItem_Line(i, j);
                ptrMatrix->deleteLine(i,j);
            }
        }
        */
        ptrMatrix->deleteAll();
    }
}
