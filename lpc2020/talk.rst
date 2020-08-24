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
- **compiler** (pick compiler flags)
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

Default Fedora Flags
=====================

.. code::

    -O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2
    -Wp,-D_GLIBCXX_ASSERTIONS -fexceptions -fstack-protector-strong
    -grecord-gcc-switches -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1
    -specs=/usr/lib/rpm/redhat/redhat-annobin-cc1 -m64 -mtune=generic
    -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection
    -Wl,-z,relro -Wl,--as-needed  -Wl,-z,now
    -specs=/usr/lib/rpm/redhat/redhat-hardened-ld

----

Default Debian Flags
=====================

.. code::

    -g -O2
    -fdebug-prefix-map=/home/sylvestre/dev/debian/pkg-llvm/llvm-toolchain/branches=.
    -fstack-protector-strong -Wformat -Werror=format-security


----

Common Library Overflows
========================

*Attack*: Exploit standard C/C++ functions misuse

*Countermeasure*: Provide fortified version of these functions

*Flag*: ``-D_FORTIFY_SOURCE`` (gcc, clang for builtin supports), ``-D_GLIBCXX_ASSERTIONS``

*Overhead*: low (fortify) to high (asserts)

----

Common Formating Attacks
========================

*Attack*: Exploit user-controlled formating arguments

*Countermeasure*: Warn about dubious patterns

*Flag*: ``-Werror=format-security`` (gcc, clang)

*Overhead*: nop (compile time)


----

Common Code Overflows
=====================

*Attack*: Exploit buffer overflow

*Countermeasure*: Range analysis

*Flag*: ``-Werror=array-bounds`` (gcc, clang)

*Overhead*: nop (compile time)

----

Uninitialized Stack Variables
=============================

*Attack*: Use unitialized variable to leak previous state

*Coutermeasure*: Always initialize stack variable

*Flag*: ``-ftrivial-auto-var-init=pattern`` (clang)

*Overhead*: yes (?)

----

GOT / PLT Overwrite
===================

*Attack*: Overwrite the GOT/PLT to overwrite executable sections

*Countermeasure*: Load everything then mark GOT/PLT read-only

*Flag*: ``-Wl,-z,norelro`` (ld.bfd, lld), ``-Wl,-z,now`` (ld.bfd, lld)

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

And now for something different
===============================

All these slides were pretty classic, right?

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

*Countermeasure*: Check Control Flow Integrity / Intel CET

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

----

Example: Stack Clash Protection
===============================

- LLVM using the GCC implementation as reference
- Test beds different (based on compiler report for GCC, and assembly reference
  for LLVM)
- Paths to explore:

    - ``valgrind`` -based verification of distance invariant?
    - Static verification?


