//Written by: Kevin Wang
//Parses DAYV2PUB.CSV data and inserts into Daytrips table
#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <pqxx/pqxx>  
#define NUMATTR 65
using namespace std; 
using namespace pqxx; 

string sqldaytrip(ifstream& file, int, int);

enum dayAttr{
	HOUSEID,PERSONID,FRSTHM,OUTOFTWN,ONTD_P1,ONTD_P2,ONTD_P3,ONTD_P4,ONTD_P5,ONTD_P6,ONTD_P7,ONTD_P8,ONTD_P9,ONTD_P10,ONTD_P11,ONTD_P12,ONTD_P13,ONTD_P14,ONTD_P15,TDCASEID,HH_HISP,HH_RACE,DRIVER,R_SEX,WORKER,DRVRCNT,HHFAMINC,HHSIZE,HHVEHCNT,NUMADLT,FLAG100,LIF_CYC,TRIPPURP,AWAYHOME,CDIVMSAR,CENSUS_D,CENSUS_R,DROP_PRK,DRVR_FLG,EDUC,ENDTIME,HH_ONTD,HHMEMDRV,HHRESP,HHSTATE,HHSTFIPS,INTSTATE,MSACAT,MSASIZE,NONHHCNT,NUMONTRP,PAYTOLL,PRMACT,PROXY,PSGR_FLG,R_AGE,RAIL,STRTTIME,TRACC1,TRACC2,TRACC3,TRACC4,TRACC5,TRACCTM,TRAVDAY,TREGR1,TREGR2,TREGR3,TREGR4,TREGR5,TREGRTM,TRPACCMP,TRPHHACC,TRPHHVEH,TRPTRANS,TRVL_MIN,TRVLCMIN,TRWAITTM,URBAN,URBANSIZE,URBRUR,USEINTST,USEPUBTR,VEHID,WHODROVE,WHYFROM,WHYTO,WHYTRP1S,WRKCOUNT,DWELTIME,WHYTRP90,TDTRPNUM,TDWKND,TDAYDATE,TRPMILES,WTTRDFIN,VMT_MILE,PUBTRANS,HOMEOWN,HOMETYPE,HBHUR,HTRESDN,HTHTNRNT,HTPPOPDN,HTEEMPDN,HBRESDN,HBHTNRNT,HBPPOPDN,GASPRICE,VEHTYPE,HH_CBSA,HHC_MSA
}day; /* Used to differentiate between used attributes in file and non used attributes */

int main(int argc, char* argv[]) { 
  int li;
  cin >> li;   
  li--;
  string user(getenv("USER")), a;
  try { 
    ifstream dfile("DAYV2PUB.CSV");
    connection C("dbname=postgres user="+user);  
    if (C.is_open()) { 
      cout << "Opened database successfully: " << C.dbname() << endl; 
    } else { 
      cout << "Can't open database" << endl; 
      return 1; 
    } 
    /* Create SQL statement */ 
    /* Create a transactional object. */ 
    getline(dfile, a);
    work W(C); 
       
    /* Execute SQL query */ 
    /* tablecreations */
    /* SPLIT UP TO FIT IN RAM */
    cout << "inserting to daytrip" << endl;
    for(int i = 0; i < 16; i++){
        W.exec( sqldaytrip( dfile, i*(li/16),(i+1)*(li/16) ) );
        cout << i+1 << "/16\n";
    }
    /* COMMIT */
    W.commit(); 
    cout << "Rows inserted successfully" << endl; 
    C.disconnect (); 
  } catch (const std::exception &e){ 
    cerr << e.what() << std::endl; 
    return 1; 
  }
  cout << "done\n";
  return 0; 
}

string sqldaytrip(ifstream& file, int start, int end){
	string query = "INSERT INTO DAYTRIP(HOUSEID,PERSONID,ONTD_P1,ONTD_P2,ONTD_P3,ONTD_P4,ONTD_P5,ONTD_P6,ONTD_P7,ONTD_P8,ONTD_P9,ONTD_P10,ONTD_P11,ONTD_P12,ONTD_P13,ONTD_P14,ONTD_P15,TDCASEID,TRIPPURP,AWAYHOME,DROP_PRK,DRVR_FLG,ENDTIME,HH_ONTD,HHMEMDRV,INTSTATE,NONHHCNT,NUMONTRP,PAYTOLL,PSGR_FLG,STRTTIME,TRACC1,TRACC2,TRACC3,TRACC4,TRACC5,TRACCTM,TREGR1,TREGR2,TREGR3,TREGR4,TREGR5,TREGRTM,TRPACCMP,TRPHHACC,TRPHHVEH,TRPTRANS,TRVL_MIN,TRVLCMIN,TRWAITTM,VEHID,WHODROVE,WHYFROM,WHYTO,WHYTRP1S,DWELTIME,WHYTRP90,TDTRPNUM,TDWKND,TDAYDATE,TRPMILES,WTTRDFIN,VMT_MILE,PUBTRANS,GASPRICE) "
					"VALUES (";
	string line, delimiter=",",token;
	int attrs[] = {HOUSEID,PERSONID,ONTD_P1,ONTD_P2,ONTD_P3,ONTD_P4,ONTD_P5,ONTD_P6,ONTD_P7,ONTD_P8,ONTD_P9,ONTD_P10,ONTD_P11,ONTD_P12,ONTD_P13,ONTD_P14,ONTD_P15,TDCASEID,TRIPPURP,AWAYHOME,DROP_PRK,DRVR_FLG,ENDTIME,HH_ONTD,HHMEMDRV,INTSTATE,NONHHCNT,NUMONTRP,PAYTOLL,PSGR_FLG,STRTTIME,TRACC1,TRACC2,TRACC3,TRACC4,TRACC5,TRACCTM,TREGR1,TREGR2,TREGR3,TREGR4,TREGR5,TREGRTM,TRPACCMP,TRPHHACC,TRPHHVEH,TRPTRANS,TRVL_MIN,TRVLCMIN,TRWAITTM,VEHID,WHODROVE,WHYFROM,WHYTO,WHYTRP1S,DWELTIME,WHYTRP90,TDTRPNUM,TDWKND,TDAYDATE,TRPMILES,WTTRDFIN,VMT_MILE,PUBTRANS,GASPRICE};
    for(int j = start; j < end; j++){ /* line by line */
        getline(file, line);
		size_t pos=0;
		int i = 0, attrNum=0;
		while((pos = line.find(delimiter))!=string::npos && attrNum < NUMATTR){ /* attribute by attribute, separated by commas */
            token = line.substr(0, pos);
            if(i==attrs[attrNum]){ /* if attribute is in the list of attributes in this table */
                if(!isdigit(token.at(0)) && token.at(0)!='-')
                    token = "'" + token + "'";
                else if(token.at(0)=='X')
                    token = "NULL";
				query.append( token);
				query.append( delimiter);
                attrNum++;
			}
            line.erase(0, pos + delimiter.length());
            i++;
		}
        query.erase(query.length()-1);
		query+=( "), (");
	}
    query.erase(query.length()-3,query.length()-1);
	query += ";";
	return query;
}