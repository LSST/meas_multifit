# -*- python -*-
#
# Setup our environment
#
# Do not change these
import glob, os.path, re, sys
import lsst.SConsUtils as scons

# List the direct *and indirect* pacakage dependencies of your package here.
# Indirect dependencies are needed to get header files.
dependencies = ["boost", "python", "minuit2", "cfitsio", "wcslib", "utils", 
    "pex_exceptions", "eigen", "daf_base", "daf_data", "daf_persistence",
        "pex_logging", "pex_exceptions", "pex_policy", "security", "gsl", "afw",
        "meas_algorithms", "fftw"
    ]

env = scons.makeEnv(
    # The name of your package goes here.
    "meas_multifit",
    # This is used to try to get some version information.
    r"$HeadURL$",
    [
        # For each dependency above, include one or more lines listing
        # at least one required header file and, if needed, a shared library.
        # For maximum safety, all header files and all shared libraries used
        # could be listed, but typically ensuring that one is available will
        # be sufficient to make sure the rest of the package is available.

        # If just a header is required, list it.
        ["boost", "boost/shared_ptr.hpp"],
        ["python", "Python.h"],
        ["m", "math.h", "m", "sqrt"],
        ["utils", "lsst/utils/Utils.h", "utils:C++"],
        ["pex_exceptions", "lsst/pex/exceptions/Runtime.h", "pex_exceptions:C++"],   
        ["cfitsio", "fitsio.h", "cfitsio", "ffopen"],
        ["wcslib", "wcslib/wcs.h", "wcs"],
        ["xpa", "xpa.h", "xpa", "XPAPuts"],
        #["minuit", "Minuit/FCNBase.h", "lcg_Minuit:C++"], 
        ["minuit2", "Minuit2/FCNBase.h", "Minuit2:C++"],
        ["daf_base", "lsst/daf/base.h", "daf_base:C++"],
        ["pex_policy", "lsst/pex/policy/Policy.h", "pex_policy:C++"],
        ["pex_logging", "lsst/pex/logging/Trace.h", "pex_logging:C++"],
        ["security", "lsst/security/Security.h", "security:C++"],
        ["daf_persistence", "lsst/daf/persistence/Persistence.h", "daf_persistence:C++"],
        ["daf_data", "lsst/daf/data/LsstBase.h", "daf_data:C++"],
        ["gsl", "gsl/gsl_rng.h", "gslcblas gsl"],       
        ["eigen", "Eigen/Core.h"],
        ["afw", "lsst/afw/image/MaskedImage.h", "afw:C++"],
        ["meas_algorithms", "lsst/meas/algorithms.h", "meas_algorithms"],
        ["fftw", "fftw3.h"],
    ]
)

# Describe what the package contains here.
env.Help("""
LSST Multifit Implementation.
""")

# Explicit build for m4-generated headers, since SCons doesn't do dependency
# checking on those.
env.Append(M4FLAGS="-I%s" % os.path.join(os.path.abspath('.'),'m4'))
generated = ["include/ndarray/Array.hpp",
             "include/ndarray/operators.hpp",
             "include/ndarray/Vector.hpp",
             "include/ndarray/fft/FFTWTraits.hpp",
             ]
headers = [env.M4(filename, "%s.m4" % filename) for filename in generated]
env.Depends(headers, Glob("#m4/*.m4"))

if True:
    #
    # Workaround SConsUtils failure to find numpy .h files. Fixed in sconsUtils >= 3.3.2
    #
    import numpy
    env.Append(CCFLAGS = ["-I", numpy.get_include()])
#

###############################################################################
# Boilerplate below here.  Do not modify.

pkg = env["eups_product"]
env.libs[pkg] += env.getlibs(" ".join(dependencies))
if True:
    #
    # Workaround SConsUtils failure to find numpy .h files. Fixed in sconsUtils >= 3.3.2
    #
    import numpy
    env.Append(CCFLAGS = ["-I", numpy.get_include()])

# Build/install things
#
for d in Split("lib python/lsst/" + re.sub(r'_', "/", pkg) + " examples tests doc"):
    try:
        SConscript(os.path.join(d, "SConscript"))
    except Exception, e:
        print >> sys.stderr, "%s: %s" % (os.path.join(d, "SConscript"), e)

env['IgnoreFiles'] = r"(~$|\.pyc$|^\.svn$|\.o$)"

Alias("install", [env.Install(env['prefix'], "python"),
                  env.Install(env['prefix'], "include"),
                  env.Install(env['prefix'], "lib"),
                  env.InstallAs(os.path.join(env['prefix'], "doc", "doxygen"),
                                os.path.join("doc", "htmlDir")),
                  env.InstallEups(env['prefix'] + "/ups")])

scons.CleanTree(r"*~ core *.so *.os *.o")

#
# Build TAGS files
#
files = scons.filesToTag()
if files:
    env.Command("TAGS", files, "etags -o $TARGET $SOURCES")

env.Declare()
