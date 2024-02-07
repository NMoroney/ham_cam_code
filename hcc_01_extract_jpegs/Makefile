# © 2024 : github.com/NMoroney
# MIT License 
#

EXE=hcc_01_extract_jpegs

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
	g++ $(OPT) $(EXE).cc -I $(OCVINC) $(OCVLIBS) -o $(EXE)
	./$(EXE)  ../HC1_daily-2024-01-26.mp4
	cp frame_0000452.jpg hamcam_2024_01_26-12:34.jpg

clean :
	rm frame*.jpg

