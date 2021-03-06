
![CI Windows](https://github.com/makra89/Visual-Odometry-Cpp/workflows/CI%20Windows/badge.svg?branch=master)
![CI Ubuntu](https://github.com/makra89/Visual-Odometry-Cpp/workflows/CI%20Ubuntu/badge.svg)
<a href="https://codeclimate.com/github/makra89/Visual-Odometry-Cpp/maintainability"><img src="https://api.codeclimate.com/v1/badges/ae131ed274ff7c6a7b7f/maintainability" /></a>

# Visual-Odometry-Cpp
Visual Odometry pipeline implementation in C++

I am a former physicist working as a software engineer in topics like sensor data fusion. I have a strong interest in computer vision and always wondered how things like feature tracking magically work in some open-source projects. 
After taking part in the coursera course Robotics:Perception, I decided to just try it out myself and learn :) 

# The task

It would be rather straightforward to use opencv and just connect the provided functionality to some kind of visual odometry pipeline. What I rather try to do:

- Creating a visual odometry pipeline from scratch (see below) extracting poses from monocular camera images
- Implementing all major algorithms (like feature detection, description,..) myself
- Using only utility functions and structs from opencv. For me also things like SVD count as such.
- Having fun, so please pardon the lack of testing and documentation. I do enough of this at work.
- ....... And finally hop on the open-source train ;) 

The goal is not to create yet another visual odometry pipeline that can easily be used by everyone. There exist multiple sophisticated open-source projects for this. But maybe this project helps someone understanding visual odometry. 

# Current functionality

All individual parts of an ORB detector-descriptor-matcher chain are implemented:
- Oriented FAST detector
- Rotated BRIEF descriptor
- LSH matcher

Additionally, fundamental matrix estimation and essential matrix decomposition, including RANSAC, is implemented. 
Having this functionality at least some (low quality) delta poses can be computed. 

A python package can be built optionally that exposes the Master interface.

Finally there is some output :) 

![Feature Detection](doc/results/FeatureDetection.gif)

![Pose Comparison](doc/results/PoseComparison.gif)

# Usage

Using the application runKittiDemo.exe in apps/ it is possible to visualize matched features in an camera image stream.
This app is currently hard coded to KITTI image sequence 0:

`runKittiDemo.exe <path-to-camera-images> e.g. runKittiDemo.exe ./test/*.jpg` 

Additionally there is a python script for running the KITTI sequence + comparing it to the ground truth

`python runKittiDemp.py -i <camera-images-dir> -r <ground_truth_textfile> ` 

# Dependencies

The following dependencies are placed in the folder 3rdparty:

- [gtest](https://github.com/google/googletest/blob/master/googletest/LICENSE)
- [opencv](https://opencv.org/license/) 

The following dependencies are required as CMake package:

- swig (optionally for python package)
- python (optionally for python package)
