---
id: rateLimiter
title: Rate Limiter
sidebar_label: Rate Limiter
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

Rate limiter blocks aim to contain the rate of increase or decrease of their input to values set by the user. The rate of change is calculated from the current and previous input values and, if violated, an output will be set that respects these rates (of increase or decrease).

The rate ($r$) is calculated according to the equation:
$$
r = \frac{u_n - y_{n-1}}{h}
$$
Where:
- $u_n$ is the current input of the block
- $y_{n-1}$ is the previous output value of the block
- $h$ is the integration step

Once the rate is obtained, the block will output a value calculated by the following conditions:
$$
\begin{cases}
h \times R_{cre}{,}	& \text{if } r > R_{cre} \\
u_n{,}		& \text{if } R_{dec} \le r \le R_{cre}\\
h \times R_{dec}{,}	& \text{if } r < R_{dec}
\end{cases}
$$
Where:
- $R_{cre}$ and $R_{dec}$ are the growth and decay rates, respectively

## Rate Limiter Data Edit Form
The figure below shows the data edit form of the rate limiter block.

<div><center><img src={useBaseUrl("images/rateLimitForm.png")} alt="Rate limiter block data edit form in PSP-UFU" title="Rate limiter block data edit form in PSP-UFU" /></center></div>

This block is defined by the **upper limit (maximum growth rate) and lower limit (maximum decay rate)** set by the user.

:::info Information
These blocks are useful to limit abrupt variations in the input that may compromise the control or physical limits of turbine models, for example.
:::
