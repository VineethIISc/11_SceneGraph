//! Dock window class
/*!
// Refernce: https://doc.qt.io/qt-5/qtwidgets-widgets-sliders-example.html
*/


#ifndef DOCK_WINDOW_H
#define DOCK_WINDOW_H

#include <QtWidgets/qdockwidget.h>

//! Compile Stacked widget class
class QStackedWidget;

class DockWindow : public QDockWidget
{
public:
	//! Constructor
	DockWindow(QWidget * parent);

	
private:
	//! Qstacked widget
	QStackedWidget* stackedWidget;

	

};
#endif // !DOCK_WINDOW_H



