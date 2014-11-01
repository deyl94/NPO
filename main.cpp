#include <cstdio>
#include <QApplication>
#include "geometryform.h"
#include "geometrywidget.h"
#include "CGL/cparse.h"
#include <iostream>
#include <QJsonObject>
#include <QVariantMap>
#include "geometrypair.h"
#include "relationdialog.h"
#include "pseudoInverse.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_ALL,"RUS");

    printf("hello!\n");
    //Geometry* theory = new Geometry("C:\\Users\\NICK\\Downloads\\model.bdf");
    //Geometry* theory2 = new Geometry("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.bdf");
    //Geometry* practic = new Geometry("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.unv");
    GeometryForm* form = new GeometryForm;
    form->readBDF("C:\\Users\\NICK\\Downloads\\model.bdf");
    form->readF06("C:\\Users\\NICK\\Downloads\\model.f06");
    form->colorizeElements(form->modes().at(4).power());//*/
//    form->readBDF("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.bdf");
//    form->readF06("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.f06");


    GeometryWidget w;
    w.setModel(form);
    w.show();

    /*
    QFile f("cnt.mod");
    f.open(QFile::WriteOnly);
    QDataStream s0(&f);
    s0 << *form;
    f.close();
    f.open(QFile::ReadOnly);
    GeometryForm form2;
    QDataStream s(&f);
    s >> form2;
    GeometryWidget w2;
    w2.setModel(form2);
    w2.show();//*/

    //*
    GeometryForm* form2 = new GeometryForm;
    form2->readUNV("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.unv");
    form2->readTXT("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.txt");

//    GeometryPair pair(form, form2);

//    RelationDialog::run(&pair);

//    pair.createTuncationForm();

//    w.setModel(pair.truncation());

//    qDebug() << "truncated";

//    w.show();

    MethodInvMat programma;

    programma.GetMatrix(form);

    programma.PseudoInversion();


    return a.exec();
}