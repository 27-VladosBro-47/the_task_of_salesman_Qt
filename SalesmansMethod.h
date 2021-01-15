#ifndef SALESMANSMETHOD_H
#define SALESMANSMETHOD_H

#include <vector>



class SalesmansMethod
{
public:
    struct Edge
    {
        int firstTop;
        int secondTop;

        Edge(int I, int J) : firstTop(I), secondTop(J)
        {

        }
    };

private:

    class Zero
    {
    public:
        int quantity;
        int i;
        int j;

        Zero(int Q, int I, int J) : quantity{ Q }, i{ I }, j{ J }
        {
        }
        ~Zero()
        {
        }
    };

private:

    int **array = nullptr;				// Матриця
    int **arraySecond = nullptr;		// Матриця
    //Перші рядок і стовпчик містять у собі назви відповідних рядків і стовпчиків

    const int firstSizeArr;				// Початковий розмір матриці
    int sizeArr;							// Розміри матриць
    int sizeArrSecond;
    int *di = nullptr, *dj = nullptr;		// "вектори" стовбець і рядок відповідно
    int *diSecond = nullptr, *djSecond = nullptr;

    int di_sum, dj_sum;					// Сума значень відповідних "векторів"

    // Тимчасово не використовую
    //========================================
    //std::vector<Edge> *X = nullptr;		// Довільний шлях
    //int HX;								// сума ребер в x0
    //========================================

    std::vector<Edge> *Ways = nullptr;	// Масив ребер, з яких формуватиметься найкоротший шлях
    int HWays;							// Сума ребер в Ways

    int H_current, H_0, H_1;			// Зберігають розмір шляхів (H_current - поточний)
                                        // (H_0 - розмір шляху, якщо виключили ребро)
                                        // (H_1 - розмір шляху, якщо включили ребро)

    std::vector<Zero> zerosArray;

public:
    SalesmansMethod(int** arr, int size, std::vector<Edge> *&ways);

    int findMinElemColumn(int j, int currI = -1);
    int findMinElemRow(int i, int currJ = -1);
    void findDi();
    void findDj();
    void reduceMatrix();
    void findZeros();
    void findZeros1();
    int findBiggerZero();
    void exclusionSimilarEdge(int indexI, int indexJ, bool choseArr);
    void edgesExclusion(int indexI, int indexJ);
    void chooseMatrix(int chose);
    void mainFunction();

    ~SalesmansMethod();
};



#endif // SALESMANSMETHOD_H
