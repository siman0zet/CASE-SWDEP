#include "CLoadAndSaveWidget.h"
#include "CViewModel.h"

CLoadAndSaveWidget::CLoadAndSaveWidget(CViewModel *_parent, bool isSave) : parent(_parent), is_save(isSave)
{
    setGeometry(30, 30, 500, 360);
        setMaximumSize(500, 360);
        setMinimumSize(500, 360);

        l_filedir = new QLabel(this);
        l_filename = new QLabel(this);
        l_message = new QLabel(this);
        new_filename = new QLineEdit(this);
        new_filedir = new QLineEdit(this);
        OK_button = new QPushButton(this);

        l_filedir->setGeometry(20, 260, 70, 30);
        l_filename->setGeometry(20, 320, 70, 30);
        l_message->setGeometry(90, 290, 390, 30);
        new_filedir->setGeometry(90, 260, 390, 30);
        new_filename->setGeometry(90, 320, 250, 30);
        OK_button->setGeometry(380, 320, 100, 30);

        l_message->setStyleSheet("QLabel{color : red;}");
        l_filedir->setText(tr("directory:"));
        l_filename->setText(tr("filename:"));
        OK_button->setText(tr("OK"));

        fs_model = new QFileSystemModel;
        fs_view = new QTreeView(this);

        QStringList filter;
        filter << "*.cdmod";
        fs_model->setNameFilters(filter);
        fs_model->setNameFilterDisables(false);
        fs_model->setRootPath(QDir::currentPath());

        fs_view->setGeometry(10, 10, 480, 240);
        fs_view->setModel(fs_model);
        fs_view->hideColumn(2);
        fs_view->hideColumn(3);
        fs_view->setColumnWidth(0, 340);

        connect(fs_view, SIGNAL(clicked(QModelIndex)), this, SLOT(on_fs_view_clicked(QModelIndex)));
        connect(OK_button, SIGNAL(clicked()), this, SLOT(on_OK_button_clicked()));

        if(is_save)
            this->setWindowTitle(tr("Save in file as..."));
        else
            this->setWindowTitle(tr("Load from file..."));

        QString str;
        QFile file("fspath.fspath");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            in >> str;
            file.close();
        }
        if (str.section('/', 0, 0) == "")
        {
            fs_view->expand(fs_model->index("/"));
            for (int i = 1; i < str.count('/'); ++i)
            {
                fs_view->expand(fs_model->index(str.section('/', 0, i)));
                //qDebug() << str.section('/', 0, i);
            }
        }
        else
        {
            for (int i = 0; i < str.count('/'); ++i)
            {
                fs_view->expand(fs_model->index(str.section('/', 0, i)));
                //qDebug() << str.section('/', 0, i);
            }
        }
        new_filedir->setText(str.section('/', 0, str.count('/') - 1));
}

CLoadAndSaveWidget::~CLoadAndSaveWidget()
{
    delete fs_view;
    delete fs_model;
}

void CLoadAndSaveWidget::saveFSviewConf(QString fsdir)
{
    QDir dirr(fsdir);
    //qDebug() << dirr.absolutePath();

    QFile file("fspath.fspath");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << dirr.absolutePath();
    file.close();
}

void CLoadAndSaveWidget::closeEvent(QCloseEvent *event)
{
    emit closing();
}

void CLoadAndSaveWidget::on_fs_view_clicked(const QModelIndex &index)
{
    QString fPath = fs_model->fileInfo(index).absolutePath();
    QString fName;
    if (!fs_model->fileInfo(index).isDir())
        fName = fs_model->fileInfo(index).baseName();
    else
    {
        fPath = fs_model->fileInfo(index).absoluteFilePath();
        fName = "";
    }
    new_filename->setText(fName);
    new_filedir->setText(fPath);
    update();
}

void CLoadAndSaveWidget::on_OK_button_clicked()
{
    QString _fname;

    if (new_filedir->text() != "")
    {
        if (new_filename->text() != "")
        {
            if (new_filedir->text().endsWith('/'))
                _fname = new_filedir->text() + new_filename->text();
            else
                _fname = new_filedir->text() + "/" + new_filename->text();
        }
        else
            _fname = "";
    }
    else
        _fname = new_filename->text();

    if (_fname != "")
    {
        if (is_save)
        {
            parent->pre_saveInFile(_fname);
            saveFSviewConf(_fname);
            emit closing();
        }
        else
        {
            if (parent->pre_loadFromFile(_fname))
            {
                saveFSviewConf(_fname);
                emit closing();
            }
            else
                l_message->setText("File \'" + new_filename->text() + "\' is not found");
        }
    }
    else
        l_message->setText("Enter a filename");
}

