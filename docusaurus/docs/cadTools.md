---
id: cadTools
title: CAD Tools
sidebar_label: CAD Tools
---
export const Highlight = ({children, color}) => ( <span style={{
      backgroundColor: color,
      borderRadius: '2px',
      color: '#000',
      padding: '0.2rem',
    }}>{children}</span> );

import useBaseUrl from "@docusaurus/useBaseUrl";

>Computer-aided design (CAD) is the use of computers (or workstations) to aid in the creation, modification, analysis, or optimization of a design. CAD software is used to increase the productivity of the designer, improve the quality of design, improve communications through documentation, and to create a database for manufacturing. Designs made through CAD software are helpful in protecting products and inventions when used in patent applications. CAD output is often in the form of electronic files for print, machining, or other manufacturing operations. [Wikipedia](https://en.wikipedia.org/wiki/Computer-aided_design)

## Menu Tools

All objects created contain various graphic attributes, such as: position, orientation, color, size, etc. To modify them, some intuitive and easy-to-use tools were inserted, which are: [**select**](cadTools#select), [**move**](cadTools#move), [**rotate**](cadTools#rotate), [**drag**](cadTools#drag), [**zoom**](cadTools#zoom), [**snap**](cadTools#snap), [**copy , paste**](cadTools#copy-and-paste) and [**delete**](cadTools#delete).

These tools can be accessed through their shortcut keys as well as through the [Tools submenu](mainScreen#menu-ribbon).

Such tools allow ** personalization **, ** navigation ** and assist in the ** creation ** of electric power networks, as well as control diagrams on the platform.

### Select
The selection of a single object is carried out by clicking with the left mouse button on the element, which will be identified by the outline in <Highlight color = "#80bfff"> blue</Highlight> of the object.

To select multiple objects simultaneously, it is necessary to click the left mouse button on an empty spot on the screen and drag it, thus creating a ** selection rectangle ** that will accompany the mouse pointer while the left button is pressed . When you release it, all objects that intersect the rectangle will be selected.

By holding down the Control (Ctrl) key on the keyboard, you can keep previous selections while new elements are selected.

To deselect all objects just click on an area without objects.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl("videos/select.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl("videos/select.webm")} type = "video/webm" />
</video>

### Move
An element can be moved to any location by means of the drag-and-drop action (by clicking and dragging) or by the move tool.

By enabling “move mode”, moving all selected components as the position of the mouse is changed. To disable this mode, just press the Escape key on the keyboard.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl("videos/move.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl("videos/move.webm")} type = "video/webm" />
</video>

### Rotate
Some objects can be rotated using the rotate tool, accessed by the shortcut key "R", rotating the object clockwise, or "Shift + R", counterclockwise (this option will affect all selected objects). The element can also be rotated through the [Tools submenu](mainScreen#menu-ribbon) or when accessing the context menu by right clicking on the element.

:::caution Attention!
The context menu will only be displayed if the element is selected.
:::

When using this tool, objects will rotate 45º from their point of origin. To achieve the desired angle, simply repeat the rotation process.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/rotate.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/rotate.webm")} type = "video/webm" />
</video>

### Drag
Three tools very useful in navigation of the circuit, mainly in large networks, are: drag, zoom and fit.

The drag tool makes it possible to move the entire circuit ** preserving the relative positions between the elements **. This tool can be accessed through the submenu "Tools" or ** by holding down the * scroll * of the mouse **, thus activating the "drag mode". Upon activating it, the entire circuit will be moved by clicking and dragging in the desired direction.

To exit “drag mode” just press the Escape key on the keyboard.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/drag.mp4")} type = "video mp4" />
  <source src = {useBaseUrl ("videos/drag.webm")} type = "video/webm" />
</video>

### Zoom
The zoom will be applied when using the ** mouse scroll ** to zoom in or out. The zoom tool will zoom in or out of the circuit from the position of the mouse pointer on the screen to find objects or highlight them.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/zoom.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/zoom.webm")} type = "video/webm" />
</video>

### Fit
The snap tool will move the circuit and zoom.necessary for all network components to be displayed on the screen. This tool can be accessed through the “Tools” submenu or by using the “Shift + F” key combination.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/fit.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/fit.webm")} type = "video/webm" />
</video>

### Copy and paste
Two important tools during the process of creating the power grid in software are copy and paste. Any circuit created in the PSP-UFU can be duplicated completely or partially in the same project or in multiples using these tools.

To copy, just select the elements you want to duplicate and access the tool in the "Tools" submenu or by pressing the "Ctrl + C" key combination, then the circuit data will be copied to the clipboard. When copying a circuit it can be pasted in the same project or in a different project, using the same submenu or pressing the combination of the keys "Ctrl + V".

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/copyPaste.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/copyPaste.webm")} type = "video/webm" />
</video>

### Delete
Any object can be deleted from the project using the delete tool, present in the Ribbon menu, by the Delete shortcut key or by accessing the context menu by right-clicking on the selected element.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/delete.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/delete.webm")} type = "video/webm" />
</video>


## Graphic customization of the elements
Some elements allow its graphic customization, such as bars and lines.

### Bar
In the case of busbars, you can change the ** length of the bar ** by clicking and dragging on the * pickbox * displayed when placing the mouse pointer over that selected element.

### Line
The lines can be inserted with ** "knots" (anchor points) **, customizing the arrangement of the lines in the single-line diagram. These nodes are inserted during the element creation process, prior to the selection of the second bar.

After insertion, new nodes can be attached via the context menu accessed by right clicking on the selected line. Its removal is also achieved by an option in the context menu.

As with buses, changing the position of the nodes is achieved by clicking and dragging the * pickbox * displayed when placing the mouse pointer over this selected element.

<video autoPlay loop muted playsInline controls>
  <source src = {useBaseUrl ("videos/graphicPropsChange.mp4")} type = "video/mp4" />
  <source src = {useBaseUrl ("videos/graphicPropsChange.webm")} type = "video/webm" />
</video>

## Connection and disconnection of elements in the bar
The elements can be disconnected and reconnected to the bars after their insertion only by the * drag-and-drop * tool. For this, the element must be selected and its connection node must be dragged to the required location.

:::warning Watch out!
The element to be disconnected / reconnected ** must be previously selected ** to carry out the operation. Otherwise, the bar will be moved or no operations will be performed.
:::

In case of disconnection, the element will be automatically removed from the simulation, being indicated by its color (the element will change to color <Highlight color = "#7f7f7f"> gray</Highlight>).

:::caution Attention!
When reconnecting the element to the circuit, one must ** pay attention to the status of its "circuit breaker" **, indicated by a square next to its connection node. A reconnected element returns to the circuit with its circuit breaker open (<Highlight color = "#ff1919"> red</Highlight>), and it is necessary to click on it to insert the element into the circuit.
:::

<video autoPlay loop muted playsInline controls>
  <source src= {useBaseUrl("videos/disconnect.mp4")} type="video/mp4" />
  <source src= {useBaseUrl("videos/disconnect.webm")} type="video/webm" />
</video>