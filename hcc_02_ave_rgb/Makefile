# © 2024 : github.com/NMoroney
# MIT License 
#

EXE=hcc_02_ave_rgb

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


png :
	python3 tsv_to_scatter_xy_rgb.py -i temp_xy_rgb.tsv -o xy_rgb.png -t Average_RGB


clean :
	rm temp*

