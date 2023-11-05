/********************************************************************************
** Form generated from reading UI file 'QtMusic.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMUSIC_H
#define UI_QTMUSIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtMusicClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditSearch;
    QToolButton *tbnSearch;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidgetSong;
    QWidget *page_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;

    void setupUi(QMainWindow *QtMusicClass)
    {
        if (QtMusicClass->objectName().isEmpty())
            QtMusicClass->setObjectName(QString::fromUtf8("QtMusicClass"));
        QtMusicClass->resize(747, 449);
        centralWidget = new QWidget(QtMusicClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditSearch = new QLineEdit(centralWidget);
        lineEditSearch->setObjectName(QString::fromUtf8("lineEditSearch"));
        lineEditSearch->setMinimumSize(QSize(0, 32));
        lineEditSearch->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(lineEditSearch);

        tbnSearch = new QToolButton(centralWidget);
        tbnSearch->setObjectName(QString::fromUtf8("tbnSearch"));
        tbnSearch->setMinimumSize(QSize(30, 35));
        tbnSearch->setMaximumSize(QSize(50, 35));

        horizontalLayout->addWidget(tbnSearch);


        verticalLayout_4->addLayout(horizontalLayout);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tableWidgetSong = new QTableWidget(page);
        tableWidgetSong->setObjectName(QString::fromUtf8("tableWidgetSong"));

        verticalLayout_2->addWidget(tableWidgetSong);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        verticalLayout_4->addWidget(stackedWidget);

        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        verticalLayout_4->addWidget(widget_2);

        QtMusicClass->setCentralWidget(centralWidget);

        retranslateUi(QtMusicClass);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QtMusicClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtMusicClass)
    {
        QtMusicClass->setWindowTitle(QCoreApplication::translate("QtMusicClass", "\351\237\263\344\271\220\345\234\250\347\272\277", nullptr));
        tbnSearch->setText(QCoreApplication::translate("QtMusicClass", "\346\220\234\347\264\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtMusicClass: public Ui_QtMusicClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMUSIC_H
