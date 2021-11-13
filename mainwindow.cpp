//Marek Kasprowicz 303020

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include<QThread>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startX_1 = ui->frame->x();
    startY_1 = ui->frame->y();
    width_1 = ui->frame->width();
    height_1 = ui->frame->height();

    startX_2 = ui->frame_2->x();
    startY_2 = ui->frame_2->y();
    width_2 = ui->frame->width();
    height_2 = ui->frame->height();

    startX_3 = ui->frame_3->x();
    startY_3 = ui->frame_3->y();
    width_3 = ui->frame->width();
    height_3 = ui->frame->height();


    img_1 = new QImage(width_1, height_1, QImage::Format_RGB32);
    img_2 = new QImage(width_2, height_2, QImage::Format_RGB32);
    img_3 = new QImage(width_2, height_2, QImage::Format_RGB32);

    obraz_1 = new QImage("..\\zad12_morphing\\szop.png");
    obraz_2 = new QImage("..\\zad12_morphing\\10000_days.jpg");

    *img_1 = obraz_1 -> copy(0, 0, width_1, height_1);
    *img_2 = obraz_2 -> copy(0, 0, width_2, height_2);

    pom1 = new QImage(width_1, height_1, QImage::Format_RGB32);
    pom2 = new QImage(width_2, height_2, QImage::Format_RGB32);
    pom_klatka = new QImage(width_3, height_3, QImage::Format_RGB32);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(startX_1, startY_1, *img_1);
    p.drawImage(startX_2, startY_2, *img_2);
    p.drawImage(startX_3, startY_3, *img_3);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int a, b, a1, b1, a2, b2;
    punkt P;

    a = event->pos().x();
    b = event->pos().y();

    a1 = a - startX_1;
    b1 = b - startY_1;
    a2 = a - startX_2;
    b2 = b - startY_2;

    if(event->button() == Qt::LeftButton)
    {

        //Lewy obraz
        if(a1 <= width_1)
        {
            P.x = a1;
            P.y = b1;

            if(wstaw)
            {
                if(punkty1.size() < 3)
                {
                    punkty1.push_back(P);
                    rysujZnacznik(a1, b1, 1);

                    if(punkty1.size() == 3)
                    {
                        rysujTrojkat(1);
                        if(punkty2.size() == 3) morphing(0);
                    }
                }
            }

            if(przesun)
            {
                index  = wyznaczNajblizszy(a1, b1, 1);
            }

        }

        //Prawy obraz
        else
        {
            P.x = a2;
            P.y = b2;

            if(wstaw)
            {
                if(punkty2.size() < 3)
                {
                    punkty2.push_back(P);
                    rysujZnacznik(a2, b2, 2);

                    if(punkty2.size() == 3)
                    {
                        rysujTrojkat(2);
                        if(punkty1.size() == 3) morphing(0);
                    }
                }
            }

            if(przesun)
            {
                index  = wyznaczNajblizszy(a2, b2, 2);
            }
        }
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int a, b, a1, b1, a2, b2;

    a = event->pos().x();
    b = event->pos().y();

    a1 = a - startX_1;
    b1 = b - startY_1;
    a2 = a - startX_2;
    b2 = b - startY_2;

    if(event->button() == Qt::LeftButton)
    {

        //Lewy obraz
        if(a1 <= width_1)
        {
            if(punkty1.size() == 3 && punkty2.size() == 3 && przesun)
            {
                wyczysc();
                punkty1[index].x = a1;
                punkty1[index].y = b1;

                rysujZnacznik(punkty1[0].x, punkty1[0].y, 1);
                rysujZnacznik(punkty1[1].x, punkty1[1].y, 1);
                rysujZnacznik(punkty1[2].x, punkty1[2].y, 1);
                rysujTrojkat(1);

                rysujZnacznik(punkty2[0].x, punkty2[0].y, 2);
                rysujZnacznik(punkty2[1].x, punkty2[1].y, 2);
                rysujZnacznik(punkty2[2].x, punkty2[2].y, 2);
                rysujTrojkat(2);

                morphing(1);
            }
        }

        //Prawy obraz
        else
        {
            if(punkty1.size() == 3 && punkty2.size() == 3 && przesun)
            {
                wyczysc();
                punkty2[index].x = a2;
                punkty2[index].y = b2;

                rysujZnacznik(punkty1[0].x, punkty1[0].y, 1);
                rysujZnacznik(punkty1[1].x, punkty1[1].y, 1);
                rysujZnacznik(punkty1[2].x, punkty1[2].y, 1);
                rysujTrojkat(1);

                rysujZnacznik(punkty2[0].x, punkty2[0].y, 2);
                rysujZnacznik(punkty2[1].x, punkty2[1].y, 2);
                rysujZnacznik(punkty2[2].x, punkty2[2].y, 2);
                rysujTrojkat(2);

                morphing(1);
            }
        }
    }
}


//funkcja rysujaca odcinki
void MainWindow::rysujOdcinek(int x1, int y1, int x2, int y2, int obraz)
{
    int x, y;
    bool ver = false;
    double m = (double)(y2-y1)/(x2-x1);

    //gdy nie ruszymy myszką, to nie rysuj
    if(x1 == x2 && y1 == y2)
    {
        return;
    }

    //jesli wartosc kierunkowa jest > 1 to trzeba zamienic zmienne (w celu eliminacji "kropkowania")
    if(abs(m) > 1)
    {
        int pom;
        pom = x1;
        x1 = y1;
        y1 = pom;

        pom = x2;
        x2 = y2;
        y2 = pom;

        m = (double)(y2-y1)/(x2-x1);
        ver = true;
    }

    //w celu mozliwosci rysowania w obie strony, zamieniamy miejscami punkty gdy drugie klikniecie jest bardziej w lewo od pierwszego
    if(x1 > x2)
    {
        int pom;
        pom = x1;
        x1 = x2;
        x2 = pom;

        pom = y1;
        y1 = y2;
        y2 = pom;
    }

    //petla rysujaca
    for(x=x1; x<=x2; x++)
    {
        y = m * (x - x1) + y1;

        //jesli wspolczynnik m byl wiekszy od 1 to nalezy zamienic wspolczynniki miejscami (obrocic uklad wspolrzednych)
        if(ver == 0) rysujPixel(x, round(y), 255, 255, 255, obraz);
        else rysujPixel(y, x, 255, 255, 255, obraz);
    }

}

//rysowanie znacznika 8x8px
void MainWindow::rysujZnacznik(int x, int y, int obraz)
{
    for(int i=-4; i<4; i++)
    {
        for(int j=-4; j<4; j++)
        {
            rysujPixel(x - i, y - j, 255, 0, 0, obraz);
        }
    }

    update();
}

void MainWindow::rysujTrojkat(int obraz)
{
    if(obraz == 1)
    {
        rysujOdcinek(punkty1[0].x, punkty1[0].y, punkty1[1].x, punkty1[1].y, obraz);
        rysujOdcinek(punkty1[1].x, punkty1[1].y, punkty1[2].x, punkty1[2].y, obraz);
        rysujOdcinek(punkty1[2].x, punkty1[2].y, punkty1[0].x, punkty1[0].y, obraz);
    }

    else
    {
        rysujOdcinek(punkty2[0].x, punkty2[0].y, punkty2[1].x, punkty2[1].y, obraz);
        rysujOdcinek(punkty2[1].x, punkty2[1].y, punkty2[2].x, punkty2[2].y, obraz);
        rysujOdcinek(punkty2[2].x, punkty2[2].y, punkty2[0].x, punkty2[0].y, obraz);
    }

}

void MainWindow::morphing(int n)
{
    punkt P1, P2, P3;
    std::vector<punkt> punkty;

    //wyznaczenie wierzcholkow nowego trojkata
    P1.x = (1 - (double)n/N) * punkty1[0].x + (double)n/N * punkty2[0].x;
    P1.y = (1 - (double)n/N) * punkty1[0].y + (double)n/N * punkty2[0].y;
    P2.x = (1 - (double)n/N) * punkty1[1].x + (double)n/N * punkty2[1].x;
    P2.y = (1 - (double)n/N) * punkty1[1].y + (double)n/N * punkty2[1].y;
    P3.x = (1 - (double)n/N) * punkty1[2].x + (double)n/N * punkty2[2].x;
    P3.y = (1 - (double)n/N) * punkty1[2].y + (double)n/N * punkty2[2].y;

    punkty.push_back(P1);
    punkty.push_back(P2);
    punkty.push_back(P3);

    //wyczyszczenie 3 obrazku
    unsigned char *wsk;

    for(int i=0; i<height_3; i++)
    {
        wsk = img_3->scanLine(i);

        for(int j=0; j<width_3; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }

    }

    teksturowanie(punkty, n);
    punkty.clear();
}


//Nie dziala poprawne teksturowanie
void MainWindow::teksturowanie(std::vector<punkt> punkty, int klatka)
{
    //scanline
    //znalezienie najwyzszego i najnizszego punktu w wielokacie
    int min_y = punkty.front().y;
    int max_y = punkty.front().y;

    for(int i = 1; i < punkty.size(); i++)
    {
        if(min_y > punkty.at(i).y)
        {
            min_y = punkty.at(i).y;
        }

        if(max_y < punkty.at(i).y)
        {
            max_y = punkty.at(i).y;
        }
    }

    //petla przechodzaca przez moj wielokat
    for(int i = min_y; i <= max_y; i++)
    {
        //tablica dla wszystkich przecietych przez y punktow
        std::vector<int> przeciete;


        //iteruje po prostych w wielokacie
        for(int j = 1; j < punkty.size(); j++)
        {
            //sprawdzam czy proste sa przeciete przez y
            if((punkty.at(j - 1).y < i && punkty.at(j).y >= i)  || (punkty.at(j - 1).y >= i && punkty.at(j).y < i))
            {
                //wyliczenie czesci wzoru talesa
                double licz = punkty.at(j).x - punkty.at(j - 1).x;
                double mian = punkty.at(j).y - punkty.at(j - 1).y;
                int x;

                if(mian != 0)
                {
                    //wyliczenie za pomoca tw talesa punktu przeciecia i dodanie go do listy
                    x = (punkty.at(j - 1).x) + ((i - punkty.at(j - 1).y) * (double)(licz / mian));
                    przeciete.push_back(x);
                }
            }
        }


        //manualne sprawdzenie dla ostatniego do pierwszego
        if((punkty.back().y < i && punkty.front().y >= i)  || (punkty.back().y >= i && punkty.front().y < i))
        {
            double licz = punkty.front().x - punkty.back().x;
            double mian = punkty.front().y - punkty.back().y;
            int x;

            if(mian != 0)
            {
                x = (punkty.back().x) + (i - punkty.back().y) * (double)(licz / mian);
                przeciete.push_back(x);
            }
        }

        //sortowanie tablicy przecietych punktow x
        sort(przeciete.begin(), przeciete.end());


        RGB kolor1, kolor2;
        double x_, y_;
        double w, w_v, w_w, v_, w_, u_;

        w = (punkty[1].x - punkty[0].x) * (punkty[2].y - punkty[0].y) - (punkty[1].y - punkty[0].y) * (punkty[2].x - punkty[0].x);

        if(przeciete.size() >= 2)
        {
            //Wyliczanie wspolrzednych barycentrycznych z dwoch tesktur
            for(int k = przeciete.at(0); k < przeciete.at(1); k++)
            {
                w_w = (punkty[1].x - punkty[0].x) * (i - punkty[0].y) - (punkty[1].y - punkty[0].y) * (k - punkty[0].x);
                w_v = (k - punkty[0].x) * (punkty[2].y - punkty[0].y) - (i - punkty[0].y) * (punkty[2].x - punkty[0].x);
                v_ = (double)w_v / w;
                w_ = (double)w_w / w;
                u_ = 1 - v_ - w_;

                //wyznaczam odpowiadajacy kolor z pierwszego trojkata, do trojkata w klatce
                x_ = (double)(u_ * punkty1[0].x + v_ * punkty1[1].x + w_ * punkty1[2].x);
                y_ = (double)(u_ * punkty1[0].y + v_ * punkty1[1].y + w_ * punkty1[2].y);

                kolor1 = czytajPiksel(x_, y_, 1);

                //wyznaczam odpowiadajacy kolor z drugiego trojkata, do trojkata w klatce
                x_ = (double)(u_ * punkty2[0].x + v_ * punkty2[1].x + w_ * punkty2[2].x);
                y_ = (double)(u_ * punkty2[0].y + v_ * punkty2[1].y + w_ * punkty2[2].y);

                kolor2 = czytajPiksel(x_, y_, 2);

                //Blend kolorow
                RGB kolor;
                kolor.r = (1 - (double)klatka/N) * kolor1.r + (double)klatka/N * kolor2.r;
                kolor.g = (1 - (double)klatka/N) * kolor1.g + (double)klatka/N * kolor2.g;
                kolor.b = (1 - (double)klatka/N) * kolor1.b + (double)klatka/N * kolor2.b;
                rysujPixel(k, i, kolor.b, kolor.g, kolor.r, 3);                             //6 == rysowanie do pomocniczej
            }

        }
    }

    /*
    //dodanie narysowanej klatki do tablicy
    klatki.push_back(pom_klatka);


    //usuniecie pomocniczej
    unsigned char *wsk;
    for(int i=0; i<height_3; i++)
    {
        wsk = pom_klatka->scanLine(i);

        for(int j=0; j<width_3; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }

    }*/
}

//funkcja rysujaca pojedynczy piksel
void MainWindow::rysujPixel(int x, int y, int b, int g, int r, int obraz)
{
    //zabezpieczenie na wypadek wyjscia poza ekran (w wypadku np. okregu)
    if(x > width_1 || x < 0 || y > height_1 || y < 0) return;

    unsigned char *wsk;

    if(obraz == 1)      wsk = img_1->scanLine(y);
    else if(obraz == 2) wsk = img_2->scanLine(y);
    else if(obraz == 3) wsk = img_3->scanLine(y);
    else if(obraz == 4) wsk = pom1->scanLine(y);
    else if(obraz == 5) wsk = pom2->scanLine(y);
    else                wsk = pom_klatka->scanLine(y);

    wsk[4*x] = b;
    wsk[4*x+1] = g;
    wsk[4*x+2] = r;
    update();
}


//funkcja ktora odczyta kolor obrazu
MainWindow::RGB MainWindow::czytajPiksel(int x, int y, int obraz)
{
    RGB kolor;
    kolor.b = 0;
    kolor.g = 0;
    kolor.r = 0;

    //w razie wyjścia za linię zwracam czarny
    if(x <= 0 || x >= width_1 || y <= 0 || y >= height_1) return kolor;

    unsigned char *wsk;
    if(obraz == 1)      wsk = obraz_1->scanLine(y);
    else if(obraz == 2) wsk = obraz_2->scanLine(y);
    else if(obraz == 3) wsk = img_3->scanLine(y);
    else if(obraz == 4) wsk = pom1->scanLine(y);
    else                wsk = pom2->scanLine(y);

    kolor.b = wsk[4*x];
    kolor.g = wsk[4*x + 1];
    kolor.r = wsk[4*x + 2];

    return kolor;
}


void MainWindow::wyczysc()
{
    unsigned char *wsk;

    for(int i=0; i<height_2; i++)
    {
        wsk = img_1->scanLine(i);

        for(int j=0; j<width_2; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }

        wsk = img_2->scanLine(i);

        for(int j=0; j<width_2; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }

        wsk = img_3->scanLine(i);

        for(int j=0; j<width_2; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }

    }

    *img_1 = obraz_1 -> copy(0, 0, width_1, height_1);
    *img_2 = obraz_2 -> copy(0, 0, width_2, height_2);

    update();
}


//funkcja wyznaczajaca odleglosc miedzy dwoma punktami
double MainWindow::wyznaczOdleglosc(int x1, int y1, int x2, int y2)
{
    return (double) sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


//funkcja wyznaczajaca indeks najblizszego punktu z vectora, do punktu klikniecia
int MainWindow::wyznaczNajblizszy(int a, int b, int obraz)
{
    int in = 0;

    if(obraz == 1)
    {
        double min_odleglosc = wyznaczOdleglosc(a, b, punkty1[0].x, punkty1[0].y);

        for(int i=1; i<punkty1.size(); i++)
        {
            if(wyznaczOdleglosc(a, b, punkty1[i].x, punkty1[i].y) < min_odleglosc)
            {
                in = i;
                min_odleglosc = wyznaczOdleglosc(a, b, punkty1[i].x, punkty1[i].y);
            }
        }
    }

    else
    {
        double min_odleglosc = wyznaczOdleglosc(a, b, punkty2[0].x, punkty2[0].y);

        for(int i=1; i<punkty2.size(); i++)
        {
            if(wyznaczOdleglosc(a, b, punkty2[i].x, punkty2[i].y) < min_odleglosc)
            {
                in = i;
                min_odleglosc = wyznaczOdleglosc(a, b, punkty2[i].x, punkty2[i].y);
            }
        }
    }

    return in;
}


void MainWindow::setA(int x, int y)
{
    this->x1 = x;
    this->y1 = y;
}

void MainWindow::setB(int x, int y)
{
    this->x2 = x;
    this->y2 = y;
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonCzysc_2_clicked()
{
    punkty1.clear();
    punkty2.clear();
    wyczysc();
}


void MainWindow::on_radioButton_clicked()
{
    wstaw = true;
    przesun = false;
}

void MainWindow::on_radioButton_2_clicked()
{
    wstaw = false;
    przesun = true;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(punkty1.size() == 3 && punkty2.size()==3)
    {
        morphing(value);
        update();
    }

}

void MainWindow::on_pushButton_clicked()
{
    if(punkty1.size() == 3 && punkty2.size()==3)
    {
        for(int i = 0; i < N; i++)
        {
            ui->horizontalSlider->setValue(i);
            morphing(i);

            QTime dieTime = QTime::currentTime().addMSecs(5);
            while (QTime::currentTime() < dieTime)
            {
                 QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }

        }
    }
}
