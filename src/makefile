all:main.o tee.o cat.o cd.o cp.o pwd.o diff.o grep1.o pt4.o
	g++ *.o -o main

main.o:heads.h main.cpp
	g++ -c main.cpp

tee.o:heads.h tee.cpp
	g++ -c tee.cpp

cat.o:heads.h cat.cpp
	g++ -c cat.cpp

cd.o:heads.h cd.cpp
	g++ -c cd.cpp

cp.o:heads.h cp.cpp
	g++ -c cp.cpp

pwd.o:heads.h pwd.cpp
	g++ -c pwd.cpp

diff.o:heads.h diff.cpp
	g++ -c diff.cpp

grep1.o:heads.h grep1.cpp
	g++ -c grep1.cpp

pt4.o:heads.h pt4.cpp
	g++ -c pt4.cpp

clean:
	cmd //C del *.o main main.exe