#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new PaintScene();
    scene->setSceneRect(0,0, ui->graphicsView->size().width()-15,ui->graphicsView->size().height()-15);
    ui->graphicsView->setScene(scene);

    Matrix = new PointsAndMatrix();

    connect(scene, &PaintScene::signalGetPtrMatrix, this, &MainWindow::sendPtrMatrix);

    //connect(Matrix, &PointsAndMatrix::signalDeleteItem_Line, scene, &PaintScene::deleteItem_Line);
    //connect(Matrix, &PointsAndMatrix::signalDeleteItem_Point, scene, &PaintScene::deleteItem_Point);

    connect(Matrix, &PointsAndMatrix::signalSendMatrixSize, this, &MainWindow::slotGetMatrixSize);

    this->setFixedSize(1024,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendPtrMatrix(PointsAndMatrix*& out_ptrMatrix)
{
    out_ptrMatrix = Matrix;
}

// Отримую кількість вершин і оновлюю випадаючі списки
// (ComboBox 0 та 2, 3-й оновлюється в залежності від 2-го)
void MainWindow::slotGetMatrixSize(int size)
{
    // Оновлюємо випадаючі списки
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();
    if(size!=-1)
    {
        for(int i = 0; i < size; i++)
        {
            ui->comboBox->addItem(QString::number(i));
            ui->comboBox_2->addItem(QString::number(i));
            ui->comboBox_4->addItem(QString::number(i));
        }
    }
}



// Кнопка видалення вершини
void MainWindow::on_pushButton_clicked()
{
    if(ui->comboBox->currentIndex() >=0 && Matrix->Points.size() >=2)
    {

    if(Matrix->HamiltonianGraph(ui->comboBox->currentIndex()))
    {
        Matrix->deletePoint(ui->comboBox->currentIndex());
    }
    else
    {
        qDebug() << "No HamiltonianGraph";
    }

    }

}

// Кнопка видалення ребра
void MainWindow::on_pushButton_2_clicked()
{
    int i = ui->comboBox_2->currentText().toInt();
    int j = ui->comboBox_3->currentText().toInt();
    if(Matrix->Points.size()>0 && i>=0 && j>=0)
    {
        if(Matrix->HamiltonianGraph(i,j))
        {
            Matrix->deleteLine(i,j);
        }
        else
        {
            qDebug() << "No HamiltonianGraph";
        }
    }
}

// Оновлення ComboBox_3
void MainWindow::updateComboBox_3(int indexOfComboBox_2)
{
    if(indexOfComboBox_2>=0)
    {
        ui->comboBox_3->clear();

        for(int j = 0; j < Matrix->Points.size(); j++)
        {
            if(Matrix->WaysMatrix[indexOfComboBox_2][j].size != -1)
            {
                ui->comboBox_3->addItem(QString::number(j));
            }
        }
    }

}

void MainWindow::updateComboBox_5(int indexOfComboBox_4)
{
    if(indexOfComboBox_4>=0)
    {
        ui->comboBox_5->clear();

        for(int j = 0; j < Matrix->Points.size(); j++)
        {
            if(Matrix->WaysMatrix[indexOfComboBox_4][j].size == -1 && indexOfComboBox_4!=j)
            {
                ui->comboBox_5->addItem(QString::number(j));
            }
        }
    }

}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    updateComboBox_3(index);
}

void MainWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    ui->lineEdit->clear();

    int i = ui->comboBox_2->currentIndex();
    int j = arg1.toInt();

    if(i >= 0 && j >= 0)
    {
        Matrix->setAllLinesOneColor(0);
        Matrix->setLineColor(i, j, 1);
        ui->lineEdit->setText(QString::number(Matrix->WaysMatrix[i][j].size));
    }

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    int i = ui->comboBox_2->currentIndex();
    int j = ui->comboBox_3->currentText().toInt();
    if(arg1.toInt() > 0 && arg1.toInt() < 101 && i >= 0 && j >= 0)
    {
        Matrix->setWaySize(i, j, arg1.toInt());
    }

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    Matrix->setZones(arg1);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index >=0)
    {
        ui->label_4->clear();
        ui->label_4->setText(QString::number(Matrix->degreeOfTop(index)));
        Matrix->setAllPointsOneColor(0);
        Matrix->setPointColor(index, 1);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    Matrix->deleteAll();
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    updateComboBox_5(index);
}

void MainWindow::on_pushButton_4_clicked()
{
    int i = ui->comboBox_4->currentIndex();
    int j = ui->comboBox_5->currentText().toInt();

    if(i!=j)
    {
    if(ui->lineEdit_2->text().toInt() > 0 && ui->lineEdit_2->text().toInt() <= 100 && i >= 0 && j >= 0)
    {
        Matrix->addLine(i, j, ui->lineEdit_2->text().toInt());
        scene->addItem_Line(i, j);
    }
    else
    {
        QMessageBox::warning(0,"Додання ребра", "Невірно вказанне значення ребра");
    }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    Matrix->ActivateSalesmansMethod();
}
