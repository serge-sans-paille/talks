:title: UniCode Review
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


UniCode Review
==============

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover / RedHat employee

*Friday'Con — 25 March 2022*

.. image:: goblin_tinkerer.jpg
    :width: 400

----

Please
======

Read:

- https://trojansource.codes/
- https://certitude.consulting/blog/en/invisible-backdoor/
- https://developers.redhat.com/articles/2022/01/12/prevent-trojan-source-attacks-gcc-12
- https://unicode.org/reports/tr36/

----

Unicode
=======

=== === ===
 ដ   ἴ   ಣ
=== === ===
 ➫   ✞   ⚫
 ば  云  Ж
=== === ===

----

But also
========

Hangul filler, U+3164: ``ㅤ``

Alveolar Click, U+01C3: ``ǃ``

Zero-width Space, U+200B: ``​``

----

Unicode properties
==================

.. code::

    [code];[name];[gc];[cc];[bc];[decomposition];;;[nv];[bm];[alias];;;;

'bc' = bidi (bidirectional) category [L, R etc]

'bm' = bidi mirrored [N or Y]

----

``U+0068``
==========

.. code::

      Age                   1.1
      ASCII                 Yes
    * Bidi_Class            Left_to_Right
    * Bidi_Mirrored         No
      General_Category      Letter

see also:

- cyrillic small letter shha U+04BB ``һ``
- planck constant: U+210E ``ℎ``

----

``U+0028``
==========

.. code::

      Age                   1.1
      ASCII                 Yes
    * Bidi_Class            Other_Neutral
    * Bidi_Mirrored         Yes
    * Bidi_Mirroring_Glyph  )
    * Bidi_Paired_Bracket   )
      Block                 Basic_Latin
      General_Category      Open_Punctuation

----

``U+05D0``
==========

.. code::

      Age                   1.1
      ASCII                 No
    * Bidi_Class            Right_to_Left
    * Bidi_Mirrored         No
      General_Category      Letter

----

Boustrophedon
=============

.. code::

    ⇒⇒⇒⇒⇒⇒⇒⇒⇒⇒↴
    ↶←←←←←←←←←↵
    ↳⇒⇒⇒⇒⇒⇒⇒⇒⇒↴
    .←←←←←←←←←↵


----

Some Bidi characters
====================


=====   ==========  ===========
Abbr. 	Code Point 	Description
=====   ==========  ===========
RLO 	U+202E 	    Force following characters to be treated as strong right-to-left characters.
LRI 	U+2066 	    Treat the following text as isolated and left-to-right.
PDI 	U+2069 	    End the scope of the last LRI, RLI, or FSI.
=====   ==========  ===========

----

UniCode Comment
===============

.. code::

    /* <U+0x202E> } <U+0x2066> if (isAdmin) <U+0x2069> <U+0x2066> begin admins only */


1. **Display** ``/*``
2. **Change ordering** to *right-to-left*
3. **Display** ``}`` (from the right)
4. **Set ordering** to *left-to-right*
5. **Display** ``if (isAdmin)`` (from the right)
6. **Pop** state ``4.``
7. **Set ordering** to *left-to-right*
8. **Display** ``begin admins only */`` (from the right)

.. code::

    /* begin admins only */ if (isAdmin) {

----

BiDi algorithm
==============

See http://www.unicode.org/reports/tr9

Important notes:

- Applied a the scope of the paragraph
- Tolerates the bidi stack to be non empty at the end of a paragraph


----

Detecting invalid BiDi (0)
==========================

Warn about Bidi characters in

- comments
- string literals

----

Detecting invalid BiDi (1)
==========================

Invariant: before the closing ``*/`` we must be back to the initial state

Pros:

- Detect the *trojan source* attack
- Anything can happen between ``/*`` and ``*/``

Cons:

- Require to implement (part of) the BiDi logic in the compiler
- Require to parse comments

----

Implementations
===============

In GCC: ``-Wbidi-chars``, see https://godbolt.org/z/MM3na11rj

In clang: not supported

In clang-tidy:  ``misc-misleading-bidirectional``, see `github rendering <https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/test/clang-tidy/checkers/misc-misleading-bidirectional.cpp>`_


----

Ordering, again
===============

.. code::

    א = ג
    ;

What's assigned to what?

----

Detection
=========

In clang-tidy: misc-misleading-identifier

----

Homoglyph, again
================

.. code::

    һ = 1
    h = 2
    print(һ)

What's printed?

----

Detection
=========

RTFM: http://www.unicode.org/reports/tr39/#def-skeleton

In gcc12: ``-Whomoglyph``

In clang-tidy: https://reviews.llvm.org/D112916

----

IANAL
=====

I am Not a Linguist, but...

code ≠ text

compiler ≠ renderer


----

♥ Security ♥
============

- Not really a compiler vuln 🔒
- Scanning the whole RedHat codebase… ø

**But**

- An interesting task to work on ☺
- In vim: Ctrl+V u 





