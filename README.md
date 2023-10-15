# ESP8266 Image Gallery

<img src="chrome_JnhATSXTLk.png" alt="HTML-Ansicht">
<img src="IMG20231015164128.jpg" alt="Aufbau mit NodeMCU 1.0 (ESP-12E Module) und einem SD-Kartenleser ">

## Overview
This example demonstrates how to create a web-based image gallery using ESP8266 and an SD card. The ESP8266 serves an HTML page that lists image files stored on the SD card and allows them to be downloaded.


## Setup
1. **Dependencies:** 
  - ESP8266WiFi
  - ESPAsyncWebServer
  - SD
2. **Hardware:**
  - ESP8266 board
  - SD card reader connected to ESP8266
3. **WiFi Credentials:**
  - Change `ssid` and `password` for your network.
  - AP mode is available with `ap_ssid` and `ap_password`.


## Endpoints


### `GET /`
Displays the image gallery.
- **Response:** HTML page listing image files from SD card.


### `GET /download?file=<filename>`
Downloads the specified image file.
- **Parameters:**
  - `file`: Name of the file to download.
- **Response:**
  - Success: The image file.
  - Fail: 404 "File not found."


## Running the Project
1. Load the code onto your ESP8266.
2. Open the Serial Monitor to get the IP address.
3. Open a web browser and go to `http://<IP_ADDRESS>`.


## Limitations
- Supports JPEG image files only.
- No pagination; all files are listed on one page.


## Future Work
- Add support for more image types.
- Implement pagination or lazy loading for large number of files.

