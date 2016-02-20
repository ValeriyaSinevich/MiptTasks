#ifndef UI_GRAPHVIZ_H
#define UI_GRAPHVIZ_H

#include <Drawer.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

using namespace visualization;

class Ui_GraphVizClass
{
public:
    Drawer *centralWidget;

    void setupUi(QMainWindow *GraphVizClass)
    {
        if (GraphVizClass->objectName().isEmpty())
            GraphVizClass->setObjectName(QStringLiteral("GraphVizClass"));
        GraphVizClass->resize(764, 568);
        centralWidget = new Drawer(GraphVizClass);
		QRect window = centralWidget->rect();
		int w = window.width();
		int h = window.height();
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
		centralWidget->Prev = new QPushButton(centralWidget);
		centralWidget->Prev->setObjectName(QStringLiteral("Prev"));
		centralWidget->Prev->setGeometry(QRect(630, 460, 93, 28));
		centralWidget->Prev->setEnabled(false);
		centralWidget->Prev->hide();
		centralWidget->Next = new QPushButton(centralWidget);
		centralWidget->Next->setObjectName(QStringLiteral("Next"));
		centralWidget->Next->setGeometry(QRect(630, 500, 93, 28));
		centralWidget->Next->setMouseTracking(true);
		centralWidget->Next->setCheckable(true);
		centralWidget->Start = new QPushButton(centralWidget);
		centralWidget->Start->setObjectName(QStringLiteral("Start"));
		centralWidget->Start->setGeometry(QRect(32, 500, 571, 28));
		centralWidget->Text = new QLabel(centralWidget);
		centralWidget->Text->setObjectName(QStringLiteral("Algorithm Finished!"));
		centralWidget->Text->setEnabled(false);
		centralWidget->Text->setGeometry(QRect(470, 40, 271, 81));
        GraphVizClass->setCentralWidget(centralWidget);

		QObject::connect(centralWidget->Start, SIGNAL(clicked(void)), centralWidget, SLOT(StartClicked(void)));
		QObject::connect(centralWidget->Next, SIGNAL(clicked(void)), centralWidget, SLOT(NextClicked(void)));
		QObject::connect(centralWidget->Net, SIGNAL(VertexRelabeled(int, int)), centralWidget, SLOT(afterRelabel(int, int)));
		QObject::connect(centralWidget->Net, SIGNAL(EdgePushed(int, int, int)), centralWidget, SLOT(afterPush(int, int, int)));
		QObject::connect(centralWidget->Net, SIGNAL(finished(void)), centralWidget, SLOT(finished(void)));

        retranslateUi(GraphVizClass);

        QMetaObject::connectSlotsByName(GraphVizClass);
    } // setupUi

    void retranslateUi(QMainWindow *GraphVizClass)
    {
        GraphVizClass->setWindowTitle(QApplication::translate("GraphVizClass", "GraphViz", 0));
		centralWidget->Prev->setText(QApplication::translate("GraphVizClass", "Prev", 0));
		centralWidget->Next->setText(QApplication::translate("GraphVizClass", "Next", 0));
		centralWidget->Start->setText(QApplication::translate("GraphVizClass", "Start", 0));
		centralWidget->Text->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GraphVizClass: public Ui_GraphVizClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHVIZ_H
