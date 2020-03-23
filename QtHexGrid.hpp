#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

struct CellData
{
	QPolygonF poly;
	QColor color;
	int index;
	float gradientScale;
	QGraphicsPolygonItem* polyItem = nullptr;
	QGraphicsTextItem* textItem = nullptr;
};

class QtHexGrid : public QWidget
{
	public:
		QGraphicsView* graphicsViewM = nullptr;
		QGraphicsScene* sceneM = nullptr;
		QGraphicsRectItem* legendM = nullptr;
		QColor lowColorM;
		QColor midColorM;
		QColor highColorM;
		int rowsM = 0;
		int colsM = 0;

		QtHexGrid(int rows, int cols,QColor lowColor = Qt::blue, QColor midColor = Qt::black, QColor highColor = Qt::yellow,  QWidget* parent = nullptr);
		virtual ~QtHexGrid() = default;

		void setData(std::vector<float> const& data);

	protected:
		void resizeEvent (QResizeEvent* event) override;

		QPolygonF getHexCenteredAt(float x = 0, float y=0, float size=20);

		float hexSizeM = 20;
		std::vector<CellData> dataM;
		QLinearGradient gradientM;
		QImage gradImgM;
};


