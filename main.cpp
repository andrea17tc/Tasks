#include "Main_Window.h"
#include "Teste.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{   
   // test_repo();
    Repo r;
    Service srv(r);
    QApplication a(argc, argv);
    Main_Window w(srv);
    w.show();
    return a.exec();
    
}
