@echo off
verify on
echo Create VyperHelp distribution disks on A:
echo Two blank disks are needed
echo Insert Disk 1
pause
rem -- PKG file is a placeholder, it will be replaced
copy vyper.pkg a:
copy install.exe a:
copy install.in_ a:
copy vyper.dsc a:
copy vyper.icf a:
copy epfihelp.inf a:
diskgen /u DRIVE vyper.pkg a: d:\proj\makeall\release
copy vyper.gen vyper2.pkg
epfipak2 vyper2.pkg vyper2.pk_
echo Re-Insert Disk 1
pause
del a:\vyper.pkg
copy vyper2.pk_ a:

