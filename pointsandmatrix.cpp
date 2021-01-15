#include "pointsandmatrix.h"

PointsAndMatrix::PointsAndMatrix(QObject *parent) : QObject(parent)
{
    WaysMatrix = nullptr;
}


int PointsAndMatrix::addPoint(int x, int y)
{
    // Створюємо нову матрицю, збільшену на один елемент від попередньої
    Way** buffWaysMatrix = new Way*[Points.size()+1];
    for(unsigned int i = 0; i<Points.size()+1; i++)
    {
        buffWaysMatrix[i] = new Way[Points.size()+1];
    }

    // Якщо WaysMatrix ще не ініціалізовано як масив
    // отже у нас ще не має ніяких вершин, і майбутня створенна вершина буде першою
    if(WaysMatrix==nullptr)
    {
        WaysMatrix = buffWaysMatrix;
        buffWaysMatrix = nullptr;
    }
    else   // інакше - копіюємо елементи з старої матриці в нову
    {
        for(unsigned i = 0; i < Points.size(); i++)
        {
            for(unsigned j = 0; j < Points.size(); j++)
            {
                buffWaysMatrix[i][j] = WaysMatrix[i][j];
            }
        }

        for(unsigned i = 0; i < Points.size(); i++)
        {
            delete[] WaysMatrix[i];
        }
        delete[] WaysMatrix;

        WaysMatrix = buffWaysMatrix;
        buffWaysMatrix = nullptr;

    }

    // Створюємо і добавляємо точку
    Points.push_back(new Point(x,y,Points.size(), activeZones));

    // Створюємо шляхи від новоствореної вершини до інших вершин
    // (Points.size()-1 - НЕ створюємо шлях сам до себе)
    for(int i = 0; i < Points.size()-1; i++)
    {
        //Генеруємо випадкову довжину шляху
        srand(time(NULL) - i*100); rand();
        int buff = 1 + rand() % 9999;
        buff = 1 + rand() % 9999 + buff;
        buff = 1 + rand() % 9999 + buff;
        srand(time(NULL) - buff); rand();

        int way = 1 + (rand()&0x7FFF) % 24;

        // Створюємо і додаємо у матрицю шлях
        addLine(i, Points.size()-1, way);
    }


    // Оновлюємо дані у випадаючих списках інтерфейсу програми
    emit signalSendMatrixSize(Points.size());

    return Points.size()-1;
}

void PointsAndMatrix::addLine(int firstTop, int secondTop, int size)
{
    // Зберігаємо розмір шляху
    WaysMatrix[firstTop][secondTop].size = size;
    // Створюємо об'єкт шляху (лінію)
    WaysMatrix[firstTop][secondTop].line = new QGraphicsLineItem(Points[firstTop]->X,
                                                                 Points[firstTop]->Y,
                                                                 Points[secondTop]->X,
                                                                 Points[secondTop]->Y);
    WaysMatrix[firstTop][secondTop].line->setPen(QPen(Qt::black));
    //WaysMatrix[firstTop][secondTop].linePen = new QPen(Qt::black);
    //Створюємо текст (який позначає номер)
    WaysMatrix[firstTop][secondTop].text = new QGraphicsTextItem(QString::number(size));
    WaysMatrix[firstTop][secondTop].text->setFont(QFont("", 10, 70, 0));
    int sizeX = 0, sizeY = 0;
    sizeX = Points[firstTop]->X < Points[secondTop]->X ? Points[firstTop]->X : Points[secondTop]->X;
    sizeY = Points[firstTop]->Y < Points[secondTop]->Y ? Points[firstTop]->Y : Points[secondTop]->Y;
    WaysMatrix[firstTop][secondTop].text->setPos(sizeX + (std::abs(Points[firstTop]->X - Points[secondTop]->X)/2) - 15
                                                 , sizeY + (std::abs(Points[firstTop]->Y - Points[secondTop]->Y)/2) - 15);

    // Заповнюємо симетричні комірки
    WaysMatrix[secondTop][firstTop].size = WaysMatrix[firstTop][secondTop].size;
    WaysMatrix[secondTop][firstTop].line = WaysMatrix[firstTop][secondTop].line;
    WaysMatrix[secondTop][firstTop].text = WaysMatrix[firstTop][secondTop].text;

    // Оновлюємо дані у випадаючих списках інтерфейсу програми
    emit signalSendMatrixSize(Points.size());
}

void PointsAndMatrix::deleteLine(int index_i, int index_j)
{
    //emit signalDeleteItem_Line(index_i, index_j);

    WaysMatrix[index_i][index_j].size = -1;
    delete WaysMatrix[index_i][index_j].line;
    delete WaysMatrix[index_i][index_j].text;
    WaysMatrix[index_i][index_j].line = nullptr;
    WaysMatrix[index_i][index_j].text = nullptr;

    WaysMatrix[index_j][index_i].size = -1;
    WaysMatrix[index_j][index_i].line = nullptr;
    WaysMatrix[index_j][index_i].text = nullptr;

    // Оновлюємо дані у випадаючих списках інтерфейсу програми
    emit signalSendMatrixSize(Points.size());
}

int PointsAndMatrix::deletePoint(int index)
{
    if(Points.size()==0)
    {
        qDebug() << "Points порожній. Елемент під індексом " << index << " не був видалений.";
        return -1;
    }
    if(index >= Points.size())
    {
        qDebug() << "Елемент під індексом " << index << " відсутній у Points. Елемент не був видалений.";
        return -2;
    }

    // Якщо матриця розміром 1х1
    if(Points.size()==1)
    {
        deleteLine(0, 0);

        delete[] WaysMatrix[0];
        delete[] WaysMatrix;
        WaysMatrix = nullptr;

    }
    else // Якщо матриця більша за матрицю розміром 1х1
    {
        // Видаляємо необхідні стовпчик і рядок
        for(int a = 0; a < Points.size(); a++)
        {
            if(a!=index)
            {
                deleteLine(index, a);   // Видалення стовпчика
                deleteLine(a, index);   // Видалення рядка
            }
        }
        deleteLine(index, index);

    // Створюємо нову матрицю, зменшену на один елемент від попередньої
    Way** buffWaysMatrix = new Way*[Points.size()-1];
    for(unsigned int i = 0; i<Points.size()-1; i++)
    {
        buffWaysMatrix[i] = new Way[Points.size()-1];
    }

    // Проводимо копіювання необхідних елементів
    // (Окрім видалених елементів рядка і стовпчика під індексами index)
    int I = 0, J = 0;
    for(unsigned i = 0; i < Points.size(); i++)
    {
        I = i;
        if(i!=index)
        {
            if(i>index) {--I;}
            for(unsigned j = 0; j < Points.size(); j++)
            {
                J = j;
                if(j!=index)
                {
                    if(j>index) {--J;}
                    buffWaysMatrix[I][J] = WaysMatrix[i][j];
                }
            }
        }
    }

    // Видаляємо старий масив
    for(unsigned i = 0; i < Points.size(); i++)
    {
        delete[] WaysMatrix[i];
    }
    delete[] WaysMatrix;

    // Зберігаємо вказівник на оновлену матрицю
    WaysMatrix = buffWaysMatrix;
    }

    // Видаляємо елемент вектора Points

    //emit signalDeleteItem_Point(index);

    delete Points[index];

    // Оновлюємо вектор Points
    for(int i = index; i < Points.size()-1;i++)
    {
        Points[i] = Points[i+1];
        Points[i]->Number = i;
        Points[i]->nameOfTop->setPlainText(QString::number(i));
    }

    Points.pop_back();

    // Оновлюємо дані у випадаючих списках інтерфейсу програми
    emit signalSendMatrixSize(Points.size());

    return 1;

}

void PointsAndMatrix::deleteAll()
{
    // Оновлюємо дані у випадаючих списках інтерфейсу програми
    // У даному випадку - очищуємо
    emit signalSendMatrixSize(-1);

    for(int i = 0; i < Points.size(); i++)
    {
        for(int j = 0; j < Points.size(); j++)
        {
            deleteLine(i,j);
        }
    }

    for(int i = 0; i < Points.size(); i++)
    {
        delete[] WaysMatrix[i];
    }
    delete[] WaysMatrix;
    WaysMatrix = nullptr;

    for(int i = 0; i < Points.size(); i++)
    {
        delete Points[i];
    }

    Points.clear();
}

void PointsAndMatrix::coutMatrix()
{
    qDebug() << "\n";
    QDebug aaa = qDebug();
    aaa << "    ";
    for(int i = 0; i < Points.size(); i++)
    {
        aaa << i << " ";
    }
    aaa << "\n";
    qDebug() = aaa;
    for(int i = 0; i < Points.size(); i++)
    {
        for(int j = 0; j < Points.size(); j++)
        {
            if(j==0) {aaa << i << " ";}
            aaa << WaysMatrix[i][j].size << " ";
        }
        aaa << "\n";
        qDebug() = aaa;
    }
}

void PointsAndMatrix::setLineColor(int firstTop, int secondTop, int chose)
{
    if(WaysMatrix[firstTop][secondTop].line != nullptr && firstTop >=0 && secondTop >= 0)
    {
        switch(chose)
        {
        case 0:
        {
            WaysMatrix[firstTop][secondTop].line->setPen(QPen(Qt::black));
            break;
        }
        case 1:
        {
            WaysMatrix[firstTop][secondTop].line->setPen(QPen(Qt::green));
            break;
        }
        case 2:
        {
            WaysMatrix[firstTop][secondTop].line->setPen(QPen(Qt::red));
            break;
        }
        }
    }
}

void PointsAndMatrix::setPointColor(int firstTop, int chose)
{
    if(Points[firstTop]->point != nullptr && firstTop >=0)
    {
        switch(chose)
        {
        case 0:
        {
            Points[firstTop]->point->setBrush(QColor(255,127,39));
            Points[firstTop]->point->setPen(QPen(QColor(255,127,39)));
            break;
        }
        case 1:
        {
            Points[firstTop]->point->setBrush(QColor(Qt::green));
            Points[firstTop]->point->setPen(QPen(Qt::green));
            break;
        }
        case 2:
        {
            Points[firstTop]->point->setBrush(QColor(Qt::red));
            Points[firstTop]->point->setPen(QPen(Qt::red));
            break;
        }
        }
    }
}

void PointsAndMatrix::setAllPointsOneColor(int chose)
{
    for(int i = 0; i < Points.size(); i++)
    {
        setPointColor(i, chose);
    }
}


void PointsAndMatrix::setWaySize(int firstTop, int secondTop, int size)
{
    WaysMatrix[firstTop][secondTop].size = size;
    WaysMatrix[secondTop][firstTop].size = size;
    WaysMatrix[firstTop][secondTop].text->setHtml(QString::number(size));
}

void PointsAndMatrix::setZones(int arg)
{
    activeZones = arg;
    if(arg)
    {
        for(int i = 0; i < Points.size(); i++)
        {
            Points[i]->zone->setPen(QPen(Qt::black));
        }
    }
    else
    {
        for(int i = 0; i < Points.size(); i++)
        {
            Points[i]->zone->setPen(QPen(Qt::NoPen));
        }
    }
}

void PointsAndMatrix::setAllLinesOneColor(int chose)
{
    for(int i = 0; i < Points.size(); i++)
    {
        for(int j = 0; j < Points.size(); j++)
        {
            setLineColor(i, j, chose);
        }
    }
}

int PointsAndMatrix::degreeOfTop(int numbOfTop)
{
    int count = 0;
    for(int j = 0; j < Points.size(); j++)
    {
        if(WaysMatrix[numbOfTop][j].size != -1) {++count;}
    }

    return count;
}

int PointsAndMatrix::gamilton(int k, int size, int* path, int* c, int** a, int v0)
{
int v,q1=0;
    for(v=0; v<size && !q1; v++)
    {
      if(a[v][path[k-1]]||a[path[k-1]][v])
      {
    if (k==size &&  v==v0 ) q1=1;
    else if (c[v]==-1)
            {
          c[v] = k ; path[k]=v;
          q1=gamilton (k+1, size, path, c, a, v0);
          if (!q1) c[v]=-1;
        } else continue;
    }
    }   return q1;
}

bool PointsAndMatrix::isHamiltonianGraph(int **ptr, int size)
{
    bool result = 0;

    int **arr = new int*[size];
    for(int i = 0; i < size; i++)
    {
        arr[i] = new int[size];
    }
    int* c = new int[size];
    int* path = new int[size];
    int v0=0;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(ptr[i][j]==-1) { arr[i][j]=0; }
            else { arr[i][j]=1; }
        }
    }

    for(int i = 0;i < size;i++)
    {
        c[i]=-1;
    }
    path[0]=v0;
    c[v0]=v0;

    if(gamilton(1, size, path, c, arr, v0)){ result = 1; }
    else { result = 0; }

    for(int i = 0; i < size; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
    delete[] c;
    delete[] path;

    return result;
}

bool PointsAndMatrix::HamiltonianGraph(int index1)
{
    int **array = new int*[Points.size()-1];
    for(int i = 0; i < Points.size()-1; i++)
    {
        array[i] = new int[Points.size()-1];
    }

    int I = 0, J = 0;
    for(unsigned i = 0; i < Points.size(); i++)
    {
        I = i;
        if(i!=index1)
        {
            if(i>index1) {--I;}
            for(unsigned j = 0; j < Points.size(); j++)
            {
                J = j;
                if(j!=index1)
                {
                    if(j>index1) {--J;}
                    array[I][J] = WaysMatrix[i][j].size;
                }
            }
        }
    }

    bool answer =  isHamiltonianGraph(array, Points.size()-1);

    for(int i = 0; i < Points.size()-1; i++)
    {
        delete[] array[i];
    }
    delete[] array;

    return answer;
}

bool PointsAndMatrix::HamiltonianGraph(int index1, int index2)
{
    int **array = new int*[Points.size()];
    for(int i = 0; i < Points.size(); i++)
    {
        array[i] = new int[Points.size()];
    }

    for(int i = 0; i < Points.size(); i++)
    {
        for(int j = 0; j < Points.size(); j++)
        {
            array[i][j] = WaysMatrix[i][j].size;
        }
    }

    array[index1][index2] = -1;
    array[index2][index1] = -1;

    bool answer =  isHamiltonianGraph(array, Points.size());

    for(int i = 0; i < Points.size(); i++)
    {
        delete[] array[i];
    }
    delete[] array;

    return answer;
}

void PointsAndMatrix::ActivateSalesmansMethod()
{
    if(Points.size() > 2)
    {
        setAllLinesOneColor(0);

        int **arr = new int*[Points.size()];
        for(int i = 0; i < Points.size(); i++)
        {
            arr[i] = new int[Points.size()];
        }

        for(int i = 0; i < Points.size(); i++)
        {
            for(int j = 0; j < Points.size(); j++)
            {
                arr[i][j] = WaysMatrix[i][j].size;
            }
        }


        std::vector<SalesmansMethod::Edge> *Ways = new std::vector<SalesmansMethod::Edge>;;

        SalesmansMethod* Method = new SalesmansMethod(arr, Points.size(), Ways);

        Method->mainFunction();

        delete Method;

        for(int i = 0; i < (*Ways).size(); i++)
        {
            setLineColor((*Ways)[i].firstTop, (*Ways)[i].secondTop, 2);
        }

        delete Ways;

        for(int i = 0; i < Points.size(); i++)
        {
           delete[] arr[i];
        }
        delete[] arr;
    }
}


PointsAndMatrix::~PointsAndMatrix()
{

}
