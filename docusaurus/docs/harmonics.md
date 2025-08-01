---
id: harmonics
title: Harmonics
sidebar_label: Harmonics
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

Two harmonic-related tools were developed in PSP-UFU:
- Total Harmonic Distortion of Voltage (THD);
- Frequency Response.

The **[Total Harmonic Distortion](https://en.wikipedia.org/wiki/Total_harmonic_distortion)** tool calculates the harmonic voltages caused by [harmonic current sources](harmSource), as well as the THD of all buses in the system.

The **Frequency Response** tool (or frequency sweep analysis) involves varying the network impedance over a frequency spectrum observed from a certain bus.

:::info Information
Frequency sweep analysis is widely used in harmonic filter design.
:::

:::tip Tip
A series arrangement of a [line](line) without *shunt* susceptances, a [bus](bus), and a [capacitor](capacitor) can be used to create a passive filter in [harmonic studies](harmonics). In future versions, a passive filter element will be implemented in PSP-UFU.
:::

:::caution Attention!
In the current version, changes in system resistance caused by skin effect are not considered. Future versions will include this feature.
:::

## Running the harmonic calculation in PSP-UFU
After building the single-line diagram in the [power editor](powerEditor), the harmonic distortion calculation is executed in the [Simulation menu](mainScreen#ribbon-menu) by clicking the **Harmonic Distortions** button.  
To access the frequency sweep tool, click the **Frequency Response** button.

<div><center><img src={useBaseUrl("images/menuSimulationHamonics.svg")} alt="Access to harmonic study tools" title="Access to harmonic study tools" /></center></div>

### Harmonic Distortions
Clicking the "Harmonic Distortions" button calculates the distortions caused by [harmonic current sources](harmSource) in all system buses.

:::caution Attention!
If no [harmonic current sources](harmSource) are inserted into the power system, the voltage distortion of all buses will be $0{,}0~\%$.
:::

**Another possibility** is execution via continuous calculation, also available in the [Simulation menu](mainScreen#ribbon-menu) by clicking **Enable solution**.  
With this option, the selected static calculations in [simulation settings](simulationConfig) are automatically executed when any network parameter is modified, such as electrical data or breaker operations (insertion/removal of elements).

:::warning Warning!
THD level calculations are **not enabled by default** in continuous calculation and must be selected in the [simulation settings](simulationConfig).
:::

The harmonic distortion results are displayed in [linked text elements](text) and when hovering the mouse over a bus.

### Frequency Response
Clicking the "Frequency Response" button opens a form to enter tool parameters:

<div><center><img src={useBaseUrl("images/injHarmCurrent.png")} alt="Frequency response tool access" title="Frequency response tool access" /></center></div>

#### Initial frequency
Defines the initial sweep frequency.

#### Final frequency
Defines the final sweep frequency.

#### Frequency step
Defines the step increment of the frequency.  
Smaller steps generate more precise plots but may increase execution time.

#### Injected current
Defines the bus where the harmonic current will be injected for analysis.
:::tip Tip
This field can be interpreted as:  
"*The response of the harmonic impedances seen by the system buses if a harmonic current source exists at the selected bus*".
:::

#### Execute button
Clicking the execute button performs the sweep and displays results in the [graph viewer](graphViewer), as shown below:

<div><center><img src={useBaseUrl("images/freqScanPlot.png")} alt="Frequency response example" title="Frequency response example" /></center></div>

:::warning Warning!
To view the harmonic impedance sweep of a bus, this option must be enabled in its [data edit form](bus#imprimir-impedância-harmônica-da-barra).
:::

## Harmonic tool calculations

Both calculations are based on the following matrix equation:
$$
[\dot{I}]^h= [Y_{bus}]^h [\dot{V}]^h
$$
Where:
- $[\dot{I}]^h$ = Vector of harmonic currents injected into buses
- $[Y_{bus}]^h$ = Harmonic bus admittance matrix
- $[\dot{V}]^h$ = Vector of harmonic voltages at buses
- $h$ = Harmonic order

The harmonic current vector is defined in the program using the “harmonic current source” element, where the user can create a list of currents (in A and/or $p.u.$) injected into a bus.

The program automatically defines harmonic orders in the simulation by analyzing all harmonic current source lists before calculations.  
Harmonic admittances are calculated by multiplying the fundamental inductive reactances ($x_{l}^1$) and capacitive susceptances ($b_{c}^1$) of each element by the harmonic orders:
$$
x_{l}^h=h×x_l^1\\
b_{c}^h=h×b_c^1
$$

Once harmonic admittances are calculated, the same conventional models and algorithms are used to construct $Y_{bus}^h$.

The matrix equation is solved using Gaussian elimination to avoid inverting each harmonic admittance matrix, making the calculation computationally efficient.

Once harmonic voltages are calculated, the THD at a generic bus $i$ can be defined as:
$$
\text{THD}_i=\dfrac{\sum_{h=2}^{n} V_{i}^h}{V_i^1}
$$

In the Frequency Response tool, a sinusoidal current is injected into the bus across a frequency range, and the matrix equations are used to calculate the voltage response. This is repeated at discrete steps over the specified spectrum.

The program uses the positive-sequence admittance matrix and a $1{,}0 p.u.$ positive-sequence injected current to calculate impedances directly in p.u.

## References
1. ARRILLAGA, J.; WATSON, N. R. Power System Harmonics. John Wiley & Sons; Chichester, 2003. doi: https://doi.org/10.1002/0470871229
