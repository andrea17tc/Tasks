#include "Main_Window.h"
#include "Observer.h"
#include <sstream>

Main_Window::Main_Window(Service& srv) : srv{ srv }
{
    srv.add_observer(this);
    layout = new QVBoxLayout(this);
    //Adauga
    add_button = new QPushButton("Adauga");
    adauga = new Adaugare(srv);
    layout->addWidget(add_button);
    adauga->setWindowTitle("Adaugare");
    this->connect(add_button, SIGNAL(clicked()), adauga, SLOT(show()));
    //table = new QTableView;
    //

    //Cauta
    cauta_button = new QPushButton("Cauta");
    cauta = new Cautare(srv);
    layout->addWidget(cauta_button);
    cauta->setWindowTitle("Cautare");
    this->connect(cauta_button, SIGNAL(clicked()), cauta, SLOT(show()));
    //

    table = new QTableView;
    model = new TableModel(table);
    model->setTasks(srv.get_all());
    table->setModel(model);

    layout->addWidget(table);

    openWindow = new StareWindow(srv, "Open");
    inProgressWindow = new StareWindow(srv, "Inprogress");
    closedWindow = new StareWindow(srv, "Close");
    openWindow->show();
    inProgressWindow->show();
    closedWindow->show();

}

void Main_Window::update(){
    reload();
}

void Main_Window::reload() {
    model->setTasks(srv.get_all());
    table->setModel(model);
}

QListWidget* Main_Window::afisare(vector<Task> rez) {
    QListWidget* lista;
    lista = new QListWidget();
    QString item, item1;
    item1.append("ID       Descriere      Programatori      Stare");
    lista->addItem(item1);
    lista->setWindowTitle("Produse\n");
    for (auto task : rez) {

        item.clear();
        item.append(QString::number(task.get_id()));
        item.append("            ");
        item.append(QString::fromStdString(task.get_descriere()));
        item.append("            ");
        for (auto p : task.get_programatori()) {
            item.append(QString::fromStdString(p));
            item.append(", ");
        }
        item.append("            ");
        item.append(QString::fromStdString(task.get_stare()));
        //item.append("\n");
        lista->addItem(item);
    }
    return lista;
}

Adaugare::Adaugare(Service& service) : srv{ service }
{
    layout = new QFormLayout(this);
    //service.add_observer(this);
    label_id = new QLabel("ID");
    label_descriere = new QLabel("Descriere");
    label_programatori = new QLabel("Programatori");
    label_stare = new QLabel("Stare");
    text_id = new QLineEdit;
    text_descriere = new QLineEdit;
    text_programatori = new QLineEdit;
    text_stare = new QLineEdit;
    submit_button = new QPushButton("Submit");
    layout->addRow(label_id, text_id);
    layout->addRow(label_descriere, text_descriere);
    layout->addRow(label_programatori, text_programatori);
    layout->addRow(label_stare, text_stare);
    layout->addRow(submit_button);
    this->connect(submit_button, SIGNAL(clicked()), this, SLOT(submit_button_clicked()));
    this->connect(submit_button, SIGNAL(clicked()), this, SLOT(close()));
}

vector<string> splitString(const string& input, char delimiter) {
    vector<std::string> result;
    stringstream ss(input);
    string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

void Adaugare::submit_button_clicked()
{
    std::string descriere = text_descriere->text().toStdString();
    //QString proba = QString::fromStdString(nume);
    std::string stare = text_stare->text().toStdString();
    bool convert = true;
    int id = text_id->text().toInt(&convert);
    if (convert == false)
    {
        QMessageBox* mesaj = new QMessageBox;
        mesaj->setText("Nu");
        mesaj->show();
        return;
    }
    std::string prog = text_programatori->text().toStdString();
    vector<string> progs{ splitString(prog,',') };
    Task t(id, descriere, progs, stare);
    try {
        srv.adauga(t);
    }
    catch (exception) {
        QMessageBox* mesaj = new QMessageBox;
        mesaj->setText("NU");
        mesaj->show();
        return;
    }
    
}


Cautare::Cautare(Service& service) : srv{ service }
{
    layout = new QFormLayout(this);
    //service.add_observer(this);
    label_programator = new QLabel("Nume programator");
    text_programator = new QLineEdit;
    submit_button = new QPushButton("Submit");
    layout->addRow(label_programator, text_programator);
    layout->addRow(submit_button);
    this->connect(submit_button, SIGNAL(clicked()), this, SLOT(submit_button_clicked()));
    this->connect(submit_button, SIGNAL(clicked()), this, SLOT(close()));
}

void Cautare::submit_button_clicked() {
    std::string nume = text_programator->text().toStdString();
    vector<Task> rez = srv.cauta_prog(nume);
    if (rez.size() == 0) {
        QMessageBox* mesaj = new QMessageBox;
        mesaj->setText("Nu exista task-uri cu asa programator");
        mesaj->show();
        return;
    }
    else{
        table = new QTableView;
        model = new TableModel(table);
        model->setTasks(rez);
        table->setModel(model);
        table->show();
    }
        
}

int TableModel::rowCount(const QModelIndex& parent) const {
    return tasks.size();
}

int TableModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant TableModel::data(const QModelIndex& index, int role) const  {
    int row = index.row();
    Task t = tasks.at(row);
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QString::number(t.get_id());
            break;
        case 1:
            return QString::fromStdString(t.get_descriere());
            break;
        case 2: {
            QString rez="";
            for (auto p : t.get_programatori())
            {
                rez.append(QString::fromStdString(p));
                rez.append(" ");
            }
            return rez;
            break;
        }
        case 3:
            return QString::fromStdString(t.get_stare());
            break;
        }
    }
    return QVariant();
}

void TableModel::setTasks(vector<Task> ts)
{
    tasks = ts;
    std::sort(tasks.begin(), tasks.end(), [](Task& a, Task& b) {
        return a.get_stare() < b.get_stare();
        });
    emit layoutChanged();
}

/*
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("ID");
        case 1:
            return QString("Descriere");
        case 2:
            return QString("Pogramatori");
        case 3:
            return QString("Stare");
        }
    }
    return QVariant();
}
*/

StareWindow::StareWindow(Service& _service, string stare) : service(_service), stare(stare)
{
    setWindowTitle(QString::fromStdString(stare));
    service.add_observer(this);
    layout = new QVBoxLayout(this);
    table = new QTableView();
    model = new TableModel(table);
    model->setTasks(service.filtrare_stare(stare));
    table->setModel(model);
    layout->addWidget(table);
    openButton = new QPushButton("open");
    inProgressButton = new QPushButton("in progress");
    closedButton = new QPushButton("closed");
    layout->addWidget(openButton);
    layout->addWidget(inProgressButton);
    layout->addWidget(closedButton);
    this->connect(openButton, &QPushButton::clicked, [this]() {
        if (!table->selectionModel()->selectedIndexes().isEmpty()) {
            int row = table->selectionModel()->selectedIndexes().at(0).row();
            auto index = table->model()->index(row, 0);
            int id = table->model()->data(index).toInt();
            service.modifyState(id, "Open");
        }
        });
    this->connect(inProgressButton, &QPushButton::clicked, [this]() {
        if (!table->selectionModel()->selectedIndexes().isEmpty()) {
            int row = table->selectionModel()->selectedIndexes().at(0).row();
            auto index = table->model()->index(row, 0);
            int id = table->model()->data(index).toInt();
            service.modifyState(id, "Inprogress");
        }
        });
    this->connect(closedButton, &QPushButton::clicked, [this]() {
        if (!table->selectionModel()->selectedIndexes().isEmpty()) {
            int row = table->selectionModel()->selectedIndexes().at(0).row();
            auto index = table->model()->index(row, 0);
            int id = table->model()->data(index).toInt();
            service.modifyState(id, "Close");
        }
        });
}

void StareWindow::update()
{
    model->setTasks(service.filtrare_stare(stare));
}

StareWindow::~StareWindow()
{
    service.remove_observer(this);
}