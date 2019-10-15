If you are interested in contributing to PSP-UFU **you are most welcome!**

## Before start
Before starting with your contributions we recommend that you get to know the project better by visiting the [PSP-UFU website](https://thales1330.github.io/PSP/) and reading the published articles:

>Oliveira, T. L., Guimarães, G. C., & Silva, L. R. C. (2019). PSP-UFU: An open-source, graphical, and multiplatform software for power system studies. _International Transactions on Electrical Energy Systems_, e12185. doi: [10.1002/2050-7038.12185](https://doi.org/10.1002/2050-7038.12185)

>Oliveira, T. L., Guimarães, G. C., Silva, L. R., & Rezende, J. O. (2019). Power system education and research applications using free and open-source, graphical and multiplatform PSP-UFU software. _The International Journal of Electrical Engineering & Education_, 0020720919879058. doi: [10.1177/0020720919879058](https://doi.org/10.1177/0020720919879058)

## Requirements and PSP-UFU setup
In order to build the PSP-UFU in your machine, [clone the GitHub repository](https://github.com/Thales1330/PSP.git) and download the [CodeLite IDE](https://codelite.org/) to open the Workspace file.
All the project configuration are already done in this file.

The CodeLite provides a C++ compiler if you don't already have one.

Another requirement to build PSP-UFU is the GUI library [wxWidgets](https://www.wxwidgets.org/). Build tutorials for wxWidgets can be found in [wxWidgets' Wiki](https://wiki.wxwidgets.org/Main_Page), [here (Windows)](https://wiki.codelite.org/pmwiki.php/Developers/BuildingWxWidgetsWin) and [here (Linux)](https://wiki.codelite.org/pmwiki.php/Main/WxWidgets31Binaries).

Questions can be placed in [Issue tracker](https://github.com/Thales1330/PSP/issues).

## Coding Guidelines

The [wxWidgets coding guide](https://www.wxwidgets.org/develop/coding-guidelines/) are used to writing code for inclusion into the PSP-UFU.
Please, follow the guidelines with the following **exceptions**:
 - _Physical Files Organization_: Always use the MVC pattern described in published papers;
 - _Use wx prefix for all public symbols_: wx prefix are not needed;
 - _Restrictions on C++ Features_: The are no C++ feature restrictions in PSP-UFU.

Use the comments to create your code documentation, using the [Doxygen style](http://www.doxygen.nl/).

The [Project Documentation](https://thales1330.github.io/PSP/doxygen/html/index.html) is your best friend! Explore it!
 
## Pull requests
After your contribution, please send a pull request with detailed description in order to make clear the modifications and new features.
Aways test your code with the samples projects before pull requests.



Good coding!

Thales
