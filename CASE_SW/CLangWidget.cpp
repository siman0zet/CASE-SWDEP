#include "CLangWidget.h"

CLangWidget::CLangWidget(QStringList list, QString selected_lang): QWidget(NULL), cb(this), l(this)
{
    this->setGeometry(300, 200, 310, 50);

    l.setGeometry(10, 10, 140, 30);
    cb.setGeometry(160, 10, 140, 30);

    l.setText(tr("Select language:"));

    cb.addItems(list);
    if (selected_lang.size())
        cb.setCurrentText(selected_lang);
    else
    {
        cb.addItem(tr("-----"));
        cb.setCurrentText(tr("-----"));
    }

    connect(&cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(lang_selected(QString)));

    this->setMaximumSize(310, 50);
    this->setMinimumSize(310, 50);
}

CLangWidget::~CLangWidget()
{

}

void CLangWidget::reset_text()
{
    l.setText(tr("Select language:"));
}

