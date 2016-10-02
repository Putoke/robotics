all: part_e

  CFLAGS=-fPIC -g -Wall
  ARIA_INCLUDE=-I/usr/local/Aria/include
  ARIA_LINK=-L/usr/local/Aria/lib -lAria -lpthread -ldl -lrt

  %: %.cpp
				g++-4.8 $(CFLAGS) $(ARIA_INCLUDE) $< -o $@ $(ARIA_LINK)
