/* 
 * File:   Operation.h
 * Author: mohtep
 *
 * Created on 22 Февраль 2010 г., 12:38
 */

#ifndef _OPERATION_H
#define	_OPERATION_H

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace sms {

    #define VectorMacro(s, n, data)                                                     \
    template < BOOST_PP_ENUM_PARAMS( n, class T ) >                                     \
    struct Vector ## n {                                                                \
        typedef boost::mpl::vector ## n < BOOST_PP_ENUM_PARAMS( n, T ) > Type;          \
    };                                                                                  

    BOOST_PP_REPEAT_FROM_TO(1, 20, VectorMacro, data)
    #undef VectorMacro

    template < class TypeList >
    class Operation {
    public:
        enum {
            OP_UNDEFINED = -1
        };

        Operation();
        Operation ( const Operation< TypeList >& orig );
        Operation& operator=(const Operation< TypeList >& orig);
        ~Operation();

        template <int K>
        static Operation< TypeList > create( const typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type& val, std::string ownerid= "", unsigned int major_priority = 0, unsigned char minor_priority = 0 );

        template <int K>
        typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type& get( ) const;

        int type( ) const {
            return __type;
        }

        unsigned int majorPriority() { return map; }
        unsigned int minorPriority() { return mip; }
        std::string ownerID() { return ownerid; }


    private:
        int __type;
        void* __type_data;
        int* __links_num;
        unsigned int map;
        unsigned char mip;
        std::string ownerid;

        boost::recursive_mutex lock;
    };

    template < class TypeList >
    Operation< TypeList >::Operation() {
        __type = OP_UNDEFINED;
        __links_num = new int;
        (*__links_num) = 1;
    }

    template < class TypeList >
    Operation< TypeList >::Operation ( const Operation< TypeList >& orig ) {
        boost::recursive_mutex::scoped_lock scoped_lock( const_cast< Operation< TypeList >& >(orig).lock);

        __type = orig.__type;
        __type_data = orig.__type_data;
        __links_num = orig.__links_num;
        map = orig.map;
        mip = orig.mip;
        ownerid = orig.ownerid;

        (*__links_num)++;
    }

    template < class TypeList >
    Operation< TypeList >& Operation< TypeList >::operator=(const Operation< TypeList >& orig) {
        boost::recursive_mutex::scoped_lock scoped_lock( const_cast< Operation< TypeList >& >(orig).lock);

        __type = orig.__type;
        __type_data = orig.__type_data;
        __links_num = orig.__links_num;
        map = orig.map;
        mip = orig.mip;
        ownerid = orig.ownerid;

        (*__links_num)++;
        
        return *this;
    }


    template < class TypeList >
    Operation< TypeList >::~Operation() {
        boost::recursive_mutex::scoped_lock scoped_lock( lock );

        (*__links_num)--;
        if ( (*__links_num) > 0 ) {
            return;
        }

        #define DeleteMacro(s, n, data)                                                                                 \
            case n :                                                                                                    \
                delete static_cast< typename boost::mpl::at< TypeList, boost::mpl::int_< n > >::type *>(__type_data);   \
                break;

        switch ( __type ) {
            BOOST_PP_REPEAT( 20, DeleteMacro, data )
        }

        delete __links_num;

        #undef DeleteMacro
    }

    template < class TypeList >
    template < int K >
    Operation< TypeList > Operation< TypeList >::create( const typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type& val, std::string ownerid, unsigned int major_priority, unsigned char minor_priority ) {
        Operation< TypeList > m;
        m.__type = K;
        m.__type_data = new typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type( val );

        m.map = major_priority;
        m.mip = minor_priority;
        m.ownerid = ownerid;

        return m;
    }

    template < class TypeList >
    template <int K>
    typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type& Operation< TypeList >::get( ) const {
        return *static_cast< typename boost::mpl::at< TypeList, boost::mpl::int_< K > >::type * >( __type_data );
    }

//    typedef Operation< Vector1< int > > SMSOperation;
}

#endif	/* _OPERATION_H */

