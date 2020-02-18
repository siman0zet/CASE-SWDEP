#include "main/mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVector<QTranslator*> translates;
    QStringList langs;

    QDir dir(LANG_PATH);

    QTranslator* translator = new QTranslator();
    QStringList filesList = dir.entryList();
    for(int i = 2;i < filesList.size(); i++)
    {
        if (translator->load(LANG_PATH + filesList.at(i)))
        {
            translates.append(translator);
            if (filesList[i].size() > 3)
            {
                if (filesList[i].at(filesList[i].size() - 3).toLatin1() == '.' &&
                        filesList[i].at(filesList[i].size() - 2).toLatin1() == 'q' &&
                        filesList[i].at(filesList[i].size() - 1).toLatin1() == 'm')
                {
                    filesList[i].remove(filesList[i].size() - 3, 3);
                }
            }

            langs.append(filesList.at(i));

            translator = new QTranslator();
        }
    }

    MainWindow w;
    //w.setLangs(translates, langs);
    w.show();

    //w.setLang("English");

    /*CViewModel view;
    view.setGeometry(20,20,800,600);
    view.show();*/

    return a.exec();
}
