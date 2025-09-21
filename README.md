## ARAP - As Rigid as Possible

An implementation of the paper [As-Rigid-As-Possible Surface Modeling](https://igl.ethz.ch/projects/ARAP/arap_web.pdf) completed for CS2240: Advanced Computer Graphics, at Brown University.

https://github.com/user-attachments/assets/06302587-92f5-4a70-b8f4-be2dcddb27ea

https://github.com/user-attachments/assets/14e355e8-6ac2-4e91-a099-4c4f285c6632


### Implementation

- [Building L Matrix](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L210)
- Apply constrains [here](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L219) and [here](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L129)
- [Precompute the decomposition of the L matrix](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L236)
- [Determine the best-fit rotations](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L68)
- [Optimize the positions](https://github.com/wiedmann-trey/arap/blob/77e369cbf97045048ab100e3341a4926d57f0a2c/src/arap.cpp#L99)

### Building and running the project
Make sure to run "git submodule update --init" after cloning. The project can be built in QT Creator. Set the working directory to the base directory of the project before running.
