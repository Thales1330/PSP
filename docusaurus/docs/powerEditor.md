---
id: powerEditor
title: Power Editor
sidebar_label: Power Editor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

The Power Elements Editor in PSP-UFU allows the insertion, manipulation, and editing of electric power elements through a single-line diagram.

>(**Single-line diagram**) A system diagram in which the polyphase links are represented by their equivalent single line [IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=601-02-04).

:::info Information
All inserted elements can be manipulated and edited using the [CAD Tools](cadTools).
:::

## New Project
The creation of a new project is done by clicking on the **New Project** item in the [**File** submenu](mainScreen#ribbon-menu).  
This action creates a blank system in the workspace, where it is possible to insert electric elements through the [**Tools** submenu](mainScreen#ribbon-menu) or using the [keyboard shortcuts](powerEditor#teclas-de-atalho).

Below are the electric power elements that can be inserted in PSP-UFU:
- [Bus](bus)
- Branch elements
	- [Line](line)
	- [Transformer](transformer)
- Shunt elements
	- [Load](load)
	- [Capacitor](capacitor)
	- [Inductor](inductor)
	- [Harmonic Current Source](harmSource)
	- Electromagnetic Transient (ATP)
- Machines
	- [Synchronous Generator](syncGenerator)
	- [Synchronous Condenser](syncMotor)
	- [Induction Motor](indMotor)

<div><center><img src={useBaseUrl("images/powerElements.svg")} alt="Power elements" title="Power elements" /></center></div>

## Inserting elements
Through the **Tools** submenu, power elements can be inserted, as shown in the image below.

<div><center><img src={useBaseUrl("images/addPowerElements.png")} alt="Add power elements" title="Add power elements" /></center></div>

When inserting a bus, the user must only click on the desired position. For the other components, the user will be prompted to select one or two buses where the new component will be connected.

Whenever new elements are inserted, a data form is displayed, and it is possible to edit the electrical data and access additional forms, such as stability data.

:::caution Attention
The first element to be inserted in the project **must** be a bus. The other electrical elements must be connected to one or more buses.
:::

In addition to the power elements, it is possible to insert *[linked text](text)* elements, which are associated with a quantity to be visualized directly in the [Workspace](mainScreen#workspace).

:::warning Warning!
When inserting a bus, it will follow the mouse pointer until you click on the desired position.  
For the other elements, **you must click on one or more buses** to display the inserted element.

**Always pay attention to the instructions in the [Status Bar](mainScreen#status-bar).**
:::

<video autoPlay loop muted playsInline controls>
  <source src= "/PSP/videos/timelapseBuild.mp4" type="video/mp4" />
</video>

### Keyboard Shortcuts
All power elements can be inserted using keyboard shortcuts, making the creation of single-line diagrams in PSP-UFU easier and faster. The table below shows these shortcuts:

| Element               | Shortcut  | Element                   | Shortcut  |
| --------------------- | :-------: | ------------------------ | :-------: |
| Bus                   | B         | Load                      | Shift + L |
| Line                  | L         | Capacitor                 | Shift + C |
| Transformer           | T         | Inductor                  | Shift + I |
| Generator             | G         | Harmonic current          | Shift + H |
| Induction motor       | I         | Electromagnetic Transient | Shift + E |
| Synchronous condenser | K         | Text                      | A         |

## Editing Electrical Data
When inserting an element, its corresponding editing form is displayed for entering the element's electrical data.  
All data can be **modified** by **double-clicking the element** or using the **context menu** displayed by right-clicking the element.

Each element’s electrical data editing form has its own particularities and is described in its specific documentation:  
[Bus](bus), [Line](line), [Transformer](transformer), [Load](load), [Capacitor](capacitor), [Inductor](inductor), [Harmonic Current Source](harmSource), [Synchronous Generator](syncGenerator), [Synchronous Condenser](syncMotor), [Induction Motor](indMotor).
