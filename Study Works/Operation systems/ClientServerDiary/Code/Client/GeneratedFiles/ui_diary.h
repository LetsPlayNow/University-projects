/********************************************************************************
** Form generated from reading UI file 'diary.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIARY_H
#define UI_DIARY_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Diary
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QListWidget *lstList;
    QGridLayout *gridLayout_3;
    QPushButton *btnCreate;
    QPushButton *btnDelete;
    QPushButton *btnSave;
    QPushButton *btnFillList;
    QGridLayout *gridLayout_4;
    QLCDNumber *lcdNoteId;
    QLabel *lblNumberOfNote;
    QGridLayout *grdEditFields;
    QListWidget *lstImportant;
    QListWidget *lstPeriodicity;
    QLineEdit *txtPlace;
    QLabel *lblPlace;
    QLabel *lblTimeLength;
    QLabel *lblHasPeriodicity;
    QLabel *lblImportant;
    QCheckBox *cbHasPeriodicity;
    QLabel *lblDateAndTime;
    QLineEdit *txtName;
    QTimeEdit *teTimeLength;
    QLabel *lblName;
    QDateTimeEdit *dteDateAndTime;
    QLabel *lblPeriodicity;
    QLabel *lblNumberOfRepeat;
    QSpinBox *spbNumberOfRepeat;
    QFrame *line_12;
    QFrame *line_13;
    QFrame *line_2;
    QFrame *line_7;
    QFrame *line_6;
    QFrame *line_3;
    QFrame *line_14;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QFrame *line_16;
    QGridLayout *QGL_IP;
    QFrame *line_15;
    QLineEdit *leIP;
    QFrame *line_4;
    QLabel *label;
    QPushButton *btn_connect;

    void setupUi(QMainWindow *Diary)
    {
        if (Diary->objectName().isEmpty())
            Diary->setObjectName(QStringLiteral("Diary"));
        Diary->resize(616, 377);
        Diary->setMinimumSize(QSize(510, 377));
        centralWidget = new QWidget(Diary);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lstList = new QListWidget(centralWidget);
        lstList->setObjectName(QStringLiteral("lstList"));

        gridLayout_2->addWidget(lstList, 3, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        btnCreate = new QPushButton(centralWidget);
        btnCreate->setObjectName(QStringLiteral("btnCreate"));

        gridLayout_3->addWidget(btnCreate, 1, 0, 1, 1);

        btnDelete = new QPushButton(centralWidget);
        btnDelete->setObjectName(QStringLiteral("btnDelete"));

        gridLayout_3->addWidget(btnDelete, 1, 2, 1, 1);

        btnSave = new QPushButton(centralWidget);
        btnSave->setObjectName(QStringLiteral("btnSave"));

        gridLayout_3->addWidget(btnSave, 1, 1, 1, 1);

        btnFillList = new QPushButton(centralWidget);
        btnFillList->setObjectName(QStringLiteral("btnFillList"));

        gridLayout_3->addWidget(btnFillList, 2, 0, 1, 3);


        gridLayout_2->addLayout(gridLayout_3, 1, 0, 2, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        lcdNoteId = new QLCDNumber(centralWidget);
        lcdNoteId->setObjectName(QStringLiteral("lcdNoteId"));
        lcdNoteId->setMinimumSize(QSize(50, 40));
        lcdNoteId->setMaximumSize(QSize(30, 60));
        lcdNoteId->setAutoFillBackground(false);
        lcdNoteId->setFrameShadow(QFrame::Raised);
        lcdNoteId->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_4->addWidget(lcdNoteId, 0, 1, 1, 1);

        lblNumberOfNote = new QLabel(centralWidget);
        lblNumberOfNote->setObjectName(QStringLiteral("lblNumberOfNote"));

        gridLayout_4->addWidget(lblNumberOfNote, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_4, 5, 0, 2, 1);

        grdEditFields = new QGridLayout();
        grdEditFields->setSpacing(6);
        grdEditFields->setObjectName(QStringLiteral("grdEditFields"));
        grdEditFields->setSizeConstraint(QLayout::SetMinimumSize);
        lstImportant = new QListWidget(centralWidget);
        lstImportant->setObjectName(QStringLiteral("lstImportant"));
        lstImportant->setMaximumSize(QSize(388, 56));

        grdEditFields->addWidget(lstImportant, 10, 2, 1, 1);

        lstPeriodicity = new QListWidget(centralWidget);
        lstPeriodicity->setObjectName(QStringLiteral("lstPeriodicity"));
        lstPeriodicity->setEnabled(false);
        lstPeriodicity->setAutoFillBackground(false);

        grdEditFields->addWidget(lstPeriodicity, 14, 2, 1, 1);

        txtPlace = new QLineEdit(centralWidget);
        txtPlace->setObjectName(QStringLiteral("txtPlace"));
        txtPlace->setMaxLength(100);
        txtPlace->setDragEnabled(false);

        grdEditFields->addWidget(txtPlace, 8, 2, 1, 1);

        lblPlace = new QLabel(centralWidget);
        lblPlace->setObjectName(QStringLiteral("lblPlace"));
        lblPlace->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblPlace, 8, 1, 1, 1);

        lblTimeLength = new QLabel(centralWidget);
        lblTimeLength->setObjectName(QStringLiteral("lblTimeLength"));
        lblTimeLength->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblTimeLength, 6, 1, 1, 1);

        lblHasPeriodicity = new QLabel(centralWidget);
        lblHasPeriodicity->setObjectName(QStringLiteral("lblHasPeriodicity"));

        grdEditFields->addWidget(lblHasPeriodicity, 12, 1, 1, 1);

        lblImportant = new QLabel(centralWidget);
        lblImportant->setObjectName(QStringLiteral("lblImportant"));
        lblImportant->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblImportant, 10, 1, 1, 1);

        cbHasPeriodicity = new QCheckBox(centralWidget);
        cbHasPeriodicity->setObjectName(QStringLiteral("cbHasPeriodicity"));

        grdEditFields->addWidget(cbHasPeriodicity, 12, 2, 1, 1);

        lblDateAndTime = new QLabel(centralWidget);
        lblDateAndTime->setObjectName(QStringLiteral("lblDateAndTime"));
        lblDateAndTime->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblDateAndTime, 3, 1, 1, 1);

        txtName = new QLineEdit(centralWidget);
        txtName->setObjectName(QStringLiteral("txtName"));
        txtName->setMaxLength(100);

        grdEditFields->addWidget(txtName, 0, 2, 1, 1);

        teTimeLength = new QTimeEdit(centralWidget);
        teTimeLength->setObjectName(QStringLiteral("teTimeLength"));

        grdEditFields->addWidget(teTimeLength, 6, 2, 1, 1);

        lblName = new QLabel(centralWidget);
        lblName->setObjectName(QStringLiteral("lblName"));
        lblName->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblName, 0, 1, 1, 1);

        dteDateAndTime = new QDateTimeEdit(centralWidget);
        dteDateAndTime->setObjectName(QStringLiteral("dteDateAndTime"));
        dteDateAndTime->setMinimumDate(QDate(1752, 9, 14));

        grdEditFields->addWidget(dteDateAndTime, 3, 2, 1, 1);

        lblPeriodicity = new QLabel(centralWidget);
        lblPeriodicity->setObjectName(QStringLiteral("lblPeriodicity"));
        lblPeriodicity->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblPeriodicity, 14, 1, 1, 1);

        lblNumberOfRepeat = new QLabel(centralWidget);
        lblNumberOfRepeat->setObjectName(QStringLiteral("lblNumberOfRepeat"));
        lblNumberOfRepeat->setAlignment(Qt::AlignCenter);

        grdEditFields->addWidget(lblNumberOfRepeat, 16, 1, 1, 1);

        spbNumberOfRepeat = new QSpinBox(centralWidget);
        spbNumberOfRepeat->setObjectName(QStringLiteral("spbNumberOfRepeat"));
        spbNumberOfRepeat->setMinimum(2);
        spbNumberOfRepeat->setMaximum(366);

        grdEditFields->addWidget(spbNumberOfRepeat, 16, 2, 1, 1);

        line_12 = new QFrame(centralWidget);
        line_12->setObjectName(QStringLiteral("line_12"));
        line_12->setLineWidth(12);
        line_12->setFrameShape(QFrame::HLine);
        line_12->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_12, 11, 1, 1, 2);

        line_13 = new QFrame(centralWidget);
        line_13->setObjectName(QStringLiteral("line_13"));
        line_13->setLineWidth(12);
        line_13->setFrameShape(QFrame::HLine);
        line_13->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_13, 13, 1, 1, 2);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setLineWidth(12);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_2, 1, 1, 1, 2);

        line_7 = new QFrame(centralWidget);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setLineWidth(12);
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_7, 9, 1, 1, 2);

        line_6 = new QFrame(centralWidget);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setLineWidth(12);
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_6, 7, 1, 1, 2);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setLineWidth(12);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        grdEditFields->addWidget(line_3, 4, 1, 1, 2);

        line_14 = new QFrame(centralWidget);
        line_14->setObjectName(QStringLiteral("line_14"));
        line_14->setFrameShadow(QFrame::Sunken);
        line_14->setLineWidth(12);
        line_14->setFrameShape(QFrame::HLine);

        grdEditFields->addWidget(line_14, 15, 1, 1, 2);


        gridLayout_2->addLayout(grdEditFields, 1, 5, 6, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 2, 6, 1);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(20);
        line->setFrameShape(QFrame::VLine);

        gridLayout_2->addWidget(line, 0, 1, 7, 1);

        line_16 = new QFrame(centralWidget);
        line_16->setObjectName(QStringLiteral("line_16"));
        line_16->setFrameShape(QFrame::HLine);
        line_16->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_16, 4, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 4, 0, 1, 1);

        QGL_IP = new QGridLayout();
        QGL_IP->setSpacing(6);
        QGL_IP->setObjectName(QStringLiteral("QGL_IP"));
        line_15 = new QFrame(centralWidget);
        line_15->setObjectName(QStringLiteral("line_15"));
        line_15->setFrameShape(QFrame::HLine);
        line_15->setFrameShadow(QFrame::Sunken);

        QGL_IP->addWidget(line_15, 5, 0, 1, 3);

        leIP = new QLineEdit(centralWidget);
        leIP->setObjectName(QStringLiteral("leIP"));
        leIP->setMaximumSize(QSize(340, 16777215));
        leIP->setLayoutDirection(Qt::LeftToRight);
        leIP->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        QGL_IP->addWidget(leIP, 0, 2, 3, 1);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        QGL_IP->addWidget(line_4, 0, 1, 3, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        QGL_IP->addWidget(label, 0, 0, 3, 1);

        btn_connect = new QPushButton(centralWidget);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));

        QGL_IP->addWidget(btn_connect, 4, 0, 1, 3);


        gridLayout->addLayout(QGL_IP, 0, 0, 1, 1);

        Diary->setCentralWidget(centralWidget);
        QWidget::setTabOrder(btnCreate, btnSave);
        QWidget::setTabOrder(btnSave, btnDelete);
        QWidget::setTabOrder(btnDelete, btnFillList);
        QWidget::setTabOrder(btnFillList, lstList);
        QWidget::setTabOrder(lstList, txtName);
        QWidget::setTabOrder(txtName, dteDateAndTime);
        QWidget::setTabOrder(dteDateAndTime, teTimeLength);
        QWidget::setTabOrder(teTimeLength, txtPlace);
        QWidget::setTabOrder(txtPlace, lstImportant);
        QWidget::setTabOrder(lstImportant, cbHasPeriodicity);
        QWidget::setTabOrder(cbHasPeriodicity, lstPeriodicity);
        QWidget::setTabOrder(lstPeriodicity, spbNumberOfRepeat);

        retranslateUi(Diary);

        QMetaObject::connectSlotsByName(Diary);
    } // setupUi

    void retranslateUi(QMainWindow *Diary)
    {
        Diary->setWindowTitle(QApplication::translate("Diary", "\320\225\320\266\320\265\320\264\320\275\320\265\320\262\320\275\320\270\320\272", 0));
        btnCreate->setText(QApplication::translate("Diary", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", 0));
        btnDelete->setText(QApplication::translate("Diary", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0));
        btnSave->setText(QApplication::translate("Diary", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        btnFillList->setText(QApplication::translate("Diary", "\320\227\320\260\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272", 0));
        lblNumberOfNote->setText(QApplication::translate("Diary", "\320\235\320\276\320\274\320\265\321\200 \321\202\320\265\320\272\321\203\321\211\320\265\320\271 \320\267\320\260\320\277\320\270\321\201\320\270", 0));
        lblPlace->setText(QApplication::translate("Diary", "\320\234\320\265\321\201\321\202\320\276 \320\277\321\200\320\276\320\262\320\265\320\264\320\265\320\275\320\270\321\217", 0));
        lblTimeLength->setText(QApplication::translate("Diary", "\320\237\321\200\320\276\321\202\321\217\320\266\320\265\320\275\320\275\320\276\321\201\321\202\321\214", 0));
        lblHasPeriodicity->setText(QApplication::translate("Diary", "\320\230\320\274\320\265\320\265\321\202\321\201\321\217 \320\277\320\265\321\200\320\270\320\276\320\264\320\270\321\207\320\275\320\276\321\201\321\202\321\214", 0));
        lblImportant->setText(QApplication::translate("Diary", "\320\222\320\260\320\266\320\275\320\276\321\201\321\202\321\214", 0));
        cbHasPeriodicity->setText(QString());
        lblDateAndTime->setText(QApplication::translate("Diary", "\320\224\320\260\321\202\320\260 \320\270 \320\262\321\200\320\265\320\274\321\217\n"
" \320\277\321\200\320\276\320\262\320\265\320\264\320\265\320\275\320\270\321\217", 0));
        txtName->setText(QString());
        lblName->setText(QApplication::translate("Diary", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265\n"
" \320\274\320\265\321\200\320\276\320\277\321\200\320\270\321\217\321\202\320\270\321\217", 0));
        lblPeriodicity->setText(QApplication::translate("Diary", "\320\237\320\265\321\200\320\270\320\276\320\264 \320\277\320\276\320\262\321\202\320\276\321\200\320\265\320\275\320\270\321\217", 0));
        lblNumberOfRepeat->setText(QApplication::translate("Diary", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276\n"
"\320\277\320\276\320\262\321\202\320\276\321\200\320\265\320\275\320\270\320\271", 0));
        label->setText(QApplication::translate("Diary", "IP", 0));
        btn_connect->setText(QApplication::translate("Diary", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", 0));
    } // retranslateUi

};

namespace Ui {
    class Diary: public Ui_Diary {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIARY_H
