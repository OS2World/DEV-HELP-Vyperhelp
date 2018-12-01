VyperHelp: OS/2 IPF help outliner, editor and converter

Introduction

VyperHelp(TM) is a visual outliner and editor for creating online help for
OS/2-based systems.  It generates IPF files, as well as HTML and WinHelp files.
VyperHelp has the following features:

   WYSIWYG text editing with codes revealed
   Powerful outline tree controls
   Keyword/index support
   Bitmap graphics support
   Full symbol support
   IPF import and export
   HTML export (single file or frames)
   HTMLHelp export
   WinHelp (HPJ, RTF, CNT) import and export
   RTF document-style export
   ASCII text import and export
   Command-line conversion program

See Usage Notes for tips, limitations, and a list of known bugs.  Help can be
viewed from the command line (in Warp 4) with:

  view vyper.hlp

Conversion Program (VYX)

VyperHelp includes a conversion program (VYX.EXE), which can be run from the
command line to convert formats in a batch mode.  VYX is faster and supports
more features than the editor.  The syntax for the program is as follows:

  VYX infile outfile

where infile is the input file, and outfile is the output file.  The file type
is determined by the filename extension.  Supported input file types are: TXT,
IPF, HPJ, and VYD.  Supported output file types are: TXT, IPF, HTM, HTML, HHP,
HPJ, RTF, H, VYD, and VYT.  VYD is a VyperHelp document.  VYT is VyperHelp
diagnostic trace output.  H is a C language header file.  Use "HTM" is single
file HTML and "HTML" for multi-file (frame) output.

outfile may also include '=' to specify the same root filename as the input file
(for example, "=.ipf").

Requirements

VyperHelp has the following requirements:

   OS/2 Warp version 3 or greater
   Pentium or faster
   4M hard drive space
   32M memory recommended
   IPF compiler (to create HLP or INF files)

Terms

VyperHelp is provided for free distribution.  Thirty days after its first use,
it will no longer work.  During that trial period, promotional text will be
inserted at the end of each section during export or conversion.

A registration key is required for un-limited and un-hindered use.  Registration
keys can be purchased from MekTek for $49 US, which includes minor upgrades and
support.

Please read the complete license for details.

Installation

To install VyperHelp, run the INSTALL.EXE program which was included as part of
the ZIP package.  The installation program will unpack the files into a
directory you specify (default is C:\VYPER), and create a VyperHelp folder.

If you opt to update your CONFIG.SYS file, then the directory will be added to
the PATH and LIBPATH, and a TABPATH setting will be added.  TABPATH gives the
location of the parsing tables (*.LLR and *.DFA).  These changes allow VyperHelp
and VYX to be run in directories other then the one to which they were
installed.

The installation program also sets up an association between VyperHelp and the
*.VYD file extension, so you can double-click a VYD file to edit it.

Removal (Uninstall)

VyperHelp and all of its associated files and folders can be removed from your
system using INSTALL.EXE program which was included as part of the original ZIP
package.

To remove VyperHelp, run the program from the command line, using the following
parameters:

  install /a:d

Contact MekTek

Please contact us with any comments, suggestions or bug reports for VyperHelp,
or if you would like to join our mailing list.

Contact MekTek by one of the following means:

   Web: http://www.vyperhelp.com
   Email: info@vyperhelp.com
   Fax: 1-651-649-0237

Thank you for trying VyperHelp!

