cp boot/target/object/main/mbr ./
cp boot/target/object/main/findLoader ./
cp boot/target/object/main/loader ./LOADER.BIN
cp kernel/target/kernel-0.0.1 ./KERNEL.BIN
lofsutil-0.0.1 -c hd.img
lofsutil-0.0.1 -f hd.img
lofsutil-0.0.1 -b mbr hd.img
lofsutil-0.0.1 -e findLoader hd.img
lofsutil-0.0.1 -p LOADER.BIN hd.img
lofsutil-0.0.1 -p KERNEL.BIN hd.img
lofsutil-0.0.1 -p a.txt hd.img
rm ./mbr
rm ./findLoader
rm ./LOADER.BIN
rm ./KERNEL.BIN
