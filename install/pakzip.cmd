@echo off
if "" == "%1" goto exit
echo Create VyperHelp distribution and ZIP in %1
pause
md %1 2>nul
del %1 /n 2>nul
verify on
copy install.exe %1
copy install.in_ %1
copy vyper.dsc %1
copy vyper.icf %1
copy epfihelp.inf %1
diskgen /u DRIVE vyper.pkg %1 d:\proj\makeall\release
copy vyper.gen vyper2.pkg
epfipak2 vyper2.pkg vyper2.pk_
copy vyper2.pk_ %1
zip -j %1\%1.zip %1\*.*
:exit

