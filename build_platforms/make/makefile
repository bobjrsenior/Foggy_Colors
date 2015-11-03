#OBJS specifies which files to compile as part of the project

OBJS = ../../src/main.c ../../src/matrixMath.c



#CC specifies which compiler we're using

CC = gcc



#COMPILER_FLAGS specifies the additional compilation options we're using

# -w suppresses all warnings
#
COMPILER_FLAGS =  -Wall
#
#
#
#  #LINKER_FLAGS specifies the libraries we're linking against

LINKER_FLAGS = -lGL -lGLU -lGLEW -lglfw -lm -lSOIL

#SOURCE_FLAGS

SOURCE_FLAGS = -I ../../src/
 

#Lib Directories

LIBRARY_DIRECTORIES = 

#OBJ_NAME specifies the name of our exectuable

OBJ_NAME = ../../Build/linux/Foggy_Colors

#This is the target that compiles our executable

all : $(OBJS)

	$(CC) $(OBJS) $(COMPILER_FLAGS) $(SOURCE_FLAGS) -o $(OBJ_NAME) $(LINKER_FLAGS)
