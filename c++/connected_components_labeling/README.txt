Author: Matthew R. England

Usage of my program.

To run the invert ability on an image, use the -invert command parameter:
ex: ./ccl -invert test_images/3_engulfed_regions.png output.png

To run the removal of objects less than a size on an image use the -size <#> command parameters:
ex: ./ccl -size 10513 test_images/bw_shapes.png output.png

To run both example below:
ex: ./ccl -invert -size 20000 test_images/3_engulfed_regions.png output.png 


To view the each objects stats use: cat ObjectDetectionStats.txt 

