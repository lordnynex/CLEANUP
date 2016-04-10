//Written by: Kevin Wang
//Parses VEHV2PUB.CSV data and inserts into Vehicles table
#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <pqxx/pqxx>  
#define NUMATTR 25
using namespace std; 
using namespace pqxx; 
string sqlvehicle(ifstream& file, int, int);

enum vehAttr{
	HOUSEID,WTHHFIN,VEHID,DRVRCNT,HHFAMINC,HHSIZE,HHVEHCNT,NUMADLT,FLAG100,CDIVMSAR,CENSUS_D,CENSUS_R,HHSTATE,HHSTFIPS,HYBRID,MAKECODE,MODLCODE,MSACAT,MSASIZE,OD_READ,RAIL,TRAVDAY,URBAN,URBANSIZE,URBRUR,VEHCOMM,VEHOWNMO,VEHYEAR,WHOMAIN,WRKCOUNT,TDAYDATE,VEHAGE,PERSONID,HH_HISP,HH_RACE,HOMEOWN,HOMETYPE,LIF_CYC,ANNMILES,HBHUR,HTRESDN,HTHTNRNT,HTPPOPDN,HTEEMPDN,HBRESDN,HBHTNRNT,HBPPOPDN,BEST_FLG,BESTMILE,BEST_EDT,BEST_OUT,FUELTYPE,GSYRGAL,GSCOST,GSTOTCST,EPATMPG,EPATMPGF,EIADMPG,VEHTYPE,HH_CBSA,HHC_MSA
};

int main(int argc, char* argv[]) { 
  int li;
  cin >> li;
  li--;
  // cout << sqlvehicle(vfile)<<endl;
  string user(getenv("USER")), a;
  try { 
    ifstream vfile("VEHV2PUB.CSV");
    connection C("dbname=postgres user="+user); 
    if (C.is_open()) { 
      cout << "Opened database successfully: " << C.dbname() << endl; 
    } else { 
      cout << "Can't open database" << endl; 
      return 1; 
    } 
      /* Create a transactional object. */ 
    work W(C); 
    getline(vfile, a);
      /* Execute SQL query */ 
    /*tablecreation*/
    //SPLIT UP TO FIT IN RAM
    cout << "inserting to vehicle" << endl;
    for(int i = 0; i < 4; i++){
        W.exec( sqlvehicle( vfile, i*(li/4),(i+1)*(li/4) ) );
        cout << i+1 << "/4\n";
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

string sqlvehicle(ifstream& file, int start, int end){
	string query = "INSERT INTO VEHICLES(HOUSEID,VEHID,HYBRID,MAKECODE,MODLCODE,OD_READ,VEHCOMM,VEHOWNMO,VEHYEAR,WHOMAIN,VEHAGE,PERSONID,ANNMILES,BEST_FLG,BESTMILE,BEST_EDT,BEST_OUT,FUELTYPE,GSYRGAL,GSCOST,GSTOTCST,EPATMPG,EPATMPGF,EIADMPG,VEHTYPE) "
					"VALUES (";
	string line, delimiter=",",token;
	int attrs[] = {HOUSEID,VEHID,HYBRID,MAKECODE,MODLCODE,OD_READ,VEHCOMM,VEHOWNMO,VEHYEAR,WHOMAIN,VEHAGE,PERSONID,ANNMILES,BEST_FLG,BESTMILE,BEST_EDT,BEST_OUT,FUELTYPE,GSYRGAL,GSCOST,GSTOTCST,EPATMPG,EPATMPGF,EIADMPG,VEHTYPE};
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