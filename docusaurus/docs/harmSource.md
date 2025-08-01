---
id: harmSource
title: Harmonic Current Source
sidebar_label: Harmonic Current Source
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

> (**Harmonic disturbance source**) Part of the electric power system or the installations connected to it that causes harmonic distortion of the current and/or voltage waveform. [IEC 60050](http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=614-01-16).

## Harmonic current source in PSP-UFU
Harmonic current sources are responsible for injecting harmonic currents and are used by the [harmonic distortion](harmonics) calculation tool.  
A list of harmonic currents can be inserted into the same element, as shown in the data editing form.

:::caution Attention!
The presence of a harmonic current source is **not considered** in [load flow](powerFlow), [short-circuit](fault), and [stability](stability) studies.
:::

## Harmonic current source editing form
The figure below shows the insertion/editing form for the harmonic current source data:

<div><center><img src={useBaseUrl("images/harmSourceForm.png")} alt="Inductor form in PSP-UFU" title="Inductor form in PSP-UFU" /></center></div>

#### Name
Electrical element identification. Any number of characters can be entered using the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

#### Order
Specifies the harmonic order to be inserted into the element’s harmonic current list.  
If the harmonic order already exists in the list, it will be replaced with the new parameters.

#### Current
Magnitude of the harmonic current.

#### Unit
Unit of the harmonic current magnitude, in A or $p.u.$

#### Angle
Angle of the harmonic current, in degrees.

#### Add / Remove
Adds or modifies the new harmonic order when clicking “Add”.  
Removes the selected harmonic order from the list when clicking “Remove”.
