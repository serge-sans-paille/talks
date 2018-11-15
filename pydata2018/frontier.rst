:title: Crossing the native code frontier
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Crossing the native code frontier
=================================

*Serge « sans paille » Guelton <serge.guelton@telecom-bretagne.eu>*

PyParis 2018 -- France / 14 -- 15 november 2018

*a talk from Namek, the ultimate frontier*

----

Foreword
========

Whenever I'll be speaking of Python, I'm referencing:

    - the Python **3** language
    - its reference interpreter, **CPython**

----

Python, a glue language
=======================

All theses modules are written in C:

- itertools
- re
- ...

And these ones relie on C extension:

- coverage
- lxml
- markupsafe
- numpy
- scipy
- tornado
- zmq


----

At the core: ``PyObject``
=========================

Every object in Python is represented as a pointer to a reference-counted,
heap-allocated structure, the ``PyObject*``.

It wraps (among other things) an *actual value*

.. code::

        ------------
        | PyObject* |
        -------------
        |  typeinfo |
        |  refcount |
        |    ...    |
        |   value   |
        -------------

    RARE VIEW OF A PYOBJECT*


----

Example: ``PyList``
===================

.. code::

                 -----------
                 | PyList* |
                 -----------
                 |   ...   |
                 |   ptr   |
                 -----------
                     |
                     |
    -------------------------------------
    | PyObject* | PyObject* | PyObject* |
    -------------------------------------

         GUTS OF [1., "TWO", 3]


----

Consequences
============

1. Great flexibility (dynamic typing, memory management etc)
2. Great costs
    - boxing/unboxing
    - managed values (``sizeof(PyObject) == 16`` on my laptop)
    - memory fragmentation

----

Crossing the Frontier
=====================

Native code typically operate on unmanaged, unboxed value.

- First step when performing a native call is to unbox values
    1. It is **tedious**
    2. It is **costly**
    3. It requires **care**

----


Being less Tedious
==================

- ``ctypes`` for manual, explicit unboxing
- https://docs.python.org/3.6/c-api/ when explicit is better than implicit
- ``cffi``, ``dragonffi`` or even ``cython`` when high-level is better than low-level
- ``pybind11`` for C++ devs
- ``swig`` for 20th century lover

----

Being Careful
=============

Updates on unboxed value may not modify the boxed value!

.. code-block:: python

    def up(l):
        l[0] += 1


``!=``

.. code-block:: c

   void up(PyObject* obj) {
       PyObject* first = PyList_GetItem(obj, 0);
       long val = PyLong_AsLong(first);
       val += 1;
   }

----

Being less Costly
=================

.. code::



   ...




----

Being less Costly
=================

Unchecked conversion
--------------------

.. code-block:: C++

    long unbox(PyObject* obj) {
        // comment this test for more fun
        if(PyLong_Check(obj))
            return PyLong_AsLong(obj);
        else
            throw std::runtime_error("...");

----

Being less Costly
=================

Mixed mode
----------

- Only convert the objects whose unboxing is costly/frequent
- Cython's speciality!

Convert scarcely
----------------

- Avoid fine-grain interface, make sure heavy computations happen in a single function

----

Being less Costly
=================

Start unboxed
-------------

- A Python List starts as boxed => high conversion cost ``O(n)``
- A Numpy Array starts as unboxed => low conversion cost ``O(1)``

This is a trade-off!


----

Why?
====

*Managed, fine grain operation on Numpy array are costly*

----

Why?
====

Because that's just crossing the frontier, the other way around!

----

Avoiding Conversion
===================

Use a ``PyCapsule``!

.. code-block:: C

    PyObject* PyCapsule_New(void *pointer, const char *name, PyCapsule_Destructor destructor);

And live in the native world for ever

*you indeed need a capsule to go to Namek*

----

Illustration x2
===============

.. code-block:: python

    import ctypes
    # capsulefactory
    PyCapsule_New = ctypes.pythonapi.PyCapsule_New
    PyCapsule_New.restype = ctypes.py_object
    PyCapsule_New.argtypes = (ctypes.c_void_p,
                              ctypes.c_char_p,
                              ctypes.c_void_p)
    # load libm
    libm = ctypes.CDLL('/lib/.../libm.so.6')
    # extract the proper symbol
    cbrt = libm.cbrt
    # wrap it
    capsule = PyCapsule_New(cbrt,
                            'double(double)',
                            None)


----

Quizz '0
========

How many conversion happens?

.. code-block:: python

    #pythran export quizz0(int)
    def quizz0(obj):
        return obj + 1

----

Quizz '1
=========

How many conversion happens?

.. code-block:: python

    #pythran export quizz1(int [])
    def quizz1(obj):
        return obj + 1

----

Quizz '2
=========

How many conversion happens?

.. code-block:: python

    #pythran export quizz2(int list)
    def quizz2(obj):
        return [x + 1 for x in obj]

----

Quizz '3
=========

Any issue there?

.. code-block:: python

    #pythran export quizz3(int set)
    def quizz3(obj):
        obj.pop(1)

----

Quizz '4
=========

And there?

.. code-block:: python

    #pythran export quizz4(float64[])
    def quizz4(obj):
        obj[0] = 1.

----

Quizz '5
=========

How many conversion happen here?

.. code-block:: python

    #pythran export quizz5(float64(int32), int32)
    def quizz5(capsule, value):
        if value > 0:
            return capsule(value)
        else:
            return 0.

----

Ending words
============

Now you should be able to understand

- Some Numpy performance behavior
- Why it's challenging to write a JIT like PyPy
- The core idea behind Cython

And if you don't, it's time for questions :-)
