#!/bin/sh

cd $PWD/rootfs_files

cp -vrfp ../output/images/rootfs.tar.bz2 rootfs/
cd rootfs_output
tar -xvf rootfs.tar.bz2
rm rootfs.tar.bz2



#autologin
cp ../autologin/autologin sbin/
cp ../autologin/inittab etc/

#automount
#cp ../automount/S10mdev etc/init.d/
#cp ../automount/mdev.conf etc/
#cp ../automount/automount.sh etc/



tar -cjvf rootfs.tar.bz2 *
find . ! -name rootfs.tar.bz2 -delete
