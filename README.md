## ARAP
To change the mesh, change the path in the ARAP class "init" method.

Features: ARAP + parallelization

### Videos
https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/one_point.mp4

https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/two_points.mp4

https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/permanent.mp4

https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/wave.mp4

https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/tet.mp4

https://github.com/brown-cs-224/arap-wiedmann-trey/blob/master/readme-videos/large_mesh.mp4

### Implementation Locations

- [Building L Matrix](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L210)
- Apply constrains [here](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L219) and [here](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L129)
- [Precompute the decomposition of the L matrix](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L236)
- [Determine the best-fit rotations](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L68)
- [Optimize the positions](https://github.com/brown-cs-224/arap-wiedmann-trey/blob/8cb15846ae3b0fb15cac3b028c27e932f07e2f6c/src/arap.cpp#L99)

### Collaboration/References
Worked alone. I referenced DeepSeek for some help with Eigen, linear algebra, and general C++ syntax.

### Known Bugs
None
