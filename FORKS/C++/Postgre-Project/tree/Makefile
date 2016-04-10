all: createTable.out hpinsert.out vinsert.out dinsert.out EIA.out Query_Database.out
createTable.out: create.cpp
	g++ -Wall -lpqxx -o createTable.out create.cpp

hpinsert.out: insertHousePerson.cpp
	g++ -Wall -lpqxx -o hpinsert.out insertHousePerson.cpp

vinsert.out: insertVehicle.cpp
	g++ -Wall -lpqxx -o vinsert.out insertVehicle.cpp

dinsert.out: insertDaytrip.cpp
	g++ -Wall -lpqxx -o dinsert.out insertDaytrip.cpp

EIA.out: EIA.cpp
	g++ -Wall -ansi -g -lpqxx -o EIA.out EIA.cpp

Query_Database.out: Database_Query.cpp
	g++ -Wall -ansi -g -lpqxx -o Query_Database.out Database_Query.cpp 

clean: 
	rm createTable.out hpinsert.out vinsert.out dinsert.out EIA.out Query_Database.out

