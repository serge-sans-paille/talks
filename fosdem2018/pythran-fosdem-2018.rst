:title: Surviving in an Open Source Niche: the Pythran case
:data-transition-duration: 150


Surviving in an Open Source Niche: the Pythran case
===================================================

*Serge « sans paille » Guelton <serge.guelton@telecom-bretagne.eu>*

FOSDEM 2018 -- Brussels / 3 & 4 February 2018

----

About me
========

- R&D engineer on compilation for security @ QuarksLab
- Associate Researcher at Telecom Bretagne
- Happy developer of `Pythran <https://github.com/serge-sans-paille/pythran>`_
- LLVM commiter

----

About Pythran
=============

- A compiler for scientific kernels
- A DSL embeded in Python

.. code:: Python

    #pythran export rosen(int[])
    #pythran export rosen(float[])
    import numpy as np
    def rosen(x):
        t0 = 100 * (x[1:] - x[:-1] ** 2) ** 2
        t1 = (1 - x[:-1]) ** 2
        return np.sum(t0 + t1)

----

A Niche Target
==============

Scientific Computing and Python
-------------------------------

- A live community built around Numpy / Scipy / Jupyter / Matplotlib…
- Wide choice of optimizing solutions: Cython / Numexpr / Numba…

Compilation and Python
----------------------

- Long term projects like PyPy, Cython (PyRex), Jython…
- Recent Projects like Numba, hope…
- Dead Projects like copperhead, Pyston, Unladen Swallow, Parakeet


----

Ideas in the Air
================

Basics (likely to work)
-----------------------

- Type variable declarations
- Do assumptions on the imported modules
- Support a subset of the language / environment

Advanced (require some time)
----------------------------

- Perform **compiler** optimization
- Just-In-Time compilation
- Support a **large** subset of the language / environment

Expert (brain damaging)
-----------------------

- Compatible with Python's lazy binding
- Compatible with native extensions


----

Pythran
=======

Open Source Software
--------------------

- Started on 2012
- BSD 3-clause
- Available on `Pypi <https://pypi.python.org/pypi/pythran>`_, `Conda <https://anaconda.org/serge-sans-paille/pythran>`_ and `GitHub <https://github.com/serge-sans-paille/pythran>`_

Dependencies
------------

- Numpy / networkx / ply / gast [easy]
- A **standard-compilant** C++11 compiler [used to be challenging]

Small Community
---------------

.. code::

    > git log --pretty='format:%an' | tr '[:upper:]' '[:lower:]' | sort -u  | wc -l
    32

----

Choice & Consquences
====================

Idea #1
-------

Generate Python-free code (apart from conversion)

Profit
------

Can release the GIL relatively early

----

Choice & Consquences
====================

Idea #2
-------

Emulate duck-typing through static polymorphism

Profit
------

Very light type inference code, let ``template`` instantiation magic do the job

----

Choice & Consquences
====================

Idea #3
-------

Generate High-level C++ code

Profit
------

- Can write a high-level C++ support library as *intrinsics*
- OpenMP support inside Python


----

Pythran
=======

Decent code base
----------------

.. code:: sh

    > git log --oneline --no-merges | wc -l
    1745

    > sloccount pythran/
    (...)
    43424   pythonic        cpp=43424
    15206   tests           python=14917,cpp=250,sh=39
    8217    top_dir         python=8217
    2502    analyses        python=2502
    1979    types           python=1979
    1397    transformations python=1397
    995     optimizations   python=995


----

How does it work?
=================

.. code::

          module.py           Pythran                 g++
              +            ----------->  module.cpp ------->  module.so
    #pythran export foo(int)

----

Just a translator?
==================

**No**

----

Just a translator?
==================

C++ is just a conveninent [*]_ backend

Source-to-Source toolbox
------------------------

.. code::

    pythran/
        analyses/         # abstract the ast
        transformations/  # transform the ast
        optimizations/    # optimize the ast


.. [*] Really convenient

----

Analyses
========

- Use-def-chains
- Memory effects, Argument effects, Global effects
- AST matcher
- Constant expressions
- Lazyness analysis
- Pure expression
- Value range

----

Optimizations
=============

- Iterator Unrolling
- Interprocedural Constant Folding
- Remove Modulo on Induction Variable
- Forward Substitution
- Inst Combine
- Range based simplification
- Dead code elimination

----

High Level Approach?
====================

.. code:: cython

    for i in range(freqs.shape[0]):

        xc = 0.
        xs = 0.
        cc = 0.
        ss = 0.
        cs = 0.

        for j in range(x.shape[0]):

            c = cos(freqs[i] * x[j])
            s = sin(freqs[i] * x[j])

            xc += y[j] * c
            xs += y[j] * s
            cc += c * c
            ss += s * s
            cs += c * s

        tau = atan2(2 * cs, cc - ss) / (2 * freqs[i])
        c_tau = cos(freqs[i] * tau)
        s_tau = sin(freqs[i] * tau)
        c_tau2 = c_tau * c_tau
        s_tau2 = s_tau * s_tau
        cs_tau = 2 * c_tau * s_tau

        pgram[i] = 0.5 * (((c_tau * xc + s_tau * xs)**2 / \
            (c_tau2 * cc + cs_tau * cs + s_tau2 * ss)) + \
            ((c_tau * xs - s_tau * xc)**2 / \
            (c_tau2 * ss - cs_tau * cs + s_tau2 * cc)))


----

High Level Approach!
====================

.. code:: python

    # Local variables
    c = np.cos(freqs[:, None] * x)
    s = np.sin(freqs[:, None] * x)
    xc = np.sum(y * c, axis=1)
    xs = np.sum(y * s, axis=1)
    cc = np.sum(c ** 2, axis=1)
    ss = np.sum(s * s, axis=1)
    cs = np.sum(c * s, axis=1)
    tau = np.arctan2(2 * cs, cc - ss) / (2 * freqs)
    c_tau = np.cos(freqs * tau)
    s_tau = np.sin(freqs * tau)
    c_tau2 = c_tau * c_tau
    s_tau2 = s_tau * s_tau
    cs_tau = 2 * c_tau * s_tau

    pgram = 0.5 * (((c_tau * xc + s_tau * xs)**2 / \
        (c_tau2 * cc + cs_tau * cs + s_tau2 * ss)) + \
        ((c_tau * xs - s_tau * xc)**2 / \
        (c_tau2 * ss - cs_tau * cs + s_tau2 * cc)))


----

Advanced Kernels
================

.. code:: python

    #from http://stackoverflow.com/questions/26823312/numba-or-cython\
    #            -acceleration-in-reaction-diffusion-algorithm
    #pythran export GrayScott(int, float, float, float, float)
    import numpy as np
    def GrayScott(counts, Du, Dv, F, k):
        n = 300
        U = np.zeros((n+2,n+2), dtype=np.float)
        V = np.zeros((n+2,n+2), dtype=np.float)
        u, v = U[1:-1,1:-1], V[1:-1,1:-1]

        r = 20
        u[:] = 1.0
        nd2 = int(n/2)
        U[nd2-r:nd2+r,nd2-r:nd2+r] = 0.50
        V[nd2-r:nd2+r,nd2-r:nd2+r] = 0.25

        u += 0.15*np.random.random((n,n))
        v += 0.15*np.random.random((n,n))

----

Advanced Kernels
================

.. code:: python

    (...)

        uvv = np.empty_like(u)
        for i in range(counts):
            Lu = (                 U[0:-2,1:-1] +
                  U[1:-1,0:-2] - 4*U[1:-1,1:-1] + U[1:-1,2:] +
                                   U[2:  ,1:-1] )
            Lv = (                 V[0:-2,1:-1] +
                  V[1:-1,0:-2] - 4*V[1:-1,1:-1] + V[1:-1,2:] +
                                   V[2:  ,1:-1] )
            uvv[:] = u*v*v
            u += Du*Lu - uvv + F*(1 - u)
            v += Dv*Lv + uvv - (F + k)*v

        return V


----

Community
=========

- Bus factor = 1
- A few power users:

  - Pythran generated-code in a mini submarine
  - A firm in Grenoble
  - Some academic work using Pythran for their research
  - Vaex (hi Marteen)
- Usual suspects that report issues

----

Long is the Road
================

- Numpy Support
- Python2 versus Python3
- OSX and Windows (♥ Visual Studio ♥)

----

Motivation
==========

- Technical challenges

  - Modern C++
  - Python Language and implications
  - Optimization, parallelism, vectorization

- Meeting People

  - Scipy/Numpy community is very friendly
  - Sharing ideas and tricks on optimization

----

Technical Discoveries
=====================

Jupyter Internals
-----------------

.. code:: python

    %%pythran -O2
    import numpy as np
    #pythran export foo(int[:,:])
    def foo(n):
        return np.sum(n + 2., axis=1)

----

Technical Discoveries
=====================

Python Capsule
--------------

.. code:: python

    #pythran capsule export foo(int[:,:])
    def foo(n):
        return np.sum(n + 2., axis=1)


----


Technical Discoveries
=====================

Complex Numbers in Numpy
------------------------

Heard about IEEE standard?

What happens when you multiply an infinite imaginary number with NaN?

Vectorization
-------------

Lear to debug AVX instructions genreated by ``Boost.simd``... or by the compiler!


----


Sand of Time
============

Family + Work + Health + OSS
----------------------------

- Find a good balance between fun and duty
- Mix them!

  - Conference <> see family | friends
  - Have your OSS dev funded
  - Learn stuff also useful at work
  - Share Knowledge

Pet Project is not work!
------------------------

- No mandatory task, fun prevails
- No tweeting or hardcore advertising, take your time

----

Next Steps
----------

On going effort to use Pythran in Scipy

- A friendly community
- Large code base, several interesting kernels
- But a lot of requirements too ☺

    - `Generated binaries size <http://serge-sans-paille.github.io/pythran-stories/shrinking-pythran-generated-binaries.html>`_
    - `Windows support <https://github.com/serge-sans-paille/pythran/pull/777>`_
    - Project maturity ``:-/``

----

Thanks
======

- `Silkan <https://www.silkan.com/>`_ for the original idea and funding
- `OpenDreamKit <https://opendreamkit.org/>`_ for a European grant
- `Group Calcul <http://calcul.math.cnrs.fr/>`_ for the regular meetings
- `LinuxFR <https://linuxfr.org/journaux>`_ *Bonjour'nal !*

----

Sometimes, good stuff happen
============================

.. code::


    Date: Tue, 27 Sep 2016 18:34:51 -0400
    From: ************
    To: serge.guelton@telecom-bretagne.eu
    Subject: Probabilistic programming with Pythran

    [-- Autoview using w3m -I 'UTF-8' -T text/html --]
    Hi,

    My name is ****; I'm a research scientist at ****, a machine learning
    startup. I wanted to get in touch with you because of your experience with
    Python source-to-source compilation and particularly your work on Pythran,
    which we've been playing around with of late to do Python to Python source
    transformation.
