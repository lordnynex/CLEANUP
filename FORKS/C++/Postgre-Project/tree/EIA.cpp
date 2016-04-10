//Written by: Jake Miller 
//Creates and inserts into EIA data 
#include<iostream>
#include<string>
#include<fstream>
#include<pqxx/pqxx>
#include<ctype.h>
#include<cstring>
#include<cstdlib>
#include<math.h>
#include<vector>

using namespace std;
using namespace pqxx;

void Load_EIA(connection &C);

int main(){
  string user(getenv("USER"));
  try {
	connection C("dbname=postgres user="+user);
	if (C.is_open()) {
	        cout << "Opened database successfully: " << C.dbname() << endl;
	}
     	else {
		cout << "Can't open database" << endl;
		return 1;
	}


	Load_EIA(C);

	C.disconnect ();
  	} 
  catch (const std::exception &e){
	cerr << e.what() << std::endl;
	return 1;
	}

  return 0;
  }

void Load_EIA(connection &C){
   /*Creating EIA_VALUES Table */
   cout<<"Parsing EIA Files:"<<endl;
   work W_1(C);
   cout << "Creating EIA_VALUES table..."<<endl;
   string EIA_value_sql = "CREATE TABLE EIA_VALUES("
	   "MSN CHAR(8),"
	   "DATE INT,"
	   "VALUE FLOAT,"
	   "PRIMARY KEY (MSN, DATE) );";
   W_1.exec( EIA_value_sql );
   W_1.commit();
   
   /* Creating EIA_CO2TRANS Table */
   cout << "Creating EIA_CO2TRANS table..."<<endl;
   work W_2(C);
   string EIA_Trans = "CREATE TABLE EIA_CO2TRANS("
	  "MSN 	CHAR(8) );";
   W_2.exec( EIA_Trans );
   W_2.commit();
   
   /* Creating EIA_CO2ELEC Table */
   cout << "Creating EIA_CO2ELEC table..."<<endl;
   work W_3(C);
   string EIA_Elec = "CREATE TABLE EIA_CO2ELEC("
 	 "MSN CHAR(8) );"; 
   W_3.exec( EIA_Elec );
   W_3.commit();
   
   /* Creating EIA_MKWH Table */
   cout << "Creating EIA_MKWH table..."<<endl;
   work W_4(C);
   string EIA_MkWh = "CREATE TABLE EIA_MKWH("
	   "MSN CHAR(8) );";
   W_4.exec( EIA_MkWh );
   W_4.commit();
   
   /* Opening EIA files */
   ifstream Transfile("EIA_CO2_Transportation_2014.csv");
   ifstream Elecfile("EIA_CO2_Electric_2014.csv");
   ifstream MkWhfile("EIA_MkWh_2014.csv");

   string line, item1, item2, item3, sql1, sql2, SQLTot1, SQLTot2;
   string header1 = "INSERT INTO EIA_VALUES(MSN, DATE, VALUE) VALUES (";
   string header2 = "INSERT INTO EIA_CO2TRANS(MSN) VALUES (";

   /* Insert EIA Trasporation File */
   getline(Transfile,line);
   while(getline(Transfile, line)){
	   item1 = line.substr(0, line.find(",") );
	   line.erase(0, line.find(",") + 1 );
	   item2 = line.substr(0, line.find(",\"") );
	   sql1 = header1 + "\'" + item1 + "\'," + item2 + ");";
	   sql2 = header2 + "\'" + item1 + "\');";

	   SQLTot1 = SQLTot1 + " " + sql1;
	   SQLTot2 = SQLTot2 + " " + sql2;
   }

   work T_1(C);
   T_1.exec( SQLTot1 );
   T_1.commit();

   work S_1(C);
   S_1.exec( SQLTot2 );
   S_1.commit();

   SQLTot1.clear();
   SQLTot2.clear();

   header2.assign("INSERT INTO EIA_CO2ELEC(MSN) VALUES (");

   /* Insert EIA Electric CO2 File */
   getline(Elecfile,line);
   while(getline(Elecfile, line)){
	   item1 = line.substr(0, line.find(",") );
           line.erase(0, line.find(",") + 1 );
	   item2 = line.substr(0, line.find(",\"") );
	   line.erase(0, line.find(",") + 1 );
	   item3 = line.substr(0, line.find(",") );
	   if(item3 != "Not Available"){ /* Disgarding Entries without Value reads */
	   	sql1 = header1 + "\'" + item1 + "\'," + item2 + ");";
	   	sql2 = header2 + "\'" + item1 + "\');";

		SQLTot1 = SQLTot1 + " " + sql1;
		SQLTot2 = SQLTot2 + " " + sql2;
	   }
   }

   work T_2(C);
   T_2.exec( SQLTot1 );
   T_2.commit();

   work S_2(C);
   S_2.exec( SQLTot2 );
   S_2.commit();

   SQLTot1.clear();
   SQLTot2.clear();

   header2.assign("INSERT INTO EIA_MKWH(MSN) VALUES (");

   /* Insert EIA Electric MkWh File */
   getline(MkWhfile,line);
   while(getline(MkWhfile, line)){
	   item1 = line.substr(0, line.find(",") );
	   line.erase(0, line.find(",") + 1 );
	   item2 = line.substr(0, line.find(",\"") );
	   line.erase(0, line.find(",") + 1 );
	   item3 = line.substr(0, line.find(",") );
           if(item3 != "Not Available"){ /* Disregarding Entries without Value reads */
		   sql1 = header1 + "\'" + item1 + "\'," + item2 + ");";
		   sql2 = header2 + "\'" + item1 + "\');";

		   SQLTot1 = SQLTot1 + " " + sql1;                    
		   SQLTot2 = SQLTot2 + " " + sql2;    
	   }
   }          
      
   work T_3(C);
   T_3.exec( SQLTot1 );
   T_3.commit();
	       
   work S_3(C);
   S_3.exec( SQLTot2 );

   S_3.commit();
		                                     
   SQLTot1.clear();                                
   SQLTot2.clear();                
   /* Closing Files */
   Transfile.close();
   Elecfile.close();
   MkWhfile.close();
   cout<<"EIA Files in Database."<<endl;
}