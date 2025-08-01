---
id: exponential
title: Exponential
sidebar_label: Exponential
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

The exponential function block in the control elements performs this operation with a real input, following the expression:
$$
y_n = \bold{A} e^{\bold{B} u_n}
$$
Where:
- $u_n$ and $y_n$ are the input and output of the exponential block, respectively;
- $\bold{A}$ and $\bold{B}$ are constants defined by the user.

## Exponential block data editing form
The figure below shows the exponential block data editing form:

<div><center><img src={useBaseUrl("images/expForm.png")} alt="Exponential block data editing form in PSP-UFU" title="Exponential block data editing form in PSP-UFU" /></center></div>

This nonlinear block is defined by the constants A and B, entered by the user.

:::info Information
Such blocks are useful for representing nonlinearities, for example, modeling the saturation of DC machines present in some automatic voltage regulators.
:::
