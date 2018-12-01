VyperHelp Source Package
========================

This package contains source and documentation necessary for building
VyperHelp.  VyperHelp source currently requires certain 3rd party tools
be installed to complete the build.  This source is equivalent to the
last shareware release of VyperHelp 1.11, except that the license text
has been updated to reference the new GPL licensing.


Required Tools
--------------
VisualAge C++ 3.0 for OS/2 with all latest fixpaks
VisualParse++ 1.1 for OS/2
IBM Software Installer 1.4.1 for OS/2
A ZIP tool such as InfoZIP


License
-------
VyperHelp(TM) and accompanying documentation are 
Copyright (c) 2000 Mat Kramer (MekTek).

VyperHelp is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You have received a copy of the GNU General Public License
along with this program (see gpl.txt); if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


Status
------
These files are currently dependent on my environment.  Here are some things
that need to be done to make the source usable by others.

- Fix build environment to remove hard-coded paths etc
- Remove unnecessary files (I deleted what was obvious to me)
- Remove license key mechanism
- Remove dependencies on commercial tools (this is a bigger job)


Folders
-------
Doc			Documentation
install			Installation settings and files
makeall			Script for making entire project
VA Project Files	VisualAge project settings
fix			Fix for VisualAge template header
Generator		C++ source folder for components...
HPJ
HtmlGenerator
IPFGenerator
ipfparse		
vedit
vfile
vydit
vygen
vykey
vyper
vyx
wingen


Author
------
Mat Kramer (mat@vyperhelp.com)
July 26, 2005
