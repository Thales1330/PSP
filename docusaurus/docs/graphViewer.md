---
id: graphViewer
title: Graph Viewer
sidebar_label: Graph Viewer
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

To display the results from [stability](stability) calculations and [frequency sweep](harmonics), a graph viewer was developed, as shown below:

<div><center><img src={useBaseUrl("images/graphViewer.png")} alt="PSP-UFU graph viewer" title="PSP-UFU graph viewer" /></center></div>

## Curve creation, manipulation, and navigation
This viewer uses the [wxMathPlot](https://wxmathplot.sourceforge.io/) library and allows the user to plot all time-domain curves from [stability](stability) and [frequency sweep](harmonics) results.  
They are organized hierarchically in a tree structure, identified by the power element type and name, as well as the curve type.

:::info Information
Curves can be plotted by **double-clicking** on them or by checking the "Plot" box in the trace properties.
:::

The printed graphs can be customized by editing their trace properties: color, thickness, and line style (six types available).  
By default, the vertical axis is time (in stability) or frequency (frequency sweep), but the user can switch to any available curve.

Several general graph properties can be edited: title, x and y-axis labels, margins, and axis limits.

The **View** menu allows enabling legends, grid lines, and mouse pointer coordinates (used to highlight points on the curve), as well as toggling between light and dark themes for better visualization or export.

In the **File** menu, the graph can be saved as an image (*.png*, *.jpg*, or *.bmp*) or copied directly to the clipboard.  
The same menu also allows exporting in *[CSV (Comma-separated Values)](https://en.wikipedia.org/wiki/Comma-separated_values)* format, which can be used in other graph viewers, like Excel or [gnuplot](http://www.gnuplot.info/).

The plotting area allows navigation using zoom tools, selection areas, mouse commands, and curve dragging.

:::tip Tip
Right-clicking on the plotting area shows manipulation options through a context menu.
:::

The following **mouse commands** are used to navigate curves:

|Command|Action|
|-------|------|
|Left button pressed + Select area|Selects zoom area|
|Right button pressed + Move mouse|Drags the curve|
|Mouse wheel|Vertical scroll|
|Mouse wheel + Shift|Horizontal scroll|
|Mouse wheel + Ctrl|Zoom in/out|
