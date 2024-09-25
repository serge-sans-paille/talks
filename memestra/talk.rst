:title: Gast, Beniget and Memestra
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Intro
=====

Python is a dynamic language

*Yet* people wants static checks

Could we develop static tooling for Python?


----

Prior Works
===========

``pyflakes``, ``pylint``, ``mypy`` …

Detect unused imports, variable shadowing, undefined variables, style issues,
type inference based on type hints…

----

Ar Tri Kompagnun
================

- ``gast``: abstraction of the ``ast`` package across Python versions

- ``beniget``: define - use chains computation for any Python program

- ``memestra``: deprecate use analysis for Python and Jupyter Notebooks

----

Dependency Graphs
=================

- ``gast`` is used by ``pythran``, ``tangent``, ``beniget``…

- ``beniget`` is used by ``pythran``, ``memestra``, ``transonic``

----

``memestra``
============

Original goal: detect use of deprecated definitions flagged with a decorator in Python modules

Extended goal: applicable to notebooks

----

Basic Example
=============

.. code-block:: python

    import decorator
    @decorator.deprecated
    def foo(): pass
    def bar():
        foo()
    foo()

    > python memestra.py test.py
    foo used at test.py:7:5
    foo used at test.py:9:1

----

Across Module Boundaries
========================

.. code-block:: python

    import some_module as Module
    def bar():
        Module.foo()  # foo is decorated as deprecated
        Module.bar()  # but bar isn't
    Module.foo()

    > python memestra.py test.py
    Module.foo used at test.py:5:5
    Module.foo used at test.py:8:1

----

Memestra API
============

.. code-block:: python

    memestra(file_descriptor : IO[str], decorator: Iterable[str])
        -> List[Tuple[str,str, int, int]]

Usable as an engine for a linter, for LSP integration…

----

Memestra Plugins
================

Plugins are registered based on the file extension and provide wrappers around
``memestra.memestra``.

E.g. Jupyter notebook handling:

1. Turn cells into plain Python (and record cell <> line mapping)

2. Call ``memestra.memestra`` on the stream

3. Use the mapping to report cell errors

----

Notebook Example
================

.. code-block:: shell

    >  memestra ./tests/misc/memestra_nb_demo.ipynb \
          --decorator decoratortest.deprecated
    some_module.foo used at Cell[0]:2:1
    some_module.foo used at Cell[0]:3:1
    some_module.foo used at Cell[2]:1:1

----

Potential LSP Applications
==========================

- More diagnostics: Dead code, unused identifiers?

- Goto declaration (~= def) / usage (~= uses)

----

Going Further
=============

- Handle IPython dialect (magics, auto variables…)

- Standard deprecation decorator with runtime behavior

- Report message from decorator string

- Handle class members

- Docstring/sphinx plugin
