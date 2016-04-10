#include "Tariff.h"
#include "SMSMessage.h"
#include "utils.h"
#include <iostream>
#include <cstring>

using namespace std;

void print_usage( ) {
    cerr << "Invalid usage:" << endl;
    cerr << "\tthelper check " << endl;

}

int main( int argc, char** argv ) {
    if ( argc != 2 ) {
        print_usage();
        return 0;
    }

    if ( std::string(argv[1]) == "check" ) {
        if ( argc != 2 ) {
            print_usage();
            return 0;
        }

        do {
            std::string tariff_name;
            std::string phone;
            int status;
            cin >> tariff_name >> phone >> status;

            Tariff tariff = TariffManager::get_mutable_instance().loadTariff( tariff_name );

            sms::MessageClassifier::CountryInfo ci = sms::MessageClassifier::get_mutable_instance().getMsgClass( phone );
            if ( ci.operators.empty() ) {
                cout << ci.cName << ";"
                     << ci.mcc << ";"
                     << ";"
                     << ";"
                     << ";"
                     << tariff.costs( ci.mcc, SMSMessage::Status( status ) );

            } else {
                cout << ci.cName << ";"
                     << ci.mcc << ";"
                     << ci.operators.begin()->second.getName() << ";"
                     << ci.operators.begin()->second.getCode() << ";"
                     << ci.operators.begin()->second.opRegion << ";"
                     << tariff.costs( ci.mcc, ci.operators.begin()->second.mnc, SMSMessage::Status( status ) );
            }
            cout << endl;
        } while ( !cin.eof() );

    }

}
