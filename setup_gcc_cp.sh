
sudo apt-get install nasm
sudo apt-get install qemu
sudo apt-get install qemu-kvm

sudo apt update
sudo apt install build-essential
sudo apt install bison
sudo apt install flex
sudo apt install libgmp3-dev
sudo apt install libmpc-dev
sudo apt install libmpfr-dev
sudo apt install texinfo

sudo apt-get install curl

export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.gz
tar xf binutils-2.39.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.39/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
sudo make all install 2>&1 | tee make.log

cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz
tar xf gcc-12.2.0.tar.gz
mkdir gcc-build
cd gcc-build
../gcc-12.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-language=c --without-headers

sudo make all-gcc
sudo make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
export PATH="$PATH:/usr/local/i386elfgcc/bin"