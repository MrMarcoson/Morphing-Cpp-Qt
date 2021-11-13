//Marek Kasprowicz 303020

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //setery dla moich dwoch puntkow
    void setA(int x, int y);
    void setB(int x, int y);

private slots:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void on_pushButtonCzysc_2_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui, *ui2;
    QImage *img_1, *img_2, *img_3, *obraz_1, *obraz_2;
    QImage *pom1, *pom2, *pom_klatka;
    int startX_1, startY_1, startX_2, startY_2, startX_3, startY_3, height_1, width_1, height_2, width_2, height_3, width_3;
    int N = 60;


    //zmienne punktow wybieranych przy kliknieciu myszki
    int x1, y1, x2, y2;

    bool wstaw = false, przesun = false;

    //struktura przechowywujaca dane o punktach uzywanych do rysowania krzywej bezjera
    struct punkt
    {
        double x;
        double y;
    };

    struct RGB
    {
        int r, g, b;
    };


    //funkcja liczaca B
    punkt wyliczB(double t);


    //wszystkie punkty dodane przez usera
    std::vector<punkt> punkty1, punkty2;
    std::vector<QImage*> klatki;


    //ilosc wierzcholkow w aktualnej krzywej, indeks pierwszego wierzcholka w nowej krzywej, oraz indeks wskazujacy na konkretny punkt (potrzebne do manipulacji punktami)
    int ilosc_wierzcholkow = 0;
    int pierwszy_wierzcholek = 0;
    int index;
    bool interpolacja = false, first_1 = true, first_2 = true;


    int wyznaczNajblizszy(int a, int b, int obraz);
    double wyznaczOdleglosc(int x1, int y1, int x2, int y2);


    //funkcje rysujace figury
    void rysujPixel(int x, int y, int b, int g, int r, int obraz);
    RGB czytajPiksel(int x, int y, int obraz);
    void rysujOdcinek(int x1, int y1, int x2, int y2, int obraz);
    void rysujZnacznik(int x, int y, int obraz);
    void rysujTrojkat(int obraz);
    void rysujKrzywa();
    void rysujObraz();
    void teksturowanie(std::vector<punkt> punkty, int klatka);
    void stworzeniePom1();
    void stworzeniePom2();
    void morphing(int klatka);
    void blending(QImage *obraz, QImage *tlo);
    void wyczysc();
};
#endif // MAINWINDOW_H
