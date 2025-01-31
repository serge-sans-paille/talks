:title: Improving Compile-Time Computation of Object Size
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Improving Compile-Time Computation of Object Size
=================================================

**Serge « sans paille » Guelton**

Compiler Engineer / Story Teller / Mozilla Employee

**FOSDEM — 1st February 2025**

----

``__builtin_object_size(ptr, 0)``
=================================

- Yields the constant number of bytes from ``ptr`` to the end of the object
  ``ptr`` points to.

- If unknown at compile time, returns **-1**.

- If ``ptr`` can point to several objects, returns the **maximum** of remaining
  bytes count across all potential objects.

----

``__builtin_object_size(ptr, 1)``
=================================

- Yields the constant number of bytes from ``ptr`` to the end of the object
  ``ptr`` points to.

- If unknown at compile time, returns **-1**.

- If ``ptr`` can point to several objects, returns the **minimum** of remaining
  bytes count across all potential objects.

----

``__builtin_object_size(ptr, 2)``
=================================

- Yields the constant number of bytes from ``ptr`` to the end of the object
  ``ptr`` points to.

- If unknown at compile time, returns **0**.

- If ``ptr`` can point to several objects, returns the **maximum** of remaining
  bytes count across all potential objects.

----

``__builtin_object_size(ptr, 3)``
=================================

- Yields the constant number of bytes from ``ptr`` to the end of the object
  ``ptr`` points to.

- If unknown at compile time, returns **0**.

- If ``ptr`` can point to several objects, returns the **minimum** of remaining
  bytes count across all potential objects.

----

Usage (0)
=========

Under ``-D_FORTIFY_SOURCE=[123]``

.. code-block:: c

    __fortify_function char *
    __NTH (stpncpy (char *__dest, const char *__src, size_t __n))
    {
      if (__bos (__dest) != (size_t) -1
          && (!__builtin_constant_p (__n) || __n > __bos (__dest)))
        return __stpncpy_chk (__dest, __src, __n, __bos (__dest));
      return __stpncpy_alias (__dest, __src, __n);
    }

----

Usage (1)
=========

For sanitizers:

.. code-block:: sh

    % git grep -l ObjectSizeOffset llvm/lib
    llvm/lib/Analysis/MemoryBuiltins.cpp
    llvm/lib/Transforms/IPO/AttributorAttributes.cpp
    llvm/lib/Transforms/Instrumentation/AddressSanitizer.cpp
    llvm/lib/Transforms/Instrumentation/BoundsChecking.cpp


----

Comparison between GCC and Clang
================================

from: https://github.com/serge-sans-paille/builtin_object_size-test-suite

With gcc 14.2.1:

.. code-block:: sh

    % make CC=gcc CXX=g++
    g++ -O2 -fno-inline    check-staticxx.cpp   -o check-staticxx
    ./check-static ; static=$? ; ./check-staticxx ; staticxx=$?; exit `echo $static + $staticxx | bc`
    [FAIL] check-static.c:243 check_if_then_negative_offset: 8 - 2 - max(0, 3) + 2 != __builtin_object_size(ptr - 2, 2), expecting 5 got 0
    [FAIL] check-static.c:374 check_static_posix_memalign: n != __builtin_object_size(m, 0), expecting 7 got -1
    [FAIL] check-static.c:464 check_conditional_storing_in_arg: max(n0, n1) != __builtin_object_size(*mem, 0), expecting 8 got -1
    [FAIL] check-static.c:465 check_conditional_storing_in_arg: max(n0, n1) != __builtin_object_size(*mem, 1), expecting 8 got -1
    [FAIL] check-static.c:466 check_conditional_storing_in_arg: min(n0, n1) != __builtin_object_size(*mem, 2), expecting 5 got 0
    [FAIL] check-staticxx.cpp:30 check_placement_new: sizeof(double) != __builtin_object_size(mem, 0), expecting 8 got -1

Could be better, but that's *correct*

----

Comparison between GCC and Clang
================================

With clang 18.1.8:

.. code-block:: sh

    % make CC=clang CXX=clang++
    clang -O2 -fno-inline    check-static.c   -o check-static
    clang++ -O2 -fno-inline    check-staticxx.cpp   -o check-staticxx
    ./check-static ; static=$? ; ./check-staticxx ; staticxx=$?; exit `echo $static + $staticxx | bc`
    [FAIL] check-static.c:241 check_if_then_negative_offset: 8 - 2 - min(0, 3) + 2 != __builtin_object_size(ptr - 2, 0), expecting 8 got -1
    [FAIL] check-static.c:242 check_if_then_negative_offset: 8 - 2 - min(0, 3) + 2 != __builtin_object_size(ptr - 2, 1), expecting 8 got -1
    [FAIL] check-static.c:243 check_if_then_negative_offset: 8 - 2 - max(0, 3) + 2 != __builtin_object_size(ptr - 2, 2), expecting 5 got 0
    [FAIL] check-static.c:335 check_static_reallocarray: n != __builtin_object_size(m, 0), expecting 100 got -1

Could be okay if those were not **OK** with previous clang ``;_;``

----

Test Case Dissection
====================

.. code-block:: c

    void check_if_then_negative_offset() {
      char buffer[8];
      char* ptr = &buffer[2];
      if(rand() & 1)
        ptr += 3;
      CHECK(__builtin_object_size(ptr - 2, 0), 8 - 2 - min(0, 3) + 2);
      ...
    }

What kind of LLVM bitcode do we have there?

----

Test Case Bitcode (``-O0``)
===========================

.. code-block:: llvm

      %12 = getelementptr inbounds [8 x i8], ptr %1, i64 0, i64 2
      store ptr %12, ptr %2, align 8
      %13 = call i32 @rand() #12
      %14 = and i32 %13, 1
      %15 = icmp ne i32 %14, 0
      br i1 %15, label %16, label %19

    16:
      %17 = load ptr, ptr %2, align 8
      %18 = getelementptr inbounds i8, ptr %17, i64 3
      store ptr %18, ptr %2, align 8
      br label %19

    19:
      br label %20

    20:
      store i64 8, ptr %3, align 8
      %21 = load ptr, ptr %2, align 8
      %22 = getelementptr inbounds i8, ptr %21, i64 -2
      %23 = call i64 @llvm.objectsize.i64.p0(ptr %22, i1 false, i1 true, i1 false)

----

Pick Your Poison
================

The previous code can be optimized into:

1. A ``phi`` instruction, which only requires pointer analysis to fold
   ``@llvm.objectsize``

2. A ``select`` instruction, which requires offset analysis to fold
   ``@llvm.objectsize``

Guess what changed in recent LLVM version?

----

About ``select`` instruction analysis
=====================================

If the pointer offset depends on a ``select`` value, let's use ``llvm::computeConstantRange``, but…

- ``llvm::computeConstantRange`` is smart
- ``llvm.builtin_object_size`` is used for security check

Can that be a problem?

----

About **not** Being Smart
=========================

**Question**: *Do you want to take advantage of undefined behaviors when you track undefined behaviors?*

**Question**: *Is there an interest in large over-approximation when folding BoS?*

**Question**: *Do approximations made by* ``llvm::computeConstantRange`` *match the definition of origins of BoS?*

If the answer to one of those is *no*, implement a simpler version of value tracking.

----

Implement a Simple Value Tracking
=================================

1. Only track values, no range optimizations

2. Simple arithmetic

3. No undefined behavior were harmed during this execution

See PR #117849

----

In the End
==========

.. code-block:: sh

    export PATH=~/sources/llvm-project/_build/bin:$PATH
    make CC=clang CXX=clang++
    clang -O2 -fno-inline    check-static.c   -o check-static
    clang++ -O2 -fno-inline    check-staticxx.cpp   -o check-staticxx
    ./check-static ; static=$? ; ./check-staticxx ; staticxx=$?; exit `echo $static + $staticxx | bc`

Yay, back to the state from 4 years ago ``\o/``

----

Bonus Point
===========

Model ``reallocarray(3)``

.. code-block:: c

   void *reallocarray(void *_Nullable ptr, size_t nmemb, size_t size);



See PR #114818

----

Did I Say Simple?
=================

Approximations and GEP

.. code-block:: c

   char *ptr = malloc(10);
   int coffset = cond ? + 5: ptr;
   long bos = __builtin_object_size(ptr + coffset, MODE)

- ``MODE==0`` ⇒ max value for ``bos`` ⇒ min value for ``coffset``
- ``MODE==2`` ⇒ min value for ``bos`` ⇒ max value for ``coffset``

----

Special Thanks
==============

- To Mozilla for letting me working on that borderline topic for a build
  engineer

- Harald van Dijk ``@hvdijk`` for reviewing my patches and providing countless
  counter examples

----

Concluding Words
================

- ``__builtin_object_size`` brings extra compiler power to the user, it's a fun intrinsic
- ``__builtin_object_size`` now supports a wider range of scenario
- `@llvm.objectsize`` is key to some security features of LLVM
- Did you know about ``__builtin_dynamic_object_size``?




