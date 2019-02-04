# chardev-example
Sample char driver linux kernel

this is simple charcter driver

creating 5 driver nodes as /dev/hello0 to /dev/hello4
where all driver shares common buffer in kernel
so any device can write to update buffer
and any device can read the buffer.

# TODO

adding lock mecahnisam to avoid race
implementing other file operation like seek and working on big buffer.
