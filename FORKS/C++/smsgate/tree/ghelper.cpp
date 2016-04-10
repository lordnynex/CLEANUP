#include "SMPPGateFilterParser.h"

#include <iostream>

using namespace sms;
using namespace std;

int main() {
    std::map< std::string, boost::any > args;

    args[ "TO" ] = string("79852970920");
    args[ "COUNTRY" ] = string("ru");
    args[ "COUNTRYCODE" ] = string("7");
    args[ "OPERATOR" ] = string("mts");
    args[ "OPERATORCODE" ] = string("mts");
    args[ "FROM" ] = string("1312");

    while ( !cin.eof() ) {
        char line[1024];
        cin.getline( line, 1024 );

        SMPPGateFilterParser parser;
        SMPPGateFilterParser::ResT res = parser.parseStr( line );

        if ( res.ok ) {
            cout << "Parsed OK" << endl;
        } else {
            cout << "Error at [" << res.where << "]" << endl;
        }
    }

    return 0;
}
