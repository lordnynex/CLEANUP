#Postgres-C-
===========
**Project from Database systems course in PostgresSQL and C++ library libpqxx**
-----------
**ECS165A Project, PostgreSQL and C++ library, libqxx**

Kevin Wang 

Jacob Miller 

###Table of Contents:
* I. Compiling
* II. Running
  - a) Necessary Configurations to Start
  - b) Necessary Files to run Insertion and Creation
  - c) Necessary Files to run Queries
* III. Runtimes and output
* IV. Files included
* V. Queries 

ALL FILES MUST BE IN THE SAME DIRECTORY, INCLUDING DATA FILES

Datafiles: 

**NHTS: National Household Travel Survey**
Download link: http://nhts.ornl.gov/download/Ascii.zip
* NHTS Data:
  - **HHV2PUB.CSV**	  Households
  - **PERV2PUB.CSV**  People in each household, connected to a household
  - **VEHV2PUB.CSV**  Vehicles in each household, connected to a household
  - **DAYV2PUB.CSV**  Daytrips, connected to people and vehicles

**EIA: US Energy Information Administration**
* EIA Data:  (Files are named differently in EIA website)
  - EIA_CO2_Electric_2014       CO2 emmisions from electric sources	http://www.eia.gov/totalenergy/data/browser/csv.cfm?tbl=T12.06
  - EIA_CO2_Transportation_2014 CO2 emissions from transportation sources	http://www.eia.gov/totalenergy/data/browser/csv.cfm?tbl=T12.05
  - EIA_MkWh_2014               CO2 emissions in MkWh units	http://www.eia.gov/totalenergy/data/browser/csv.cfm?tbl=T07.02A

* **I. COMPILING:**
You must compile the six programs (Query_Database.out, CreateTable.out, hpinsert.out, dinsert.out, vinsert.out EIA.out) before running using the Makefile included. 
Run 'make' to compile all these programs.

* **II. Running:**
To load the database 'postgres' (create tables and load values into them), you must run the script 'CreatAndInsert.sh'. In order to perform the queries run the executable Query_Database.out.
	
  - a) Necessary Configurations to Start
  
    PostgreSQL should be installed and set up. The server should have been started already, with the database 'postgres' created. Default values of localhost and port are used, and 'user' is the logged in user with no password set.  
    
  - b) Necessary Files to run Insertion and Creation:
  
	In addtion to the script 'CreateAndInsert.sh' you also must have the programs 'CreateTable.out', 'hpinsert.out', 'dinsert.out', 'vinsert.out', and 'EIA.out' compiled. You must have the data 'HHV2PUB.CSV', 'VEHV2PUB.CSV', 'PERV2PUB.CSV', 'DAYV2PUB.CSV', 'EIA_CO2_Transportation_2014.csv', 'EIA_CO2_Electric_2014.csv' and 'EIA_MkWh_2014.csv' in your local directory.

  - c) Necessary Files to run Queries:
  
	In addition to the executable 'Query_Database.out' you also must have the text files 'Problem_3a_Query.txt', 'Problem_3b_Query.txt', 'Problem_3c_Query.txt', and 'Problem_3d_Query.txt' in your local directory.
    
* **III. Runtimes and output:**

The script that creates and the tables and inserts values into them runs in about 30 minutes for the full files (less or more depending on the load of the given computer) and the Query program runs in under 5 minutes.

The insertion and creation script will give notices to the cout stream about how far along it is.

The Query executable will output all its results to cout.

* **IV. Files Included:**
```
  - Read_me.txt

  - CreatAndInsert.sh:		Run table creations, insertions, and queries
    
  - Makefile:				Compile cpp files using gcc and -lpqxx 
    
  - Database_Query.cpp:		Reads in query files and executes them
    
  - create.cpp:				Creates sql tables for NHTS data 
    
  - insertDaytrip.cpp:		Parses DAYV2PUB.CSV data and inserts into Daytrips table
    
  - insertHousePerson.cpp:	Parses HHV2PUB.CSV + PERV2PUB.CSV data and inserts into House and Person table
    
  - insertVehicle.cpp:		Parses VEHV2PUB.CSV data and inserts into Vehicles table
    
  - EIA.cpp:				Creates and inserts into EIA data 
    
  - Problem_3a_Query.txt:	Query a
    
  - Problem_3b_Query.txt:	Query b
    
  - Problem_3c_Query.txt:	Query c
    
  - Problem_3d_Query.txt:	Query d
```

