---
id: syncGenerator
title: Synchronous Generator
sidebar_label: Synchronous Generator
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

> An alternating current machine in which the frequency of the generated voltages and the speed of the machine are in a constant ratio. [IEC 60050](
http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-31-08).

## Synchronous Generator in PSP-UFU
Synchronous generators are the power source for load flow and stability calculations, and they are also one of the main contributors to fault currents.

:::caution Attention!
Only buses that have this element connected can be considered [reference buses](powerFlow). [PV buses](powerFlow) must contain either a synchronous generator or a [synchronous motor (synchronous condenser)](syncMotor).
:::

The data form is divided into two sections: the first for general, [load flow](powerFlow), and [fault](fault) data, and the second for [stability](stability) data. The latter also allows access to synchronous machine controls handled by the control editor.

:::info Information
The data regarding the **positive sequence impedances of the synchronous generator** entered in the [“Fault” tab](syncGenerator#sequence-impedances) are used **both for [short-circuit](fault) studies and for [harmonic](harmonics) studies**.

These data are ignored in [load flow](powerFlow) studies (*not used in this study*) and in [stability](stability) studies (*data entered in a specific stability form are used*).
:::

### Synchronous Generator in load flow
The synchronous generator is the power source in PSP-UFU’s load flow study. Its behavior differs depending on the type of bus connected:
- [Reference bus](powerFlow): The entered active and reactive power data are ignored, as this element will be used to complete the power balance in the load flow study;
- [PV bus](powerFlow): The active power data is considered, but the reactive power data is ignored. The reactive power value is used to keep the voltage magnitude constant at the connected bus;
- [PQ bus](powerFlow): For generators connected to this bus, both the entered active and reactive power values are considered.

:::caution Attention
If the reactive power limit is exceeded, the program automatically converts the connected [PV bus](powerFlow) into a [PQ bus](powerFlow), using the reactive power limit value that would have been exceeded.
:::

### Synchronous Generator in short-circuit study
While generators in the load flow study are modeled only by currents injected into the buses, for short-circuit analysis a voltage behind an impedance is used. The figure below shows the current path and equivalent circuit of each sequence in the generators.

<div><center><img src={useBaseUrl("images/sycGeneratorSeq.svg")} alt="Current paths and equivalent circuit: (a) positive sequence; (b) negative sequence; (c) zero sequence" title="Current paths and equivalent circuit: (a) positive sequence; (b) negative sequence; (c) zero sequence" /></center></div>

The generated voltages are only positive sequence, since the generator always supplies balanced three-phase voltages. Therefore, the positive sequence network consists of a pre-fault voltage behind a positive sequence impedance. The negative and zero sequence networks contain no electromotive forces but include the generator’s negative and zero sequence impedances.

The current flowing through the impedance $\overline{z}_n$ between neutral and ground is $3\dot{I}_{a0}$. From figure (c) above, it can be seen that the zero sequence voltage drop from point a to ground ($\dot{V}_{a0}$) is:

$$
\dot{V}_{a0} = -3\dot{I}_{a0}\overline{z}_n - \dot{I}_{a0}\overline{z}_{g0}
$$

The zero sequence network, which is a single-phase circuit assumed to carry only zero sequence current, must therefore have an impedance defined by the following equation:

$$
\overline{z}_{0} = 3\overline{z}_n + \overline{z}_{g0}
$$

:::tip Tip
If the generator is not grounded, no zero sequence current will flow through it. In this case, depending on the transformer connection near the ungrounded generator, the following error message may be displayed:

<p><center>"<i>Failed to invert zero sequence admittance matrix</i>"</center></p>

This happens because the zero sequence admittance matrix is singular. To work around this issue, choose **one** of the two solutions below:
1. Check the "Neutral grounded" option and enter a high grounding reactance value ($j9999~p.u.$, for example);
2. **Or**, at the generator bus, insert a [reactor](inductor) with a low reactive power value ($1.0~var$, for example).
:::

### Synchronous Generator in stability study
The relationship of values observed in proper tests (defined in [IEEE Std. 115-2019](https://doi.org/10.1109/IEEESTD.2020.9050934)), called standard parameters, are used to model the synchronous machine in PSP-UFU’s stability study.

Parameters of the synchronous machine that influence quickly the decay of values are called **subtransient** (denoted by $''$), those that influence more slowly are called **transient** (denoted by $'$), and those that influence continuously are called **synchronous** parameters (without superscript).

A set of algebraic-differential equations determines the synchronous machine’s behavior in the stability study:

$$
E_{q}' - V_q = r_aI_q - x_{ds}'Id\\
E_{d}' - V_d = r_aI_d - x_{qs}'Iq\\
E_{q}'' - V_q = r_aI_q - x_{ds}''Id\\
E_{d}'' - V_d = r_aI_d - x_{qs}''Iq\\
~\\
\frac{dE_{q}'}{dt} = \frac{V_{fd} + \left( x_d - x_{d}' \right)I_d - s_d E_{q}'}{T_{d0}'}\\
\frac{dE_{d}'}{dt} = \frac{- \left( x_q - x_{q}' \right)I_q - s_q E_{d}'}{T_{q0}'}\\

\frac{dE_{q}''}{dt} = \frac{s_dE_{q}' + \left( x_{d}' - x_{d}'' \right)I_d - s_d E_{q}''}{T_{d0}''}\\
\frac{dE_{d}''}{dt} = \frac{s_dE_{d}' + \left( x_{q}' - x_{q}'' \right)I_q - s_d E_{d}''}{T_{q0}''}\\
~\\
\frac{d\omega}{dt} = \frac{\omega_r}{2H} \left[ P_m - P_e - D_a \left( \omega - \omega_r \right) \right]\\
\frac{d\delta}{dt} = \Omega_b\left( \omega - \omega_r \right)
$$

The last two differential equations are the mechanical equations of the machine; the others are electrical equations (see this [thesis](https://dx.doi.org/10.14393/ufu.te.2019.2444) for more details on these parameters).

Using the transient and subtransient equations, five models of varying complexity can be defined.

:::caution Attention!
The synchronous machine model is **automatically selected** based on the data provided to the program.
:::

The following models are presented below, including magnetic saturation effects, along with their block diagrams:

- **Model 1**: Corresponds to a constant voltage behind a direct-axis transient reactance ($x_{d}'$), requiring no differential equations;
- **Model 2**: Represents direct-axis transient effects, requiring one differential equation ($\frac{dE_{q}'}{dt}$), block diagram shown below:

<div><center><img src={useBaseUrl("images/model2SyncGenerator.svg")} alt="Block diagram of Synchronous Machine Model 2" title="Block diagram of Synchronous Machine Model 2" /></center></div>

- **Model 3**: Represents direct and quadrature-axis transient effects, requiring two differential equations ($\frac{dE_{q}'}{dt}$ and $\frac{dE_{d}'}{dt}$), block diagram shown below:

<div><center><img src={useBaseUrl("images/model3SyncGenerator.svg")} alt="Block diagram of Synchronous Machine Model 3" title="Block diagram of Synchronous Machine Model 3" /></center></div>

- **Model 4**: Represents direct and quadrature-axis subtransient effects, requiring three differential equations ($\frac{dE_{q}'}{dt}$, $\frac{dE_{q}''}{dt}$, and $\frac{dE_{d}''}{dt}$), block diagram shown below:

<div><center><img src={useBaseUrl("images/model4SyncGenerator.svg")} alt="Block diagram of Synchronous Machine Model 4" title="Block diagram of Synchronous Machine Model 4" /></center></div>

- **Model 5**: Represents direct and quadrature-axis subtransient effects, requiring four differential equations ($\frac{dE_{q}'}{dt}$, $\frac{dE_{d}'}{dt}$, $\frac{dE_{q}''}{dt}$, and $\frac{dE_{d}''}{dt}$), block diagram shown below:

<div><center><img src={useBaseUrl("images/model5SyncGenerator.svg")} alt="Block diagram of Synchronous Machine Model 5" title="Block diagram of Synchronous Machine Model 5" /></center></div>

:::info Information
In all models, the mechanical differential equations are solved.
:::

#### Saturation
To mathematically represent the effect of saturation in synchronous machine equations, “saturation factors” are introduced that modify the impedances of the equivalent circuit, depending on an effective leakage reactance called Potier reactance ($x_p$).

This reactance can be obtained through tests (using open-circuit and zero power factor load saturation curves) or approximately estimated from other machine parameters. The leakage reactance ($x_l$), here approximated by $x_p$, represents the portion of the machine reactance originating from magnetic flux traveling mostly through air, and therefore independent of saturation.

The implemented method reproduces saturation in both axes (direct and quadrature), which differ due to air gap size differences. It is assumed that the vector sum of the two saturated flux components is in phase with the m.m.f. and proportional to the Potier Voltage ($E_p$, i.e., voltage behind Potier reactance).

Thus, two saturation factors are used internally: one in the direct axis ($s_d$) and another in the quadrature axis ($s_q$). These saturation factors are **automatically calculated** at each integration step and depend on the machine’s saturation curve defined by the [saturation factor entered in the data form](syncGenerator#saturation-factor).

Therefore, the saturated reactances, to be entered into the machine’s algebraic equations, are defined by the following equations:
$$
x_{ds}=\frac{x_d-x_p}{s_d +x_p}\\
x_{qs}=\frac{x_q-x_p}{s_q +x_p}
$$

These equations are also used for transient and subtransient reactances, since the Potier (or leakage) reactance value is not altered.

#### Infinite bus
Some references include a model without differential equations, where the machine is represented only by a constant voltage behind a direct-axis transient reactance. This is used to represent an **infinite bus**, which is usually a subsystem much larger than the one being simulated.

In PSP-UFU, an **infinite bus** can be represented by a machine modeled as Model 1 whose inertia constant (H) is infinite or much larger ($9999~s$, for example) compared to the rest of the system, and the value of $x_{d}'$ is very small ($10^{-3}~p.u.$, for example).

#### Center of inertia
The reference speed is usually taken as synchronous, so in this case $\omega_r = \omega_b = 1.0~p.u.$ This approach, adopted by many stability textbooks, considers as reference a fictitious machine always running at synchronous speed regardless of disturbances applied to the system. In PSP-UFU, the concept of Center of Inertia (COI) is implemented, which is a weighted sum of the speeds of the machines present in the system:

$$
\omega_r=\frac{\left( \sum_{i=1}^{n} H_i \omega_i \right)}{\left( \sum_{i=1}^{n} H_i \right)}
$$
Where:
$n$ is the number of **connected** synchronous machines in the system.

Applying the COI results in output data, such as rotor angle, that are easier to analyze. In the program’s implementation, the use of this feature is optional and can be [set by the user](simulationConfig).

## Synchronous generator data form
The image below shows the insertion/editing form for synchronous generator data:

<div><center><img src={useBaseUrl("images/syncGeneratorForm.png")} alt="Synchronous generator form in PSP-UFU" title="Synchronous generator form in PSP-UFU" /></center></div>

The second section contains stability data, as shown below, accessed by clicking the "Stability" button in the main form. Here you can also access synchronous machine controls managed by the [control editor](controlEditor).

<div><center><img src={useBaseUrl("images/syncGeneratorStabForm.png")} alt="Synchronous generator stability form in PSP-UFU" title="Synchronous generator stability form in PSP-UFU" /></center></div>

In the stability form, the "Switching" button can be seen in the lower left corner. This form, common to several other elements, allows inserting and/or removing the generator during the [stability](stability) study.

<div><center><img src={useBaseUrl("images/syncGeneratorSw.png")} alt="Synchronous generator switching form" title="Synchronous generator switching form" /></center></div>

<Tabs
  groupId="syncGenerator-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Fault', value: 'fault'},
    {label: 'Stability Button', value: 'stability'},
    {label: 'Switching Button', value: 'switching'},
  ]
}>
<TabItem value="general">

#### Name
Identification of the electrical element. Any number of characters can be entered in the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

All PSP-UFU power components have this field.

#### Rated power
Rated power of the generator, entered in MVA, kVA, or VA.

This field is especially important if the "Use nominal power as base" option is checked.

#### Active and reactive power
Active (entered in W, kW, MW, or p.u.) and reactive (entered in var, kvar, Mvar, or p.u.) power of the generator.

If the connected bus is PV, the reactive power value is ignored; if it is a reference bus, both values are ignored.

:::caution Attention!
If more than one generator is connected to the same bus, the reactive power (in reference and PV buses) and active power (in reference buses) values are equally distributed, respecting the individual reactive power limits.
:::

#### Maximum and minimum reactive power
Maximum and minimum reactive power limits of the generator for voltage control on PV buses. If these values are exceeded, the unit’s reactive power will be limited to the entered value and the connected bus will be converted to PQ, no longer controlling the set voltage.

#### Use nominal power as base
If this option is checked, the program will use the generator’s nominal power as the base for unit conversion, including those in the stability form; otherwise, the [system base power](simulationConfig) is used.

</TabItem>
<TabItem value="fault">

#### Sequence impedances
Resistance and reactance values for fault current calculations. Positive, negative, and zero sequence data are entered.

#### Grounding impedance
Values used for phase-to-ground and double-phase-to-ground fault calculations. If the generator neutral is not grounded, the value entered in this field is ignored.

#### Neutral grounded
Indicates whether the generator neutral is grounded.

</TabItem>
<TabItem value="stability">

#### Print synchronous machine data
Displays synchronous generator data in time-domain plots. The following data are displayed:
- terminal voltage
- active and reactive powers
- mechanical power
- frequency
- field voltage
- angle $\delta$.

#### Use AVR and speed governor
These fields enable or disable the synchronous machine controls, such as turbine and exciter models.

If selected, the controls can be inserted and modified via the “Edit AVR” and “Edit speed governor” buttons, which access the [PSP-UFU control editor](controlEditor).

#### Armature resistance
Synchronous machine armature resistance, in $p.u.$

#### Potier reactance
Potier reactance for saturation calculation, in $p.u.$

#### Saturation factor
Value used in the calculation of the saturation curve.

Represents the field current value (in $p.u.$) required to reach 1.2 p.u. terminal voltage.

:::caution Attention!
This value must be **greater than 1.2**, or it will cause simulation errors. If not specified, the machine saturation is not considered in calculations.
:::

#### Open-circuit frequency
Indicates the machine speed when starting the simulation disconnected from the grid.

:::info Information
This information is particularly useful for analyzing the connection of out-of-step generators to the grid.
:::

#### Synchronous reactances
Steady-state synchronous reactance values of the machine. Direct and quadrature-axis values should be equal or very close for cylindrical-rotor machines, while salient-pole machines have distinct values.

#### Transient reactances and time constants
Transient parameters of the synchronous machine in $p.u.$ or seconds.

:::warning Warning!
The direct-axis transient reactance value must be nonzero, or the program will produce an error.
:::

The number of entered parameters determines the model used internally by the program.

#### Subtransient reactances and time constants
Subtransient parameters of the synchronous machine in $p.u.$ or seconds, detailing the presence of damper windings. Like transient data, these parameters define the machine model.

</TabItem>
<TabItem value="switching">

The "Switching" button opens a form, common to several other elements, that allows inserting and/or removing the generator during the [stability](stability) study.

In this form, a generic list of line insertions and removals over time can be created, customized by switching property contexts where the switching type (insertion or removal) and the event time (in seconds) are edited. These properties are added and removed from the generic list via the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>

## Access to synchronous machine controls
As [mentioned earlier](syncGenerator#use-avr-and-speed-governor), the synchronous machine’s voltage and speed regulators can be enabled or disabled via the "[Use AVR and speed governor](syncGenerator#use-avr-and-speed-governor)" checkboxes. Both options will access the [control editor](controlEditor).

Access to **AVR** controls can then be created and manipulated by clicking the "Edit AVR" button, and the **Speed Governor** is accessed via the "Edit speed governor" button.

:::caution Attention!
In PSP-UFU, the option to edit the **AVR** covers more than just the machine’s voltage control. It **must** include the synchronous machine’s control loop as well as the **synchronous machine exciter**. Optional control strategies, such as PSS (*Power System Stabilizer*) and/or over- and under-excitation controls, are also implemented together.
:::

:::caution Attention!
As with the AVR, the **Speed Governor** covers more than just primary speed regulation. This option **must** include at least the primary speed control loop as well as the **turbine model**. Optional speed control strategies are also inserted here.
:::

## References
1. MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6
2. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286
3. KUNDUR, P. Power System Stability and Control. McGraw-Hill, New York, 1994.
4. DOMMEL, H. W.; SATO, N. Fast Transient Stability Solutions. IEEE Transactions on Power Apparatus and Systems, v. PAS-91, n. 4, Jul 1972, p. 1643-1650. doi: https://doi.org/10.1109/TPAS.1972.293341
5. IEEE Std 1110-2002 IEEE Guide for Synchronous Generator Modeling Practices and Applications in Power System Stability Analyses. IEEE, New York, Nov. 2003. doi: https://doi.org/10.1109/IEEESTD.2003.94408
6. KIMBARK, E. W. Power System Stability: Volume III – Synchronous Machine. New York: Wiley-IEEE Press, 1995.
