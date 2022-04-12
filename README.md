## Surround-View / Birds Eye View ##

Surround view is a vehicle camera system that provides several views like Top view, Rear view and panorama view  to assist the driver when parking,
either manually or automatically.

The current script is written to Read fish eye images generated from ADAS cameras , Process them using image processing library OpenCV - C++

## Dependancies##
1. Language Used - C++
2. Vision Library - OpenCV 4.XX

The high level overview of the implimentation can be shown in below diagram.


![SVS](https://user-images.githubusercontent.com/53288493/162886037-c8ce43b8-c04e-4be1-a411-3a4d55829c25.JPG)

## Output of Script ##


![SorroundViewOutput](https://user-images.githubusercontent.com/53288493/162886634-38fb86a5-ffec-4f17-a633-cdfe15ee530c.jpg)


## Limitation in current code ##

1. Most of the code contains hardcoded layout design 
2. Camera Images are Dummy and there is no LDC done as had no details regarding intrinsic and extrinsic parameters of camera 
3. Code is not Modular

## Implimentation In Pipeline ##

1. Camera Calibration and LDC code for Image Flatten and lens noise removal
2. Object oriented refactoring to make it modular

