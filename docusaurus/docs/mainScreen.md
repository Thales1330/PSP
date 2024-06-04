---
id: mainScreen
title: Main Screen
sidebar_label: Main Screen
---
import useBaseUrl from "@docusaurus/useBaseUrl";

## Organization of the main screen
The main screen of the PSP-UFU is divided into three sections: *[Menu "Ribbon"](mainScreen#ribbon-menu)*, *[Workspace](mainScreen#workspace)* and *[Toolbar Status](mainScreen#status-bar)*.

<div> <center> <img src = {useBaseUrl ("images/mainScreen.svg")} alt = "Main screen" title = "PSP-UFU main screen on Windows" /> </center> </div>

Briefly, in the **Ribbon Menu** the PSP-UFU tools are accessed; in **Work Area** single-line power diagrams are created; and the **Status Bar** displays important information about the status of the Work Area.


### Ribbon Menu
> In computing, the ribbon menu is a GUI-based interface presentation format where the toolbar is shown through a wider bar with larger icons enabling applications to be used by *touch screen* devices.
[Wikipedia](https://en.wikipedia.org/wiki/Ribbon_(computing))

The Ribbon menu consists of three submenus: "File", "Tools" and "Simulation".

<div> <center> <img src = {useBaseUrl ("images/menuFile.png")} alt = "File menu" title = "File menu" /> </center> </div>

In **File** are the general tools of the program, which allow the user to create and save new projects, in addition to those that make it possible to load existing projects and import files from other programs. This submenu also has general program options and access to information about the program.

<div> <center> <img src = {useBaseUrl ("images/menuTools.png")} alt = "Tools Menu" title = "Tools Menu" /> </center> </div>

In **Tools** there are specific items, which are related to the creation and manipulation of the power grid, tabular data output reports and access to the form for creating graphs over time. Some settings specific to the active project are accessed through this menu.

<div> <center> <img src = {useBaseUrl ("images/menuSimulation.png")} alt = "Simulation Menu" title = "Simulation Menu" /> </center> </div>

Finally, the **Simulation** submenu provides user access to the calculations performed by the software and its settings, in addition to the “continuous solution” tool, which solves the static calculations (power flow, short circuit and harmonics) after any changes to the power grid, such as removing a component or changing its data. All simulation settings for the active project are accessed through this menu.

### Workspace
In the work area are present the projects in execution, which are divided by tabs identified by the name of the project (if already recorded on the disc). It is in this area that the electrical elements are inserted and excluded, whose tools are accessed through the [Menu Ribbon](mainScreen#ribbon-menu).

The graphic edits of the elements, such as position, connection and size of the busbars, are performed through the action drag-and-drop using the mouse. The editing of the electrical data is done through data forms accessed by double clicking on the elements.

<div> <center> <img src = {useBaseUrl ("images/workspace.png")} alt = "Workspace" title = "Workspace" /> </center> </div>

### Status Bar
The status bar is responsible for interesting information about the current circumstances of the program: mode of operation of the mouse (edit, move, drag), zoom applied and position of the mouse on the desktop, in addition to data on the actions of users, such as : insertion of elements, information about copying and pasting, etc.

:::tip Tip
**Always keep an eye on the status bar**, as important information about the program's operation is provided. If you have any questions follow the instructions presented in that component.
:::

## General settings
The general settings of the program are accessed in the Ribbon ** File ** submenu. These settings are applied to all projects and remain recorded on the disc.

<div> <center> <img src = {useBaseUrl ("images/generalSettings.png")} alt = "General settings" title = "General settings" /> </center> </div>

:::warning Caution
Some security settings on your computer (especially on Windows systems) may generate an error message when confirming changes to general settings.

To solve this problem just **run PSP-UFU as an administrator** (right click on the program shortcut and then run as administrator). Change the settings again and restart the program.
:::

### Language
The following languages ​​are currently available on PSP-UFU:
- English
- Portuguese

:::info Information
The program must be restarted to have the effect of changing the language.
:::

### Rendering
Defines how the graphic elements of the [power editor] (powerEditor) and [control editor] (controlEditor) are drawn on the screen. Currently two options are available:
- [OpenGL](https://www.opengl.org/about/)
- [Device Context](https://docs.wxwidgets.org/3.0/classwx_graphics_context.html)

:::warning Watch out!
OpenGL may not be supported by your computer. In this case, when creating a new project, the work screen is not displayed or a black screen is displayed. The solution is to change the renderer to "Device Context".

If an error message appears, run the program as an administrator and repeat the process.
:::

:::info Information
The program must be restarted to have the effect of changing the renderer.
:::

## PSP-UFU project files
The options for creating, saving and opening projects on the disk, as well as importing files from other programs are present in the Ribbon **File** submenu.

### New project
The creation of a new project is done by clicking the **New project** button in the [submenu **File**](mainScreen#ribbon-menu). This action creates a blank system in the work area, where it is possible to insert the electrical elements through the [submenu **Tools**](mainScreen#ribbon-menu) or by the [shortcut keys](powerEditor#teclas-de-atalho).

This blank system can be created using the [Power Editor](powerEditor).

### Save and Save As ...
The "Save" option overrides the changes made to the open project and saves it to disk. The "Save as ..." option creates a new file and saves the project to the disk with the help of a folder selection window (and definition of the file name).

:::info Information
For projects that are being recorded for the first time, the "Save" option behaves in the same way as the "Save as ..." option.
:::

:::tip Tip
PSP-UFU saves files with the extension * .psp *. These files are nothing more than text files using [markup language](https://en.wikipedia.org/wiki/Markup_language) XML ([eXtensible Markup Language](https://en.wikipedia.org/wiki/XML)), which defines a series of rules for formatting data so that it is both human and machine readable.

Therefore, the electrical data contained in them can be easily identified and changed, if necessary.
:::

### Open project
This option opens the projects recorded on the disc through the aid of a file selection window.

### Import project
PSP-UFU allows the import of files from the following programs:
- [ANAREDE](http://www.cepel.br/pt_br/produtos/programas-computacionais-por-categoria/menu/anarede-analise-de-redes-eletricas.htm)
- [Matpower](https://matpower.org/)

To import **ANAREDE** files, both the electrical data file (*.pwf*) and the graphic data of the elements (*.lst*) are used.

Since the **Matpower** file (*.m*) does not have graphic data of the electrical elements, the single-line diagram is automatically generated by PSP-UFU when importing it.

#### Automatic layout generation of single-line diagrams
To create the automatic layout, we used the theory of graphs based on positioning directed to the force and application of graphs with weighted edges. According to graph theory, the system is modeled as vertices and edges. In this context, the busbars are the vertices and the branches of the system (lines and transformers) are the edges. The locations of branch elements, as well as the nodes of the elements, are automatically controlled by the PSP-UFU.

The automatic layout tool is composed of an iterative process and the results are more refined with a greater number of iterations. The best results are obtained using the number of iterations equal to or greater than five times the number of buses in the system.

:::caution Attention!
The automatic layout is still under development and some improvements can be made to avoid intersections and overlaps, however, in its current state, it is fully functional and satisfactory.
:::

:::tip Tip
Since Matpower can convert CDF (Common Data Format) and PSS/E RAW formats to *.m* files, these can be imported indirectly into PSP-UFU.
:::