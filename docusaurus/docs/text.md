---
id: text
title: Linked Text
sidebar_label: Linked Text
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

The linked text element (to the power element) can be inserted in the [power elements editor](powerEditor). This text element can be linked to numerous results and parameters of the power system.

This element is useful for identifying and analyzing the results of power elements.

:::tip Tip
Another important application is updating its value when applying “continuous solution” in the project, helping visualize changes in the power grid’s state.

The results presented by these elements can also be used together with the single-line diagram when creating project reports.
:::

<div><center><img src={useBaseUrl("images/textExp.png")} alt="Example of the text element in PSP-UFU" title="Example of text element in PSP-UFU" /></center></div>

## Inserting text elements in PSP-UFU
Through the **Tools** submenu, you can insert text elements, as shown in the image below.

<div><center><img src={useBaseUrl("images/addTextElement.svg")} alt="Add text elements" title="Add text elements" /></center></div>

By selecting the "Text" button or using the **"A" shortcut key**, the user must click the desired position in the [workspace](powerEditor). This will add a temporary element with the value "Text". To configure this element, double-click on it.

:::caution Caution
The text element, like [control elements](controlEditor), does not automatically display the data editing form when inserted for the first time.
:::

## Text element data editing form
The figure below shows the data editing form for this graphic component.

<div><center><img src={useBaseUrl("images/textForm.png")} alt="Text element form in PSP-UFU" title="Text element form in PSP-UFU" /></center></div>

Its configuration is carried out through fields that associate the component with the electrical power elements, in addition to the numeric result format. The fields are enabled as previous data is entered, and an error message is displayed if any required parameter is not filled in.

The association process begins by choosing the type of power element that will be linked to the text. Next, a list is displayed with the name of all components previously inserted in the power elements editor of the type defined in the previous field, thus associating the text with the element.

In “Text type” you define what output will be displayed. This field depends on the type of element selected, as shown in the table below. Some other data are required for branch elements, such as source and destination buses, to represent the correct power and current flow. Finally, the user must enter the number of decimal places for numeric data. A preview can be seen in the form.

|Power Element|Text Type|Available Units|
|:-----------:|---------|---------------|
|[Bus](bus)|Name<br/>Voltage<br/>Angle<br/>Fault current<br/>Fault voltage<br/>Short-circuit level<br/>Voltage THD|-<br/>$p.u.$, V, kV<br/>Degrees, Radians<br/>$p.u.$, A, kA<br/>$p.u.$, V, kV<br/>$p.u.$, VA, kVA, MVA<br/>%|
|[Generator](syncGenerator)|Name<br/>Active power<br/>Reactive power<br/>Fault current|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar<br/>$p.u.$, A, kA|
|[Line](line) and [transformer](transformer)|Name<br/>Active power flow<br/>Reactive power flow<br/>Losses<br/>Current<br/>Fault current|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar<br/>$p.u.$, W, kW, MW<br/>$p.u.$, A, kA<br/>$p.u.$, A, kA|
|[Load](load), [Induction Motor](indMotor)<br/>and [Synchronous Condenser](syncMotor)|Name<br/>Active power<br/>Reactive power|-<br/>$p.u.$, W, kW, MW<br/>$p.u.$, var, kvar, Mvar|
|[Capacitor](capacitor) and [Inductor](inductor)|Name<br/>Reactive power|-<br/>$p.u.$, var, kvar, Mvar|

:::info Information
Once inserted, these text elements can be manipulated in the [same way as power elements](cadTools).
:::
