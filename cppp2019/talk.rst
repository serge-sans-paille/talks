:title: Merci le Compilo
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css

Intro
=====

Il parait qu'avec le langage C++ on peut écrire du code

- haut niveau
- efficace
- portable
- adapté au matériel

Pourtant le langage est une hydre protozoaire... Donc

----

Merci le Compilo o/
===================

**Serge « sans paille » Guelton**

Apprendre à communiquer avec le meilleur ami du langage

**CPPP — 15 juin 2019**

----


Contexte
========

Environnement
*************

- ``clang`` top-of-tree
- RHEL7
- ``sq=https://raw.githubusercontent.com/azadkuh/sqlite-amalgamation/master/sqlite3.c``

*Houat else?*

.. image:: HouatCarte.png
    :class: bg
    :align: right


----

C++, ce language si rapide
==========================

.. code-block:: C++

    #include <iostream>
    int main(int argc, char** argv) {
      long s = 0;
      while (std::cin) {
        long tmp = 0;
        std::cin >> tmp;
        s += tmp;
      }
      std::cout << s << std::endl;
      return 0;
    }

----

Python, ce language si lent
===========================

.. code-block:: Python

    import sys
    print(sum(int(x)
              for x in sys.stdin.readlines()))


----


Ze Bench, Marc !
================

.. code-block:: sh

    $ seq 1000000 > numbers
    $ clang++ sum.cpp -o sum
    $ time ./sum < numbers
    0.61s user 0.01s system 94% cpu 0.659 total

    $ time python sum.py < numbers
    0.77s user 0.04s system 99% cpu 0.818 total

**L'utilisateur n'a pas précisé son intention**

----


Spécifier son intention
=======================

.. code-block:: sh

    $ clang++ -O2 sum.cpp -o sum
    $ time ./count < numbers
    0.34s user 0.00s system 99% cpu 0.348 total


----


Optimisation multi-critère
==========================


::

         #
         ##                           #
         ##                           ##
         ##            ##             ##
         ##            ##             ##
         ##            ##             ##
         ##    ##      ##             ##
         ##    ##      ##      #      ##
         ##    ##      ##      ##     ##
        PERF  DEBUG  ÉDITION  SÉCU  TAILLE




----


Performance
===========

« Je veux que le code généré soit efficace »

- ``-O0`` : pas d'optimisation
- ``-O1`` : :math:`\text{O1} = \frac{\text{O0} + \text{O2}}{2}`
- ``-O2`` : optimisation qui ne devraient jamais dégrader les perfs
- ``-O3`` : optimisation avec risque d'impact négatif sur les perfs
- ``-O4`` : :math:`\text{O3} = \text{O4}`

----


Debug
=====

« Je veux que le code généré soit facile à déverminer »

- ``-g`` : inclut les infos de debug
- ``-Og`` : ``== -O1 -g``

.. code-block:: sh

    $ curl $sq | clang -xc -c -g - -o sq.o
    $ objdump -h sq.o | grep debug
      #  name            size      ...
       9 .debug_str      00012b2d  ...
      10 .debug_abbrev   0000038d  ...
      11 .debug_info     0005056c  ...
      12 .debug_ranges   00000240  ...
      13 .debug_macinfo  00000001  ...
      14 .debug_pubnames 0000c73a  ...
      15 .debug_pubtypes 00001068  ...
      19 .debug_line     00073402  ...



----


Sécurité
========

« Je veux me protéger de moi-même »

- ``-D_FORTIFY_SOURCE=2``

.. code:: sh

    $ clang -xc -c -O2 - -S -emit-llvm -o \
        - -D_FORTIFY_SOURCE=2 << EOF
    #include <stdio.h>
    void foo(char *s) {
      printf(s, s);
    }
    EOF
    define void @foo(i8*) {
      %2 = tail call i32 (i32, i8*, ...) \
       @__printf_chk(i32 1, i8* %0, i8* %0)
      ret void
    }

----

Taille
======

« Je veux un binaire de petite taille »

- ``-Os`` : ``-O2`` avec des optimisations de taille
- ``-Oz`` : ``-Os`` avec plus d'optimisations de taille

.. code:: sh

    $ curl $sq|clang -xc - -O2 -c -o-|wc -c
    1488400
    $ curl $sq|clang -xc - -Os -c -o-|wc -c
    850696
    $ curl $sq|clang -xc - -Oz -c -o-|wc -c
    796976

----


Édition
=======

- ``-Wall`` : (presque) tous les avertissements
- ``-Werror[=...]`` : pour des soirées compil de folie
- ``-w`` : les vrais savent
- ``-Xclang -code-completion-at`` : *Intellisense*

.. code:: sh

    $ cat hello.cpp
    #include <iostream>
    int main(int argc, char**argv) {
      std::co
    $ clang++ -Xclang \
      -code-completion-at=hello.cpp:3:10 \
      -fsyntax-only hello.cpp
    COMPLETION: codecvt : codecvt<<#typename _InternT#>, <#typename _ExternT#>, <#typename _StateT#>>
    COMPLETION: codecvt_base : codecvt_base
    ...
    COMPLETION: cout : [#ostream#]cout



----


Faire les con (promis)
======================

*Debug vs Size*

- ``-g1``
- ``-g2``
- ``-g3``
- alt. : ``objcopy --only-keep-debug``
- alt. : ``objcopy --compress-debug-sections``
- ``-fdebug-macro`` : attention à l'explosion en taille!

----

Impact du niveau de debug sur la taille
=======================================

.. code:: sh

    $ for g in 1 2 3 ""
      do
      printf "-g$g: \t" && \
      curl $sq|clang -c -O2 -g$g -xc - -o-|\
        wc -c
      done
    -g1   : 3168632
    -g2   : 7025488
    -g3   : 7025488
    -g    : 7025488


**Bonus** : ``-fdebug-macro -g : 7167752``

----

Impact du niveau d'optimisation sur temps de compilation
========================================================

.. code:: sh

    $ for O in 0 1 2 3
      do
      /usr/bin/time -f "-O$O: %e s" \
        clang sqlite3.c -c -O$O
      done
    -O0: 22.15 s
    -O1: 24.02 s
    -O2: 22.68 s
    -O3: 22.36 s

*exercice* : proposez une explication


----

Faire les con (promis)
======================

*Précision vs Performance*

- ``-ffp-contract=fast|on|off`` : floating-point expression contraction
- ``-ffast-math`` : associativité + pas de NaN
- ``-freciprocal-math`` : optimise la division par un littéral
- ``-Ofast`` : :math:`\text{-O3} + \text{-ffast-math} = \text{-Ofast}`

.. code:: sh

    $ clang -xc - -o- -S -emit-llvm -O2 \
         -freciprocal-math << EOF
    double rm(double x) {
      return x / 10.;
    }
    EOF
    define double @rm(double) {
      %2 = fmul arcp double %0, 1.000000e-01
      ret double %2
    }

----

Variation
=========

.. code:: sh

    $ clang -xc++ - -o- -S -emit-llvm \
      -Ofast << EOF
    #include <numeric>
    #include <vector>
    using namespace std;
    double acc(vector<double> const& some)
    {
      return accumulate(
               some.begin(),
               some.end(),
               0.);
    }
    EOF
    ...
    %95 = fadd fast <2 x double> %94, %93
    ...

**Pourquoi** la vectorisation est-elle légale ici ?


----


Faire des compromis
===================

*Portabilité vs Performance*

- ``-march=native`` : spécialise pour la machine hôte
- ``-mavx`` : autorise ce jeu d'instruction

.. code:: sh

    $ clang++ -O2 -S -o- -march=native \
      -ffp-contract=fast << EOF
    double fma(double x, double y, double z) {
      return x + y * z;
    }
    EOF
    ...
    vfmadd213sd %xmm0, %xmm2, %xmm1


----


Faire des compromis
===================

*Performance vs Sureté*

- ``-fsanitize=address`` : instrumente les accès mémoires 
- ``-fsanitize=memory`` : trace les accès à des valeurs non initialisées
- ``-fsanitize=undefined`` : trace les comportements indéfinis
- ``-fsanitize=thread`` : détecte les *data race*

.. code-block:: c++

    // mem.cpp
    #include <memory>
    double x(std::unique_ptr<double> y) {
      return *y;
    }

*QUIZZ* : combien de déréférencements ?

----

...
===

.. code-block:: sh

    $ clang++ -fsanitize=address mem.cpp -S -emit-llvm -o- -O2

.. code-block:: llvm

    ...
    %h = getelementptr inbounds %"class.std::unique_ptr", %"class.std::unique_ptr"* %y, i64 0, i32 0, i32 0, i32 0, i32 0, i32 0
    %1 = ptrtoint double** %h to i64
    %2 = lshr i64 %1, 3
    %3 = add i64 %2, 2147450880
    %4 = inttoptr i64 %3 to i8*
    %5 = load i8, i8* %4
    %6 = icmp ne i8 %5, 0
    br i1 %6, label %7, label %8

    ; <label>:7:
    call void @__asan_report_load8(i64 %1)
    call void asm sideeffect "", ""()
    unreachable

    ; <label>:8:
    %9 = load double*, double** %h, align 8, !tbaa !2


----

``from __future__ import``
==========================

- ``-std=c++11/14/17`` : choisir son standard préféré
- ``-std=gnu11/...`` : *pick your poison*
- ``-fcoroutines-ts`` : visions du sérum

.. code:: sh

    $ clang --autocomplete=-std=,
    ...
    c++2a
    ...
    cuda
    ...
    gnu1x
    ...
    iso9899:2011



----


Controler le compilateur avec plus de précision
===============================================

*Security*

- ``-fstack-protector`` : ajoute un petit 🐦 sur la pile
- ``-fstack-protector-strong`` : *itou*, sur plus de fonction
- ``-fstack-protector-all`` : *itou*, sur toutes les fonctions
- ``-fsanitize=safe-stack`` : scinde la pile en deux
- ``-fsanitize=cfi`` : vérifie un invariant sur le flot de contrôle

----

...
===

.. code-block:: sh

    $ clang -O2 -fstack-protector-all -S \
    -o- -xc++ - << EOF
    #include <array>
    using namespace std;
    auto access(array<__int128_t, 10> a,
                unsigned i)
    {
      return a[i];
    }
    EOF
    ...
    cmpq    (%rsp), %rcx
    jne .LBB0_2
    popq    %rcx
    retq
    .LBB0_2:
    callq   __stack_chk_fail

----

Optimisation Top Tier (1/2)
===========================

**P** rofile **G** uided **O** ptimisation

1. Compiler avec ``-fprofile-generate``
2. Exécuter son code sur des cas représentatifs
3. Recompiler son code avec ``-fprofile-use``

⇒ Influe sur

- Le placement des fonctions
- Le placement des blocs de base
- Diverses passes d'optimisation (*unroll*, *inlining*...)

----

Optimisation Top Tier (2/2)
===========================

**L** ink **T** ime **O** ptimisation

- ``-flto=full`` : perf > temps de compil
- ``-flto=thin`` : temps de compil > perf

.. code-block:: sh

    $ echo 'foo() { return 0;}' | \
      clang -flto -O2 -xc - -c -ofoo.o
    $ file foo.o
    foo.o: LLVM bitcode


----

Quelques leviers d'optimisation supplémentaires
===============================================

*Rubrique à brac*

- ``-mllvm -inline-threshold=n`` : contrôle l'expansion de procédure
- ``-mllvm -unroll-threshold=500`` : contrôle le déroulement de boucle
- ``-O3 -mllvm -polly`` : active les optimisations polyédriques
- ``-fwhole-program-vtables`` : essaye de simplifier les tables virtuelles


----


Restons ``#pragma`` tic (1/3)
=============================

Pour ne pas optimiser certaines fonctions (*e.g.* pour du debug) :

.. code::

    #pragma clang optimize off
    ...
    #pragma clang optimize on



----


Restons ``#pragma`` tic (2/3)
=============================

Pour bien optimiser ses boucles :

.. code-block:: c++

    #pragma clang loop unroll(enable|full)
    #pragma clang loop unroll_count(8)
    #pragma clang loop distribute(enable)
    #pragma clang loop vectorize_width(4)

----


Restons ``#pragma`` tic (3/3)
=============================

Pour la précision :

.. code-block:: c++

    #pragma clang fp contract(fast)

Pour l'éditeur de lien :

.. code::

    #pragma clang section bss="myBSS" \
                  data="myData" \
                  rodata="myRodata" \
                  text="myText"

----


Medley
======

.. code::

    #if __has_attribute(always_inline)

    #if __has_builtin(__builtin_trap)

    #if __has_include("myinclude.h")

    __clang__

    typedef float float4 \
     __attribute__((ext_vector_type(4)));

     __fp16

    __is_pod (GNU, Microsoft)

----

Aide au développement
=====================

- ``--analyze`` : effectue une analyse poussée du code
    - Augmente les *temps de compilation*
    - *faux positifs* possibles !

- ``scan-build make`` : automatise l'étape du dessus pour un projet


----

...
===

.. code-block:: c++

    // d.cpp
    double x(double* y, bool cond) {
      if(cond)
        delete y;
      bool ncond = !cond;
      if(ncond)
        return 1.;
      else
        return *y;
    }

----

...
===

.. code-block:: sh

    $ clang++ --analyze -Xanalyzer -analyzer-output=text d.cpp

    d.cpp:8:12: warning: Use of memory after it is freed
        return *y;
               ^~
    d.cpp:2:6: note: Assuming 'cond' is not equal to 0
      if(cond)
         ^~~~
    d.cpp:2:3: note: Taking true branch
      if(cond)
      ^
    d.cpp:3:5: note: Memory is released
        delete y;
        ^~~~~~~~
    d.cpp:5:3: note: Taking false branch
      if(ncond)
      ^
    d.cpp:8:12: note: Use of memory after it is freed
        return *y;


----

Pour comprendre...
==================

Un compilateur un peu lent :

- ``-ftime-report`` : rapport détaillé par passe de compil

Une optimisation qui aurait mal tournée

- ``-Rpass=inline``
- ``-Rpass=unroll``
- ``-Rpass=loop-vectorize``


----

...
===

.. code-block:: sh

    $ { clang -xc++ - -c \
      -O2 -Rpass=inline << EOF
    #include <numeric>
    #include <vector>
    using namespace std;
    double acc(vector<double> const& some)
    {
      return accumulate(
               some.begin(),
               some.end(),
               0.);
    }
    EOF
    } 2>&1 | c++filt
    ...
    ... remark: __gnu_cxx::__normal_iterator<double const*, std::vector<double, std::allocator<double> > >::__normal_iterator(double const* const&) \
    ... inlined into std::vector<double, std::allocator<double> >::begin() const with cost=-40 (threshold=337) [-Rpass=inline]

----

Une exploration sans fin
========================

.. code-block:: sh

    $ clang --autocomplete=- | wc -l
    2847

En compilation comme dans la vie, il faut une forme de **balance**

.. figure:: balance.jpg
    :width: 300px
    :align: right

    *credit: Wotc / Mark Poole*
