# Read an image with xli.
all : application

testing : main.c TP.c
	gcc -ggdb main.c TP.c -o test

# Test : dep with .h versus dep with .c
targa : main_targa.o 
	gcc -ggdb main_targa.o -o tp7targa 

application : strhelpers.o serveur.o client.o TP.o mem_targa.o
	gcc -ggdb strhelpers.o serveur.o TP.o mem_targa.o -o enstaserveur
	gcc -ggdb strhelpers.o client.o -o enstaclient

serveur.o : serveur.c  strhelpers.h TP.h mem_targa.h
	gcc -ggdb -c serveur.c -o serveur.o

client.o : client.c strhelpers.h 
	gcc -ggdb -c client.c -o client.o

strhelpers.o : strhelpers.c 
	gcc -ggdb -c strhelpers.c -o strhelpers.o

mem_targa.o : mem_targa.c mem_targa.h 
	gcc -ggdb -c mem_targa.c -o mem_targa.o

fun_targa.o : fun_targa.c fun_targa.h
	gcc -ggdb -c fun_targa.c -o fun_targa.o

main_targa.o : main_targa.c mem_targa.o fun_targa.o 
	gcc -ggdb -c main_targa.c -o main_targa.o

info : 
	@echo "make testj : incrust image \n\
	make testr : reduce image\n\
	make testf : apply filter x5\n\
	make testmh : apply horizontal mirroring\n\
	make testmv : apply vertical mirroring\n\
	make testmono : make image monochrome\n\
	make testcombo : apply reduce + vertical mirroring + extraction\n\
	make test : apply all tests"


test : testj testr testf testmh testmv testmono testmonomid testcombo
	@echo "All test run"
testclone : targa
	./tp7targa -c -i GRG.tga -o outputclone.tga
testj : targa
	./tp7targa -j petitpin.tga -i GRG.tga -o outputr2.tga
testr : targa
	./tp7targa -r 2 -i pin.tga -o outputpinr2.tga
testf : targa
	./tp7targa -f 5 -i GRG.tga -o outputfilter.tga
testmh : targa
	./tp7targa -m H -i GRG.tga -o outputmh.tga
testmv : targa
	./tp7targa -m V -i GRG.tga -o outputmv.tga
testmono : targa
	./tp7targa -b -i GRG.tga -o outputmono.tga
testmonomid: targa
	./tp7targa -d -i pin.tga -o outputmonomid.tga
testcombo : 
	./tp7targa -r 2 -m V -e G -i GRG.tga -o outputcombo.tga

clean :
	rm *.o tp7targa
