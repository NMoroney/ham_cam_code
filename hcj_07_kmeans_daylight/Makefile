# © 2024 : github.com/NMoroney
# MIT License 
#

EXE=hcc_07_kmeans_daylight

OCV=/usr/local/

OCVINC=$(OCV)include/opencv4
OCVLIB=$(OCV)lib

LCORE=-lopencv_core
LCODEC=-lopencv_imgcodecs
LPROC=-lopencv_imgproc
LVIDEO=-lopencv_videoio

OCVLIBS=-Wl,-rpath,$(OCVLIB) $(LCORE) $(LCODEC) $(LPROC) $(LVIDEO)

OPT=-std=c++17 -O3 -w


cam :
	g++ $(OPT) $(EXE).cc -I $(OCVINC) $(OCVLIBS) -larmadillo -o $(EXE)
	./$(EXE)  ../HC1_daily-2024-01-30.mp4
#	./$(EXE)  ../HC1_daily-2024-01-28.mp4
#	./$(EXE)  ../HC1_daily-2024-01-27.mp4
#	./$(EXE)  ../HC1_daily-2024-01-26.mp4


clean :
	rm frame*

