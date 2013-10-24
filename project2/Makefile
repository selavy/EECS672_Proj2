CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -lmvc
OGL_LIBRARIES = -lglut -lGLU -lGL

OBJS = project2.o Square.o MatrixIF.o BoardMV.o

project2: $(OBJS) ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
	$(LINK) -o project2 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project2.o: project2.c++
	$(CPP) $(C_FLAGS) project2.c++

Square.o: MatrixIF.o Square.h Square.c++ BoardMV.o
	$(CPP) $(C_FLAGS) Square.c++

BoardMV.o: BoardMV.h BoardMV.c++
	$(CPP) $(C_FLAGS) BoardMV.c++

MatrixIF.o: MatrixIF.h MatrixIF.c++
	$(CPP) $(C_FLAGS) MatrixIF.c++

clean:
	rm -rf *.o *~ project2
