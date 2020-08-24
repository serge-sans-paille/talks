:title: Security Related Flags: a Common GCC/LLVM View
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css

Intro
=====

Security countermeasure strikes at several level:

- hardware (buy a new machine)
- microcode (update your kernel)
- *compiler* (pick compiler flags)
- codebase (update your code)

----


Always Two They Are
===================

GCC Toolchain (mostly gcc + ld.bfd)

LLVM Toolchain (mostly clang + lld)

----

Security Related Flags: a Common GCC/LLVM View
==============================================

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover / RedHat employee

**LPC'20 — 26th of August 2020**

----

Common Library Overflows
========================

*Attack*: Exploit standard C/C++ functions misuse

*Countermeasure*: Provide foritified version of these functions

*Flag*: ``-D_FORITFY_SOURCE`` (gcc, clang for builtin supports), ``-D_GLIBCXX_ASSERTIONS``

*Overhead*: low (fortify) to high (asserts)


----

Common Code Overflows
=====================

*Attack*: Exploit buffer overflow

*Countermeasure*: Range analysis

*Flag*: ``-Werror=array-bounds`` (gcc, clang)

*Overhead*: nop (compile time)

----

Common Formating Attacks
========================

*Attack*: Exploit user-controlled formating arguments

*Countermeasure*: Warn about dubious patterns

*Flag*: ``-Werror=format-security`` (gcc, clang)

*Overhead*: nop (compile time)

----

Common Uninitialized Variables
==============================

*Attack*: Use unitialized variable to leak previous state

*Coutermeasure*: Always initialize stack variable

*Flag*: ``-ftrivial-auto-var-init=pattern`` (clang)

*Overhead*: yes (?)

----

GOT / PLT Overwrite
===================

*Attack*: Overwrite the GOT/PLT to overwrite executable sections

*Countermeasure*: Mark GOT/PLT read-only

*Flag*: ``-Wl,-z,norelro`` (ld.bfd, lld), ``-Wl,-z,lazy`` (ld.bfd, lld)

*Overhead*: increased startup time

----

Executable Stack
================

*Attack*: Overwrite an executable stack with malicious code

*Countermeasure*: Mark the stack as non-executable

*Flag*: ``-Wl,-z,noexecstack`` (ld.bfd, lld)

*Overhead*: nop (?)

----

Security through Diversity
==========================

*Attack*: Write reusable shellcodes / attacks

*Countermeasure*: Randomize process adresses (ASLR)

*Flag*: ``-pie -fPIE`` or ``-fPIC`` (gcc/ld.bfd, clang/lld)

*Overhead*: increased startup time (?)

----

Stack Clash
===========

*Attack*: Make the stack and the heap grow so that they overlap

*Countermeasure*: Probe each page to trigger the kernel page guard

*Flag*: ``-fstack-clash-protector`` (gcc, clang)

*Overhead*: only for functions with large / dynamic stack alloc


----

Stack Smash
===========

*Attack*: Modify the stack thanks to an overflow

*Countermeasure*: Stack Canary

*Flag*: ``-fstack-protector`` (gcc, clang)

*Overhead*: one check per function, user-controlled granularity


----

Spectre V1
==========

*Attack*: Trick branch prediction into filling the cache with secret data

*Countermeasure*: create a data dependency between data access and predicate state

*Flag*: ``-mspeculative-load-hardening`` (clang)

*Overhead*: non-neglectible (todo: add ref)

----

Spectre V2
==========

*Attack*: Trick branch prediction into executing a controlled function pointer

*Countermeasure*: Use return prediction instead of branch prediction

*Flag*: ``-mretpoline`` (clang) ``-mindirect-branch, -mfunction-return`` (gcc)

*Overhead*: non-neglectible (todo: add ref)

----

Return Oriented Programing
==========================

*Attack*: Execute arbitrary code through a chain of gadget

*Countermeasure*: Check control flow integrity / Intel CET

*Flag*: ``-fsanitize=cfi`` (clang) ``-mcet`` (clang, gcc) ``-fcf-protection`` (clang,
gcc)

----

Certification
=============

Want to double-check the flags used in the build process?

- ``-fplugin=annobin`` (gcc, clang)
- ``-grecord-gcc-switches`` (gcc)

----

Follow-ups
==========

- Convergence of options names is ~OK
- But beside names, implementation differ!

  - Discussing implementation across mlist (or on a common medium?)
  - Sharing compiler-agnostic test beds?
