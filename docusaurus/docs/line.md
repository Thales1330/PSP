---
id: line
title: Line
sidebar_label: Line
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

>A manufactured transmission medium used to transmit electromagnetic energy between two points with minimal radiation. [*free translation* - IEC 60050](http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=704-02-02).

## Line in PSP-UFU
Lines in PSP-UFU are balanced $\pi$ models. They can be used as transmission and distribution lines in power systems.

The figure below shows the $\pi$ model implemented in PSP-UFU:

<div><center><img src={useBaseUrl("images/lineModel.svg")} alt="Line PI Model" title="Line PI Model" /></center></div>

Where:
- $\bold{r_L}$	is the line resistance;
- $\bold{x_L}$	is the line inductive reactance;
- $\bold{b_{L}^{d}}$	is the line *shunt* capacitive susceptance.

:::caution Caution
The model used in PSP-UFU is **balanced** and **does not have mutual impedances** between phases.
:::

The line can be inserted with anchoring points, or "nodes," for greater graphical customization of the element, as shown in the [CAD Tools](cadTools#line).

:::warning Warning!
The line must be inserted between two buses of **the same nominal voltage**. If you try to insert it between buses of different voltages, an error message will be displayed.
:::

:::tip Tip
A series arrangement of a [line](line) without the *shunt* susceptances, a [bus](bus), and a [capacitor](capacitor) can be used to build a passive filter in [harmonic studies](harmonics). In future versions, a passive filter element will be implemented in PSP-UFU.
:::

## Line editing form

The image below shows the insertion/modification form for transmission line data:

<div><center><img src={useBaseUrl("images/lineForm.png")} alt="Lines form in PSP-UFU" title="Lines form in PSP-UFU" /></center></div>

This form is subdivided into two distinct contexts:
- **General**: where general line information and load flow data are entered;
- **Fault**: where zero sequence impedances are entered.

:::info Information
The parameters needed to build the line according to its $\pi$ model are entered in the General tab, used to build the positive and negative sequence admittance matrix.

Additional zero sequence impedance data necessary for unbalanced short-circuit calculations are edited in the Fault tab, used in the construction of the zero sequence admittance matrix.
:::

Besides these two contexts, there is a "Stability" button at the bottom left of the form. This form, common to several other elements, allows insertion and/or removal of the line during [stability](stability) studies.

<div><center><img src={useBaseUrl("images/lineSw.png")} alt="Line switching form" title="Line switching form" /></center></div>

<Tabs
  groupId="line-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Fault', value: 'fault'},
    {label: 'Stability Button', value: 'stability'},
  ]}
>
<TabItem value="general">

#### Name
Identification of the electrical element. Any number of characters can be entered using the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

All power components in PSP-UFU have this field.

#### Nominal voltage
*Non-editable* field showing the nominal voltage of the line. To change this field, it is necessary to edit the corresponding field of the connected [bus](bus#name).

#### Nominal power
Nominal power of the line, entered in MVA, kVA, or VA.

This field is especially important if the option "Use nominal power as base" is checked.

#### Parameters of the $\pi$ model
Resistance ($r_L$), series inductive reactance ($x_L$), and **total** shunt capacitive susceptance ($2b_{L}^{d}$) present in the line's $\pi$ model.

These parameters are entered in $p.u.$, $\Omega$ or $\Omega/km$ ($S$ or $S/km$ for $b_{L}^{d}$).

#### Line length
Used to calculate the value of line parameters entered in $\Omega/km$ (or $S/km$).

:::info Information
The line length is ignored if units per kilometer are not used.
:::

#### Use nominal power as base
If this option is checked, the program will use the nominal power of the line as the base for unit conversion; otherwise, the [system base power](simulationConfig) will be used.

</TabItem>
<TabItem value="fault">

#### Zero sequence $\pi$ model parameters
These parameters are necessary for the correct calculation of [unbalanced faults](fault) (phase-phase, phase-phase-ground, and phase-ground) currents and must be entered in $p.u.$

:::warning Warning!
Failure to enter these data will result in incorrect results for [unbalanced faults](fault).
:::

</TabItem>
<TabItem value="stability">

The "Stability" button will open a form, common to several other elements, that allows insertion and/or removal of the line during [stability](stability) studies.

In this form, a generic list of line insertions and removals over time can be created, customized by a switching property context where the switching type (insertion or removal) and the event instant (in seconds) are edited. These properties are added to and removed from the generic list via the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>
