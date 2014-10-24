AirPainter
==========

A virtual air painter application, that allows a user to paint in the computer, by interacting with it only using his index finger. It was developed by using OpenCV 2.4.9.

### Running the project

First of all, in order to run this project, you must have a web camera(not an embedded one) that must be facing the computer screen.

Once the program starts, you will notice three different windows.

* The window named "camera" it is the first you should deal with. It is usefull for placing the camera into the right spot. This window constantly displays what the camera "sees". It is necessary that the camera can "see", the top left and bottom right corners of your screen. When you have placed your camera at the right spot, press "c" to continue.(Make sure your camera isn't facing any other stuff besides your screen).

* The second window is the one named "calibrator". As it's name implies it is used for calibration. The program will try to detect the white dots on the black background, so it will place our canvas later. Just let this image be one for a while and then press "s".

* The third and final window, is the one named "canvas". It is the window that we can paint on. In order for the program to be able to change tools, or start painting, you should first press "d". This will allow it to compute the "Differential Image" so that it "knows" each time where your finger is. But we are not done yet! In order to start painting you should also press the "p" key. Now you are ready to paint. You can also press the "d" and "p" keyes any time, in order to enable/disable the features stated above. Finally, it is good to be mentioned that if you press the "c" key while on "canvas" window, it will erase everything you have painted. This is good if you are bored to use the eraser.


### Important Notes

##### Note 1 : Most of the values for the parameters of the functions used, are optimized in dark backgrounds, where the screen brightness is maximum. If you want to run this program in different conditions you should experiment with other values.

##### Note 2 : In order for the painting to work correct, just use only your "pointing" finger, placed vertically at the screen. If you use more that one finger, or weird finger positions, the results of the painting would be something like "Abstract Art".
