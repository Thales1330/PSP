---
id: transformer
title: Transformer
sidebar_label: Transformer
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

>A static device with two or more windings that, by electromagnetic induction, transforms a system of alternating voltage and current into another system of voltage and current generally of different values and at the same frequency, with the purpose of transmitting electrical energy. [*free translation* - IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=421-01-01).

## Transformer in PSP-UFU
Transformers with any connections and phase shifts between primary and secondary voltages can be inserted in PSP-UFU. For this, transformers must be modeled representing their tap ($t$) and phase shift ($\phi$), using an ideal transformer with transformation ratio $\overline{t}:1$ in series with its impedance ($\overline{y}_T = r_T + jx_T$), where $\overline{t}$ is a complex number ($t\angle\phi$).

:::caution Caution
The model used in PSP-UFU **does not consider** the magnetizing branch or mutual impedances between phases.
:::

The figure below shows the transformer model implemented in PSP-UFU:

<div><center><img src={useBaseUrl("images/transformerModel.svg")} alt="PI Model of the line" title="PI Model of the line" /></center></div>

Where:
- $\bold{r_T}$ is the total transformer resistance;
- $\bold{x_T}$ is the total inductive reactance of the transformer;
- $\bold{\overline{t}}$ is the complex tap of the transformer.

The relationship between the voltages and currents of generic buses $i$ and $j$ is given by the matrix equation:

$$
\begin{bmatrix}
\dot{I}_{ij}\\
\dot{I}_{ji}
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle \frac{\overline{y}_T}{t^2} & -\displaystyle \frac{\overline{y}_T}{\overline{t}^*}\\
-\displaystyle \frac{\overline{y}_T}{\overline{t}} & \overline{y}_T
\end{bmatrix}
\times
\begin{bmatrix}
\dot{V}_{i}\\
\dot{V}_{j}
\end{bmatrix}
$$

As in [lines](line), the two-winding transformer must be inserted between buses, but the nominal voltages of these buses do not need to be identical.

:::caution Caution!
The first selected bus will be the **primary** side, which is indicated by a circle. To change the primary side after insertion, simply disconnect the transformer nodes and reconnect them switching the buses, using the [CAD tools](cadTools#connection-and-disconnection-of-elements-in-the-bus).
:::

### Transformers in short-circuit studies
Just as in load flow studies, the representation of system elements for short-circuit studies is done through equivalent circuits inserted into the bus admittance matrix. In [unbalanced faults](fault) it is necessary to form three sequence admittance matrices: positive, negative, and zero.

The positive sequence admittance matrix is built the same way as in load flow studies. In the negative sequence, the phase shift angle between primary and secondary voltages ($\phi$) must be inverted, since negative sequence phasors **are displaced in the opposite direction**. Thus, the matrix representing the admittances in the previous equation must be replaced by the following expression:

$$
\begin{bmatrix}
\overline{Y}_{ii} & \overline{Y}_{ij}\\
\overline{Y}_{ji} & \overline{Y}_{jj}
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle \frac{\overline{y}_T}{t^2} & -\displaystyle \frac{\overline{y}_T}{\overline{t}}\\
-\displaystyle \frac{\overline{y}_T}{\overline{t}^*} & \overline{y}_T
\end{bmatrix}
$$

The negative sequence impedance in transformers must be treated specifically due to different types of connections. The figure below shows the equivalent circuits for each type of two-winding transformer connection. The arrows indicate possible paths for zero-sequence current flow.

<div><center><img src={useBaseUrl("images/seqZeroTrafo.svg")} alt="Zero sequence equivalent circuits of transformers" title="Zero sequence equivalent circuits of transformers" /></center></div>

For transformers with grounded wye connection, if the neutral-to-ground connection has an impedance $\overline{z}_n$, the zero sequence equivalent circuit must have an impedance of $3\overline{z}_n$ in series with the equivalent resistance and reactance of the transformer.

## Transformer Data Editing Form

The image below shows the data insertion/modification form for transformers:

<div><center><img src={useBaseUrl("images/trafoForm.png")} alt="Transformer form in PSP-UFU" title="Transformer form in PSP-UFU" /></center></div>

This form is subdivided into two distinct contexts:
- **General**: where general transformer information, load flow data, its connection, and phase shift are entered;
- **Fault**: where zero sequence impedances and grounding impedances are entered.

Besides these two contexts, the "Stability" button can be seen in the lower left of the form. This form, common to several other elements, allows insertion and/or removal of the transformer during the [stability](stability) study.

<div><center><img src={useBaseUrl("images/trafoSw.png")} alt="Transformer switching form" title="Transformer switching form" /></center></div>

<Tabs
  groupId="transformer-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Fault', value: 'fault'},
    {label: 'Stability Button', value: 'stability'},
  ]}
>
<TabItem value="general">

#### Name
Identification of the electrical element. Any number of characters can be entered in [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

All power components in PSP-UFU have this field.

#### Nominal voltage
*Non-editable* field showing the primary and secondary voltage of the transformer. To change this field, the corresponding field in the connected [bus](bus#name) must be edited.

#### Base voltage
Voltage used to convert the entered impedance parameters from $\Omega$ to $p.u.$ The voltages selected by a dropdown box are edited in the connected [bus](bus#name) forms.

:::info Information
Base voltage is ignored if the data entered is already in $p.u.$
:::

#### Rated power
Transformer rated power, entered in MVA, kVA, or VA.

This field is especially important if the option "Use rated power as base" is selected.

#### Transformer impedance
Transformer series impedance (resistance and inductive reactance), entered in $p.u.$ or $\Omega$. If impedances are entered in $\Omega$, the selected base voltage will be used for conversion to $p.u.$

#### Connection
Transformer connection used for zero sequence current calculation in unbalanced faults involving ground (phase-ground and phase-phase-ground).

The following connections are available:
1. $Y_{grounded}-Y_{grounded}$
2. $Y-Y_{grounded}$
3. $Y_{grounded}-Y$
4. $Y-Y$
5. $\Delta-Y_{grounded}$
6. $\Delta-Y$
7. $Y_{grounded}-\Delta$
8. $Y-\Delta$
9. $\Delta-\Delta$

#### TAP
Transformer tap **relative to the primary**.

:::caution Caution!
The tap in PSP-UFU must be entered as the ratio between the nominal primary voltage and the tap voltage $\left(\text{TAP} = \frac{V_{nominal}}{V_{tap}}\right)$.
:::

#### Phase shift
Transformer phase shift relative to the primary, in degrees. This value is independent of the connection type, so for connections other than $Y-Y$ (grounded or not) and $\Delta-\Delta$, the correct phase angle must be entered.

#### Use rated power as base
If this option is selected, the program will use the transformer's rated power as the base for unit conversion; otherwise, the [system base power](simulationConfig) will be used.

</TabItem>
<TabItem value="fault">

#### Zero sequence impedance of transformer
These parameters are necessary for the correct calculation of currents in [unbalanced faults](fault) (phase-phase, phase-phase-ground, and phase-ground) and must be entered in $p.u.$

:::warning Warning!
Failure to enter these data will result in incorrect results for [unbalanced faults](fault).
:::

#### Grounding impedances
Grounding impedances are only used for $Y_{grounded}$ connection and must be entered in $p.u.$

</TabItem>
<TabItem value="stability">

The "Stability" button will open a form, common to several other elements, which allows insertion and/or removal of the transformer during the [stability](stability) study.

In this form, a generic list of insertions and removals of the line over time can be created, customized by a switching property context where the switching type (insertion or removal) and the instant (in seconds) of the event are edited. These properties are added and removed from the generic list using the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>
