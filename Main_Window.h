#pragma once

#include "Service.h"
#include <QtWidgets/QMainWindow>
#include "ui_Main_Window.h"
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qmessagebox>
#include <QListWidget>
#include <QRandomGenerator>
#include <QTableView>
#include <vector>
#include <qtablewidget.h>
#include "Observer.h"


class TableModel : public QAbstractTableModel {
    Q_OBJECT
private:
    vector<Task> tasks;
public:
    TableModel(QWidget* parent) : QAbstractTableModel(parent) {};
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void setTasks(vector<Task> ts);
   // QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

class Adaugare : public QWidget
{
    Q_OBJECT
public:
    Adaugare(Service& service);

private:
    Service& srv;
    QFormLayout* layout;
    QLabel* label_id;
    QLabel* label_descriere;
    QLabel* label_programatori;
    QLabel* label_stare;
    QLineEdit* text_id;
    QLineEdit* text_descriere;
    QLineEdit* text_programatori;
    QLineEdit* text_stare;
    QPushButton* submit_button;

signals:

private slots:
    void submit_button_clicked();
};


class Cautare : public QWidget
{
    Q_OBJECT
public:
    Cautare(Service& service);

private:
    Service& srv;
    QFormLayout* layout;
    QLabel* label_programator;
    QLineEdit* text_programator;
    QPushButton* submit_button;
    QTableView* table;
    TableModel* model;

signals:

private slots:
    void submit_button_clicked();
};


class StareWindow : public QWidget, public Observer
{
    Q_OBJECT
private:
    string stare;
    Service& service;
    QVBoxLayout* layout;
    QTableView* table;
    TableModel* model;
    QPushButton* openButton;
    QPushButton* inProgressButton;
    QPushButton* closedButton;
public:
    StareWindow(Service& _service, string stare);
    void update() override;
    ~StareWindow();
};

class Main_Window : public QWidget, public Observer
{
    Q_OBJECT

public:
    Main_Window(Service& srv);
    QListWidget* afisare(vector<Task> rez);
    void update() override;

private:
    QVBoxLayout* layout;
    Service& srv;
    QTableView* table;
    TableModel* model;
    QPushButton* add_button;
    QPushButton* cauta_button;
    Adaugare* adauga;
    Cautare* cauta;
    StareWindow* openWindow;
    StareWindow* inProgressWindow;
    StareWindow* closedWindow;
    //QPushButton* afisare;

private slots:
    //void afisare_button_clicked();
    void reload();
};

vector<string> splitString(const string& input, char delimiter);


