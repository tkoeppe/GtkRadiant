# scons build script
# http://scons.sourceforge.net

import commands, re, sys, os, pickle, string, popen2
from makeversion import radiant_makeversion, get_version
from osx_setup import do_osx_setup

# to access some internal stuff
import SCons

conf_filename='site.conf'
# there is a default hardcoded value, you can override on command line, those are saved between runs
# we only handle strings
serialized=['CC', 'CXX', 'JOBS', 'BUILD', 'SETUP']

# help -------------------------------------------

Help("""
Usage: scons [OPTIONS] [TARGET] [CONFIG]

[OPTIONS] and [TARGET] are covered in command line options, use scons -H

[CONFIG]: KEY="VALUE" [...]
a number of configuration options saved between runs in the """ + conf_filename + """ file
erase """ + conf_filename + """ to start with default settings again

CC
CXX
	Specify C and C++ compilers (defaults gcc and g++)
	ex: CC="gcc-3.2"
	You can use ccache and distcc, for instance:
	CC="ccache distcc gcc" CXX="ccache distcc g++"

JOBS
	Parallel build
	ex: JOBS="4" is a good setting on SMP machines

BUILD
	Use debug/release to select build settings
	ex: BUILD="release" - default is debug
	OSX: use BUILD="info" to generate the set of release files

SETUP
  Build a setup - default 0
"""
)

# end help ---------------------------------------
  
# sanity -----------------------------------------

# use q decently recent python release
EnsurePythonVersion( 2, 1 )
# above 0.90
EnsureSConsVersion( 0, 95 )
print 'SCons ' + SCons.__version__

# end sanity -------------------------------------

# system detection -------------------------------

# CPU type
g_cpu = commands.getoutput('uname -m')
exp = re.compile('.*i?86.*')
if (g_cpu == 'Power Macintosh'):
  g_cpu = 'ppc'
elif exp.match(g_cpu):
  g_cpu = 'x86'
else:
  g_cpu = 'cpu'

# OS
OS = commands.getoutput('uname')

if (OS == 'Linux'):
  # libc .. do the little magic!
  # NOTE: this used to work fine up to libc 2.3
  libc = commands.getoutput('/lib/libc.so.6 |grep "GNU C "|grep version|awk -F "version " \'{ print $2 }\'|cut -b -3')

# end system detection ---------------------------

# default settings -------------------------------

CC='gcc'
CXX='g++'
JOBS='1'
BUILD='debug'
INSTALL='#install'
SETUP='0'
g_build_root = 'build'

# end default settings ---------------------------

# site settings ----------------------------------

site_dict = {}
if (os.path.exists(conf_filename)):
	site_file = open(conf_filename, 'r')
	p = pickle.Unpickler(site_file)
	site_dict = p.load()
	print 'Loading build configuration from ' + conf_filename
	for k, v in site_dict.items():
		exec_cmd = k + '=\"' + v + '\"'
		print exec_cmd
		exec(exec_cmd)

# end site settings ------------------------------

# command line settings --------------------------

for k in serialized:
	if (ARGUMENTS.has_key(k)):
		exec_cmd = k + '=\"' + ARGUMENTS[k] + '\"'
		print 'Command line: ' + exec_cmd
		exec(exec_cmd)

# end command line settings ----------------------

# sanity check -----------------------------------

if (SETUP == '1' and BUILD != 'release' and BUILD != 'info'):
  print 'Forcing release build for setup'
  BUILD = 'release'

def GetGCCVersion(name):
  ret = commands.getstatusoutput('%s -dumpversion' % name)
  if ( ret[0] != 0 ):
    return None
  vers = string.split(ret[1], '.')
  if ( len(vers) == 2 ):
    return [ vers[0], vers[1], 0 ]
  elif ( len(vers) == 3 ):
    return vers
  return None

ver_cc = GetGCCVersion(CC)
ver_cxx = GetGCCVersion(CXX)

# end sanity check -------------------------------

# save site configuration ----------------------

for k in serialized:
	exec_cmd = 'site_dict[\'' + k + '\'] = ' + k
	exec(exec_cmd)

site_file = open(conf_filename, 'w')
p = pickle.Pickler(site_file)
p.dump(site_dict)
site_file.close()

# end save site configuration ------------------

# general configuration, target selection --------

SConsignFile( "scons.signatures" )

g_build = g_build_root + '/' + BUILD

SetOption('num_jobs', JOBS)

LINK = CXX
# common flags
CCFLAGS = ''
CXXFLAGS = '-pipe -DQ_NO_STLPORT '
CPPPATH = []
if (BUILD == 'debug'):
	CXXFLAGS += '-g -D_DEBUG '
	CCFLAGS += '-g -D_DEBUG '
elif (BUILD == 'release'):
	CXXFLAGS += '-g -O2 '
	CCFLAGS += '-g -O2 '
elif ( BUILD == 'info' ):
	print 'Preparing OSX release'
	( line, major, minor ) = get_version()
	do_osx_setup( major, minor, 'osx-radiant-%s.run' % line )
	sys.exit( 0 )
else:
	print 'Unknown build configuration ' + BUILD
	sys.exit( 0 )

LINKFLAGS = ''
if ( OS == 'Linux' ):
  LINKFLAGS += '-Wl,-fini,fini_stub '
if ( OS == 'Darwin' ):
  CCFLAGS += '-force_cpusubtype_ALL -fPIC '
  CXXFLAGS += '-force_cpusubtype_ALL -fPIC -fno-exceptions -fno-rtti '
  CPPPATH.append('/sw/include')
  CPPPATH.append('/usr/X11R6/include')
  LINKFLAGS += '-L/sw/lib -L/usr/lib -L/usr/X11R6/lib '

CPPPATH.append('libs')

# extend the standard Environment a bit
class idEnvironment(Environment):

  def useGlib2(self):
    self['CXXFLAGS'] += '`pkg-config glib-2.0 --cflags` '
    self['CCFLAGS'] += '`pkg-config glib-2.0 --cflags` '
    self['LINKFLAGS'] += '`pkg-config glib-2.0 --libs` '
    
  def useXML2(self):
    self['CXXFLAGS'] += '`xml2-config --cflags` '      
    self['CCFLAGS'] += '`xml2-config --cflags` '      
    self['LINKFLAGS'] += '`xml2-config --libs` '

  def useGtk2(self):
    self['CXXFLAGS'] += '`pkg-config gtk+-2.0 --cflags` '
    self['CCFLAGS'] += '`pkg-config gtk+-2.0 --cflags` '
    self['LINKFLAGS'] += '`pkg-config gtk+-2.0 --libs-only-L` `pkg-config gtk+-2.0 --libs-only-l` '
   
  def useGtkGLExt(self):
    self['CXXFLAGS'] += '`pkg-config gtkglext-1.0 --cflags` '
    self['CCFLAGS'] += '`pkg-config gtkglext-1.0 --cflags` '
    self['LINKFLAGS'] += '`pkg-config gtkglext-1.0 --libs-only-L` `pkg-config gtkglext-1.0 --libs-only-l` '      
    
  def usePNG(self):
    self['CXXFLAGS'] += '`libpng-config --cflags` '
    self['CCFLAGS'] += '`libpng-config --cflags` '
    self['LINKFLAGS'] += '`libpng-config --ldflags` '

  def usePThread(self):
    if ( OS == 'Darwin' ):
      self['LINKFLAGS'] += '-lpthread -Wl,-stack_size,0x400000 '
    else:
      self['LINKFLAGS'] += '-lpthread '

  def CheckLDD(self, target, source, env):
    file = target[0]
    if (not os.path.isfile(file.abspath)):
        print('ERROR: CheckLDD: target %s not found\n' % target[0])
        Exit(1)
    # not using os.popen3 as I want to check the return code
    ldd = popen2.Popen3('`which ldd` -r %s' % target[0], 1)
    stdout_lines = ldd.fromchild.readlines()
    stderr_lines = ldd.childerr.readlines()
    ldd_ret = ldd.wait()
    del ldd
    have_undef = 0
    if ( ldd_ret != 0 ):
        print "ERROR: ldd command returned with exit code %d" % ldd_ret
        os.system('rm %s' % target[0])
        Exit()
    for i_line in stderr_lines:
        print repr(i_line)
        regex = re.compile('undefined symbol: (.*)\t\\((.*)\\)\n')
        if ( regex.match(i_line) ):
            symbol = regex.sub('\\1', i_line)
            try:
                env['ALLOWED_SYMBOLS'].index(symbol)
            except:
                have_undef = 1
        else:
            print "ERROR: failed to parse ldd stderr line: %s" % i_line
            os.system('rm %s' % target[0])
            Exit(1)
    if ( have_undef ):
        print "ERROR: undefined symbols"
        os.system('rm %s' % target[0])
        Exit(1)
  
  def SharedLibrarySafe(self, target, source):
    self.SharedLibrary(target, source)
    if (OS != 'Darwin'):
      AddPostAction(target + '.so', self.CheckLDD)

g_env = idEnvironment(ENV = os.environ, 
  CC = CC,
  CXX = CXX,
  LINK = LINK,
  CCFLAGS = CCFLAGS,
  CXXFLAGS = CXXFLAGS,
  CPPPATH = CPPPATH,
  LINKFLAGS = LINKFLAGS)

# export the globals
GLOBALS = 'g_env INSTALL SETUP g_cpu'

radiant_makeversion('\\ngcc version: %s.%s.%s' % ( ver_cc[0], ver_cc[1], ver_cc[2] ) )

# end general configuration ----------------------

# targets ----------------------------------------

Default('.')

Export('GLOBALS ' + GLOBALS)
BuildDir(g_build, '.', duplicate = 0)
SConscript(g_build + '/SConscript')

# end targets ------------------------------------
