---
id: indMotor
title: Induction Motor
sidebar_label: Induction Motor
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

> An asynchronous machine of which only one winding is energized. [IEC 60050](http://www.electropedia.org/iev/iev.nsf/display?openform&ievref=411-31-10).

## Three-phase induction motor in PSP-UFU
In PSP-UFU, induction motors are included in [load flow](powerFlow.md) and [stability](stability) studies.

:::info Information
The stability data of the induction machine are used together with the load flow, **correctly calculating the reactive power of the machines** and consequently the voltage at the connected bus.
:::

:::warning Warning!
When selecting the option "Calculate reactive power in load flow", the correct data must be entered in the "Stability" tab, otherwise an incorrect reactive power will be assigned to the motor.
:::

### Three-phase induction motor in load flow calculation
The figure below shows the model of a simple cage three-phase induction motor (IM).

<div><center><img src={useBaseUrl("images/indMotorModel.svg")} alt="Equivalent circuit of the induction motor" title="Equivalent circuit of the induction motor" /></center></div>

The active and reactive powers can be calculated in relation to the motor variables and parameters in $p.u.$ as:

$$
P = \frac{V^2 \left\{ \left( \frac{r_2}{s} \right) \left[\left(\frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]+K_1 \left[\left(\frac{r_2}{s} \right) \left(x_m+x_1 \right) + r_1 K_1 \right] \right\} }{\left[\left( \frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]^2+\left[\left( \frac{r_2}{s} \right)(x_m+x_1 )+r_1 K_1 \right]^2}\\
Q = \frac{-V^2 \left\{ K_1 \left[\left(\frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]- \left( \frac{r_2}{s} \right) \left[\left(\frac{r_2}{s} \right) \left(x_m+x_1 \right) + r_1 K_1 \right] \right\} }{\left[\left( \frac{r_2}{s} \right) r_1-x_1 K_1-x_2 x_m \right]^2+\left[\left( \frac{r_2}{s} \right)(x_m+x_1 )+r_1 K_1 \right]^2}\\
~\\
K_1=x_2+x_m
$$

As seen in the above equations, there are four variables and only two equations. In practice, the variables can be reduced to three since the voltage magnitude (V) is obtained from the [power flow](powerFlow.md) results.  
To solve the equations, one more variable must be defined. In PSP-UFU, the chosen fixed variable is active power (P), as it provides numerically correct and suitable results for motors under stability conditions.

Thus, in this static model, active power is kept constant during load flow calculation, and slip (s) is updated at each iteration. The active power equation can be rewritten in terms of slip:

$$
\left(\frac{r_2}{s} \right)^2 A + \left(\frac{r_2}{s} \right) B + C = 0
$$
Where:
$$
A = P \left( r_1^2 + K_3^2 \right) - V^2 r_1\\
B = 2P(r_1 K_2 + K_3 K_4) - V^2 \left(K_2 + K_1 K_3 \right)\\
C = P \left( K_2^2 + K_4^2 \right) - V^2 K_1 K_4\\
K_2 = -x_1 K_1 -x_2 x_m\\
K_3 = x_m + x_1\\
K_4 = r_1 K_1
$$

This model can be inserted into the [load flow](powerFlow.md) solution by following these steps:

1. Calculate constants $K_1$ to $K_4$. These values remain constant throughout the solution;
2. In each iteration, calculate coefficients $A$, $B$, and $C$ using the updated value of $V$;
3. Solve the quadratic equation to obtain two values of $\left(\frac{r_2}{s} \right)$; choose the larger one as it is in the stable region of the torque-slip characteristic;
4. Using the new $\left(\frac{r_2}{s} \right)$ value, obtain the reactive power ($Q$). The power vector is updated and conventional load flow solution steps are executed.

Steps 2 to 4 are repeated until convergence is achieved.

:::caution Attention!
In PSP-UFU, induction motors are not considered in short-circuit calculations.
:::

### Three-phase induction motor in stability study
An important dynamic load is the induction motor since it accounts for a significant share of system loads.  
The [induction machine model presented earlier](indMotor#three-phase-induction-motor-in-load-flow-calculation), which can operate as both motor and generator, is well established in the literature.

As described in the [previous section](indMotor#three-phase-induction-motor-in-load-flow-calculation), initialization of this machine is done together with the power flow since the reactive power required by the induction machine depends on motor parameters as well as bus voltage. This approach is necessary because conventional methods lead to incorrect results in heavily loaded systems.

The induction machine motion equation must be expressed in terms of torque, not power, as done with synchronous machines. Rotor symmetry means angular position is not important, and slip ($s$) is used instead of speed ($\omega$), where:

$$
s = \frac{\Omega_0 - \omega}{\Omega_0}
$$

Neglecting friction and windage losses and shaft power, the mechanical equations of the motor are expressed as:

$$
T_m = A -Bs + Cs^2\\
T_e = \frac{Re\left\{ \dot{E}\dot{I}^* \right\}}{\Omega_b}\\
\frac{ds}{dt} = \frac{\left( T_m - T_e \right)}{2H}
$$
Where:
- $T_m$ = mechanical torque
- $T_e$ = electrical torque
- $H$ = inertia of the motor–mechanical load set

The terms $A$, $B$, and $C$ define the load mechanical torque behavior according to slip. Mechanical torque normally varies with **speed**, which can be expressed proportionally by the quadratic equation:

$$
T_m \propto a + b\omega + c\omega^2
$$
Where:
$$
\begin{cases}
	A \propto a + b + c \\
	B \propto b + 2c \\
	C \propto c
\end{cases}
$$

The electrical equations of the simple cage induction motor are based on the equivalent circuit from the [previous figure](indMotor#three-phase-induction-motor-in-load-flow-calculation).  
Similarly to the synchronous machine transient model, the induction motor can be represented by the Thevenin equivalent circuit of transient voltage $E'$ behind a stator resistance $r_1$ and transient reactance $x'$. The blocked-rotor apparent transient reactance is:
$$
x' = x_1 + \frac{x_2 x_m}{x_2 + x_m}
$$

The rotor open-circuit time constant ($T_0'$) is:
$$
T_0' = \frac{x_2 + x_m}{\Omega_b r_2}
$$

And the open-circuit reactance is:
$$
x_0 = x_1 + x_m
$$

Since the reactances are unaffected by rotor position, the DAEs of the induction motor can be expressed directly in real ($r$) and imaginary ($m$) components. The complete model is thus represented by the following system of differential-algebraic equations:
$$
V_r - E_r' = r_1 I_r - x' I_m\\
V_m - E_m' = r_1 I_m - x' I_m\\
\frac{dE_r'}{dt} = \Omega_b s E_m' - \frac{E_r' + \left( x_0 - x' \right) I_m}{T_0'}\\
\frac{dE_m'}{dt} = \Omega_b s E_r' - \frac{E_m' + \left( x_0 - x' \right) I_r}{T_0'}
$$

## Induction motor data editing form
The figure below shows the data insertion/editing form for induction motors:

<div><center><img src={useBaseUrl("images/indMotorForm.png")} alt="Induction motor form in PSP-UFU" title="Induction motor form in PSP-UFU" /></center></div>

The form includes the "Switching" button in the lower-left corner.  
This form, common to several other elements, allows the insertion and/or removal of the motor during the [stability](stability) study.

<div><center><img src={useBaseUrl("images/indMotorSw.png")} alt="Induction motor switching form" title="Induction motor switching form" /></center></div>

<Tabs
  groupId="indMotor-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Stability', value: 'stability'},
    {label: 'Switching Button', value: 'switching'},
  ]
}>
<TabItem value="general">

#### Name
Electrical element identification. Any number of characters can be entered using the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

All PSP-UFU power components have this field.

#### Rated power
Motor rated power, entered in MVA, kVA, or VA.

This field is especially important if the "Use rated power as base" option is checked.

#### Active and reactive powers
Motor active power (entered in W, kW, MW, or $p.u.$) and reactive power (entered in var, kvar, Mvar, or $p.u.$).

:::caution Attention!
If the “Calculate reactive power in load flow” option is enabled, the reactive power field is disabled for editing.
:::

#### Calculate reactive power in load flow
If selected, the program uses the stability form data to calculate the motor’s reactive power during the iterative load flow process.

:::warning Warning!
If this option is not used, the motor will be considered a [constant power load](load) in the [load flow](powerFlow.md) study.

Not using this option may cause steady-state errors in the [stability](stability) study.
:::

#### Use rated power as base
If selected, the program uses the motor’s rated power as the base for unit conversion, including those in the stability form; otherwise, the system base power is used.

</TabItem>
<TabItem value="stability">

#### Print induction machine data
Displays induction motor data in time-domain plots. The following data are shown:
- terminal voltage
- active and reactive powers
- current, electrical torque
- mechanical torque
- speed
- slip

#### Inertia
Inertia of the motor-load set, in seconds.

#### Stator resistance and reactance
Stator resistance and reactance of the induction motor, in $p.u.$

#### Rotor resistance and reactance
Rotor resistance and reactance of the induction motor, in $p.u.$

#### Magnetizing reactance
Magnetizing branch reactance of the induction motor, in $p.u.$

#### Use cage factor
Allows inserting a cage factor ($K_g$) in the induction motor.  
This factor increases rotor resistance under high slip conditions, as in double-cage or deep-bar motors. Rotor resistance follows:
$$
r_2 = r_{2_0} \left( 1+K_g s \right)
$$
Where:
- $r_{2_0}$ = rotor resistance entered in the form.

#### Load characteristic
Describes the mechanical load torque characteristic on the motor shaft according to the mechanical torque equation:
$$
T_m \propto \bold{a} + \bold{b}\omega + \bold{c}\omega^2
$$

:::warning Warning!
The sum of the coefficients ($a + b + c$) must be equal to one.
:::

</TabItem>
<TabItem value="switching">

The "Switching" button opens a form, common to several other elements, allowing insertion and/or removal of the motor during the [stability](stability) study.

In this form, a generic list of insertions and removals over time can be created, customized by a switching properties context where the switching type (insertion or removal) and event time (in seconds) are edited. These properties are added or removed from the list using the "Add" and "Remove" buttons, respectively.

</TabItem>
</Tabs>

## References
1. SÁNCHEZ, J. C.; OLIVARES, T. I. A.; ORTIZ, G. R.; VEGA, D. R. Induction Motor Static Models for Power Flow and Voltage Stability Studies. In: IEEE Power and Energy Society General Meeting, 2012, San Diego. doi: https://doi.org/10.1109/PESGM.2012.6345618  
2. IEEE Std 399-1997. IEEE Recommended Practice for Industrial and Commercial Power Systems Analysis (Brown Book). IEEE, New York, Aug. 1998. doi: https://doi.org/10.1109/IEEESTD.1998.88568  
3. MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6  
4. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286
