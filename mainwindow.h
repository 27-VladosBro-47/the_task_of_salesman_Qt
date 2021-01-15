#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "paintscene.h"
#include "point.h"
#include "pointsandmatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    PaintScene* scene = nullptr;
    PointsAndMatrix* Matrix  = nullptr;

    void updateComboBox_3(int indexOfComboBox_2);
    void updateComboBox_5(int indexOfComboBox_4);

private slots:
    void sendPtrMatrix(PointsAndMatrix*& out_ptrMatrix);
    void slotGetMatrixSize(int size);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_2_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_checkBox_stateChanged(int arg1);
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButton_3_clicked();
    void on_comboBox_4_currentIndexChanged(int index);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
};
#endif // MAINWINDOW_H
