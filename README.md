# Wordl Arduino
At KdG we had a 6 week embedded devices course. The exam assignment was to create a game for the Arduino. I went ahead and did a little extra adding a functioning UI written in Java + JavaFX that can communicate between the Arduino and a PC.<br/>
One of the limitations for this course was that we were not allowed to use "Arduino.h", so we had to create our own IO and utilities libraries.

In order to accomplish this I used a Java library to interact with a serial port (the USB cable to an Arduino is running in Serial Mode). The communication between the two devices uses a very simple, but functional communication protocol that I designed myself.<br/>

![]()

## Project Structure
- `/ui`: Embedded C Arduino application
- `/wordl`: Optional JavaFX app to improve the UX

## Getting started
An Arduino with the Velleman VMA209 extension board is required to run this project.

### Arduino
1. Clone the project
2. Make sure you have VSCode installed with the PlatformIO extension
3. Open the `/ui` directory in VSCode
4. Run the app through the PlatformIO extension

### JavaFX
1. Clone the project
2. Open the app in any Java IDE with support for Gradle
3. Make sure the arduino is connected to the pc with the program flashed (optional)
4. Run the app using `gradle run`

