SloMo: View videos in slow motion
=================================

Author: Sourabh Daptardar <saurabh.daptardar@gmail.com>

Slows the motion of a video.


Usage:
-----
slomo <inputvideo> <outputvideo> <slowdownfactor>

Example:
-------
./slomo jump.mp4 jump-slomo.avi 4

Note, only AVI output is supported to convert to some other format use tools like ffmpeg:
For example: ffmpeg -i jump-slomo.avi -vcodec copy jump-slomo.mp4

Result:
--------


Input Video 1 (240 frames)

[![Input1](http://img.youtube.com/vi/FIgHQ_ouPrI/0.jpg)](http://www.youtube.com/watch?v=FIgHQ_ouPrI)



Output Video 1 (Slowed by a factor of 4 : 960 frames)

[![Output1](http://img.youtube.com/vi/99fxIxs7UE8/0.jpg)](http://www.youtube.com/watch?v=99fxIxs7UE8)
