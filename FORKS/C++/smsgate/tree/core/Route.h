#ifndef ROUTE_H
#define ROUTE_H

#define BOOST_MPL_LIMIT_STRING_SIZE 60

#include <string>
#include <list>
#include <vector>
#include <map>
#include <boost/serialization/singleton.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/set.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/logic/tribool.hpp>

#include "MessageClassifier.h"
#include "SMSMessage.h"
#include "PGSql.h"

template < class ValueDescr, class DefaultValueT >
class RouteValueChoise {
public:
    typedef std::list< std::string > DescriptionList;
    typedef std::string ValueT;

    RouteValueChoise( std::string _value = boost::mpl::c_str< DefaultValueT >::value ) {
        setValue( _value );
    }

    void setValue( std::string _value ) {
        bool value_correct = false;
        boost::mpl::for_each< ValueDescr >( checkValue( _value, value_correct ) );
        if ( !value_correct )
            value = boost::mpl::c_str< DefaultValueT >::value;
        else
            value = _value;
    }

    ValueT getValue() { return value; }

    static DescriptionList getDescriptions() {
        DescriptionList descrList;
        boost::mpl::for_each< ValueDescr >( generateDescrList( descrList ) );
        return descrList;
    }

    template<class Archive>
        void serialize(Archive & ar, const unsigned int) {
            ar & BOOST_SERIALIZATION_NVP( value );
        }

protected:
    std::string value;

private:
    class generateDescrList {
    public:
        generateDescrList( DescriptionList& _descrList ): descrList( _descrList ) {}
        template < class Element >
        void operator() (Element) {
            descrList.push_back( boost::mpl::c_str< Element >::value );
        }
    private:
        DescriptionList& descrList;
    };

    class checkValue {
    public:
        checkValue( std::string _value, bool& _res ): value( _value ), res( _res ) {}
        template < class Element >
        void operator() (Element) {
            if ( value == boost::mpl::c_str< Element >::value )
            res = true;
        }

    private:
        std::string value;
        bool& res;
    };
};

template < class ValueDescr >
class RouteValueMulti {
public:
    typedef std::list< std::string > DescriptionList;
    typedef std::set< std::string > ValuesListT;
    typedef ValuesListT ValueT;

    RouteValueMulti( ValuesListT _values = ValuesListT() ) {
        setValues( _values );
    }

    void setValues( ValuesListT _values ) {
        value.clear();
        for ( ValuesListT::iterator it = _values.begin(); it != _values.end(); it++ ) {
            bool value_correct = false;
            boost::mpl::for_each< ValueDescr >( checkValue( *it, value_correct ) );
            if ( !value_correct )
                continue;
            value.insert( *it );
        }
    }

    ValuesListT getValues( ) { return value; }

    static DescriptionList getDescriptions() {
        DescriptionList descrList;
        boost::mpl::for_each< ValueDescr >( generateDescrList( descrList ) );
        return descrList;
    }

    template<class Archive>
        void serialize(Archive & ar, const unsigned int) {
            ar & BOOST_SERIALIZATION_NVP( value );
        }

protected:
    ValuesListT value;

private:
    class generateDescrList {
    public:
        generateDescrList( DescriptionList& _descrList ): descrList( _descrList ) {}
        template < class Element >
        void operator() ( Element ) {
            descrList.push_back( boost::mpl::c_str< Element >::value );
        }
    private:
        DescriptionList& descrList;
    };

    class checkValue {
    public:
        checkValue( std::string _value, bool& _res ): value( _value ), res( _res ) {}
        template < class Element >
        void operator() (Element) {
            if ( value == boost::mpl::c_str< Element >::value )
            res = true;
        }

    private:
        std::string value;
        bool& res;
    };
};

class RouteValueDouble {
public:
    typedef double ValueT;

    RouteValueDouble( double _values = -1.0 ) {
        setValue( _values );
    }

    void setValue( ValueT _value ) {
        value = _value;
    }

    ValueT getValue( ) { return value; }

    template<class Archive>
        void serialize(Archive & ar, const unsigned int) {
            ar & BOOST_SERIALIZATION_NVP( value );
        }

protected:
    ValueT value;
};

template < class Name, class Storage >
class RouteOption: public Storage {
public:
    static std::string getName() { return boost::mpl::c_str< Name >::value; }

    RouteOption() {}
    RouteOption( std::string src ) {
        std::istringstream ifs( src );
        try {
            typename Storage::ValueT& value( Storage::value );
            boost::archive::xml_iarchive ia( ifs );
            ia >> BOOST_SERIALIZATION_NVP( value );
        } catch ( std::exception& err ) {}
    }

    std::string serialize() {
        std::ostringstream ofs;
        try {
            typename Storage::ValueT& value( Storage::value );
            boost::archive::xml_oarchive oa( ofs );
            oa << BOOST_SERIALIZATION_NVP( value );
        } catch (...) {}
        return ofs.str();
    }

};

class Route {
public:
    static const double INVALID_VALUE = -1.0;
    enum OptionLevel {
        OPT_GLOBAL,
        OPT_COUNTRY,
        OPT_OPERATOR
    };

    typedef RouteOption<
                            boost::mpl::string< 'R','e','s','e','n','d','O','p','t','i','o','n','s' >,
                            RouteValueMulti
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< 'O','n','R','E','J','E','C','T' >,
                                    boost::mpl::string< 'O','n','A','C','K','_','E','X','P','I','R','E'>
                                >
                            >
                        > RouteResendOptions;

    typedef RouteOption<
                            boost::mpl::string< '1','s','t',' ','G','A','T','E','W','A','Y' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< 'm','t','_','n','u','l','l' >,
                                    boost::mpl::string< 'm','t','_','i','n','p','l','a','t' >,
                                    boost::mpl::string< 'm','t','_','b','e','e','p','s','e','n','d' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','f','r' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u','s' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','z','r' >,
                                    boost::mpl::string< 'm','t','_','i','f','r','e','e' >,
                                    boost::mpl::string< 'm','t','_','r','o','u','t','e','s','m','s' >,
                                    boost::mpl::string< 'm','t','_','m','o','b','i','w','e','b' >,
                                    boost::mpl::string< 'm','t','_','f','o','r','t','y','t','w','o' >
				>,
                                boost::mpl::string< 'm','t','_','n','u','l','l' >
                            >
                        > RouteFirstGate;


    typedef RouteOption<
                            boost::mpl::string< '2','n','d',' ','G','A','T','E','W','A','Y' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< 'm','t','_','n','u','l','l' >,
                                    boost::mpl::string< 'm','t','_','b','e','e','p','s','e','n','d' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','f','r' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u','s' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','z','r' >,
                                    boost::mpl::string< 'm','t','_','i','f','r','e','e' >,
                                    boost::mpl::string< 'm','t','_','r','o','u','t','e','s','m','s' >,
                                    boost::mpl::string< 'm','t','_','m','o','b','i','w','e','b' >,
				    boost::mpl::string< 'm','t','_','f','o','r','t','y','t','w','o' >
                                >,
                                boost::mpl::string< 'm','t','_','n','u','l','l' >
                            >
                        > RouteSecondGate;

    typedef RouteOption<
                            boost::mpl::string< '3','r','d',' ','G','A','T','E','W','A','Y' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< 'm','t','_','n','u','l','l' >,
                                    boost::mpl::string< 'm','t','_','b','e','e','p','s','e','n','d' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','f','r' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','r','u','s' >,
                                    boost::mpl::string< 'm','t','_','s','m','s','t','_','z','r' >,
                                    boost::mpl::string< 'm','t','_','i','f','r','e','e' >,
                                    boost::mpl::string< 'm','t','_','r','o','u','t','e','s','m','s' >,
                                    boost::mpl::string< 'm','t','_','m','o','b','i','w','e','b' >
                                >,
                                boost::mpl::string< 'm','t','_','n','u','l','l' >
                            >
                        > RouteThirdGate;

    typedef RouteOption<
                            boost::mpl::string< '1','g','w',' ','S','L','I','P','P','A','G','E' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< '0','.','0','0' >,
                                    boost::mpl::string< '0','.','0','1' >,
                                    boost::mpl::string< '0','.','0','2' >,
                                    boost::mpl::string< '0','.','0','3' >,
                                    boost::mpl::string< '0','.','0','5' >,
                                    boost::mpl::string< '0','.','1','0' >,
                                    boost::mpl::string< '0','.','2','0' >,
                                    boost::mpl::string< '0','.','3','0' >,
                                    boost::mpl::string< '0','.','5','0' >
                                >,
                                boost::mpl::string< '0','.','0','0' >
                            >
                        > FirstGwSlippage;

    typedef RouteOption<
                            boost::mpl::string< '2','g','w',' ','S','L','I','P','P','A','G','E' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< '0','.','0','0' >,
                                    boost::mpl::string< '0','.','0','1' >,
                                    boost::mpl::string< '0','.','0','2' >,
                                    boost::mpl::string< '0','.','0','3' >,
                                    boost::mpl::string< '0','.','0','5' >,
                                    boost::mpl::string< '0','.','1','0' >,
                                    boost::mpl::string< '0','.','2','0' >,
                                    boost::mpl::string< '0','.','3','0' >,
                                    boost::mpl::string< '0','.','5','0' >
                                >,
                                boost::mpl::string< '0','.','0','0' >
                            >
                        > SecondGwSlippage;

    typedef RouteOption<
                            boost::mpl::string< 'R','o','u','t','i','n','g',' ','P','o','l','i','c','y' >,
                            RouteValueChoise
                            <
                                boost::mpl::vector<
                                    boost::mpl::string< 'n','o','r','m','a','l' >,
                                    boost::mpl::string< 'f','a','s','t' >,
                                    boost::mpl::string< 'c','h','e','a','p' >,
                                    boost::mpl::string< 'd','i','s','a','b','l','e','d' >
                                >,
                                boost::mpl::string< 'n','o','r','m','a','l' >
                            >
                        > UserRouteFirstGate;

    struct RouteOperatorInfo {
        std::map< std::string, std::string > options;

        template<class Archive>
            void serialize(Archive & ar, const unsigned int) {
                ar & BOOST_SERIALIZATION_NVP(options);
            }
    };

    struct RouteCountryInfo {
        std::map< std::string, std::string > options;
        std::map< std::string, RouteOperatorInfo > operators;

        template<class Archive>
            void serialize(Archive & ar, const unsigned int) {
                ar & BOOST_SERIALIZATION_NVP(options);
                ar & BOOST_SERIALIZATION_NVP(operators);
            }
    };

    struct RouteInfo {
        std::string name;
        std::map< std::string, std::string > options;
        std::map< std::string, RouteCountryInfo > countries;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int) {
                ar & BOOST_SERIALIZATION_NVP(name);
                ar & BOOST_SERIALIZATION_NVP(options);
                ar & BOOST_SERIALIZATION_NVP(countries);
            }
    };

    Route( );
    Route( std::string name );
    Route( std::string name, std::string source );

    std::string serialize();

    void setName( std::string n ) { route.name = n; }
    std::string getName( ) const { return route.name; };

    template<class Archive>
        void serialize(Archive & ar, const unsigned int) {
            ar & BOOST_SERIALIZATION_NVP( route );
        }

    template < class Option >
    boost::logic::tribool hasOption() { return hasOption( Option::getName() ); }

    template < class Option >
    boost::logic::tribool hasOption( std::string country ) { return hasOption( Option::getName(), country ); }

    template < class Option >
    boost::logic::tribool hasOption( std::string country, std::string oper ) { return hasOption( Option::getName(), country, oper ); }

    template < class Option >
    Option getOption() { return Option( getOption( Option::getName() ) ); }

    template < class Option >
    Option getOption( std::string country ) { return Option( getOption( Option::getName(), country ) ); }

    template < class Option >
    Option getOption( std::string country, std::string oper ) { return Option( getOption( Option::getName(), country, oper ) ); }

    template < class Option >
    void setOption( Option option ) { setOption( Option::getName(), option.serialize() ); }

    template < class Option >
    void setOption( Option option, std::string country ) { setOption( Option::getName(), country, option.serialize() ); }

    template < class Option >
    void setOption( Option option, std::string country, std::string oper ) { setOption( Option::getName(), country, oper, option.serialize() ); }

    template < class Option >
    void removeOption() { return removeOption( Option::getName() ); }

    template < class Option >
    void removeOption( std::string country ) { return removeOption( Option::getName(), country ); }

    template < class Option >
    void removeOption( std::string country, std::string oper ) { return removeOption( Option::getName(), country, oper ); }


private:
    RouteInfo route;

    boost::logic::tribool hasOption( std::string name );
    boost::logic::tribool hasOption( std::string name, std::string country );
    boost::logic::tribool hasOption( std::string name, std::string country, std::string oper );

    std::string getOption( std::string name );
    std::string getOption( std::string name, std::string country );
    std::string getOption( std::string name, std::string country, std::string oper );

    void setOption( std::string name, std::string value );
    void setOption( std::string name, std::string country, std::string value );
    void setOption( std::string name, std::string country, std::string oper, std::string value );

    void removeOption( std::string name );
    void removeOption( std::string name, std::string country );
    void removeOption( std::string name, std::string country, std::string oper );

    friend class RouteManager;
};

class RouteManager: public boost::serialization::singleton< RouteManager > {
public:
    typedef std::map< std::string, Route > RouteMapT;
    typedef std::list< std::string > RouteListT;

    RouteManager();
    ~RouteManager();

    void updateRouteList();
    Route loadRoute( std::string name );
    void saveRoute( std::string name, std::string owner, Route t );
    void removeRoute( std::string name );
    RouteListT routes_list( std::string owner);

private:
    RouteListT tlist;
    RouteMapT tmap;
    int updateTimerID;

    PGSql& db;
    boost::recursive_mutex lock;
};

#endif // ROUTE_H
