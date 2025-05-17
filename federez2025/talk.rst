:title: Un an de nettoyage du système de build de Firefox
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css

----

.. image:: jack.jpg
   :width: 1000px

----

Un an de nettoyage du système de build de Firefox
=================================================

**Serge « sans paille » Guelton**

Ingénieur en Compil' / Conteur / Employé Mozilla

**Federez — 17 Mai 2025**

----

.. image:: AncestralRecall_2048x2048.jpg
   :width: 1000px

----

Plongeon dans le passé
======================

- **31 mars 1998** : Libération du code source de Netscape

.. code-block:: sh

    $ curl /ftp.mozilla.org/pub/mozilla/source/mozilla-19980331-unix.tar.gz
    $ tar xf mozilla-19980331-unix.tar.gz
    $ ls ns
    ... Makefile ... README ... unxbuild.txt ... build/

Ni ``./configure``, ni ``autoconf``.

``build/`` contient un répertoire ``CVS`` ``:-)``

----

Avec un autre outil
===================

.. code-block:: sh

   $ git tag
   ... FIREFOX_RELEASE_30_END ..
   $ git checkout FIREFOX_RELEASE_30_END
   $ ls
   ... mach ... Makefile.in ... build/moz.configure

c'est bien trop récent !

----

.. code-block:: sh

   $ git log mach | tail
   commit 0f8e4c10e371883b55c99bb82a69fad301ce6e1f
   Author: Gregory Szorc <gps@mozilla.com>
   Date:   Wed Sep 26 09:43:54 2012 -0700

       Bug 751795 - Part 1: mach, the new frontend to
         mozilla-central; r=jhammel

----

.. code-block:: sh

   $ git ls-tree 0f8e4c10e371883b55c99bb82a69fad301ce6e1f
   ... aclocal.m4 ... allmakefiles.sh ... configure.in

----

Fin de l'enquète
================

1. Un système basé uniquement sur Make
2. Autoconf + Make
3. Système hybride ( autoconf | moz.configure ) + Make
4. Le présent

----

Chesterton’s Fence
==================

    Do not remove a fence

    until you know

    why it was put up

    in the first place.

----

Supprimons Autoconf ?
=====================

- Autoconf est un super outil dans un monde centré sur \*nix
- Mozilla doit être (cross-)compilé vers :

    OSX, Android, Linux, Windows
    
    x
    
    arm32 aarch64 x86 x86_64

----

Pourquoi changer ?
==================

- Faire plus propre n'est pas toujours suffisant !

- Trouver d'autres métriques d'améliorations

----

Bug 1247781
===========

(9 years ago)

Our configure script is slow and hard to maintain. We'd like to convert it to a
Python script

(3 days ago)

Wait, there's no configure script anymore. It's time to close this bug.

----

Approche globale
================

1. Sauter le pas d'un coup ?
2. Faire des petits pas ?
    1. Reproduire le comportement précédent ?
    2. En profiter pour dégager les reliquats ?


----

.. image:: UrzaTowerFall_2048x2048.jpg
   :width: 1000px


----

Bug 1882025
===========

Remove duplicate ``is_gcc`` and ``building_with_gcc`` checks from ``toolchain.configure``

----

.. image:: UrzaTowerIsland_2048x2048.jpg
   :width: 1000px

----

Bug 1915837
===========

Remove unused reference to ``PYTHON3_VERSION`` from ``configure``

Unused since ``Bug 1755530``

----

.. image:: UrzaTowerMountain_2048x2048.jpg
   :width: 1000px

----

Bug 1904612
===========

Move ``HOST_OPTIMIZE_FLAGS`` from ``old-configure`` to ``moz.configure``

With two changes:

- no longer honors ``HOST_OPTIMIZE_FLAGS`` environment variable, which it previously did but without advertising it
- the value is now based on the compiler type and not the target

----

.. image:: UrzaTowerSpring_2048x2048.jpg
   :width: 1000px

----

Bug 1883782
===========

Move ``pthread`` checks from ``old-configure`` to ``moz.configure``

- Get rid of ``-pthreads`` because (according to gcc info page) it's only
  there on solaris and as an alias to ``-pthread``.
- ``-D_REENTRANT`` is always defined by gcc and clang when -pthreads is on.
- ``-D_THREAD_SAFE`` is only defined on AIX by clang
- ``libpthreads`` is an AIX thing, I assume we can remove it too.

----

.. image:: Balance1993_2048x2048.jpg
   :width: 1000px

----

Tester l'intestable
===================

Firefox est construit par Mozilla, mais aussi par

- des packageurs (Debian, RedHat, BSD…)
- des projets tiers (LibreWorlf, IceCat…)
- des particuliers

Avec plein de configurations différentes !

.. code-block:: sh

   $ ./mach configure -- --help | wc -l
   436

----

The Scream Test
===============

    If you see something
    
    and you don’t know what it does,
    
    remove it
    
    and see if anyone screams.

----

Dette Technique
===============

Les bons choix d'hier ne sont pas forcément les bons choix de demain

Comme en histoire : ne pas juger une époque avec notre regard contemporain

----

Rince and Repeat?
=================

Comment approcher positivement le découplage du système de build vis-à-vis de
GNU make ?


----

.. image:: BirdsofParadise_2048x2048.jpg
   :width: 1000px
