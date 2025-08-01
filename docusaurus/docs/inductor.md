---
id: inductor
title: Inductor
sidebar_label: Inductor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

>A reactor intended for shunt connection in a network to compensate the capacitive current. [IEC 60050](http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=421-01-03).

## Inductor in PSP-UFU
The inductor (or reactor) element generally represents a *shunt* reactor in the PSP-UFU circuit.

Capacitors and reactors require only their reactive power as a parameter.  
These passive elements are inserted into the admittance matrix, with impedance calculated from their rated power and voltage.

## Inductor editing form
The figure below shows the insertion/editing form for inductor data:

<div><center><img src={useBaseUrl("images/indForm.png")} alt="Inductor form in PSP-UFU" title="Inductor form in PSP-UFU" /></center></div>

In addition to the single general context, the "Stability" button can be seen in the lower-left corner of the form.  
This form, common to several other elements, allows the insertion and/or removal of the inductor during the [stability](stability) study.

<div><center><img src={useBaseUrl("images/indSw.png")} alt="Inductor switching form" title="Inductor switching form" /></center></div>

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
Capacitors and reactors require only their reactive power as a parameter, entered in Mvar, kvar, var, or $p.u.$ (based on the system base power).

</TabItem>
<TabItem value="stability">

The "Stability" button opens a form, common to several other elements, allowing the insertion and/or removal of the transformer during the [stability](stability) study.

In this form, a generic list of insertions and removals over time can be created, customized by a switching properties context where the switching type (insertion or removal) and the event time (in seconds) are edited.  
These properties are added or removed from the list using the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>
