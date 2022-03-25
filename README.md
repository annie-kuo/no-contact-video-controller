# no-contact-video-controller
This program allows the user to control YouTube videos using a proximity sensor. Without contact, the user will be able to:
- rewind the video by 10 seconds;
- pause the video;
- forward the video by 10 seconds;
- display the proximity value on the serial display.

## Features
- To rewind the video, hover your hand over the sensor about 20cm away.
- To pause the video, hover your hand over the sensor about 5cm away.
- To forward the video, hover your hand over the sensor about 2cm away.
- The LED lights light up in different colors according to the function used: pink for rewind, cyan for pause, and purple for forward.

# Details
This code is meant to be uploaded onto a proximity sensor. The model of the board used is the *Adafruit Proximity Trinkey - USB APDS9960 Sensor Dev Board*. A template code from *Kattni Rembor* was used and then modified to include the above features.
