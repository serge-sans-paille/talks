:title: Discover Pythran through 10 code samples
:data-transition-duration: 150
:skip-help: true
:slide-numbers: true
:css: font.css


Discover Pythran through 10 code samples
========================================

**Serge « sans paille » Guelton**

Compiler Engineer / Wood Craft Lover

**EuroScipy — 31st August 2022**

----

#0 Template Slide
=================

.. code-block:: Python

    #pythran export hello()
    def hello():
      print("hello world")

.. code-block:: shell

    $ pythran hello.py

----

#1 From `scipy's _hypotests_pythran.py <https://github.com/scipy/scipy/blob/main/scipy/stats/_hypotests_pythran.py>`_
=====================================================================================================================

.. code-block:: Python

    #pythran export _Aij(float[:,:], int, int)
    #pythran export _Aij(int[:,:], int, int)
    def _Aij(A, i, j):
      """Sum of upper-left and lower right blocks of
         contingency table."""
      # See `somersd` References [2] bottom of page 309
      return A[:i, :j].sum() + A[i+1:, j+1:].sum()

----

#2 From `pyfiberamp's inner_loop_functions.py <https://github.com/Jomiri/pyfiberamp/blob/master/pyfiberamp/dynamic/inner_loop_functions.py>`_
=============================================================================================================================================

.. code-block:: Python

    #pythran export apply_reflection(float[][], int[], int[], float[],
    #                                int)
    def apply_reflection(P, source_idx, target_idx, R, n_forward):
      """Modifies `P` in-place."""
      for _source_idx, _target_idx, _R in zip(source_idx,
                                              target_idx,
                                              R):
        if _source_idx < n_forward:
          P[_target_idx, -1] += _R * P[_source_idx, -2]
        else:
          P[_target_idx, 0] += _R * P[_source_idx, 1]

----

#3 From `HandTracking's transforms.py <https://github.com/gianpy15/HandTracking/blob/997bcd834f20f9bfdf2053f29e5484c84441c98a/source/library/geometry/ptran/transforms.py>`_
============================================================================================================================================================================

.. code-block:: Python

    import numpy as np

    ...

    #pythran export get_rotation_angle_around_axis(float[],
    #                                              float[],
    #                                              float[])
    def get_rotation_angle_around_axis(axis, p1, p2):
      v1 = normalize(p1 - axis * np.dot(p1, axis))
      v2 = normalize(p2 - axis * np.dot(p2, axis))
      cross = np.cross(v1, v2)
      if np.dot(axis, cross) > 0:
        return np.arccos(np.dot(v1, v2))
      ...


----

#4 From `spectralDNS's pythran_maths.py <https://github.com/spectralDNS/spectralDNS/blob/master/spectralDNS/optimization/pythran_maths.py>`_
============================================================================================================================================

.. code-block:: Python

    #pythran export _mult_K1j(float64[:], float64[:],
    #                         complex128[:, :, :], complex128[:, :, :, :])
    def _mult_K1j(Ky, Kz, a, f):
      for i in range(a.shape[0]):
        for j in range(a.shape[1]):
          for k in range(a.shape[2]):
            f[0, i, j, k] = 1j * Kz[k] * a[i, j, k]
            f[1, i, j, k] = -1j * Ky[j] * a[i, j, k]
      return f


----

#5 From `obvz's pythran_funcs.py <https://github.com/swhalemwo/obvz/blob/master/layout_optim/pythran_funcs.py>`_
================================================================================================================

.. code-block:: Python

    #pythran export pythran_ovlp(float64[:,:])
    def pythran_ovlp(pos):
      nbr_nds = int(pos.shape[0]/2)
      nbr_pts = pos.shape[0]

      d = pos[:,np.newaxis] - pos[np.newaxis,:]

      h, w = d.shape
      # nrows/ncols : size of each cell -> 2
      nrows = ncols = 2

      res_ar = d.reshape(int(h/nrows), nrows, -1, ncols)
      ...


----

#6 From `scikit-image's _hessian_det_appx_pythran.py <https://github.com/scikit-image/scikit-image/blob/main/skimage/feature/_hessian_det_appx_pythran.py>`_
============================================================================================================================================================

.. code-block:: Python

    #pythran export _hessian_matrix_det(float64[:,:], float or int)
    def _hessian_matrix_det(img, sigma):
      ...
      size = int(3 * sigma)
      height, width = img.shape
      s2 = (size - 1) // 2
      s3 = size // 3
      l = size // 3
      w = size
      b = (size - 1) // 2
      out = np.empty_like(img, dtype=np.float64)
      w_i = 1.0 / size / size
      ...


----

#7 From `scikit-image's brief_pythran.py <https://github.com/scikit-image/scikit-image/blob/main/skimage/feature/brief_pythran.py>`_
====================================================================================================================================

.. code-block:: Python

    import numpy as np

    #pythran export _brief_loop(float32[:,:] or float64[:,:], uint8[:,:],
    #                           int64[:,2], int32[:,2], int32[:,2])
    def _brief_loop(image, descriptors, keypoints, pos0, pos1):
      for p in range(pos0.shape[0]):
        pr0, pc0 = pos0[p]
        pr1, pc1 = pos1[p]
        for k in range(keypoints.shape[0]):
          kr, kc = keypoints[k]
          if image[kr + pr0, kc + pc0] < image[kr + pr1, kc + pc1]:
            descriptors[k, p] = True

----

#8 From `scipy's _matfuncs_sqrtm_triu.py <https://github.com/scipy/scipy/blob/main/scipy/linalg/_matfuncs_sqrtm_triu.py>`_
==========================================================================================================================

.. code-block:: Python

    # pythran export within_block_loop(float64[:,:], float64[:,:],
    #                                  (int, int) list, intp)
    # pythran export within_block_loop(complex128[:,:], complex128[:,:],
    #                                  (int, int) list, intp)

    def within_block_loop(R, T, start_stop_pairs, nblocks):
      for start, stop in start_stop_pairs:
        for j in range(start, stop):
          for i in range(j-1, start-1, -1):
            s = 0
            if j - i > 1:
                # s = R[i, i+1:j] @ R[i+1:j, j]
                for k in range(i + 1, j):
                    s += R[i, k] * R[k, j]

            denom = R[i, i] + R[j, j]
            num = T[i, j] - s
            if denom != 0:
                R[i, j] = (T[i, j] - s) / denom
            elif denom == 0 and num == 0:
                R[i, j] = 0
            else:
                raise RuntimeError('failed to find the matrix square root')

----

#9 From `scikit-image's setup.py <https://github.com/scikit-image/scikit-image/blob/main/setup.py>`_
====================================================================================================

.. code-block:: Python

    from numpy.distutils.command.build_ext import build_ext as npy_build_ext
    # ...
    from pythran.dist import PythranBuildExt as pythran_build_ext
    # ...

    class ConditionalOpenMP(pythran_build_ext[npy_build_ext]):

      def can_compile_link(self, compile_flags, link_flags):
        ...

      def build_extensions(self):
        ...
        super(ConditionalOpenMP, self).build_extensions()

----

#10 From `scipy's meson.build <https://github.com/scipy/scipy/blob/main/scipy/optimize/meson.build>`_
=====================================================================================================

.. code-block:: Python

    if use_pythran
      _group_columns = custom_target('_group_columns',
        output: ['_group_columns.cpp'],
        input: '_group_columns.py',
        command: [pythran, '-E', '@INPUT@', '-o',
                  '@OUTDIR@/_group_columns.cpp']
      )

      _group_columns = py3.extension_module('_group_columns',
        [_group_columns],
        cpp_args: [
          '-Wno-unused-function', '-Wno-unused-variable',
          '-Wno-deprecated-declarations',
          '-Wno-cpp', '-Wno-int-in-bool-context'
        ] + cpp_args_pythran,
        include_directories: [incdir_pythran, incdir_numpy],
        install: true,
        subdir: 'scipy/optimize'
      )


----

Concluding Words
================

- Documentation: https://pythran.readthedocs.io/
- PyPI: https://pypi.org/project/pythran/
- Github: https://github.com/serge-sans-paille/pythran
- IRC: #pythran on https://oftc.net
- flame and praise: serge.guelton@telecom-bretagne.eu
