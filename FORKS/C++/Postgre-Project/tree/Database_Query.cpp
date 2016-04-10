//Written by: Jake Miller
//Reads in query files and executes them
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<pqxx/pqxx>
#include<vector>
#include<cstdlib>
using namespace std;
using namespace pqxx;

void PrintResult(result &r, string s);

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

	ifstream Problem_3a("Problem_3a_Query.txt");
	string Prob_3a_q( (istreambuf_iterator<char>(Problem_3a)), ( istreambuf_iterator<char>() ) );

        ifstream Problem_3b("Problem_3b_Query.txt");
        string Prob_3b_q( (istreambuf_iterator<char>(Problem_3b)), ( istreambuf_iterator<char>() ) );

        ifstream Problem_3c("Problem_3c_Query.txt");
        string Prob_3c_q( (istreambuf_iterator<char>(Problem_3c)), ( istreambuf_iterator<char>() ) );

        ifstream Problem_3d("Problem_3d_Query.txt");
        string Prob_3d_q( (istreambuf_iterator<char>(Problem_3d)), ( istreambuf_iterator<char>() ) );
	work W(C);
    
	cout << "Query a...\n";
	result r_a = W.exec(Prob_3a_q);
        
    cout << "Printing result...\n";
	PrintResult(r_a, "Problem 3a (Miles Traveled, Percentile)");	
    cout << "Query b...\n";
        
    result r_b = W.exec(Prob_3b_q);
        
    cout << "Printing result...\n";
    PrintResult(r_b, "Problem 3b (Trip Length, Percentile)");
    cout << "Query c...\n";
        
    result r_c = W.exec(Prob_3c_q);
        
    cout << "Printing result...\n";
    PrintResult(r_c, "Problem 3c (Date, Percent CO2 emissions)");
    cout << "Query d...\n";
        
    result r_d = W.exec(Prob_3d_q);
        
    cout << "Printing result...\n";
    PrintResult(r_d, "Problem 3d (20 mile range CO2 change, 40 mile range CO2 change, 60 mile range CO2 change), in metric tonnes");
    W.commit();

	Problem_3a.close();
	Problem_3b.close();
	Problem_3c.close();
	Problem_3d.close();

        C.disconnect ();
        }
  catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
        }
    cout << "Done!\n";
	return 0;
}

void PrintResult(result &r, string s){
	int row_size = r.size();

	cout<<"Result Table:"<<s<<endl;
	for(int row_num = 0; row_num < row_size; row_num++){
		int col_size = r[row_num].size();
		for(int col_num = 0; col_num < col_size; col_num++){
			cout<<setw(10)<<r[row_num][col_num].c_str()<<setw(2)<<"|";
		}
		cout<<endl;
	}

}
