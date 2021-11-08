:title: Packaging LLVM
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Packaging LLVM
==============

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover / RedHat employee

*PackaginCon — 9 November 2021*

.. image:: goblin_tinkerer.jpg
    :width: 400

----

Fedora in one slide
===================


.. code-block::

    .~= Fedora =~.           Close to upstream philosophy


    Several Releases             /:-------------:\\
    . rawhide ~ Debian sid    :-------------------::
    . f33, f34, f35         :-----------/shhOHbmp---:\\
                          /-----------omMMMNNNMMD  ---:
    Architectures        :-----------sMMMMNMNMP.    ---:
    . X86_64            :-----------:MMMdP-------    ---\\
    . arm              ,------------:MMMd--------    ---:
    . aarch64          :------------:MMMd-------    .---:
    . ppc64le          :----    oNMMMMMMMMMNho     .----:
                       :--     .+shhhMMMmhhy++   .------/
    . riscV            :-    -------:MMMd--------------:
    . mips             :-   --------/MMMd-------------;
                       :-    ------/hMMMy------------:
    Package            :-- :dMNdhhdNMMNo------------;
    . RPM              :---:sdNMMMMNds:------------:
                       :------:://:-------------::
                       :---------------------://

----

LLVM in one slide
=================

A compiler Infrastructure

.. code-block::

        +---------------------+ +---------------+
        | - Inlining          | | - ADT         |
        | - Unrolling         | | - System      |
        | - ...               | | - STLExtra    |
        |       ______________| | - ...         |
        |      / Optimizations| |           ____|
        +---------------------+ |          /Core|
                                +---------------+
        +---------------------+
        | X86, ARM, RiscV...  +
        |             ________|
        |            /Backends|
        +---------------------+

----

LLVM Project dependency tree
============================

.. code-block::

                +------------> mlir > ---------+
               /                                \
              +-----> lld                        \
             /                                    +--> flang
    LLVM >--+--> compiler-rt >--+                /
             \                   \              /
              +--> polly          +--> clang >-+
               \                                \
                +-------------- +----------------+--> lldb
                               /
    libomp >------------------+

    libcxx >

    libunwind >


----

LLVM Upstream
=============

- Monorepo: https://github.com/llvm/llvm-project
- Release cycle:
    - One major release every 6 months
    - Minor release in between
    - Release candidate for major and minor releases
- CMake-based build system
- Mostly C++14

----

LLVM Packaging In Fedora
========================

1. Build the world (git based)

   - ``(+)`` Supported upstream: easy packaging
   - ``(-)`` Small patch rebuilds the world

2. Build package independently (tarball based)

   - ``(+)`` Faster recompilation
   - ``(-)`` More downstream maintenance

Pick your poison! We chose ``2.``

----

ABI Compatibility
=================

LLVM Ships two kinds of libraries

1. C++ libraries, no ABI stability, major version bump each release

2. C interface, stable ABI across releases

Minor releases should not break the ABI

----

Compiling the compiler
======================

Building with...

1. GCC:

   - ``(+)`` Fedora default compiler
   - ``(-)`` LTO build fails

2. Clang

   - ``(-)`` Policy only recently evolved to allow it
   - ``(+)`` ``-flto=thin``
   - ``(-)`` LTO build fails

----

Library or Archive
==================

1. Shared Libs:

   - ``(+)`` follow Fedora Policy
   - ``(+)`` much better disk usage
   - ``(-)`` less configuration tested upstream

2. Archive:

   - ``(+)`` upstream default
   - ``(-)`` huge disk usage

We chose ``1.``

----

Slow linking
============

Linking ``libLLVM.so`` is super-slow

- ``[x]`` Decrease debug quality: ``-g1``
- ``[x]`` Break parallelism:
    - ``[x]`` Split build: ``cmake --build --target LLVM``
    - ``[x]`` Sequential link: ``-DLLVM_PARALLEL_LINK_JOBS=1``

- ``[ ]`` Use ``lld`` linker

----

Multi-versionning
=================

We don't support multiple LLVM version at the same time, but...

- Use `update-alternatives` to ease transition between major releases
- Provide versioned compatibility packages for packages that don't live on the
  bleeding edge

----

Testing
=======

Yeah! Tests! Breakage! Pain!

- ``[x]`` unit tests in each package, from upstream
- ``[x]`` Fedora CI
    - ``[x]`` integration test `LLVM Toolchain Integration Test Suite <https://github.com/opencollab/llvm-toolchain-integration-test-suite>`_

----

.. image:: https://llvm.org/img/LLVMWyvernSmall.png
    :width: 500

--

    The LLVM logo is a stylized wyvern (a kind of dragon). Dragons have connotations
    of power, speed and intelligence, and can also be sleek, elegant, and modular
    (err, maybe not).

