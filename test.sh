#!/bin/bash
#insmod char.ko
for i in {0..4}
do 
	echo "hello$((4-$i))" > /dev/hello$1$((4-$i)) 
	cat /dev/hello$1$i
done

dmesg
#rmmod char
