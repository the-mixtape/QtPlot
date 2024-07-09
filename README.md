
<h1 align="center"> QtPlot </h1>
<h5 align="center"> QT C++ widget for plotting and visualizing data </h5> 

This library is based on [QCustomPlot](https://www.qcustomplot.com/). The library adds missing functionality to QCustomPlot and implements the Waterfall graph type. New functionality has been added to new layers, which allows you not to redraw the entire graph and work quickly. 
Also in the solution there is an [ExampleProject](/ExampleProject) in which you can see how to use these classes.

Classes:
* [QtPlot](#QtPlot) 
* [WaterfallPlot](#WaterfallPlot)

---

<h3 align="center"><a name="QtPlot"></a>QtPlot</h3> 

<p align="center">
  <img src="https://github.com/the-mixtape/QtPlot/blob/main/Resources/Markers.gif">
</p>

#### Implemented features
* movable markers
* clamp move & zoom zone
* full customization of marker styles
* synchronization of the zoom (2 modes)

---

<h3 align="center"><a name="WaterfallPlot"></a>WaterfallPlot</h3> 

<p align="center">
  <img src="https://github.com/the-mixtape/QtPlot/blob/main/Resources/Waterfall.gif">
</p>

Based on Qimage of pure Qt and QCPItemPixmap of QCustomPlot.
#### Implemented features
* data feed from top, bottom, left and right.
* clearing plot
* thread for append data
* full customization color, resolution, position
