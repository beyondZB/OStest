cp boot/target/object/main/mbr ./
cp boot/target/object/main/findLoader ./
cp boot/target/object/main/loader ./LOADER.BIN
cp kernel/target/kernel-0.0.1 ./KERNEL.BIN
cp userProc/target/userProc-0.0.1 ./
lofsutil-0.0.1 -c /dev/sdb
lofsutil-0.0.1 -f /dev/sdb
lofsutil-0.0.1 -b mbr /dev/sdb
lofsutil-0.0.1 -e findLoader /dev/sdb
lofsutil-0.0.1 -p LOADER.BIN /dev/sdb
lofsutil-0.0.1 -p KERNEL.BIN /dev/sdb
lofsutil-0.0.1 -p a.txt	/dev/sdb
lofsutil-0.0.1 -p userProc-0.0.1 /dev/sdb
rm ./mbr
rm ./findLoader
rm ./LOADER.BIN
rm ./KERNEL.BIN
