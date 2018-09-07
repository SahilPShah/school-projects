EXENAME = mp1
OBJS = mp1.o HSLAPixel.o PNG.o main.o lodepng.o

CXX = clang++
CXXFLAGS = -c -stdlib=libc++ -g -Wall -Werror -pedantic
LD = clang++
LDFLAGS = -stdlib=libc++ -lpthread -lm

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp mp1.h cs225/HSLAPixel.h cs225/PNG.h
	$(CXX) $(CXXFLAGS) main.cpp

mp1.o : mp1.cpp mp1.h
	$(CXX) $(CXXFLAGS) mp1.cpp

PNG.o : cs225/PNG.cpp cs225/PNG.h cs225/HSLAPixel.h cs225/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs225/PNG.cpp

HSLAPixel.o : cs225/HSLAPixel.cpp cs225/HSLAPixel.h
	$(CXX) $(CXXFLAGS) cs225/HSLAPixel.cpp

lodepng.o : cs225/lodepng/lodepng.cpp cs225/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs225/lodepng/lodepng.cpp

clean:
	rm $(EXENAME) *.o
