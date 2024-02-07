# © 2024 : github.com/NMoroney
# MIT License 
#

EXE=hcc_04_to_gif

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

gif :
	ffmpeg -framerate 4 -pattern_type glob -i 'frame*.jpg' ham_cam_jam-04.gif


clean :
	rm frame*.jpg

