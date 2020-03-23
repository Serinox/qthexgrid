//
// Created by Serinox on 3/22/20.
//

#include "QtHexGrid.hpp"
#include <QSizePolicy>
#include <QResizeEvent>
#include <QPolygonF>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <cmath>
#include <iostream>

QGradient::~QGradient(){}
QLinearGradient::~QLinearGradient(){}

QtHexGrid::QtHexGrid(int rows, int cols, QColor lowColor,QColor midColor, QColor highColor, QWidget *parent) : QWidget(parent), highColorM(highColor), midColorM(midColor), lowColorM(lowColor), rowsM(rows), colsM(cols), gradImgM(QSize(1,100), QImage::Format_RGB32 )
{
	graphicsViewM = new QGraphicsView(this);
	//graphicsViewM->resize(800,600);
	sceneM = new QGraphicsScene(this);

	gradientM = QLinearGradient( 0.0, 0.0, 0.0, 1.0 );
	gradientM.setCoordinateMode(QGradient::ObjectBoundingMode);
	gradientM.setColorAt(0.0, lowColor);
	gradientM.setColorAt(0.5, midColor);
	gradientM.setColorAt(1, highColor);

	graphicsViewM->setScene(sceneM);

	//we need to paint this into a lookup image because  you can't actually get the damn
	// color from a qgradient directly and that's terrible.
	QPainter p(&gradImgM);
	p.fillRect(gradImgM.rect(), gradientM);

	dataM = std::vector<CellData>(rows*cols);

	float width = sqrt(3)*hexSizeM;
	float height = 2*hexSizeM;
	int index = 0;

	for (int col = 0; col < cols; col++)
	{
		for(int row = 0; row<rows; row++)
		{
			dataM[index].index = index;
			dataM[index].color = lowColor;
			dataM[index].poly = getHexCenteredAt(col*width + (row%2==1 ? .5*width : 0.0), row*(.75*height),hexSizeM);
			dataM[index].polyItem = sceneM->addPolygon(dataM[index].poly);
			dataM[index].polyItem->setBrush(QBrush(dataM[index].color));
			dataM[index].textItem = sceneM->addText(QString::fromStdString(std::to_string(index)));
			QRectF r = dataM[index].textItem->boundingRect();
			dataM[index].textItem->setPos(col*width + (row%2==1 ? .5*width : 0.0) - r.width()/2.0, row*(.75*height)- r.height()/2.0);
			index++;
		}
	}

	QGraphicsRectItem* rectItem = sceneM->addRect((cols+1)*width,0,10,(rows-1)*(.75*height),QPen(),gradientM);

	//sceneM->addPolygon(getHexCenteredAt(0,0,size));
	//sceneM->addPolygon(getHexCenteredAt(.5*width,.75*height,size));
	//sceneM->addPolygon(getHexCenteredAt(0,2*(.75*height),size));


}

QPointF hex_corner(QPointF center, float size, int i, bool pointy = true)
{
	float angle_deg = 60 * i - (pointy ? 30 : 0);
	float angle_rad = 3.14159 / 180 * angle_deg;
	return QPointF (center.x() + size * cos (angle_rad),
	              center.y() + size * sin (angle_rad));
}

QPolygonF QtHexGrid::getHexCenteredAt(float x, float y, float size)
{
	QPolygonF polygon;
	float dx = std::sqrt(3.0) / 2 * size;
	polygon
			// standing on a point
			<< hex_corner(QPointF(x,y),size,0)
			<< hex_corner(QPointF(x,y),size,1)
			<< hex_corner(QPointF(x,y),size,2)
			<< hex_corner(QPointF(x,y),size,3)
			<< hex_corner(QPointF(x,y),size,4)
			<< hex_corner(QPointF(x,y),size,5);
	return polygon;
}

void QtHexGrid::resizeEvent (QResizeEvent* event)
{
	QWidget::resizeEvent (event);

	graphicsViewM->resize(event->size());

	graphicsViewM->fitInView(sceneM->sceneRect(), Qt::KeepAspectRatio);
}

void QtHexGrid::setData(std::vector<float> const& data)
{
	if (data.size()!= dataM.size())
		throw std::runtime_error("Data size mismatch!");


	float min = *std::min_element(data.begin(),data.end());
	float max = *std::max_element(data.begin(),data.end());
	float dx = max- min;

	std::cout << "min is " << min <<std::endl;
	std::cout << "max is " << max <<std::endl;
	std::cout << "dx is " << dx <<std::endl;
	for (size_t i = 0; i < data.size(); i++)
	{
		dataM[i].gradientScale = (data[i] - min)/dx;
		int yval = std::min((int)(100 * (dataM[i].gradientScale/dx)),99);
		yval = std::max(0,yval);
		QColor color = gradImgM.pixel( 0,  yval);

		std::cout << "gradientScale is " << (data[i] - min)/dx << " which gives " << (int)(100 * (dataM[i].gradientScale/dx)) << " for y" << std::endl;

		dataM[i].polyItem->setBrush(QBrush(color));

	}
}