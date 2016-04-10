#include "SMPPGateFilterParser.h"

namespace sms {
    void SMPPFilterBuilder::addRangeArgMark() {
        Operand top = ops_list.back();
        ops_list.pop_back();
        switch( top.type() ) {
                case OP_STRING:
            ops_list.push_back( Operand::create<OP_MARKED_STRING>( top.get<OP_STRING>() ) );
            return;
                case OP_NUMBER:
            ops_list.push_back( Operand::create<OP_MARKED_NUMBER>( top.get<OP_NUMBER>() ) );
            return;
        }
        ops_list.push_back( top );
    }

    SMPPFilterBuilder::Operand SMPPFilterBuilder::delRangeArgMark( Operand op ) {
        switch( op.type() ) {
                case OP_MARKED_STRING:
            return Operand::create<OP_STRING>( op.get<OP_STRING>() );
                case OP_MARKED_NUMBER:
            return Operand::create<OP_NUMBER>( op.get<OP_NUMBER>() );
        }
        return op;
    }

    bool SMPPFilterBuilder::isMarked( Operand op ) {
        switch ( op.type() ) {
                case OP_MARKED_STRING:
            return true;
                case OP_MARKED_NUMBER:
            return true;
        }
        return false;
    }

    bool SMPPFilterBuilder::isLinked( Operand op ) {
        switch ( op.type() ) {
                case OP_LINKED_STRING:
            return true;
                case OP_LINKED_NUMBER:
            return true;
        }
        return false;
    }

    void SMPPFilterBuilder::addNumericArg( const char* begin, const char* end ) {
        std::string s ( begin, end );
        ops_list.push_back( Operand::create<OP_NUMBER>( boost::lexical_cast< long >( std::string( begin, end ) ) ) );
    }
    void SMPPFilterBuilder::addStringArg( const char* begin, const char* end ) {
        std::string s ( begin, end );
        ops_list.push_back( Operand::create<OP_STRING>( std::string( begin, end ) ) );
    }
    void SMPPFilterBuilder::addLinkedNumericArg( const char* begin, const char* end ) {
        std::string s ( begin, end );
        ops_list.push_back( Operand::create<OP_LINKED_NUMBER>( std::string( begin, end ) ) );
    }
    void SMPPFilterBuilder::addLinkedStringArg( const char* begin, const char* end ) {
        ops_list.push_back( Operand::create<OP_LINKED_STRING>( std::string( begin, end ) ) );
    }
    void SMPPFilterBuilder::add_AND_Operation() { funcList.push_back( "AND" ); }
    void SMPPFilterBuilder::add_OR_Operation() { funcList.push_back( "OR" ); }
    void SMPPFilterBuilder::add_NOT_Operation() { funcList.push_back( "NOT" ); }
    void SMPPFilterBuilder::add_EQUAL_Operation() { funcList.push_back( "EQUAL" ); }
    void SMPPFilterBuilder::add_LESS_Operation() { funcList.push_back( "LESS" ); }
    void SMPPFilterBuilder::add_MORE_Operation() { funcList.push_back( "MORE" ); }
    void SMPPFilterBuilder::add_EQUAL_OR_LESS_Operation() { funcList.push_back( "EQUAL_OR_LESS" ); }
    void SMPPFilterBuilder::add_EQUAL_OR_MORE_Operation() { funcList.push_back( "EQUAL_OR_MORE" ); }
    void SMPPFilterBuilder::add_NOT_EQUAL_Operation() { funcList.push_back( "NOT EQUAL" ); }
    void SMPPFilterBuilder::add_IN_Operation() { funcList.push_back( "IN" ); }
    void SMPPFilterBuilder::add_NEGATIVE_Operation() { funcList.push_back( "NEGATIVE" ); }
    void SMPPFilterBuilder::add_POSITIVE_Operation() { funcList.push_back( "POSITIVE" ); }

    SMPPFilterBuilder::opsListT SMPPFilterBuilder::args_link( valsMapT& map, const opsListT& req_ops ) {
        opsListT ops;
        opsListT::const_iterator it;
        for ( it = req_ops.begin(); it != req_ops.end(); it++ ) {
            Operand op = *it;
            if ( !isLinked( op ) ) {
                ops.push_back( op );
                continue;
            }

            std::string s = op.get<OP_LINKED_STRING>();

            switch( op.type() ) {
            case OP_LINKED_STRING:
                if ( map.find( op.get<OP_LINKED_STRING>() ) == map.end() ) {
                    ops.push_back( Operand::create<OP_STRING>( "NULL" ) );
                    break;
                }
                ops.push_back( Operand::create<OP_STRING>( boost::any_cast<std::string>( map[ op.get<OP_LINKED_STRING>() ] ) ) );
                break;
            case OP_LINKED_NUMBER:
                if ( map.find( op.get<OP_LINKED_NUMBER>() ) == map.end() ) {
                    ops.push_back( Operand::create<OP_NUMBER>( -1 ) );
                    break;
                }
                ops.push_back( Operand::create<OP_NUMBER>( boost::any_cast< long >( map[ op.get<OP_LINKED_STRING>() ] ) ) );
                break;
            default:
                ops.push_back( op );
            }

        }

        return ops;
    }

    bool SMPPFilterBuilder::check( valsMapT vals) {
        funcListT fList = funcList;
        opsListT oList = args_link( vals, ops_list );
        while (!fList.empty()) {
            std::string op = fList.front(); fList.pop_front();
            if ( op == "AND" ) {
                Operand l = oList.front(); oList.pop_front(); if ( l.type() != OP_BOOL )  return false;
                Operand r = oList.front(); oList.pop_front(); if ( r.type() != OP_BOOL )  return false;
                oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_BOOL>() && r.get<OP_BOOL>() ) ) );
                continue;
            }
            if ( op == "OR" ) {
                Operand l = oList.front(); oList.pop_front(); if ( l.type() != OP_BOOL )  return false;
                Operand r = oList.front(); oList.pop_front(); if ( r.type() != OP_BOOL )  return false;
                oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_BOOL>() || r.get<OP_BOOL>() ) ) );
                continue;
            }
            if ( op == "NOT" ) {
                Operand l = oList.back(); oList.pop_back(); if ( l.type() != OP_BOOL )  return false;
                oList.push_back( Operand::create<OP_BOOL>( !l.get<OP_BOOL>() ) );
                continue;
            }
            if ( op == "EQUAL" ) {
                Operand l = oList.front(); oList.pop_front();
                Operand r = oList.front(); oList.pop_front();
                if ( l.type() != r.type() )  return false;
                switch ( l.type() ) {
                     case OP_STRING:
                        oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_STRING>() == r.get<OP_STRING>() ) ) );
                        break;
                    case OP_NUMBER:
                        oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_NUMBER>() == r.get<OP_NUMBER>() ) ) );
                        break;
                    case OP_BOOL:
                        oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_BOOL>() == r.get<OP_BOOL>() ) ) );
                        break;
                    default:
                        return false;
                }
                continue;
            }
            if ( op == "LESS" ) {
                Operand l = oList.front(); oList.pop_front();
                Operand r = oList.front(); oList.pop_front();
                if ( l.type() != r.type() )  return false;
                switch ( l.type() ) {
                        case OP_NUMBER:
                    oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_NUMBER>() < r.get<OP_NUMBER>() ) ) );
                    break;
                        default:
                    return false;
                }
                continue;
            }
            if ( op == "MORE" ) {
                Operand l = oList.front(); oList.pop_front();
                Operand r = oList.front(); oList.pop_front();
                if ( l.type() != r.type() )  return false;
                switch ( l.type() ) {
                        case OP_NUMBER:
                    oList.push_back( Operand::create<OP_BOOL>( ( l.get<OP_NUMBER>() > r.get<OP_NUMBER>() ) ) );
                    break;
                        default:
                    return false;
                }
                continue;
            }
            if ( op == "NOT EQUAL" ) { fList.push_front("NOT");fList.push_front("EQUAL"); continue; }
            if ( op == "EQUAL OR LESS" ) { fList.push_front("NOT");fList.push_front("MORE"); continue; }
            if ( op == "EQUAL OR MORE" ) { fList.push_front("NOT");fList.push_front("LESS"); continue; }
            if ( op == "NEGATIVE" ) {
                Operand l = oList.back(); oList.pop_back(); if ( l.type() != OP_NUMBER )  return false;
                oList.push_back( Operand::create<OP_NUMBER>( -l.get<OP_NUMBER>() ) );
                continue;
            }
            if ( op == "POSITIVE" ) {
                Operand l = oList.back(); oList.pop_back(); if ( l.type() != OP_NUMBER )  return false;
                oList.push_back( Operand::create<OP_NUMBER>( +l.get<OP_NUMBER>() ) );
                continue;
            }
            if ( op == "IN" ) {
                Operand l = oList.front(); oList.pop_front();
                Operand r = oList.front();
                Operand k;
                bool found = false;
                while ( isMarked( r ) ) {
                    k = delRangeArgMark( r );
                    if ( l.type() != k.type() ) return false;

                    switch ( l.type() ) {
                    case OP_STRING:
                        if ( l.get<OP_STRING>() == k.get<OP_STRING>() ) found = true;
                        break;
                    case OP_NUMBER:
                        if ( l.get<OP_NUMBER>() == k.get<OP_NUMBER>() ) found = true;
                        break;
                    default:
                        return false;
                    }

                    oList.pop_front();
                    if ( !oList.empty() )
                        r = oList.front();
                    else
                        break;
                }
                oList.push_back( Operand::create<OP_BOOL>( found ) );
                continue;
            }
        }


        if ( ( oList.size() == 1 ) && ( oList.front().type() == OP_BOOL ) ) {
            bool res = oList.front().get<OP_BOOL>();
            return res;
        } else if ( oList.size() == 0 ) {
            return true;
        } else
            return false;

    }

}

