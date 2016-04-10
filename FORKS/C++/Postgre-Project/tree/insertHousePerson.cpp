//Written by: Kevin Wang 
//Parses HHV2PUB.CSV + PERV2PUB.CSV data and inserts into House and Person table
#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <pqxx/pqxx>  
#define NUMATTR 71
using namespace std; 
using namespace pqxx; 
string sqlhouse(ifstream& file,int,int);
string sqlperson(ifstream& file,int,int);

enum personAttr{
	HOUSEID,PERSONID,VARSTRAT,WTPERFIN,SFWGT,HH_HISP,HH_RACE,DRVRCNT,HHFAMINC,HHSIZE,HHVEHCNT,NUMADLT,WRKCOUNT,FLAG100,LIF_CYC,CNTTDTR,BORNINUS,CARRODE,CDIVMSAR,CENSUS_D,CENSUS_R,CONDNIGH,CONDPUB,CONDRIDE,CONDRIVE,CONDSPEC,CONDTAX,CONDTRAV,DELIVER,DIARY,DISTTOSC,DRIVER,DTACDT,DTCONJ,DTCOST,DTRAGE,DTRAN,DTWALK,EDUC,EVERDROV,FLEXTIME,FMSCSIZE,FRSTHM,FXDWKPL,GCDWORK,GRADE,GT1JBLWK,HHRESP,HHSTATE,HHSTFIPS,ISSUE,OCCAT,LSTTRDAY,MCUSED,MEDCOND,MEDCOND6,MOROFTEN,MSACAT,MSASIZE,NBIKETRP,NWALKTRP,OUTCNTRY,OUTOFTWN,PAYPROF,PRMACT,PROXY,PTUSED,PURCHASE,R_AGE,R_RELAT,R_SEX,RAIL,SAMEPLC,SCHCARE,SCHCRIM,SCHDIST,SCHSPD,SCHTRAF,SCHTRN1,SCHTRN2,SCHTYP,SCHWTHR,SELF_EMP,TIMETOSC,TIMETOWK,TOSCSIZE,TRAVDAY,URBAN,URBANSIZE,URBRUR,USEINTST,USEPUBTR,WEBUSE,WKFMHMXX,WKFTPT,WKRMHM,WKSTFIPS,WORKER,WRKTIME,WRKTRANS,YEARMILE,YRMLCAP,YRTOUS,DISTTOWK,TDAYDATE,HOMEOWN,HOMETYPE,HBHUR,HTRESDN,HTHTNRNT,HTPPOPDN,HTEEMPDN,HBRESDN,HBHTNRNT,HBPPOPDN,HH_CBSA,HHC_MSA
}; /* Used to differentiate between used attributes in file and non used attributes */

int main(int argc, char* argv[]) { 
  int li, li2;
  string h, p;
  cin >> li >> h >> li2 >> p;
  li--; li2--;
  /* Obtain number of lines in file through shell pipes (wc -l <filename>) */
  string user(getenv("USER")), a;
  try { 
    ifstream hfile("HHV2PUB.CSV"), pfile("PERV2PUB.CSV");
    connection C("dbname=postgres user="+user); 
    if (C.is_open()) { 
      cout << "Opened database successfully: " << C.dbname() << endl; 
    } else { 
      cout << "Can't open database" << endl; 
      return 1; 
    } 
    /* Create SQL statement */ 
	getline(hfile,a);
    getline(pfile,a);
   
    /* Create a transactional object. */ 
    work W(C); 
       
    /* Execute SQL query */ 
    /* tablecreations */
    /*SPLIT UP TO FIT IN RAM */
    
    cout << "inserting to house" << endl;
    for(int i = 0; i < 3; i++){
        W.exec( sqlhouse( hfile, i*(li/3),(i+1)*(li/3) ) );
        cout << i+1 << "/3\n"; 
    }
    cout << "inserting to person" << endl;
    for(int i = 0; i < 6; i++){
        W.exec( sqlperson( pfile, i*(li2/6),(i+1)*(li2/6) ) );
        cout << i+1 << "/6\n";
    }
    
    /*COMMIT*/
    W.commit(); 
    cout << "Rows inserted successfully" << endl; 
    C.disconnect (); 
  } catch (const std::exception &e){ 
    cerr << e.what() << std::endl; 
    return 1; 
  }
  return 0; 
}

string sqlhouse(ifstream& file, int start, int end){
	string query = "INSERT INTO HOUSES "
				   "VALUES (";
	string line, delimiter=",",token;
    int i = 0;
    for(int j = start; j < end; j++){ getline(file, line);
		size_t pos=1;
		while((pos = line.find(delimiter)) != string::npos){ //takes values between commas, adds it to query, deletes from line
			token = line.substr(0, pos);
            if(!isdigit(token.at(0)) && token.at(0)!='-')
                token = "'" + token + "'";
            if(token.substr(0,3)=="'XX")
                token = "NULL";
			query += token;
			query += ",";
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

string sqlperson(ifstream& file, int start, int end){
	string query = "INSERT INTO PERSONS(HOUSEID,PERSONID,VARSTRAT,WTPERFIN,SFWGT,CNTTDTR,BORNINUS,CARRODE,CONDNIGH,CONDPUB,CONDRIDE,CONDRIVE,CONDSPEC,CONDTAX,CONDTRAV,DELIVER,DIARY,DISTTOSC,DTACDT,DTCONJ,DTCOST,DTRAGE,DTRAN,DTWALK,EVERDROV,FLEXTIME,FMSCSIZE,FRSTHM,FXDWKPL,GCDWORK,GRADE,GT1JBLWK,ISSUE,OCCAT,LSTTRDAY,MCUSED,MEDCOND,MEDCOND6,MOROFTEN,NBIKETRP,NWALKTRP,OUTCNTRY,PAYPROF,PTUSED,PURCHASE,R_RELAT,SAMEPLC,SCHCARE,SCHCRIM,SCHDIST,SCHSPD,SCHTRAF,SCHTRN1,SCHTRN2,SCHTYP,SCHWTHR,SELF_EMP,TIMETOSC,TIMETOWK,TOSCSIZE,WEBUSE,WKFMHMXX,WKFTPT,WKRMHM,WKSTFIPS,WRKTIME,WRKTRANS,YEARMILE,YRMLCAP,YRTOUS,DISTTOWK) "
				   "VALUES (";
	string line, delimiter=",",token;
	int attrs[] = {HOUSEID,PERSONID,VARSTRAT,WTPERFIN,SFWGT,CNTTDTR,BORNINUS,CARRODE,CONDNIGH,CONDPUB,CONDRIDE,CONDRIVE,CONDSPEC,CONDTAX,CONDTRAV,DELIVER,DIARY,DISTTOSC,DTACDT,DTCONJ,DTCOST,DTRAGE,DTRAN,DTWALK,EVERDROV,FLEXTIME,FMSCSIZE,FRSTHM,FXDWKPL,GCDWORK,GRADE,GT1JBLWK,ISSUE,OCCAT,LSTTRDAY,MCUSED,MEDCOND,MEDCOND6,MOROFTEN,NBIKETRP,NWALKTRP,OUTCNTRY,PAYPROF,PTUSED,PURCHASE,R_RELAT,SAMEPLC,SCHCARE,SCHCRIM,SCHDIST,SCHSPD,SCHTRAF,SCHTRN1,SCHTRN2,SCHTYP,SCHWTHR,SELF_EMP,TIMETOSC,TIMETOWK,TOSCSIZE,WEBUSE,WKFMHMXX,WKFTPT,WKRMHM,WKSTFIPS,WRKTIME,WRKTRANS,YEARMILE,YRMLCAP,YRTOUS,DISTTOWK};
    for(int j = start; j < end; j++){ /* line by line */
        getline(file, line); 
		size_t pos=0;
		int i = 0, attrNum=0;
        while((pos = line.find(delimiter))!=string::npos && attrNum < NUMATTR){ /* attribute by attribute, separated by commas */
            token = line.substr(0, pos);
            if(i==attrs[attrNum]){ /* if attribute is in the list of attributes in this table */
                if(!isdigit(token.at(0)) && token.at(0)!='-')
                    token = "'" + token + "'";
                if(token.at(0)=='X')
                    token = "NULL";
                if((token.find("AM")!=string::npos) || (token.find("PM")!=string::npos))
                    token = "'" + token + "'";
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