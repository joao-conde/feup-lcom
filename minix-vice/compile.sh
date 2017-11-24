cd src
make clean install
mv minix-vice ../
cd ..
strip --strip-all minix-vice
