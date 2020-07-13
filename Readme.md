# dataviewer

Qt project to visualize data from serial port, based on [datetimeaxis](https://doc.qt.io/qt-5/qtcharts-datetimeaxis-example.html) sample.

## Features

* Reads numeric serial data from the USB port
* Displays data values on varous series on different charts
* Layout can be configured as needed using a json file

## Usage

### Preparation

Create a json file to define the layout. Example: [layout.json](./layouts/layout.json)


### Start the program

Launch dataviewer as follows:

    dataviewer -layout=layout.json

(Depending on your system it may be necessary to specify the full path to the dataviewer executable and/or the layout file.)

### Connect to the Serial Port

In the bottom of the window there is a combobox that lists all serial ports that the dataviewer can connect to. Click on the _Connect_ button to open the serial port and display the data in the charts.

While the dataviewer is connected it writes a log file. Click on the browse button (3rd Button in the status bar) to open the log file folder. Here you can view the generated logs.

### Demo Sketch

A [demo sketch](./demo-sketch/demo-sketch.ino) can be used to demonstrate the dataviewer. Upload the sketch to your arduino and wait until it runs. In the dataviewer press the connect button. The streamed data should appear in the charts.

