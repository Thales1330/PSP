---
id: divider
title: Divider
sidebar_label: Divider
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

This element is quite simple and performs the mathematical function of its name.

It has two real-value inputs, and its output is computed as the top node value divided by the bottom node value, i.e., output = node 1 ÷ node 2.

:::caution Attention!
Unconnected nodes in the divider are considered as zero.  
If the division has a zero numerator, the output value is defined as $10^{15}$.
:::
