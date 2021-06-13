CC=g++
COPT=-Wall -std=c++1z

IMG=mono/Lighthouse.bmp

.PHONY: clean

HIST=hist1.png hist2.png hist3.png
GRPH=graph1.png graph2.png
DAT_HIST=$(HIST:.png=.dat)
DAT_GRPH=$(GRPH:.png=.dat)
SO=libgamma.so libgauss.so
SO_CPP=$(subst lib,,$(SO:.so=.cpp))

all: $(HIST) $(GRPH) hist.plt graph.plt

$(HIST): $(DAT_HIST)
	gnuplot hist.plt -e 'set out "$@"; plot "$(subst .png,.dat,$@)" using 1:2 with boxes'

$(GRPH): $(DAT_GRPH)
	gnuplot graph.plt -e 'set out "$@"; plot "$(subst .png,.dat,$@)" using 1:2 with lines'

graph1.dat: graph libgamma.so
	./graph "$(shell pwd)/libgamma.so" graph1.dat

graph2.dat: graph libgauss.so
	./graph "$(shell pwd)/libgauss.so" graph2.dat

hist1.dat: histgram
	./histgram $(IMG) hist1.dat

hist2.dat: histgram gamma.bmp
	./histgram gamma.bmp hist2.dat

hist3.dat: histgram gauss.bmp
	./histgram gauss.bmp hist3.dat

histgram: histgram.cpp
	$(CC) $(COPT) -o histgram histgram.cpp

gamma.bmp: libgamma.so transimg
	./transimg "$(shell pwd)/libgamma.so" $(IMG) gamma.bmp

gauss.bmp: libgauss.so transimg
	./transimg "$(shell pwd)/libgauss.so" $(IMG) gauss.bmp

$(SO): $(SO_CPP)
	$(CC) $(COPT) -shared -fPIC -o $@ $(subst .so,.cpp,$(subst lib,,$@))

transimg: transimg.cpp
	$(CC) $(COPT) -o transimg transimg.cpp -ldl

graph: graph.cpp
	$(CC) $(COPT) -o graph graph.cpp -ldl

clean:
	rm transimg
	rm graph
	rm histgram
	rm libgamma.so
	rm libgauss.so
	rm gamma.bmp
	rm *.png
	rm *.dat