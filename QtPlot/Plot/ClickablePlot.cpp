#include "ClickablePlot.h"

#include "Library/QtPlotMathLibrary.h"
#include "Managers/QtPlotSettingsManager.h"

ClickablePlot::ClickablePlot(QWidget* parent)
: InfoPlot(parent),
clickButton(Qt::RightButton),
clickModifier(Qt::NoModifier),
resetZoomByGraph(1),
turnOnGridText("Turn on Grid"),
turnOffGridText("Turn off Grid")
{
    defaultZoomMap[EA_xAxis] = QCPRange(0, 100);
    defaultZoomMap[EA_yAxis] = QCPRange(0, 100);

    resetActionText[ECA_ClickX] = "Reset X axis";
    resetActionText[ECA_ClickY] = "Reset Y axis";
    resetActionText[ECA_ClickPlot] = "Reset axes";

    enablingClickRules[ECA_ClickX] = true;
    enablingClickRules[ECA_ClickY] = true;
    enablingClickRules[ECA_ClickPlot] = true;

    menuStyle = "QMenu::item{"
	    "background-color: rgb(255, 255, 255);"
	    "color: rgb(0, 0, 0);"
	    "}"
	    "QMenu::item:selected{"
	    "background-color: rgb(0, 85, 127);"
	    "color: rgb(255, 255, 255);"
	    "}"
	    "QMenu::item:disabled{"
	    "background-color: rgb(200, 200, 200);"
	    "color: rgb(0, 0, 0);"
	    "}";

    clickMenu = new QMenu(this);
    clickMenu->setStyleSheet(menuStyle);

    switchGridAction = new QAction("Turn Grid", this);
    connect(switchGridAction, &QAction::triggered, this, &ClickablePlot::switchGridTriggeredSlot);

	resetZoomAction = new QAction("", this);
    connect(resetZoomAction, &QAction::triggered, this, &ClickablePlot::resetZoomTriggeredSlot);

    actionsMap[ECA_ClickPlot].append(switchGridAction);
}

ClickablePlot::~ClickablePlot()
{
    delete clickMenu;
}

void ClickablePlot::setResetAxisActionText(EClickPos pos, const QString& text)
{
    resetActionText[pos] = text;
}

void ClickablePlot::setSwitchGridTexts(const QString& turnOnText, const QString& turnOffText)
{
    turnOnGridText = turnOnText;
    turnOffGridText = turnOffText;
}

void ClickablePlot::setMenuStyleSheet(const QString& styleSheet)
{
    menuStyle = styleSheet;
    clickMenu->setStyleSheet(styleSheet);
}

void ClickablePlot::setClickEnable(EClickPos pos, bool enable)
{
    enablingClickRules[pos] = enable;
}

void ClickablePlot::addAction(EClickPos pos, QAction* action)
{
    actionsMap[pos].append(action);
}

void ClickablePlot::addMenu(EClickPos pos, QMenu* menu)
{
    menuMap[pos].append(menu);
    menu->setStyleSheet(menuStyle);
}

bool ClickablePlot::removeAction(EClickPos pos, QAction* action)
{
	const bool success = actionsMap[pos].removeOne(action);
    return success;
}

bool ClickablePlot::removeMenu(EClickPos pos, QMenu* menu)
{
    bool success = menuMap[pos].removeOne(menu);
    return success;
}

void ClickablePlot::clearActions(EClickPos pos)
{
    actionsMap[pos].clear();

    if(pos == ECA_ClickPlot)
    {
        actionsMap[pos].append(switchGridAction);
    }
}

void ClickablePlot::clearAllMenu(EClickPos pos)
{
    menuMap[pos].clear();
}

void ClickablePlot::setDefaultZoom(EAxis axis, QCPRange defaultZoom)
{
    defaultZoomMap[axis] = defaultZoom;
}

void ClickablePlot::mousePressEvent(QMouseEvent* event)
{
	InfoPlot::mousePressEvent(event);

	if (event->button() != clickButton || event->modifiers() != clickModifier) return;

	const double clickXPos = xAxis->pixelToCoord(event->pos().x());
	const double clickYPos = yAxis->pixelToCoord(event->pos().y());
    
    //click Y
    if (clickYPos >= yAxis->range().lower && clickYPos <= yAxis->range().upper && clickXPos < xAxis->range().lower)
    {
        if (enablingClickRules[ECA_ClickY] == false) return;
        clickPos = ECA_ClickY;
    }
    //click X
    else if (clickXPos >= xAxis->range().lower && clickXPos <= xAxis->range().upper && clickYPos < yAxis->range().lower)
    {
        if (enablingClickRules[ECA_ClickX] == false) return;
        clickPos = ECA_ClickX;
    }
    //reset all
    else if (clickYPos >= yAxis->range().lower && clickYPos <= yAxis->range().upper
        && clickXPos >= xAxis->range().lower && clickXPos <= xAxis->range().upper)
    {
        if (enablingClickRules[ECA_ClickPlot] == false) return;

        clickPos = ECA_ClickPlot;

        bNeedEnableGrid = !xAxis->grid()->visible() && !yAxis->grid()->visible();
        switchGridAction->setText(bNeedEnableGrid ? turnOnGridText : turnOffGridText);
    }
    else
    {
        return;
    }

    resetZoomAction->setText(resetActionText[clickPos]);

    reinitializeMenu();

    clickMenu->popup(event->globalPos());
}

void ClickablePlot::switchGridTriggeredSlot(bool)
{
    xAxis->grid()->setVisible(bNeedEnableGrid);
    yAxis->grid()->setVisible(bNeedEnableGrid);
    layer("grid")->replot();

    QtPlotSettingsManager::instance().setGridVisible(objectName(), bNeedEnableGrid);
}

void ClickablePlot::resetZoomTriggeredSlot(bool)
{
	switch (clickPos)
	{
	case ECA_ClickX:
        resetZoomX();
        break;
	case ECA_ClickY:
        resetZoomY();
        break;
	case ECA_ClickPlot:
        resetZoomXY();
		break;
	}
    replot();
}

void ClickablePlot::configureSlot(const QString& objectName)
{
	InfoPlot::configureSlot(objectName);

    const bool visible = QtPlotSettingsManager::instance().getGridVisible(objectName);
    setGridVisible(visible);
}

void ClickablePlot::resetZoomX()
{
    resetZoom(EA_xAxis, xAxis);
}

void ClickablePlot::resetZoomY()
{
    resetZoom(EA_yAxis, yAxis);
}

void ClickablePlot::resetZoomXY()
{
    resetZoomX();
    resetZoomY();
}

void ClickablePlot::setGridVisible(bool visible)
{
    bNeedEnableGrid = visible;
    switchGridTriggeredSlot(bNeedEnableGrid);
}

void ClickablePlot::resetZoom(EAxis axis, QCPAxis* axisObj)
{
    QCPRange clampRange;

    if (getAxisClamp(axis))
    {
        clampRange = getLimitRange(axis);
    }
    else
    {
        if(axis == EA_xAxis)
        {
            clampRange = resetByGraphX();
        }
        else
        {
            clampRange = resetByGraphY();
        }
    }

    axisObj->setRange(clampRange);
}

QCPRange ClickablePlot::resetByGraphX()
{
    const QCPGraph* mainGraph = graph(resetZoomByGraph);

    if (!mainGraph || mainGraph->dataCount() == 0) return defaultZoomMap[EA_xAxis];

    const double min = mainGraph->data()->at(0)->key;
    const double max = mainGraph->data()->at(mainGraph->data()->size())->key;

    return { min, max };
}

QCPRange ClickablePlot::resetByGraphY()
{
    const QCPGraph* mainGraph = graph(resetZoomByGraph);

    if (!mainGraph || mainGraph->dataCount() == 0) return defaultZoomMap[EA_yAxis];

    double min = mainGraph->data()->at(0)->value;
    double max = min;

    for (auto it = mainGraph->data()->begin(); it != mainGraph->data()->end(); it++)
    {
        if (it->value > max)
        {
            max = it->value;
        }

        if (it->value < min)
        {
            min = it->value;
        }
    }

    double delta = (max - max) * 0.2;

    if (equals(delta, 0.0))
    {
        delta = 10.0;
    }

    return { min - delta, max + delta };
}

void ClickablePlot::reinitializeMenu()
{
    clickMenu->clear();

    QVector<QMenu*> menu = menuMap[clickPos];
    for (const auto m : menu)
    {
        clickMenu->addMenu(m);
    }

    QVector<QAction*> actions = actionsMap[clickPos];
    for (const auto action : actions)
    {
        clickMenu->addAction(action);
    }

    clickMenu->addAction(resetZoomAction);
}
