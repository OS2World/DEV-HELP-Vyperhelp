/***************************************************************************
 * File...... makeall.cmd
 * Author.... Mat
 * Date...... 6/9/98
 *
 * MAKEALL will build all Vyper modules (DLLs and EXEs).
 * MAKEALL takes different type of options, based on the initial character.
 *
 * -options are passed to the compiler (e.g., -Os)
 * /options are passed to the linker (e.g., /NOE)
 * @option specifies the version to be built (e.g., @1.00)
 * other options give a makeall option string, where characters enable functions:
 *   m  enable memory debugging
 *   a  enable assert checking (#)
 *   o  enable optimization (!)
 *   d  define DEBUG to enable debugging code (#)
 *   y  include debugging symbols (#)
 *   p  enable profiler
 *   b  enable browser
 *   c  enable checking in library source (inlines and templates) (#)
 *   k  enable extra checking in library source (#)
 *   t  enable all trace output (#)
 *   n  pack linker info (!)
 *   s  skip make, just set environment or backup
 *   x  backup source
 *   e  remake everything (including templates)
 *   z  create distribution directory
 *
 *   !  can be used for default release options
 *   #  can be used for default debug options
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

/* intialize options */
cflags = '-Gm -Gd -G5 -Tdp'
lflags = '/M'
mflags = ''
version = '"\"0.00\""'
copyright = '"\"Copyright (c)' substr(date('Sorted'),1,4) 'MekTek. All Rights Reserved.\""'
skip = 0
everything = 0
assemble = 0
backup = 0
releaseoptions = 'on'
debugoptions = 'adckty'

/* load REXX functions */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* determine versions */
parse version . rexxversion .
os2version = SysOS2Ver()

/* set up signals */
signal on error name cleanup

/* parse all options */
parse arg alloptions
moreoptions = alloptions
do while moreoptions \= ''
  parse value moreoptions with opt moreoptions
  type = substr(opt,1,1)
  select
    when type = '-' then cflags = cflags opt
    when type = '/' then lflags = lflags opt
    when type = '@' then version = '"\"' || substr(opt,2) || '\""'
  otherwise
    /* check for default debug or release options */
    if pos('!',opt) \= 0 then opt = opt releaseoptions
    if pos('#',opt) \= 0 then opt = opt debugoptions
    /* make upper case */
    opt = translate(opt)
    /* check required options */
    if pos('P',opt) \= 0 then opt = opt || 'Y'
    /* compiler options */
    if pos('M',opt) \= 0 then cflags = cflags '-Tm'
    if pos('A',opt)  = 0 then cflags = cflags '-DNDEBUG'
    if pos('O',opt) \= 0 then cflags = cflags '-O -Oc'
    if pos('D',opt) \= 0 then cflags = cflags '-DDEBUG'
    if pos('Y',opt) \= 0 then cflags = cflags '-Ti'
    if pos('P',opt) \= 0 then cflags = cflags '-Gh'
    if pos('B',opt) \= 0 then cflags = cflags '-Fb'
    if pos('C',opt) \= 0 then cflags = cflags '-DIC_DEVELOP'
    if pos('C',opt)  = 0 then cflags = cflags '-DINO_CHECKS'
    if pos('K',opt) \= 0 then cflags = cflags '-DIALL_CHECKS'
    if pos('T',opt) \= 0 then cflags = cflags '-DIC_TRACE_ALL'
    /* linker options */
    if pos('Y',opt) \= 0 then lflags = lflags '/DEBUG'
    if pos('B',opt) \= 0 then lflags = lflags '/BROWSE'
    if pos('N',opt) \= 0 then lflags = lflags '/EXEPACK:2 /PACKD /OPTFUNC /PACKC'
    /* nmake options */
    if pos('E',opt) \= 0 then mflags = mflags '/a'
    /* other flags */
    if pos('S',opt) \= 0 then skip = 1
    if pos('E',opt) \= 0 then everything = 1
    if pos('Z',opt) \= 0 then assemble = 1
    if pos('X',opt) \= 0 then backup = 1
  end
end

/* embed version and copyright */
cflags = cflags '-DVVERSION::' || version
cflags = cflags '-DVCOPYRIGHT::' || copyright

/* output general info */
say '****'
say '**** MAKEALL on' date() 'at' time()
say '****'
say 'options:' alloptions
say 'versions: REXX=' || rexxversion || ', OS/2=' || os2version
say 'cflags:' cflags
say 'lflags:' lflags
say 'mflags:' mflags
if skip then say '- skip make'
if everything then say '- make everything'
if assemble then say '- assemble distribution'
if backup then say '- backup source'

/* set environment variables */
env = 'OS2ENVIRONMENT'
call value 'CFLAGS', cflags, env
call value 'LFLAGS', lflags, env

/* start timer */
call time 'Reset'

/* backup source */
if backup then do
  say 'Zipping source...'
  say
  'd:'

  /* clear archive directory */
  call wipedir '\proj\makeall\archive'

  /* backup source */
  call archive 'generator',       'gen'
  call archive 'vfile',           'vfile'
  call archive 'ipfgenerator',    'ipfgen'
  call archive 'htmlgenerator',   'htmlgen'
  call archive 'hpj',             'winhelp'
  call archive 'ipfparse',        'ipfparse'
  call archive 'wingen',          'wingen'
  call archive 'vygen',           'vygen'
  call archive 'vydit',           'vydit'
  call archive 'generator\test',  'gentest'
  call archive 'vedit',           'vedit'
  call archive 'vykey',           'vykey'
  call archive 'vyx',             'vyx'
  call archive 'vyper',           'vyper'
  call archive 'makeall',         'makeall'
  call archive 'install',         'install'
end

/* build & promote all components */
if skip = 0 then do
  say 'Building components...'
  say
  'd:'
  call make 'generator',      'makefile'
  call make 'vfile',          'makefile'
  call make 'ipfgenerator',   'makefile'
  call make 'htmlgenerator',  'makefile'
  call make 'hpj',            'makefile'
  call make 'ipfparse',       'makefile'
  call make 'wingen',         'makefile'
  call make 'vygen',          'vygen.mak'
  call make 'vydit',          'vydit.mak'
  call make 'generator\test', 'makefile'
  call make 'vedit',          'vedit.mak'
  call make 'vykey',          'makefile'
  call make 'vyx',            'makefile'
  call make 'vyper',          'makefile'
end

/* assemble contents */
if assemble then do

  /* clear directory and go there */
  call wipedir '\proj\makeall\release'
  call directory '\proj\makeall\release'

  /* setup DLL renaming */
  mappings = 'cppom30=vycom30 cppoob3=vycoob3 cppoou3=vycoou3 cppoov3=vycoov3'

  /* vygen.dll */
  'copy \proj\vygen\vygen.dll'
  'copy \proj\vygen\vygen.map'
  'dllrname vygen.dll' mappings

  /* vydit.dll */
  'copy \proj\vydit\vydit.dll'
  'copy \proj\vydit\vydit.map'
  'dllrname vydit.dll' mappings

  /* parser tables */
  'copy %TABPATH%\cdefine.*'
  'copy %TABPATH%\hpj.*'
  'copy %TABPATH%\hpjlist.*'
  'copy %TABPATH%\rtf.*'
  'copy %TABPATH%\cnt.*'
  'copy %TABPATH%\ipf.*'

  /* vyx.exe */
  'copy \proj\vyx\vyx.exe'
  'copy \proj\vyx\vyx.map'
  'dllrname vyx.exe' mappings

  /* vyper.exe and vyper.hlp */
  'copy \proj\vyper\vyper.exe'
  'copy \proj\vyper\vyper.map'
  'copy \proj\vyper\vyper.vyd'
  'copy \proj\vyper\vyper.hlp'
  'copy \proj\vyper\readme.txt'
  'dllrname vyper.exe' mappings

  /* gentest.exe (private) */
  'copy \proj\generator\test\gentest.exe'
  'copy \proj\generator\test\gentest.map'
  dllrname gentest.exe mappings

  /* vedit.exe (private) */
  'copy \proj\vedit\vedit.exe'
  'copy \proj\vedit\vedit.map'
  dllrname vedit.exe mappings

  /* vykey.exe (private) */
  'copy \proj\vykey\vykey.exe'
  'copy \proj\vykey\vykey.map'
  'dllrname vykey.exe' mappings

  /* VisualAge DLLs */
  cppmain = value('cppmain',,env)
  'copy'  cppmain || '\dll\cppom30.dll'
  'dllrname cppom30.dll' mappings
  'copy'  cppmain || '\dll\cppoob3.dll'
  'dllrname cppoob3.dll' mappings
  'copy'  cppmain || '\dll\cppoou3.dll'
  'dllrname cppoou3.dll' mappings
  'copy'  cppmain || '\dll\cppoov3.dll'
  'dllrname cppoov3.dll' mappings
end

/* that's all! */
say
say 'MAKEALL completed successfully, elapsed minutes =' time('Elapsed') / 60
exit


/* clean-up */
cleanup:
say
say 'Error detected during makeall on line' SIGL
say 'Aborting...'
exit


/***************************************************************************
 * Procedure.. make
 *
 * makes the given project
 ***************************************************************************/
make:
procedure expose everything mflags
parse arg dir, makefile

/* move to correct directory */
call directory '\proj\' || dir

/* if remaking everything, then delete all templates */
if everything then call wipedir 'tempinc'

/* call nmake */
say
say '****'
say '**** nmake /f' makefile mflags
say '****'
'nmake /f' makefile mflags

/* call the local promotion script */
'if exist promote.cmd call promote.cmd'

return



/***************************************************************************
 * Procedure.. archive
 *
 * makes the given project
 ***************************************************************************/
archive:
parse arg dir, zipfile

/* move to correct directory */
call directory '\proj\' || dir

/* zip all source files */
'pjsave' zipfile '\proj\makeall\archive\'

return



/***************************************************************************
 * Procedure.. wipedir
 *
 * delete all files from the specified directory
 ***************************************************************************/
wipedir:
parse arg dir

call SysFileTree dir || '\*', 'file', 'FO'
do i=1 to file.0
  call SysFileDelete file.i
end

return

