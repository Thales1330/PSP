---
id: simulationConfig
title: Simulation Settings
sidebar_label: Simulation Settings
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

Simulation parameterizations and settings are accessed in the [Simulation menu](mainScreen#ribbon-menu) by clicking the **Simulation Settings** button icon.

<div><center><img src={useBaseUrl("images/menuSimulationSettings.svg")} alt="Simulation Menu" title="Simulation Menu" /></center></div>

## Simulation Settings Edit Form

The image below shows the simulation settings edit form:

<div><center><img src={useBaseUrl("images/simulationConfigForm.png")} alt="Simulation Settings" title="Simulation Settings" /></center></div>

This form is subdivided into four distinct contexts:
- **General**: where general and common information to various simulations is entered;
- **Power Flow**: where the [numerical solution method](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu) is selected, as well as its parameters;
- **Stability**: containing time simulation options and parameters of the [integration method](stability#integração-numérica);
- **ZIP Load**: contains general options of [ZIP loads](load#carga-no-estudo-de-estabilidade) in the [stability](stability) study.

---
<Tabs
  groupId="simulationConfigs-tabs"
  defaultValue="general"
  values={[
    {label: 'General', value: 'general'},
    {label: 'Power Flow', value: 'powerFlow'},
    {label: 'Stability', value: 'stability'},
    {label: 'ZIP Load', value: 'zipLoad'},
  ]
}>
<TabItem value="general">

#### Base Power
Base power of the system used for conversion of real data into $p.u.$ and vice versa. Can be entered in VA, kVA, or MVA.

:::tip Tip
The base power of each element can be different from the system base power. To do this, just check the option "Use nominal power \[*of the element*\] as base," present in their respective [data edit forms](powerEditor).
:::

#### System Frequency
Defines the nominal frequency of the system.

:::warning Warning!
When changing the nominal frequency, pay attention to the "[Open Circuit Frequency](syncGenerator#frequência-de-circuito-aberto)" field of synchronous machines.
:::

#### Continuous Calculation
Enables or disables continuous calculation for [short circuit](fault), [short circuit level](fault), and [harmonic distortions](harmonics) calculations. Continuous calculation for [power flow](powerFlow) is always enabled.

:::tip Tip
To enable continuous calculation, press the **Enable Solution** button present in the [Simulation menu](mainScreen#ribbon-menu).

With this option, the selected static calculations are automatically performed when modifying any network parameters, such as electrical data and breaker operation (removal or insertion).
:::

</TabItem>
<TabItem value="powerFlow">

#### Solution Method
Defines the numerical solution method for the [power flow](powerFlow) study. Currently, the PSP-UFU includes the following methods:
- [Gauss-Seidel](powerFlow#gauss-seidel)
- [Newton-Raphson](powerFlow#newton-raphson)
- [Hybrid Gauss-Newton](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu)

#### Acceleration Factor
Factor used to speed up convergence in Gauss methods. The use of the acceleration factor is done through the equation:
$$
\dot{V}_{i_{AC}}^{(v + 1)} = \alpha \left( \dot{V}_{i}^{(v + 1)}- \dot{V}_{i_{AC}}^{(v)} \right) + \dot{V}_{i_{AC}}^{(v)}
$$
Where:
- $\dot{V}_{i_{AC}}^{(v + 1)}$ is the complex voltage of the current iteration with acceleration factor applied
- $\dot{V}_{i}^{(v + 1)}$ is the complex voltage of the current iteration
- $\dot{V}_{i_{AC}}^{(v)}$ is the complex voltage of the previous iteration with acceleration factor applied
- $\alpha$ is the acceleration factor

This option is enabled only for the [Gauss-Seidel](powerFlow#gauss-seidel) and [Hybrid Gauss-Newton](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu) methods.

#### Tolerance
It is the error value between iterations (in $p.u.$) at which the numerical method convergence is reached.

For the [Gauss-Seidel](powerFlow#gauss-seidel) method, the error is calculated by the largest magnitude of the difference between voltages in the previous and current iteration ($e = \left| V_{i}^{(v + 1)} - V_{i}^{(v)} \right|$); while for the [Newton-Raphson](powerFlow#newton-raphson) method, this error is the magnitude of the largest power adjustment ($|\Delta P|$ or $|\Delta Q|$).

#### Max Iterations
Maximum number of iterations a method can reach. If this value is reached, the [power flow](powerFlow) calculation is stopped with an error.

#### Reference Bus Angle
The value of the angle of the [reference bus](powerFlow), in degrees.

#### Newton Inertia
Value that multiplies the power corrections. Values greater than $1.0$ increase the correction, and values less than $1.0$ decrease the correction.

:::info Information
Values less than $1.0$ *may* help convergence, but increase the number of iterations and consequently the processing time of the method.
:::

This option is enabled only for the [Newton-Raphson](powerFlow#newton-raphson) method.

#### Gauss Tolerance
Defines the tolerance of the Gauss-Seidel for the hybrid method. This option is enabled only for the [Hybrid Gauss-Newton](powerFlow#métodos-de-solução-numérica-do-fluxo-de-carga-no-psp-ufu) method.

Since Newton-Raphson is considerably sensitive to initial conditions, the first iterations are calculated using Gauss-Seidel until the error is less than the tolerance specified in the “Gauss Tolerance” field. From that point, the calculation is done using the Newton-Raphson method until convergence is achieved with an error smaller than the tolerance stipulated in the “Tolerance” field.

:::info Information
This option helps convergence of systems that cannot be solved using conventional Newton-Raphson.
:::

</TabItem>
<TabItem value="stability">

#### Integration Step
Defines the integration step for the [Implicit Trapezoidal](stability#integração-numérica) integration method.

:::caution Caution!
Very high values may generate simulation errors, while very small values will significantly increase the processing time of the stability calculation.
:::

#### Simulation Time
Defines the total simulation time, in seconds.

#### Tolerance
Tolerance of the iterative process (in $p.u.$) of the [Implicit Trapezoidal](stability#integração-numérica) integration method.

#### Max Iterations
Maximum number of iterations of the iterative process of the [Implicit Trapezoidal](stability#integração-numérica) integration method. If the number of iterations exceeds this value, the stability calculation will be stopped with an error.

#### Control Step Ratio
Defines the number of integration steps the control system will undergo for each integration step of the power system.

For example, if the integration step is set to $0.01~s$ and the step ratio is $10$, the effective integration step for the control systems will be $0.001~s$ (10 times smaller).

:::info Information
The use of this field is justified in situations of large differences in the time constants of synchronous machines (usually larger) and control systems (usually smaller).
:::

:::tip Tip
A control step ratio value of $1$ will not alter the control system's integration step.
:::

#### Printing Time
Interval at which the program will store data for printing in [time graphs](graphViewer).

:::warning Warning!
Very small values (less than $0.001~s$) may significantly increase the processing time of the stability calculation.

The printing time **must be greater than or equal to** the integration step.
:::

</TabItem>
<TabItem value="zipLoad">

#### Use general composition for all system loads
Enables the composition of the [ZIP load](load#carga-no-estudo-de-estabilidade) for all loads that **do not** have individual parameterization.

#### Load Composition (active and reactive powers)
Defines the composition, in percentage, of the Impedance, Current, and Constant Power components of the load. This composition can be implemented individually for active and reactive power.

:::caution Caution!
The sum of the ZIP load components for a power must be 100%, otherwise an error message will be displayed.
:::

#### Undervoltage at which loads are modeled as constant impedance
Defines the percentage voltage value ($V_{low}$) below which the load is considered as pure constant impedance.

:::info Information
Constant current and power loads have problems at very low voltages. As the voltage decreases, the currents of these loads do not decrease, resulting in loss of precision and problems in convergence of iterative processes. To circumvent this problem, a predefined voltage ($V_{low}$) is used, at which constant current and power loads (or components) are modeled as constant impedance.
:::

</TabItem>
</Tabs>

---
