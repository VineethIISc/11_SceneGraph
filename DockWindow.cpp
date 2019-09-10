// Stacked widget class
#include <QtWidgets/qstackedwidget.h>

// Slider class
#include <QtWidgets/qslider.h>

// Group box class
#include <QtWidgets/qgroupbox.h>

#include "DockWindow.h"



DockWindow::DockWindow(QWidget* parent): QDockWidget(parent)
{
	// Area of docking in the main window
	QDockWidget::setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);		

	// Set its title
	QDockWidget::setWindowTitle(tr("Property Manager"));

	// Stacked Widget
	QWidget* firstPageWidget = new QWidget;
	QWidget* secondPageWidget = new QWidget;
	stackedWidget = new QStackedWidget;
	stackedWidget->addWidget(firstPageWidget);
	stackedWidget->addWidget(secondPageWidget);

	// Sliders
	QSlider* m_slider = new QSlider;
	
	// Group Box
	QGroupBox* box = new QGroupBox(tr("Object Configuration"));

	// Set the widget on dock window
	QDockWidget::setWidget(box);
}

