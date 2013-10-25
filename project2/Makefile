CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -lmvc
OGL_LIBRARIES = -lglut -lGLU -lGL

OBJS = project2.o Prism.o MatrixIF.o

all: project2
	./project2

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

TEMPLATE_Subclass.o: TEMPLATE_Subclass.h TEMPLATE_Subclass.c++
	$(CPP) $(C_FLAGS) TEMPLATE_Subclass.c++

MatrixIF.o: MatrixIF.h MatrixIF.c++
	$(CPP) $(C_FLAGS) MatrixIF.c++

Prism.o: MatrixIF.o Prism.h Prism.c++
	$(CPP) $(C_FLAGS) Prism.c++

clean:
	rm -rf *.o *~ project2
