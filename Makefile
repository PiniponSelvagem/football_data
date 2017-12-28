#gcc football_data.c getdata.c common.c competitions.c teams.c fixtures.c -Wall -pedantic -std=c99 -o football_data -lcurl -ljansson

CFLAGS = -c -Wall -pedantic -std=c99 -g
LCFLAGS = -fPIC $(CFLAGS)
LDFLAGS = -lcurl -ljansson
HEADER_FOOTB = competitions.h teams.h fixtures.h
HEADER_GETDT = getdata.h
HEADER_COMMO = common.h getdata.h
HEADER_COMPS = competitions.h getdata.h common.h
HEADER_TEAMS = teams.h getdata.h common.h
HEADER_FIXTU = fixtures.h getdata.h common.h

ALL = football_data

all: $(ALL)

football_data: football_data.o libfootball.so
	gcc -o $@ football_data.o ./libfootball.so

football_data.o: football_data.c $(HEADER_FOOTB)
	gcc football_data.c $(CFLAGS)
	

libfootball.so: getdata.o common.o competitions.o teams.o fixtures.o
	gcc -o $@ $^ $(LDFLAGS) -shared


getdata.o: getdata.c $(HEADER_GETDT)
	gcc getdata.c $(LCFLAGS) $(LDFLAGS)

common.o: common.c $(HEADER_COMMO)
	gcc common.c $(LCFLAGS) $(LDFLAGS)

competitions.o: competitions.c $(HEADER_COMPS)
	gcc competitions.c $(LCFLAGS) $(LDFLAGS)

teams.o: teams.c $(HEADER_TEAMS)
	gcc teams.c $(LCFLAGS) $(LDFLAGS)

fixtures.o: fixtures.c $(HEADER_FIXTU)
	gcc fixtures.c $(LCFLAGS) $(LDFLAGS)


clean:
	rm -f $(ALL) *.o *.a *.so *.gch
