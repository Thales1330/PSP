---
id: limiter
title: Limiter (absolute value)
sidebar_label: Limiter (absolute value)
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Limiter blocks serve to restrict input values according to the maximum and minimum limits set by the user. If the input exceeds any of the imposed limits, the output will be equal to that limit value until the input returns to the allowed range.

The maximum and minimum value constraints of control systems are modeled using a Limiter, whose implementation is quite simple and follows the following expression:
$$
y_n = 
\begin{cases}
L_{sup}{,}	& \text{if } u_n > L_{sup} \\
u_n{,}		& \text{if } L_{inf} \le u_n \le L_{sup}\\
L_{inf}{,}	& \text{if } u_n < L_{inf}
\end{cases}
$$
Where:
- $y_n$	is the current output value of the block
- $L_{sup}$ and $L_{inf}$	are the upper and lower limits, respectively
- $u_n$	is the current input value of the block

## Limiter block editing form
The figure below shows the data editing form of the limiter block.

<div><center><img src={useBaseUrl("images/limiterForm.png")} alt="Limiter block data editing form in PSP-UFU" title="Limiter block data editing form in PSP-UFU" /></center></div>

This nonlinear block is defined by the **upper and lower limits**, set by the user.

:::info Information
Such blocks are commonly found in control systems to limit the controller action, as well as to represent physical limits of implemented models, such as, for example, maximum excitation current values or mechanical power supplied by turbines.
:::
