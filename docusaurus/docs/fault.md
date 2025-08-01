---
id: fault
title: Short-Circuit
sidebar_label: Short-Circuit
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

The main objective of short-circuit analysis is to calculate fault currents and voltages for specifying circuit breakers, current transformers, and setting protection relays.  
About 70–80% of transmission line faults are single-phase-to-ground faults, occurring due to arcing from a single line phase to the tower and then to the ground.  
The smallest number of faults, about 5%, involve all three phases (three-phase faults). Other fault types involve two phases or two phases to ground.

All these faults, except the three-phase fault, are asymmetrical and cause phase unbalance.

## Short-circuit calculation in PSP-UFU
The first stage of short-circuit calculation is determining pre-fault voltages, system generation powers, and loads.  
These data are obtained from the [load flow](powerFlow) study.

:::info Information
Currently, PSP-UFU provides results for the following fault types:
- Three-phase-to-ground (3F-G);
- Single-phase-to-ground (1F-G);
- Phase-to-phase (F-F);
- Phase-to-phase-to-ground (F-F-G).
:::

The models of the power system elements for short-circuit studies are similar to those in load flow, with some differences for unbalanced faults (1F-G, F-F, F-F-G).

The most frequent faults in power systems are asymmetrical.  
Since any asymmetrical fault causes unbalanced current flow, it is necessary to use the symmetrical components method.  
This method allows the study of balanced systems with unbalanced loads.

## Running short-circuit calculation in PSP-UFU
There are two ways to calculate short-circuits in PSP-UFU:
- **Fault**: Calculates the fault inserted at [buses](bus). This type of calculation allows computing *shunt* faults (balanced or unbalanced) at [buses](bus).
- **Short-circuit level**: Calculates the short-circuit level (three-phase fault) at all [buses](bus) in the system.

After building the single-line diagram in the [power editor](powerEditor), the short-circuit calculation is executed in the [Simulation menu](mainScreen#ribbon-menu) by clicking the **Fault** button.  
To calculate the short-circuit level (three-phase fault) at all [buses](bus), click **Short-circuit level**.

<div><center><img src={useBaseUrl("images/menuSimulationFaulta.svg")} alt="Short-circuit calculation execution" title="Short-circuit calculation execution" /></center></div>

:::caution Attention
It is possible to calculate faults without running the [load flow](powerFlow), but **it is not recommended**, since fault current values are significantly affected.
:::

**Another possibility** is execution via continuous calculation, also available in the [Simulation menu](mainScreen#ribbon-menu) by clicking **Enable solution**.  
With this option, the static calculations selected in [simulation settings](simulationConfig) are automatically performed whenever network parameters change, such as electrical data or breaker operations (element insertion/removal).

:::warning Warning!
Short-circuit calculations are **not enabled by default** in continuous calculation and must be selected in [simulation settings](simulationConfig).
:::

The short-circuit calculation results are displayed in [linked text elements](text), when hovering the mouse over buses, and in [tabular reports](tabularReport).

### Common errors in short-circuit calculation
Below are the most common errors related to short-circuit calculation.

#### Error message: "Failed to invert zero-sequence admittance matrix"
- **Zero-sequence current cannot flow.**  
If the generator is not grounded, zero-sequence current will not flow through it.  
Depending on the connection of the transformer near the ungrounded generator, the zero-sequence admittance matrix may be singular.  
To solve this issue, choose one of the following:
	- Check the "Grounded neutral" option and insert a high grounding reactance value ($j9999~p.u.$, for example);
	- Or insert a low reactive power reactor ($1{,}0~var$, for example) at the generator bus.

## Short-circuit equations
As mentioned earlier, the most frequent faults are asymmetrical.  
Since any asymmetrical fault causes unbalanced current flow, the symmetrical components method is required.  
This method allows studying balanced systems with unbalanced loads.

### Symmetrical components method
This method, proposed by C. L. Fortescue, allows defining a system of n unbalanced phasors as n−1 systems of n balanced phases plus one zero-sequence system.  
The zero-sequence system is defined by all phases having the same magnitude and angle.

For a three-phase system, three sequence components can be defined:
1. Positive-sequence components: three phasors equal in magnitude, 120° apart, with the same phase sequence as the original phasors;
2. Negative-sequence components: three phasors equal in magnitude, 120° apart, with the opposite phase sequence;
3. Zero-sequence components: three phasors equal in magnitude and with zero phase shift.

The phase voltages can be decomposed into symmetrical components using the following equations:
$$
\begin{cases}
\dot{V}_a = \dot{V}_{a1} + \dot{V}_{a2} + \dot{V}_{a0}\\
\dot{V}_b = \dot{V}_{b1} + \dot{V}_{b2} + \dot{V}_{b0}\\
\dot{V}_c = \dot{V}_{c1} + \dot{V}_{c2} + \dot{V}_{c0}
\end{cases}
$$

The figure below shows an example of symmetrical components and their sum to obtain the unbalanced phasors.

<div><center><img src={useBaseUrl("images/fortescue.svg")} alt="Example of symmetrical components" title="Example of symmetrical components" /></center></div>

To simplify calculations, the operator “$\overline{a}$” is adopted to indicate phasor rotation.  
This operator is a complex number with unit magnitude and 120° angle:
$$
\overline{a} = 1 \angle 120^{\circ} = 1 e^{j2\pi/3} = -0{,}5 + j0{,}866
$$

Using the previous voltage equations with the operator “$\overline{a}$”, we can build the following matrix equation:
$$
\begin{bmatrix}
\dot{V}_a\\
\dot{V}_b\\
\dot{V}_c
\end{bmatrix}
=
\overbrace{
\begin{bmatrix}
1 & 1 & 1\\
1 & \overline{a}^2 & \overline{a}\\
1 & \overline{a} & \overline{a}^2
\end{bmatrix}
}^{\left[ \bold{A} \right]}
\begin{bmatrix}
\dot{V}_{a0}\\
\dot{V}_{a1}\\
\dot{V}_{a2}
\end{bmatrix}
$$

By pre-multiplying both sides by $\left[ \bold{A} \right]^{-1}$, the symmetrical components can be obtained.

Similar to the load flow study, the representation of system elements for short-circuit study is made using equivalent circuits in the bus admittance matrix.  
For unbalanced faults (1F-G, F-F, F-F-G), three sequence admittance matrices are required: positive, negative, and zero.

:::info Information
Details about specific element models for short-circuit studies are presented individually in the [power elements](powerEditor#editando-dados-elétricos).
:::

### Short-circuit equations
First, balanced fault equations are addressed, then extended to unbalanced faults using the symmetrical components method.

#### Balanced faults
The bus impedance matrix is used for short-circuit calculation, defined by:
$$
[\dot{V}] = [Z_{bus}][\dot{I}]
$$
Where:
- $[Z_{bus}]$ = Inverse of the bus admittance matrix (bus impedance matrix).

Expanding this equation allows calculating the three-phase fault current at a generic bus $i$:
$$
\dot{I}_f = \frac{\dot{E}_i}{\overline{z}_{ii}+\overline{z}_{f}}
$$
Where:
- $\dot{I}_f$ = three-phase fault current at bus $i$
- $\dot{E}_i$ = pre-fault voltage at bus $i$
- $\overline{z}_{ii}$ = Thevenin equivalent impedance seen from bus $i$
- $\overline{z}_{f}$ = fault impedance

#### Unbalanced faults
The short-circuit calculation for unbalanced faults follows this procedure:
1. Define the diagrams at the fault point, showing phase connections.  
   Assume only balanced impedances are present on both sides, and the Thevenin equivalent up to the point is known;
2. Write boundary conditions relating known voltages and currents for the studied fault;
3. Transform currents and voltages from a-b-c to 0-1-2 coordinates;
4. Find the fault current using the following system of equations (for phase A):
	$$
	\begin{cases}
	\dot{V}_{a1} = \dot{E}_a - \dot{I}_{a1} \overline{z}_1\\
	\dot{V}_{a2} = - \dot{I}_{a2} \overline{z}_2\\
	\dot{V}_{a0} = - \dot{I}_{a0} \overline{z}_0
	\end{cases}
	$$

The table below shows the equations for unbalanced faults after executing the procedure:

|**Fault**|**Positive Seq. ($\dot{I}_{f}^{1}$)**|**Negative Seq. ($\dot{I}_{f}^{2}$)**|**Zero Seq. ($\dot{I}_{f}^{0}$)**|
|-----|:-----------:|:-----------:|:-------:|
|**1F-G**| $\dfrac{\dot{E}_i}{\overline{z}_{ii}^{1} + \overline{z}_{ii}^{2} + \overline{z}_{ii}^{0} + 3 \overline{z}_{f}}$ | $\dot{I}_{f}^{1}$ | $\dot{I}_{f}^{1}$|
|**F-F**| $\dfrac{\dot{E}_i}{\overline{z}_{ii}^{1} + \overline{z}_{ii}^{2} + \overline{z}_{f}}$ | $- \dot{I}_{f}^{1}$ | $0{,}0$|
|**F-F-G**| $\dfrac{\dot{E}_i \left( \overline{z}_{ii}^{2} + \overline{z}_{ii}^{0} + 3 \overline{z}_{f} \right)}{\overline{z}_{ii}^{1} \overline{z}_{ii}^{2} + 3 \overline{z}_{ii}^{2} \overline{z}_{f} + \overline{z}_{ii}^{2} \overline{z}_{ii}^{0} + 3 \overline{z}_{ii}^{1} \overline{z}_{f} + \overline{z}_{ii}^{1} \overline{z}_{ii}^{0} }$ | $- \dfrac{\dot{E}_i - \overline{z}_{ii}^{1} \dot{I}_{f}^{1}}{\overline{z}_{ii}^{2}}$ | $- \dfrac{\dot{E}_i - \overline{z}_{ii}^{1} \dot{I}_{f}^{1}}{\overline{z}_{ii}^{0} + 3 \overline{z}_{f}}$|

To obtain values in a-b-c, the previously shown matrix equation is used, completing the short-circuit calculation.

## References
1. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286  
2. STEVENSON JR.; WILLIAN, D. *Power System Analysis Elements*. 2nd ed. São Paulo: McGraw-Hill, 1986.  
3. ANDERSON, P. M.; FOUAD, A. A. *Power System Control and Stability*. Wiley-IEEE Press, 2002. doi: https://doi.org/10.1109/9780470545577  
4. FORTESCUE, C. L. *Method of Symmetrical Coordinates Applied to the Solution of Polyphase Networks*. Trans. AIEE, v.37, p.1027-1140, 1918. doi: https://doi.org/10.1109/T-AIEE.1918.4765570  
5. ANDERSON, P. M. *Analysis of Faulted Power Systems*. IEEE Press, 1995.
