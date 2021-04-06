#!/bin/sh
./mk_raw_image test 0
rm raw_image
sleep 2 
./fb_test
