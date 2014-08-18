gr-buffertest
=============

GNU Radio Test for Deep Buffer Manipulation. Test whether the system of global
variables works for multiple blocks. Also tests get_user_pages against the 
gnuradio buffers. 

Requires GNU Radio that can pass buffer object to the work function available:
https://github.com/muniza/gnuradio

Requires kernel module for get_user_pages and ioctl:
https://github.com/muniza/gsoc_2014
