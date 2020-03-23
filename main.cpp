#include <iostream>
#include "QtHexGrid.hpp"
#include <QApplication>
#include <QMainWindow>
int main (int argc, char** argv)
{
	QApplication* app = new QApplication(argc, argv);

	QMainWindow* mw = new QMainWindow();
	mw->resize(800,600);

	QtHexGrid* hexGrid = new QtHexGrid(20,20);
	mw->setCentralWidget(hexGrid);
	mw->show();

	std::vector<float> data;
	for (int i = 0; i < 400; i++)
	{
		data.push_back(i*0.0025);
	}

	hexGrid->setData(data);
	return app->exec();
}
