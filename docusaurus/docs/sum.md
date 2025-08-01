---
id: sum
title: Adder
sidebar_label: Adder
---
import useBaseUrl from "@docusaurus/useBaseUrl";

<link rel="stylesheet" href={useBaseUrl("katex/katex.min.css")} />

As the name suggests, adders are control elements capable of summing any number of real values.

## Adder data edit form

The number of inputs, as well as their signs, are defined by the user, as shown in the figure below.

<div><center><img src={useBaseUrl("images/sumForm.png")} alt="Adder data edit form in PSP-UFU" title="Adder data edit form in PSP-UFU" /></center></div>

In its single field "**Signs**", a list of signs **separated by spaces** is entered, containing the symbols “+” for a positive input and “-” for a negative input.

:::info Information
The user can insert two or more input signs in the block.
:::

Once new inputs are added, corresponding nodes will be included in the graphical element of the adder, which can then be connected through connection lines.

Similarly, the user may remove pre-existing input signs, which will be removed from the graphical icon by deleting the node and the line (if one is connected).
