# PRG=gnu.exe
# all:
# 	latex -interaction=nonstopmode -halt-on-error report.tex
# 	if test -f "report.dvi"; then dvipdf report.dvi; fi
# 0:
# 	file report.pdf | grep "PDF document" > studentout$@
# 	@echo "lines after the next are mismatches with master output -- see out$@"
# 	diff out$@ studentout$@ $(DIFFLAGS)
# clean:
# 	rm -f report.aux report.dvi report.pdf report.log studentout* difference*

# PRG=gcc0.exe

GCC=g++
GCCFLAGS=-std=c++11 -O3

MSC=cl
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL

VALGRIND_OPTIONS=-q --leak-check=full
DIFFLAGS=--strip-trailing-cr -y --suppress-common-lines

DRIVER0=driver.cpp

OSTYPE := $(shell uname)
PRG = gcc0
ifeq ($(OSTYPE),Linux)
CYGWIN=gcc0
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)  $(GCCFLAGS)

gendata:
	for i in 100 200 300 400 500 750 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000; do ./$(PRG) $$i 100> data.$$i; done
latex:
	pdflatex report.tex
clean:
	rm -f *.exe *.o *.obj report.aux report.log 

target:
	for number in 1 2 3 4 ; do \
		echo $$number ; \
	done