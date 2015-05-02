lofsutil-0.0.1 -c hd.img
lofsutil-0.0.1 -f hd.img
lofsutil-0.0.1 -b boot/mbr hd.img
lofsutil-0.0.1 -e boot/findLoader hd.img
cp boot/LOADER.BIN ./
cp kernel/KERNEL.BIN ./
lofsutil-0.0.1 -p LOADER.BIN hd.img
lofsutil-0.0.1 -p KERNEL.BIN hd.img
rm ./LOADER.BIN
rm ./KERNEL.BIN
