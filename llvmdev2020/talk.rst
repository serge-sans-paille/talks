:title: Building compiler extension for LLVM 10.0.1
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css

Intro
=====

How do you write a plugin for LLVM?

→ RTFM https://llvm.org/docs/WritingAnLLVMPass.html

----

After a dozen of minutes
========================

.. code-block:: c++

    [...]
    struct Hello : public FunctionPass {
        [...]
      bool runOnFunction(Function &F) override {
        [...]
      }
    };

    static RegisterPass<Hello> X(
        "hello",
        "Hello World Pass",
        false, false);

.. code-block:: console

    $ opt -load lib/LLVMHello.so \
          -hello < hello.bc > /dev/null

----

Building compiler extension for LLVM 10.0.1
===========================================

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover / Red Hat employee

**LLVM Developers Meeting — 8th of October 2020**

----

What about Using the pass from Clang?
=====================================

.. code-block:: c++

    static RegisterStandardPasses Y(
        PassManagerBuilder::EP_EarlyAsPossible,
        [](const PassManagerBuilder &Builder,
           legacy::PassManagerBase &PM) {
             PM.add(new Hello());
           });

.. code-block:: console

    $ clang -Xclang -load -Xclang lib/LLVMHello.so \
            -O1 hello.c -o /dev/null

----

What about Using the pass from new PM?
======================================

credits: https://github.com/banach-space/llvm-tutor/

.. code-block:: c++

    void run(Function &F) { [...] }

    // New PM implementation
    struct HelloWorld : PassInfoMixin<HelloWorld> {
      PreservedAnalyses run(Function &F,
                            FunctionAnalysisManager &) {
        run(F);
        return PreservedAnalyses::all();
      }
    };

    llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
      return {LLVM_PLUGIN_API_VERSION, "HelloWorld",
              LLVM_VERSION_STRING,
              [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback([...]);
              }};
    }

    extern "C"
    LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
    llvmGetPassPluginInfo() {
      return getHelloWorldPluginInfo();
    }


----

Mixing the two
==============

.. code-block:: c++

    // Legacy PM implementation
    struct LegacyHelloWorld : public FunctionPass {
      static char ID;
      LegacyHelloWorld() : FunctionPass(ID) {}
      bool runOnFunction(Function &F) override {
        /* >>>> */ run(F); /* <<<< */
        return false;
      }
    };

----

How do I ship my passes?
========================

1. Compatibility issues: a pass built for LLVM X is not compatible with LLVM Y

2. Usability issues: ``-Xclang -load -Xclang MyPass0.so`` is not super
   user-friendly

3. Forking llvm, building an in-tree pass:
   
   - heavier repo
   - less code locality
   - tied to a given LLVM version

----

The Polly case
========================

*a.k.a* a set of passes available both as in and out of tree passes

.. code-block:: console

    $ % git grep -i  polly origin/release/9.x | grep -E -v '(www)|(test)|(doc)|(:polly)' -c
    58

Intrusive in both ``cmake``, ``llvm`` and ``clang`` codebase :-/

What about generalizing the approach?

----

Turning out-of-tree passes in tree
==================================

A combination of CMake and C++ glue to provide:

- Built on top of plugin infrastructure
- Compatible with out-of-tree layout
- Configurable in/out of tree builds
- Compatible with componenets, ``llvm-config`` and ``libLLVM``

See ``llvm-project/llvm/examples/Bye/``

----

Change to existing extension
============================

.. code-block:: diff

    + #ifndef LLVM_BYE_LINK_INTO_TOOLS
      extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
      llvmGetPassPluginInfo() {
        return getByePluginInfo();
      }
    + #endif

----

CMake configuration
===================

.. code-block:: cmake

  add_llvm_pass_plugin(Bye
    Bye.cpp
    DEPENDS
    intrinsics_gen
   )


.. code-block:: console

    $ cmake -DLLVM_BYE_LINK_INTO_TOOLS=$(($RANDOM %2))

----

Introducing a new Component: Extension
======================================

Without Polly

.. code-block:: console

    $ llvm-config --components
    (...) extensions (...]

    $ llvm-config --libs extensions
    -lLLVMExtensions -lLLVMSupport -lLLVMDemangle

----

Introducing a new Component: Extension
======================================

With Polly and ``-DLLVM_BYE_LINK_INTO_TOOLS=OFF``

.. code-block:: console

    $ llvm-config --libs extensions
    -lLLVMExtensions -lLLVMSupport -lLLVMDemangle

With Polly and ``-DLLVM_BYE_LINK_INTO_TOOLS=ON``

.. code-block:: console

    $ llvm-config --libs extensions
    -lLLVMExtensions -lPolly -lPollyISL (...) -lLLVMDemangle

----

Interactions with libLLVM
=========================

``libLLVM10`` aggregates all components, so:

- When a project is linked in: it's part of ``libLLVM10``

- When a project is *not* linked in: it's *not* part of ``libLLVM10``

----

Back from the sixties: X Macro
==============================

from ``clang/lib/CodeGen/BackendUtil.cpp``

.. code-block:: c++

    #define HANDLE_EXTENSION(Ext) \
      llvm::PassPluginLibraryInfo get##Ext##PluginInfo();
    #include "llvm/Support/Extension.def"

   [...]

   void EmitAssemblyHelper::
   EmitAssemblyWithNewPassManager(...) {

   [...]

   #define HANDLE_EXTENSION(Ext) \
     get##Ext##PluginInfo().RegisterPassBuilderCallbacks(PB);
     #include "llvm/Support/Extension.def"

   [...]

   }


----

Concluding Words
================

Thanks to that work:

- Polly is no longer an exception, it is now an exemple
- It is *easier* to move *from out-of-tree to in-tree* passes
- Buildbot has been red on several occasion :-/
