#include "aboutdialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDialogButtonBox>
#include <QApplication>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("About Student Manager");
    setFixedSize(420, 360);
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(36, 32, 36, 24);
    layout->setSpacing(0);

    // Big emoji icon
    QLabel *icon = new QLabel("🎓");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 56px; margin-bottom: 8px;");
    layout->addWidget(icon);

    QLabel *appName = new QLabel("Student Manager Pro");
    appName->setAlignment(Qt::AlignCenter);
    appName->setStyleSheet("font-size: 20px; font-weight: bold; color: #d0d0ff; margin-bottom: 4px;");
    layout->addWidget(appName);

    QLabel *version = new QLabel("Version 1.0");
    version->setAlignment(Qt::AlignCenter);
    version->setStyleSheet("font-size: 12px; color: #5555aa; margin-bottom: 20px;");
    layout->addWidget(version);

    QFrame *div = new QFrame();
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("color: #2a2a45; margin: 8px 0;");
    layout->addWidget(div);

    layout->addSpacing(12);

    QLabel *desc = new QLabel(
        "A student records management application\nbuilt with Qt C++ Framework.\n\n"
        "Features: Add, Edit, Delete, Search, Filter,\n"
        "Sort, Import/Export JSON."
    );
    desc->setAlignment(Qt::AlignCenter);
    desc->setStyleSheet("font-size: 12px; color: #7070a0; line-height: 160%;");
    layout->addWidget(desc);

    layout->addSpacing(16);

    QLabel *author = new QLabel("Author: <b style='color:#8090e0'>Nikita Chervinskyi (CHE0086)</b>");
    author->setAlignment(Qt::AlignCenter);
    author->setStyleSheet("font-size: 13px; color: #6070b0;");
    layout->addWidget(author);

    QLabel *built = new QLabel(QString("Built with Qt %1").arg(QT_VERSION_STR));
    built->setAlignment(Qt::AlignCenter);
    built->setStyleSheet("font-size: 11px; color: #3a3a60; margin-top: 4px;");
    layout->addWidget(built);

    layout->addStretch();

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttons->button(QDialogButtonBox::Ok)->setText("  Close  ");
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttons);
}
