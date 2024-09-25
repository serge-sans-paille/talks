:title: Optimizing the core of Python Scientific Stack
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Optimizing the core of Python Scientific Stack
==============================================

*Serge « sans paille » Guelton <serge.guelton@telecom-bretagne.eu>*

Journée Python — PySciDataGre — mars 2018

*adapted from a talk at PyData Paris 2017 and one at FOSDEM 2018*

----

About me
========

- R&D engineer on compilation for security @ QuarksLab
- Associate Researcher at Telecom Bretagne
- Happy developer of `Pythran <https://github.com/serge-sans-paille/pythran>`_
- `Soldier of Fortune <http://gatherer.wizards.com/Pages/Card/Details.aspx?name=soldier%20of%20fortune>`_

----

About Python
============

Python is getting wide adoption in the Scientific community:

- Nice plots (Matplotlib)
- Scientific libraries (Scipy)
- Domain-specific libraries (Scikit-learn, Astropy…)
- Notebooks (Jupyter)

----

About Numpy
===========

The core of all the stack!

- data-centric abstraction
- elegant array DSL
- relatively good performance

----

Don't forget FORTRAN
====================

Sometimes, performance is paramount, and Numpy does not fit

- no parallelisation
- no vectorization
- intermediate result
- genericity >> performance

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

Performance and Python
----------------------

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


----

Choice & Consequences
=====================

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

Technical Discoveries
=====================

OpenMP Support
--------------

.. code:: python

    #pythran export some_stuff(float64[])
    #pythran export some_stuff(float32[;,:])
    def some_stuff(l):
        #omp parallel for reduction(+:r)
        for i, v in enumerate(l):
            r += i * v
        return r

Including OpenMP's atomic, critical, task and libomp :-)

----

Technical Discoveries
=====================

Cython Support
--------------

.. code:: python

    # cython: np_pythran=True
    import numpy as np
    cimport numpy as cnp

    def diffuse_numpy(cnp.ndarray[double, ndim=2] u, int N):
        cdef cnp.ndarray[double, ndim=2] temp = np.zeros_like(u)
        mu = 0.1

        for n in range(N):
            temp[1:-1, 1:-1] = u[1:-1, 1:-1] + mu * (
                u[2:, 1:-1] - 2L * u[1:-1, 1:-1] + u[0:-2, 1:-1] +
                u[1:-1, 2:] - 2L * u[1:-1, 1:-1] + u[1:-1, 0:-2])
            u[:, :] = temp[:, :]
            temp[:, :] = 0.0

Thanks to Adrien Guinet & OpenDreamKit!

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

Thanks
======

- `Silkan <https://www.silkan.com/>`_ for the original idea and funding
- `OpenDreamKit <https://opendreamkit.org/>`_ for a European grant
- `Group Calcul <http://calcul.math.cnrs.fr/>`_ for the regular meetings
- `LinuxFR <https://linuxfr.org/journaux>`_ *Bonjour'nal !*

Contribute!
===========

In random order:

- Bug reports
- Pull requests
- Fundings
- Praise, Flame and Smalltalk
