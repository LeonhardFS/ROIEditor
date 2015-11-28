# ROIEditor
![Image of editor](https://raw.githubusercontent.com/LeonhardFS/ROIEditor/master/screen.jpg)
ROIEditor (Region of Interst Editor) is a tiny little QT Application that lets you specify for a batch of pictures rectangular pixel regions of interest. The program was initially developed by me for a Computer Vision course as I needed to label >500 images and did not find fast enough a good tool. So I decided to code up a slim editor real quick. In the current early version the size of an ROI is fixed to 128x128px and there are no fancy image tools (resizing, cropping, zooming, scrolling). Those might be added in a future release.

## Usage
Drag 'n' drop some picture files or a folder onto the program to have it assigned a batch. By using the arrow keys you can navigate between the images. ROIs are initialized to be in the middle.

## File format
The editor exports all information into csv files (comma-separated) for easy access.

Region of interest:
- x 
- y 
- width 
- height 

Image attributes:
- image width 
- image height
- filename 
- absolute path


## Binaries
Currently I can only provide some MAC OS X binaries, which can be downloaded from 
[get it here!](https://github.com/LeonhardFS/ROIEditor/blob/master/bin/ROIEditor.dmg?raw=true)
