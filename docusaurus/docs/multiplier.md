---
id: multiplier
title: Multiplier
sidebar_label: Multiplier
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

This element is quite simple and fulfills the mathematical function implied by its name.

It has two real value inputs and its output is computed sequentially from the upper node to the lower node, that is, the output will be the value present at node 1 multiplied by node 2.

:::caution Warning!
Nodes not connected to the multiplier are assumed to have a unit value, and for the divisor, a zero value is considered.
:::
