---
id: powerFlow
title: Power Flow
sidebar_label: Power Flow
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

A fundamental study in the planning of the expansion and operation of an electrical system is the **power flow (or load flow)** since the satisfactory operation of this system depends on the knowledge of the effects of interconnection, new loads, new generating plants, and new lines before they are installed. This study aims to find the power flow in the branches and the nodal voltages of the Power Electric System (PES) imposed by generation and load.

The equations formulated from the models of the elements present in the load flow study are nonlinear and do not have an explicit analytical solution. Therefore, it is necessary to use iterative numerical methods to solve the load flow problem.

## Formulation of the load flow problem
The load flow problem can be represented by a system of nonlinear algebraic equations and inequalities that correspond to Kirchhoff's laws and a set of operational constraints imposed by the components of an electrical network.
In the problem formulation, each bus of the network is associated with four variables, two of which are given data and two are unknowns (at a bus of index $i$):
- $V_i$ is the voltage magnitude at bus $i$;
- $θ_i$ is the voltage angle at bus $i$;
- $P_i$ is the net active power injection at bus $i$;
- $Q_i$ is the net reactive power injection at bus $i$.

Regarding the variables that are unknowns and the system data, **three types of buses** can be defined:
- **PQ Bus**: $P_i$ and $Q_i$ are given, $V_i$ and $θ_i$ are calculated;
- **PV Bus**: $P_i$ and $V_i$ are given, $Q_i$ and $θ_i$ are calculated;
- **Reference Bus**: $V_i$ and $θ_i$ are given, $P_i$ and $Q_i$ are calculated.

:::info Information
**PQ buses** are generally represented by load buses, which have no generation and voltage control.

**PV buses** are characterized by voltage control through the injection or absorption of reactive power via excitation control of a synchronous machine.

The **Reference bus** (or slack bus) functions, as its name indicates, as a voltage and angle reference for the system. This bus is necessarily a generator because it is responsible for balancing the system power.

:::

:::caution Warning!
The bus type must be defined in the [bus element](bus).

Note that **the system must have only one reference bus**.
:::

As mentioned earlier, the equations are nonlinear and the analytical solution is not practical. The solutions of these equations follow **[iterative processes](https://en.wikipedia.org/wiki/Iterative_method)**, where estimated (or initial) values are assigned to the buses with unknown voltages and, based on the specified active and reactive power and voltage magnitude, the new complex voltages at each system node are calculated using the previously presented equations.

Next, this set of voltage values at each bus is used again to calculate another group of voltages. Each calculation of a new set of voltages is called an iteration. The iterative process is repeated until the changes in all buses are less than a preset value, thus achieving convergence.

## Running the load flow in PSP-UFU
After constructing the one-line diagram in the [power editor](powerEditor), the load flow execution is performed in the [Simulation menu](mainScreen#ribbon-menu) by clicking the **Load Flow** button.

<div><center><img src={useBaseUrl("images/menuSimulationPF.svg")} alt="Simulation Menu" title="Simulation Menu" /></center></div>

:::tip Tip
If the load flow has been successfully executed, the power arrows will be displayed, the status bar will indicate success, and the [text elements](text) will be updated.
:::

**Another option** is execution via continuous calculation, also present in the [Simulation menu](mainScreen#ribbon-menu), activated by clicking the **Enable Solution** button. With this option, the static calculations selected in the [simulation settings](simulationConfig) are automatically performed when any network parameters are modified, such as electrical data or circuit breaker operations (removal or insertion).

:::caution Warning!
Avoid building the circuit with continuous calculation enabled, as temporary configurations may lead to simulation execution errors.

To disable continuous calculation, click the **Disable Solution** button.
:::

The load flow results are displayed in the [linked text elements](text) when hovering over the elements and in [tabular reports](tabularReport).

### Common errors when running load flow
The following are the most common errors related to load flow.

#### The following error message is displayed: "Maximum number of iterations reached"
This error message appears when the numerical solution method selected in the [simulation settings](simulationConfig) reaches the maximum number of iterations entered. The following situations may cause this error:
- **Circuit parameters are incorrect.** If parameters far from the standard values of electrical elements are entered, the load flow calculation may diverge. Check if the data was correctly entered.
- **Some element has parameters discrepant from others.** If an element inserted has an impedance value very different from others, for example, a line with very high impedance or a very small load, it can cause numerical method divergence. In this case, reconsider the need for representing these elements in the circuit and alternate between available numerical solution methods.
- **The maximum number of iterations is too low.** Some circuits require a higher number of iterations, so change the maximum iterations value in the [simulation settings](simulationConfig). Also, try switching between available numerical solution methods.
- **Simulation parameters are inadequate.** If a solution method parameter such as acceleration factor or tolerance is unsuitable, the calculation may not converge. Adjust these parameters in the [simulation settings](simulationConfig).

#### Output data is displayed as "NaN" or "nan"
This occurs due to errors in mathematical operations in the load flow calculations. "NaN" means *Not a Number*.
- **Some bus is isolated.** This error is quite common and may occur when inserting a bus without connecting it to the system or when removing branch elements that connect a bus to the system. The solution is to eliminate this bus from the diagram.
- **Circuit parameters are incorrect.** If parameters far from the standard values of electrical elements are entered, the load flow calculation may diverge. Check if the data was correctly entered.
- **Some element has parameters discrepant from others.** If an element inserted has an impedance value very different from others, for example, a line with very high impedance or a very small load, it can cause numerical method divergence. In this case, reconsider the need for representing these elements in the circuit and alternate between available numerical solution methods.

## Numerical solution methods for load flow in PSP-UFU
The methods implemented in the program for solving the load flow problem in PSP-UFU are **Gauss-Seidel (GS)** and **Newton-Raphson (NR)**. In addition to these classic methods, a hybrid method can be used (defined in the [simulation settings](simulationConfig)), where GS is initially used followed by NR, increasing the chance of NR convergence.

### Gauss-Seidel
The Gauss-Seidel method has been widely used in recent decades to solve the load flow problem, as there is no need to factorize matrices, reducing computational effort. Nowadays, computational constraints are less problematic and other methods are usually chosen, but Gauss-Seidel still has educational value, and since PSP-UFU also has educational purposes, this method was implemented.

To start iterations of the method, initial values for voltages ($\dot{V}_i^0$) must be calculated. Normally for PQ buses $\dot{V}_i^0=1.0 + j0.0~p.u.$ and for PV buses $\dot{V}_i^0=V_i^{spec} + j0.0$ [p.u.], where $V_i^{spec}$ is the voltage magnitude specified for the PV bus. The reference bus has fixed voltage magnitude and angle and does not participate in the iterative process.

The Gauss-Seidel method shows an **excessive number of iterations**, and to reduce them, voltage corrections are multiplied by a constant. This operation increases the correction value, bringing the voltage closer to the final value. The multipliers that perform this improved convergence are called acceleration factors. For any system, there are optimal values for acceleration factors, and an inadequate choice may result in slower convergence or make it impossible. Usually, an acceleration factor equal to 1.6 is used, which is the default value in the program. The use of the acceleration factor is done by the equation:
$$
\dot{V}_{i_{AC}}^{(v + 1)} = \alpha \left( \dot{V}_{i}^{(v + 1)}- \dot{V}_{i_{AC}}^{(v)} \right) + \dot{V}_{i_{AC}}^{(v)}
$$
Where:
- $\dot{V}_{i_{AC}}^{(v + 1)}$ is the complex voltage of the current iteration with acceleration factor applied
- $\dot{V}_{i}^{(v + 1)}$ is the complex voltage of the current iteration
- $\dot{V}_{i_{AC}}^{(v)}$ is the complex voltage of the previous iteration with acceleration factor applied
- $\alpha$ is the acceleration factor

The flowchart below shows how the Gauss-Seidel method was implemented in PSP-UFU:

<div><center><img src={useBaseUrl("images/gaussSeidel.svg")} alt="Gauss-Seidel numerical method for load flow" title="Gauss-Seidel numerical method for load flow" /></center></div>

### Newton-Raphson
The Newton-Raphson method (also known as Newton or Newton-Fourier method) for load flow solution is described in many books and articles. It is currently the most used algorithm for load flow solution. For well-conditioned cases, this method generally converges in 4 to 5 iterations, but there is a possibility that the technique bypasses the solution point without ever reaching it, which also justifies the implementation of the Gauss-Seidel method in the software.

The expansion of the [Taylor series](https://en.wikipedia.org/wiki/Taylor_series) for a function of two or more variables is the basis of the Newton-Raphson method to solve the load flow problem. Performing the Taylor series expansion for two equations and two unknowns ($f_1(x_1, x_2)$ and $f_2(x_1, x_2)$), without listing partial derivatives higher than 1 in matrix form, we get:
$$
\begin{bmatrix}
K_1 - f_1\left( x_{1}^{(0)}, x_{2}^{(0)} \right)\\
K_2 - f_2\left( x_{1}^{(0)}, x_{2}^{(0)} \right)
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle\frac{\partial f_1}{\partial x_1} & \displaystyle\frac{\partial f_1}{\partial x_2}\\
\displaystyle\frac{\partial f_2}{\partial x_1} & \displaystyle\frac{\partial f_2}{\partial x_2}
\end{bmatrix}
\begin{bmatrix}
\Delta x_{1}^{(0)}\\
\Delta x_{2}^{(0)}
\end{bmatrix}
$$
Where:
- $K$ is the result of the equation $f(x_1, x_2)$
- $x^{(0)}$ is the initial estimate of $x$
- $\Delta x$ is the value added to $x^{(0)}$ that results in $x$, i.e., $f(x_1, x_2) = f_1(x_1^{(0)} + \Delta x_1, x_2^{(0)} + \Delta x_2)$

This expression can be summarized as:
$$
\begin{bmatrix}
\Delta K_1^{(0)}\\
\Delta K_2^{(0)}
\end{bmatrix}
=
\begin{bmatrix}
J
\end{bmatrix}^{(0)}
\begin{bmatrix}
\Delta x_{1}^{(0)}\\
\Delta x_{2}^{(0)}
\end{bmatrix}
$$
Where $[J]$ is the **Jacobian Matrix**

With the equation above, it is possible to calculate the values of $\Delta x_1^{(0)}$ and $\Delta x_2^{(0)}$. However, these values added to the initial estimates do not determine the correct solution, so the process of determining constants, forming the Jacobian matrix, and solving the above equation must be repeated, producing new estimates $\Delta x_1^{(1)}$ and $\Delta x_2^{(1)}$.

This process is repeated until the corrections become so small that they satisfy a chosen precision.

To apply the Newton-Raphson method to solve the load flow equations, the equations representing active and reactive power injected at a bus are used.

As in the Gauss-Seidel method, the reference bus is omitted from the iterative solution to determine voltages, since the complex voltage of this bus is specified. Since the value of injected active power ($P_i^{spec}$) at PQ and PV buses is known, as well as the injected reactive power ($Q_i^{spec}$) at PQ buses, $P_i^{spec}$ and $Q_i^{spec}$ can be defined as values of $K$. The estimated voltage magnitude and angle values correspond to estimates for $x_1$ and $x_2$.

The Jacobian consists of the partial derivatives of $P_i$ and $Q_i$ with respect to each of the variables of the net power injection equations at bus $i$. The column matrix formed by $\Delta x_1$ and $\Delta x_2$ corresponds to the corrections of angle ($\Delta \theta_i$) and magnitude ($\Delta V_i$) of the bus voltages.

Thus, the matrix equation of a system with $n$ buses can be written, where bus number 1 is the reference bus and buses numbered 2 to n are PQ buses:
$$
\begin{array}{r}
\text{NPQ} + \text{NPV}
\begin{cases}
~\\
~
\end{cases}\\
~\\
\text{NPQ}
\begin{cases}
~\\
~
\end{cases}
\end{array}
\begin{bmatrix}
\Delta P_2\\
\vdots\\
\Delta P_n\\
\Delta Q_2\\
\vdots\\
\Delta Q_n\\
\end{bmatrix}
=
\begin{bmatrix}
\displaystyle\frac{\partial P_2}{\partial \theta_2} & \dots & \displaystyle\frac{\partial P_2}{\partial \theta_n} & \displaystyle\frac{\partial P_2}{\partial V_2} & \dots & \displaystyle\frac{\partial P_2}{\partial V_n}\\
\vdots & \ddots & \vdots & \vdots & \ddots & \vdots\\
\displaystyle\frac{\partial P_n}{\partial \theta_2} & \dots & \displaystyle\frac{\partial P_n}{\partial \theta_n} & \displaystyle\frac{\partial P_n}{\partial V_2} & \dots & \displaystyle\frac{\partial P_n}{\partial V_n}\\
\displaystyle\frac{\partial Q_2}{\partial \theta_2} & \dots & \displaystyle\frac{\partial Q_2}{\partial \theta_n} & \displaystyle\frac{\partial Q_2}{\partial V_2} & \dots & \displaystyle\frac{\partial Q_2}{\partial V_n}\\
\vdots & \ddots & \vdots & \vdots & \ddots & \vdots\\
\displaystyle\frac{\partial Q_n}{\partial \theta_2} & \dots & \displaystyle\frac{\partial Q_n}{\partial \theta_n} & \displaystyle\frac{\partial Q_n}{\partial V_2} & \dots & \displaystyle\frac{\partial Q_n}{\partial V_n}
\end{bmatrix}
\begin{bmatrix}
\Delta \theta_2\\
\vdots\\
\Delta \theta_n\\
\Delta V_2\\
\vdots\\
\Delta V_n\\
\end{bmatrix}
$$
Where:
- $\text{NPQ}$ is the number of PQ buses
- $\text{NPV}$ is the number of PV buses

The iterative process starts by calculating active powers ($P_i^{calc}$) for PQ and PV buses and reactive powers ($Q_i^{calc}$) for PQ buses, both using the initial estimates of the complex voltages. Then, the power corrections ($\Delta P$ and $\Delta Q$) are calculated:
$$
\Delta P = P_i^{spec} - P_i^{calc}\\
\Delta Q = Q_i^{spec} - Q_i^{calc}
$$

The next step is the formation of the Jacobian matrix. With this, it is possible to calculate the corrections of magnitude and angle of the voltages of all buses (except the reference bus). For this, PSP-UFU uses the [Gaussian Elimination](https://en.wikipedia.org/wiki/Gaussian_elimination) method followed by back substitution. This procedure reduces computational effort since the Jacobian matrix inversion at each iteration is avoided.

With the calculated corrections of voltage magnitude and angle, the following equations are applied:
$$
\theta_i^{(v+1)} = \theta_i^{(v)} + \Delta \theta_i^{(v)}\\
V_i^{(v+1)} = V_i^{(v)} + \Delta V_i^{(v)}
$$

The process is then restarted and repeated until convergence is achieved, i.e., when corrections become so small that they satisfy a predefined tolerance.

The flowchart below shows the Newton-Raphson method for load flow solution implemented.

<div><center><img src={useBaseUrl("images/newtonRaphson.svg")} alt="Newton-Raphson numerical method for load flow" title="Newton-Raphson numerical method for load flow" /></center></div>

## Controls and limits in a load flow problem
At [generation](syncGenerator) buses and those with a connected [synchronous compensator](syncMotor), voltage magnitude control at the bus is performed by adjusting the field current of synchronous machines, which can operate overexcited (injecting reactive power) or underexcited (absorbing reactive power). The reactive power limits that can be injected or absorbed depend on the synchronous machine under study. These limits are included in the load flow with the creation of two new parameters: maximum reactive power ($Q_i^{max}$) and minimum reactive power ($Q_i^{min}$), and the voltage ceases to be controlled from then on.

The violation check and bus type switching can be performed at each iteration or at the end of the convergence of the calculation. In PSP-UFU, the latter strategy has been implemented, since it separates the concepts of calculation and limit verification, making it easier to develop new numerical methods and limits. In this approach, when adjusting the system to the new non-violating situation, the iterative calculation must be resumed until convergence is achieved again.

## References
1. MONTICELLI, A. J. Power Flow in Electric Power Networks. São Paulo: Edgar Blücher, 1983.  
1. STEVENSON JR.; WILLIAN, D. Elements of Power System Analysis. 2nd ed. São Paulo: McGraw-Hill, 1986.  
1. MILANO, F. Power System Modelling and Scripting. London: Springer, 2010. doi: https://doi.org/10.1007/978-3-642-13669-6  
1. ARRILLAGA, J.; WATSON, N. R. Computer Modelling of Electrical Power Systems. Wiley & Sons, New York, 2001. doi: https://doi.org/10.1002/9781118878286  
1. TINNEY, W. F.; HART, C. E. Power Flow Solution by Newton’s Method. IEEE Transaction on Power Apparatus and Systems, v. PAS-86, n. 11, Nov. 1967. doi: https://doi.org/10.1109/TPAS.1967.291823  

