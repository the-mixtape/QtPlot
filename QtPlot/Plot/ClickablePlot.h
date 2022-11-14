#pragma once
#include "InfoPlot.h"


class QTPLOT_EXPORT ClickablePlot : public InfoPlot
{
	Q_OBJECT

public:
	explicit ClickablePlot(QWidget* parent = nullptr);
	~ClickablePlot();

public:
	void setResetAxisActionText(EClickPos pos, const QString& text);
	void setSwitchGridTexts(const QString& turnOnText, const QString& turnOffText);

	void setMenuStyleSheet(const QString& styleSheet);

	void setClickEnable(EClickPos pos, bool enable);

	void addAction(EClickPos pos, QAction* action);
	void addMenu(EClickPos pos, QMenu* menu);

	bool removeAction(EClickPos pos, QAction* action);
	bool removeMenu(EClickPos pos, QMenu* menu);

	void clearActions(EClickPos pos);
	void clearAllMenu(EClickPos pos);

	void setDefaultZoom(EAxis axis, QCPRange defaultZoom);

protected:
	void mousePressEvent(QMouseEvent* event) override;

private slots:
	void switchGridTriggeredSlot(bool);
	void resetZoomTriggeredSlot(bool);

protected slots:
	void configureSlot(const QString& objectName) override;

protected:
	virtual void resetZoomX();
	virtual void resetZoomY();
	virtual void resetZoomXY();

private:
	void setGridVisible(bool visible);

	void resetZoom(EAxis axis, QCPAxis* axisObj);

	QCPRange resetByGraphX();
	QCPRange resetByGraphY();

	void reinitializeMenu();

private:
	Qt::MouseButton			clickButton;
	Qt::KeyboardModifier	clickModifier;

	QMap<EClickPos, QVector<QAction*>> actionsMap;
	QMap<EClickPos, QVector<QMenu*>>	menuMap;

	QMenu*						clickMenu;

	QAction*					switchGridAction;
	QString						turnOnGridText;
	QString						turnOffGridText;
	bool						bNeedEnableGrid;

	QAction*					resetZoomAction;
	QMap<EClickPos, QString>	resetActionText;

	QMap<EAxis, QCPRange>		defaultZoomMap;
	int							resetZoomByGraph;

	QMap<EClickPos, bool>		enablingClickRules;

	EClickPos					clickPos;
	QString						menuStyle;

};
