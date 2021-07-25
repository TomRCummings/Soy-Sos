#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/soy-sos.kernel isodir/boot/soy-sos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "soy-sos" {
	multiboot /boot/soy-sos.kernel
}
EOF
grub-mkrescue -o soy-sos.iso isodir
