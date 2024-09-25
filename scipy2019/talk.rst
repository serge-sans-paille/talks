:title: Pythran, 6 years later
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css

This is a Pythran Kernel
========================

Yup, it's plain Python

.. code-block:: python

    #pythran export l1norm(float64[:,:], float64[:,:])
    import numpy as np
    def l1norm(x, y):
        return np.sum(np.abs(x[:, None, :] - y), axis=-1)

And you compile it like this:

.. code-block:: sh

    > pythran l1norm.py

Running the generated native module yields a x6 speedup compared to CPython,

:math:`\times 9` when simd instructions are active.

----

Pythran, 6 Years Ago
====================

First presented at Scipy 2013

Core Ideas:

- Ahead of time compiler
- Embeded in Python: Pure Python + top-level type comments
- Lazy evaluation of Numpy expression
- Optimize high-level kernels, no-loops!
- Support most Python functional aspects
- Automatic vectorization
- Explicit OpenMP parallel constructs
- No Python mode

Everything is still valid! That's a firm ground for a compiler.

----

Pythran, 6 Years Later
======================

Steady code growth, more users, more optimizations, more Numpy support:

- Python3 and Python2 Support
- Linux, OSC and Windows Support
- Conda integration

- Support for C++ code generation
- Efficient support for a large part of Numpy broadcasting
- Many Numpy functions are implemented
- Correct and efficient typing within is None tests
- GIL-free code

----

Integrate into the Ecosystem
============================

- Notebook integration: ``%%pythran cells``
- Cython integration:  ``#cython: np_pythran=True``
- Native function generation: ``#pythran export capsule``...

----

Some Supported Kernels
======================

Local functions, numpy support, default parameters

.. code-block:: python

    #pythran export evolve(float64[][], float)
    import numpy as np
    def laplacian(grid):
        return np.roll(grid, +1, 0) + np.roll(grid, -1, 0) + np.roll(grid, +1, 1) + np.roll(grid, -1, 1) - 4 * grid

    def evolve(grid, dt, D=1):
        return grid + dt * D * laplacian(grid)

----

Some Supported Kernels
======================

Default parameters, keyword arguments, numpy expression, enumerate, tuples...

.. code-block:: python

    #pythran export check_mask(bool[][])
    import numpy as np
    def check_mask(db, mask=[1, 0, 1]):
        out = np.zeros(db.shape[0],dtype=bool)
        for idx, line in enumerate(db):
            target, vector = line[0], line[1:]
            if (mask == np.bitwise_and(mask, vector)).all():
                if target == 1:
                    out[idx] = 1
        return out


----

Numbers Look Serious
====================

Number of commits:

.. code-block:: sh

    > git rev-list --count 0.9.2
    2834

Number of Authors (some duplicate in there):

.. code-block:: sh

    > git shortlog -s -n | wc -l
    39

Number of closed Issues:

.. code-block:: sh

    > curl -s https://github.com/serge-sans-paille/pythran/issues | grep Closed
          361 Closed

----

Get It
======

- Source: https://github.com/serge-sans-paille/pythran
- Packages:
    - https://pypi.org/project/pythran
    - https://anaconda.org/conda-forge/pythran
- Doc: https://pythran.readthedocs.io
- Air Speed Velocity: https://serge-sans-paille.github.io/pythran-asv/
- Blog: http://serge-sans-paille.github.io/pythran-stories/
- Contact:
    - https://www.freelists.org/list/pythran
    - #pythran on IRC, freenode
