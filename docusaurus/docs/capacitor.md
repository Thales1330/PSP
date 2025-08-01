---
id: capacitor
title: Capacitor
sidebar_label: Capacitor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

>A device consisting essentially of two electrodes separated by a dielectric. [IEC 60050](http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=436-01-03).

## Capacitor in PSP-UFU
The capacitor element usually represents a *shunt* capacitor bank in the PSP-UFU circuit.

:::tip Tip
A series arrangement of a [line](line) without *shunt* susceptances, a [bus](bus), and a [capacitor](capacitor) can be used to create a passive filter in [harmonic studies](harmonics).  
In future versions, a passive filter element will be implemented in PSP-UFU.
:::

Capacitors and reactors require only their reactive power as a parameter.  
These passive elements are only inserted into the admittance matrix, and their impedance is calculated from their nominal power and voltage.

## Capacitor editing form
The figure below shows the capacitor data insertion/editing form:

<div><center><img src={useBaseUrl("images/capacitorForm.png")} alt="Capacitor form in PSP-UFU" title="Capacitor form in PSP-UFU" /></center></div>

Besides the single general context, the "Stability" button can be seen in the lower-left corner of the form.  
This form, common to several other elements, allows the insertion and/or removal of the capacitor during the [stability](stability) study.

<div><center><img src={useBaseUrl("images/swCapacitor.png")} alt="Capacitor switching form" title="Capacitor switching form" /></center></div>

<Tabs
  groupId="transformer-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Stability Button', value: 'stability'},
  ]
}>
<TabItem value="general">

#### Name
Electrical element identification. Any number of characters can be entered using the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

All PSP-UFU power components have this field.

#### Reactive power
Capacitors and reactors only require their reactive power as a parameter, entered in Mvar, kvar, var, or $p.u.$ (in the system base power).

</TabItem>
<TabItem value="stability">

The "Stability" button opens a form, common to several other elements, that allows the insertion and/or removal of the transformer during the [stability](stability) study.

In this form, a generic list of insertions and removals over time can be created, customized by a switching properties context where the switching type (insertion or removal) and the event time (in seconds) are edited.  
These properties are added to or removed from the list using the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>
