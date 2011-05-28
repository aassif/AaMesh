L_X11=			-L/usr/X11R6/lib -lXmu -lXi
L_GLU=			-lGL -lGLU -lglut $(L_X11)

I_GLIB=			-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

P_GTS=			$(HOME)/3rdPartyLibs/gts-0.7.6
I_GTS=			-I$(P_GTS)/src
L_GTS=			-L$(P_GTS)/src/.libs -lgts

P_FFMPEG=			$(HOME)/3rdPartyLibs/ffmpeg
I_FFMPEG=			-I$(P_FFMPEG) #-I(P_FFMPEG)/libavutil -I$(P_FFMPEG)/libavformat -I$(P_FFMPEG)/libavcodec
L_FFMPEG=			-L$(P_FFMPEG)/libavformat -lavformat -L$(P_FFMPEG)/libavcodec -lavcodec -L$(P_FFMPEG)/libavutil -lavutil -lbz2 -lz

P_GLEW=			$(HOME)/3rdPartyLibs/glew
I_GLEW=			-I$(P_GLEW)/include
#L_GLEW=			-L$(P_GLEW)/lib -lGLEW -lCg -lCgGL -lpthread $(L_GLU)
L_GLEW=			-L$(P_GLEW)/lib -lGLEW -lpthread $(L_GLU)

P_TOOLKIT=			$(HOME)/wrk/toolkit
I_TOOLKIT=			-I$(P_TOOLKIT)/include
L_TOOLKIT=			-L$(P_TOOLKIT)/lib -lAaToolkit

gts:	gts.cc
	g++ -ggdb3 gts.cc -I ../include $(I_TOOLKIT) $(L_TOOLKIT) $(I_GTS) $(L_GTS) $(I_GLIB) $(L_GLU) $(I_FFMPEG) $(L_FFMPEG) -o gts
