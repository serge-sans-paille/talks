:title: Tooling for Static Analysis of Python Programs
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Intro
=====

**Python** and **Static Analysis**…

Should be Python **or** Static Analysis!


----

Tooling for Static Analysis of Python Programs
==============================================

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover / RedHat employee

**EuroPython'20 — 24th of february 2020**


----

Prelude
=======

For **7** years or so, I've playing with the **Pythran** project, building a DSL
embedded in Python.

I've been building a bunch of tools in the process:

- ``gast``, a Python Abstract Syntax Tree abstraction
- ``beniget``, an analysis that builds Use-Def Chains (more about this later)

----

``gast``
========

    ``['gast]`` *f* - good time girl


Tool started in 2016 and presented at PyConFR 2016

Originally meant to ease the transition of Pythran to Python3

Core component of AST manipulation:

> Downloads last day: 155,526

*(don't trust numbers)*

----

A Simple Expression Across the Ages
===================================

.. code-block:: console

    > python -c "import ast; print(ast.dump(ast.parse('a[1, ...]')))"
    ...

----

In Python2
==========

.. code-block::

    Module(body=[
        Expr(value=
                Subscript(value=Name(id='a', ctx=Load()),
                          slice=
                            ExtSlice(dims=
                                    [Index(value=Num(n=1)), Ellipsis()]),
                          ctx=Load()))
    ])

----

In Python3.6
============

.. code-block::

    Module(body=[
        Expr(value=
                Subscript(value=Name(id='a', ctx=Load()),
                          slice=
                            Index(value=Tuple(elts=[Num(n=1), Ellipsis()],
                                  ctx=Load())),
                          ctx=Load()))
    ])

----

In Python3.9
============

.. code-block::

    Module(body=[
        Expr(value=
                Subscript(value=Name(id='a', ctx=Load()),
                          slice=
                            Tuple(elts=[Constant(value=1), Constant(value=Ellipsis)],
                                  ctx=Load()),
                          ctx=Load()))
    ], type_ignores=[])

----

With ``gast``
=============


Whatever the Python version

.. code-block:: console

    > python -c "import gast as ast; \
                 print(ast.dump(ast.parse('a[1, ...]')))"
    Module(body=[Expr(value=Subscript(value=Name(id='a', ctx=Load(),
    annotation=None, type_comment=None), slice=Tuple(elts=[Constant(value=1,
    kind=None), Constant(value=Ellipsis, kind=None)], ctx=Load()),
    ctx=Load()))], type_ignores=[])

----

Tradeoffs
=========

1. Slightly more verbose than Python 3.9 because of Python2 compatibility
2. Extra translation step, slight performance impact when parsing the world

----

``beniget``
===========

    ``['beniget]`` *adj.* - blessed

Compute use-def chains for Python

Foundation of several ``Pythran`` analyses

----

About Use-Def chains
====================

From https://en.wikipedia.org/wiki/Use-define_chain

    A Use-Definition Chain (UD Chain) is a data structure that consists of a use, U,
    of a variable, and all the definitions, D, of that variable that can reach that
    use without any other intervening definitions

----

Typical usage
=============

A def *without* a use means a useless def:

- Detect unused imports
- Detect useless assignment

*tip*: ``_`` is often used to state a useless assignment


----

Being pedantic
==============

In Python, one does **not** assign a value to a variable,

One sets an **identifier** on a **value**.

----

Tricky cases (0)
================

.. code-block:: python

    for i in l:
        if i:
            print(j)
        else:
            j = i

Is the ``print`` statement faulty?

----

Tricky cases (1)
================

.. code-block:: python

    def foo():
        global x
        x = 1

    def bar():
        print(x)

Does calling ``bar`` raise an exception?

----

Tricky cases (2)
================

.. code-block:: python

    x = 1
    for x in y:
        pass
    print(x)

Which value is ``x`` bound to?

----

Application: a simple linter
============================

.. code-block:: python

    for local_def in self.defuses.locals[node]:
        if local_def.users():
            continue

        if local_def.name() == "_":
            continue  # typical naming by-pass

        # [...]

        print(
            "W: '{}' is defined but not used at {}:{}:{}".format(
                local_def.name(),
                self.filename,
                location.lineno,
                location.col_offset,
            )
        )

----

Limitations: did you say static?
================================

.. code-block:: python

    eval("expr")
    globals()[name] = 1

And as an extension, any method call…

----

``memestra``
============

    ``['memestra]`` *adv.* - Oh, please!

``Memestra`` checks code for places where deprecated functions are called.

How would you do that after that talk?

----

Finding deprecated usage
========================

Simple!

1. Track a given decorator usage
2. Track usage of decorated definitions
3. Print


----

Example
=======

.. code-block:: console

    > cat test.py
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

Cross-Module Exploration
========================

When we import a function from a module, is that function deprecated?

→ Statically resolve imports, and walk them recursively

→ Quickly end-up parsing hundreds of Python package

→ Use a caching mechanism

----

Advertising Deprecated Usage
============================

.. code-block:: console

    > pip install deprecated

.. code-block:: python

    from deprecated import deprecated
    @deprecated(reason="You should use another function")
    def some_old_function(x, y):
        pass

----

Limitations: Did You Say Typing?
================================

.. code-block:: python

    class Foo:

        @deprecated
        def foo():
            pass

    def bar(f):
        return f.foo()  # Is this call deprecated?

----

Postlude
========

- Python isn't meant for static analysis
- We can still get some tooling —with known limits
- We can share the tooling
- Perfect for embded DSLs
