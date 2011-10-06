#!/bin/bash

CPU=4

#checkout & compile GIMX
if ! test -d GIMX
then
  mkdir -p GIMX
  cd GIMX
  svn checkout http://diyps3controller.googlecode.com/svn/trunk/sixaxis-emu
  svn checkout http://diyps3controller.googlecode.com/svn/trunk/sixaxis-emu-configurator
  svn checkout http://diyps3controller.googlecode.com/svn/trunk/sixemugui-serial
  svn checkout http://diyps3controller.googlecode.com/svn/trunk/sixstatus
  svn checkout http://diyps3controller.googlecode.com/svn/trunk/fpsconfig
  svn export http://diyps3controller.googlecode.com/svn/trunk/Makefile.win
  cd ..
fi

cd GIMX
make -f Makefile.win -j $CPU

#copy GIMX binaries & config file
if ! test -d setup
then
  mkdir -p setup
  mkdir -p setup/config
  mkdir -p setup/macros
  cp $PREFIX/bin/libgcc_s_dw2-1.dll setup
  cp $PREFIX/bin/libiconv-2.dll setup
  cp $PREFIX/bin/libpthread-2.dll setup
  cp $PREFIX/bin/libstdc++-6.dll setup
  cp $PREFIX/bin/libxml2.dll setup
  cp $PREFIX/bin/SDL.dll setup
  cp $PREFIX/bin/zlib1.dll setup
fi

cp sixaxis-emu/emuclient.exe setup
cp -r sixaxis-emu/config/*.xml setup/config
cp -r sixaxis-emu/macros/*.txt setup/macros
cp sixaxis-emu-configurator/bin/Release/gimx-config.exe setup
cp sixemugui-serial/bin/Release/gimx-serial.exe setup
cp sixstatus/bin/Release/gimx-status.exe setup
cp fpsconfig/bin/Release/gimx-fpsconfig.exe setup
