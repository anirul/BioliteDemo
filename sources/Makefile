ALL = Biolite.irrlicht
CC = gcc
CXX = g++
CFLAGS = -g -Wall -I/usr/include -I.. -I/usr/local/include -I/usr/local/include/irrlicht
LIBS = \
	-L/usr/lib \
	-L/usr/local/lib \
	-lpthread \
	-lIrrlicht \
	-lGL \
	-lXext \
	-lX11

all: $(ALL)

HEADERS = \
	CGUIColorPickDialog.h \
	game_state_xml_gui.h \
	simple_ai.h \
	CSceneNodeAnimatorCameraArcball.h \
	game_state_xml_still.h \
	win.h \
	IGUIColorPickDialog.h \
	main.h \
	world.h \
	ISceneNodeAnimatorCameraArcball.h \
	math.h \
	xml_button.h \
	game.h \
	planet.h \
	xml_image.h \
	game_ai.h \
	plant.h \
	parameter_set.h \
	resolution_chooser.h

CGUIColorPickDialog.o: CGUIColorPickDialog.cpp CGUIColorPickDialog.h IGUIColorPickDialog.h
	$(CXX) -o  CGUIColorPickDialog.o $(CFLAGS) -c  CGUIColorPickDialog.cpp
CSceneNodeAnimatorCameraArcball.o: CSceneNodeAnimatorCameraArcball.cpp CSceneNodeAnimatorCameraArcball.h ISceneNodeAnimatorCameraArcball.h
	$(CXX) -o CSceneNodeAnimatorCameraArcball.o $(CFLAGS) -c CSceneNodeAnimatorCameraArcball.cpp
game.o: game.cpp $(HEADERS)
	$(CXX) -o game.o $(CFLAGS) -c game.cpp
main.o: main.cpp $(HEADERS)
	$(CXX) -o main.o $(CFLAGS) -c main.cpp
math.o: math.cpp $(HEADERS)
	$(CXX) -o math.o $(CFLAGS) -c math.cpp
planet.o: planet.cpp $(HEADERS)
	$(CXX) -o planet.o $(CFLAGS) -c planet.cpp
plant.o: plant.cpp $(HEADERS)
	$(CXX) -o plant.o $(CFLAGS) -c plant.cpp
simple_ai.o: simple_ai.cpp $(HEADERS)
	$(CXX) -o simple_ai.o $(CFLAGS) -c simple_ai.cpp
title.o: title.cpp $(HEADERS)
	$(CXX) -o title.o $(CFLAGS) -c title.cpp
win.o: win.cpp $(HEADERS)
	$(CXX) -o win.o $(CFLAGS) -c win.cpp
world.o: world.cpp $(HEADERS)
	$(CXX) -o world.o $(CFLAGS) -c world.cpp
game_state_xml_still.o: game_state_xml_still.cpp $(HEADERS)
	$(CXX) -o game_state_xml_still.o $(CFLAGS) -c game_state_xml_still.cpp
game_state_xml_gui.o: game_state_xml_gui.cpp $(HEADERS)
	$(CXX) -o game_state_xml_gui.o $(CFLAGS) -c game_state_xml_gui.cpp
xml_image.o: xml_image.cpp $(HEADERS)
	$(CXX) -o xml_image.o $(CFLAGS) -c xml_image.cpp
xml_button.o: xml_button.cpp $(HEADERS)
	$(CXX) -o xml_button.o $(CFLAGS) -c xml_button.cpp
parameter_set.o: parameter_set.cpp $(HEADERS)
	$(CXX) -o parameter_set.o $(CFLAGS) -c parameter_set.cpp
resolution_chooser.o: resolution_chooser.cpp $(HEADERS)
	$(CXX) -o resolution_chooser.o $(CFLAGS) -c resolution_chooser.cpp

OBJ = \
	game.o \
	main.o \
	math.o \
	planet.o \
	plant.o \
	simple_ai.o \
	win.o \
	world.o \
	CGUIColorPickDialog.o \
	CSceneNodeAnimatorCameraArcball.o \
	game_state_xml_still.o \
	game_state_xml_gui.o \
	xml_image.o \
	xml_button.o \
	parameter_set.o \
	resolution_chooser.o

Biolite.irrlicht: $(OBJ)
	$(CXX) -o Biolite.irrlicht $(OBJ) $(LIBS)

clean:
	rm -f *.o core $(ALL)
